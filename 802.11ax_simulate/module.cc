#include <iostream>
#include <vector>
#include <time.h>
#include "myClass.h"
#include "functionDeclaration.h"
using namespace std; 
/** event 1 **/
struct Event apSendTriggerRandom( struct Event &nextEvent, vector<STA> &stations )
{
    cout << endl << endl << "--------------------a new TF period--------------------" << endl;
    //cout << "event[1] numSta = " << numSta << endl;
    std::cout << endl << "Operation [1]: ap send TFR" << std::endl;
    cout << "myclock : " << myClock << endl;
    displayULQueue(stations);
    // + local parameters
    vector<STA>::iterator it;
    int newEventID = 0;
    int newEventTime = 0;
    int newEventDuration = 0;

    //+ decide who will RR 
    bool flag = true; // no sta will UL
    for ( it = stations.begin(); it != stations.end(); it++ )
        if ( it->getULQueue() )
        {
            it->setContendSucIndicate(true); // set according to ULQueue
            it->setContendThisTFP(true);
            flag = false;
        }

    //+ update stations working time
    it = stations.begin();
    for ( int i = 0; it != stations.end(); it++, i++ )
    {
        if ( it->contendSucIndicate() ) // STAs with UL 
        {
            cout << "$ [1] STA " << i+1 << " with UL, RX = " << timeTFR << endl;
            updateStaState(stations[i], 0, timeTFR, SIFS, 0); 
            updateStaStatePS(stations[i], 0, timeTFR, SIFS, 0); 
        }else // STAs without UL will sleep or idle until next TF period
        {
            cout << "$ [1] STA " << i+1 << " without UL, idle = " << TFPeriod << endl;
            updateStaState(stations[i], 0, 0, TFPeriod, 0); 
            updateStaStatePS(stations[i], 0, 0, 0, TFPeriod); 
        }
    }
    // determine next event
    if ( !flag ) // with UL
    {
        // for test, display the RR STAs
        cout << "RR Staions ID (both suc or not): ";
        for ( it = stations.begin(); it != stations.end(); it++ )
            if ( it->contendSucIndicate() )
                cout << it->getID() << ' ' ;
        cout << endl;
        newEventID = 2;
        newEventTime = myClock + nextEvent.eventDuration; // the endtime of this event
        newEventDuration = timeRR + timeContending + SIFS;
    }
    else // no sta UL
    {
        cout << "No stations will UL. " << endl;
        newEventID = 1;
        int lastTFRtime = ((myClock%TFPeriod)?(myClock/TFPeriod):(myClock/TFPeriod-1))*TFPeriod;
        cout << "lastTFRtime: " << lastTFRtime << endl;
        newEventTime = lastTFRtime + 2*TFPeriod  ; // the endtime of this event
        newEventDuration = timeTFR + SIFS;
    }
    // generate next event
    struct Event lastEvent;
    updateEvent( lastEvent, nextEvent );
    updateEvent( nextEvent, newEventID, newEventTime, newEventDuration ); 
    displayNextEvent( nextEvent );
    return lastEvent;
}

/** event 2 **/
struct Event RR( struct Event &nextEvent, vector<STA> &stations )
{
    std::cout << endl << "Operation [2]: Resource Request" << std::endl;
    cout << "myclock : " << myClock << endl;
    displayULQueue(stations);
    // local parameters
    vector<STA>::iterator it;
    int newEventID = 0;
    int newEventTime = 0;
    int newEventDuration = 0;
    srand ( time(NULL));
    vector<int> contendSta(numSta, 0);
    for ( int i = 0; i < numSta; i++ )
    {
        contendSta[i] = rand() % contendWindow;
    }
    //+ update stations working time
    it = stations.begin();
    for ( int i = 0; it != stations.end(); it++, i++ )
    {
        if ( it->getContendThisTFP() )// any STAs who need RR
        {
            cout << "$ [2] STA " << i+1 << " with UL, TX = " << timeRR <<  endl;
            updateStaState(stations[i], timeRR, 0, SIFS, 0); 
            updateStaStatePS(stations[i], timeRR, 0, SIFS, 0); 
        }
    }

    // determine who contend successfully
    int flag = 0;
    for ( int i = 0; i < numSta-1; i++ ) 
    {
        if ( stations[i].contendSucIndicate() )
        {
            for ( int j = i+1; j < numSta; j++ )
            {
                if ( contendSta[i] == contendSta[j] )
                    if ( stations[i].contendSucIndicate() )
                        if ( stations[j].contendSucIndicate() )
                        {
                            stations[j].setContendSucIndicate(false);// an pitfall here. 
                            flag = 1;
                        }
            }
            if ( flag )
            {
                stations[i].setContendSucIndicate(false); // when more than 2 stas with the same value, only two will be shutdown. 
                flag = 0;
            }
        }
    }
    //+ test contend 
    /*
    cout << "suc contend STAs(ID): " ;
    for ( it = stations.begin(); it != stations.end(); it++ )
        if ( it->contendSucIndicate() )
        {
            cout << it->getID() << ' ';
        }
    cout << endl;
    */
    // generate next event 
    newEventID = 3;
    newEventTime = myClock + nextEvent.eventDuration; // the endtime of this event
    newEventDuration = timeTF + SIFS;
    struct Event lastEvent;
    updateEvent( lastEvent, nextEvent );
    updateEvent( nextEvent, newEventID, newEventTime, newEventDuration );// ID = 2, means RR 
    return lastEvent;
}

/** event 3 **/
struct Event triggerAllocation( struct Event &nextEvent, vector<STA> &stations )
{
    std::cout << endl << "Operation [3]: Trigger Allocation" << std::endl;
    cout << "myclock : " << myClock << endl;
    displayULQueue(stations);
    //+ RR procedure +//
    vector<STA>::iterator it;
    int newEventID = 0;
    int newEventTime = 0;
    int newEventDuration = 0;
    int ns = 0; // num of suc contending stations 
    int Ns = 0;
    double dataRate = 1.0;

    bool flagLast_2= false;
    bool flagLast_4= false;
    vector<int> contendSucSta; // contain the STA ID  
    // determine lastEvent and compute ns 
    it = stations.begin();
    for ( int i = 0; it != stations.end(); i++,it++ )
    {
        if ( it->contendSucIndicate() )
        {
            contendSucSta.push_back(it->getID());
            ns++;
            flagLast_2 = true;
        }
        if ( it->getMoreBit() )
        {
            flagLast_4 = true;
        }
    }

    // come from event [2]
    if ( flagLast_2 )
    {
        flagLast_2 = false;
        //cout << "last event is event [2]. " << endl;
        srand ( time(NULL));
        int index = 0;
        Ns = ns;
        // set RRSucIndicator
        for ( it = stations.begin(); it != stations.end(); it++ )
            if ( it->contendSucIndicate() )
                it->setRRSucIndicate(true);

        if ( ns > nch )
        {
            index = rand() % nch;
            int numToUnselect = ns - nch; 
            for ( int i = 0; i < numToUnselect; i++ )
            {
                stations[contendSucSta[index]].setRRSucIndicate(false);
                index = (index+1)%ns;
            }
            Ns = nch;
        }
        // reset contend indicate
        for ( it = stations.begin(); it != stations.end(); it++ )
            it->setContendSucIndicate(false);
        // test RR
        displayRRSucIndicate(stations);

        //+ update stations working time
        it = stations.begin();
        for ( int i = 0; i < numSta; i++, it++ )
        {
            if ( it->getContendThisTFP() ) // STAs who need RR
            {
                cout << "$ [3] STA " << i+1 << " RR, RX = " << timeTF <<  endl;
                updateStaState(stations[i], 0, timeTF, SIFS, 0); 
                updateStaStatePS(stations[i], 0, timeTF, SIFS, 0); 
            }//else, non-active STAs have been considered in [1].
        }
    }

    // come from event [4]
    if ( flagLast_4 )
    {
        // display the suc STAs or More bit stations
        flagLast_4 = false;
        Ns = 0;
        cout << "last event is event [4]. " << endl;
        //displayMoreBit(stations);
        it = stations.begin();
        for ( int i = 0; it != stations.end(); i++, it++ )
            if ( it->getMoreBit() )
            {
                it->setRRSucIndicate(true);
                //+ update stations working time
                cout << "$ [3] STA " << i+1 << " more bit, RX = " << timeTF <<  endl;
                updateStaState(stations[i], 0, timeTF, SIFS, 0); 
                updateStaStatePS(stations[i], 0, timeTF, SIFS, 0); 
                // get the number of STAs who has more UL packets.
                Ns++;
                // reset morebit
                it->setMoreBit(false);
            }
        // test RR
        //displayRRSucIndicate(stations);
    }
    //++ set Rate ++//
    for ( it = stations.begin(); it != stations.end(); it++ )
        if ( it->RRSucIndicate() )
        {
            it->setRRSuc(true); // for working time compute
            //+++ set data rate +++//
            dataRate = double(nch)/double(Ns);
            it->setRate(dataRate);
            //cout << "STA " << stations[i].getID() << " data rate : " << dataRate  << endl;
        }

    //+ generate next event 
    newEventID = 4;
    newEventTime = myClock + nextEvent.eventDuration; // the endtime of this event also the start time of next event
    newEventDuration = double(timePkt)/double(dataRate) + SIFS; // datarate
    //++ check whether too many UL packets or not ++// 
    int ULThreshold = 0;
    int lastTFRtime = ((myClock%TFPeriod)?(myClock/TFPeriod):(myClock/TFPeriod-1))*TFPeriod;
    cout << "lastTFRtime = " << lastTFRtime << endl;
    ULThreshold = timeMaxUL + lastTFRtime;
    if ( newEventTime >= ULThreshold )
    {
        tooManyUL = true;
        cout << "[3] Too many UL packets.+++++++++++++++++++++++ " << endl;
        cout << "nextEventTime: " << newEventTime << endl;
        newEventID = 1; 
        newEventTime = lastTFRtime + TFPeriod  ; // the endtime of this event
        newEventDuration = timeTFR + SIFS;
        // reset more bit
        for ( it = stations.begin(); it != stations.end(); it++ )
        {
            if ( it->getMoreBit() )
                it->setMoreBit(false);
        }
    }
    struct Event lastEvent;
    updateEvent( lastEvent, nextEvent );
    updateEvent( nextEvent, newEventID, newEventTime, newEventDuration ); 
    return lastEvent;
}

/** event 4 **/
struct Event ULTransmit( struct Event &nextEvent, vector<STA> &stations )
{
    std::cout << endl << "Operation [4]: UL Transmit" << std::endl;
    cout << "myclock : " << myClock << endl;
    //displayULQueue(stations);
    vector<STA>::iterator it;
    int newEventID = 0;
    int newEventTime = 0;
    int newEventDuration = 0;
    double dataRate = 0;
    // pop the queue, reset RRSucIndicate, set More bit
    it = stations.begin();
    for ( int i = 0; it != stations.end(); i++, it++ )
        if ( it->RRSucIndicate() ) // stasions who will up packet
        {
            //+ update queue +//
            it->popULQueue();
            // reset RRSucIndicate
            it->setRRSucIndicate(false);
            // reset data rate
            it->setRate(1.0);
            //+ update stations working time // STAs who RR suc
            cout << "$ [4] STA " << i+1 << " RR suc, TX = " << nextEvent.eventDuration - SIFS <<  endl;
            updateStaState(stations[i], nextEvent.eventDuration-SIFS, 0, SIFS, 0); 
            updateStaStatePS(stations[i], nextEvent.eventDuration-SIFS, 0, SIFS, 0); 
            // set more bit
            if ( it->getULQueue() ) // all packets have been sent.
            {
                it->setMoreBit(true);
            }else // STAs who will not UL later
            {
                //+ update stations working time // STAs who has nothing UL
                int tempDoze = (myClock/TFPeriod+1)*TFPeriod-myClock-nextEvent.eventDuration-SIFS-timeACK;
                cout << "$ [5] STA " << i+1 << " RR suc, idle = " << tempDoze 
                    << ", RX = " << timeACK <<  endl;
                updateStaState(stations[i], 0, timeACK, tempDoze, 0); 
                updateStaStatePS(stations[i], 0, timeACK, 0, tempDoze ); 
            }
        }

    //+ update stations working time
    for ( int i = 0; i < numSta; i++ )
    {
        if ( stations[i].getContendThisTFP() )
        {
            if ( !stations[i].getRRSuc() ) // STAs who RR failed 
            {
                int tempDoze = (myClock/TFPeriod+1)*TFPeriod - myClock;
                cout << "$ [4] STA " << i+1 << " RR failed, idle = " << tempDoze <<  endl;
                updateStaState(stations[i], 0, 0, tempDoze, 0); 
                updateStaStatePS(stations[i], 0, 0, 0, tempDoze); 
            }
            // reset contend this TFP
            stations[i].setContendThisTFP(false);
        }//else, non-active STAs have been considered in [1].
    }
    // display the UL queue
    //cout << "after pop: ";
    displayULQueue(stations);
    // update next event //
    newEventID = 5;
    for ( int i = 0; i < numSta; i++ )
        if ( stations[i].getMoreBit() )
            newEventID = 3;
    newEventTime = myClock + nextEvent.eventDuration; // the endtime of this event
    newEventDuration = timeACK;

    // check whether too many UL arrives or not
    int ULThreshold = 0;
    int lastTFRtime = ((myClock%TFPeriod)?(myClock/TFPeriod):(myClock/TFPeriod-1))*TFPeriod;
    cout << "lastTFRtime = " << lastTFRtime << endl;
    ULThreshold = timeMaxUL + lastTFRtime;
    if ( newEventTime >= ULThreshold )
    {
        tooManyUL = true;
        cout << "[4] Too many UL packets.+++++++++++++++++++++++ " << endl;
        cout << "nextEventTime: " << newEventTime << endl;
        newEventID = 1;
        newEventTime = lastTFRtime + TFPeriod  ; 
        newEventDuration = timeTFR + SIFS;
        // reset more bit
        for ( it = stations.begin(); it != stations.end(); it++ )
        {
            if ( it->getMoreBit() )
                it->setMoreBit(false);
        }
    }
    struct Event lastEvent;
    updateEvent( lastEvent, nextEvent );
    updateEvent( nextEvent, newEventID, newEventTime, newEventDuration ); 
    return lastEvent;
}

/** event 5 **/
struct Event triggerAck( struct Event &nextEvent, vector<STA> &stations )
{
    std::cout << endl << "Operation [5]: trigger ACK" << std::endl;
    cout << "myclock : " << myClock << endl;
    vector<STA>::iterator it;
    int newEventID = 0;
    int newEventTime = 0;
    int newEventDuration = 0;
    //+ update stations working time
    for ( int i = 0; i < numSta; i++ )
    {
        if ( stations[i].getRRSuc() ) // STAs who RR
        {
            //int tempDoze = (myClock/TFPeriod+1)*TFPeriod-myClock-timeACK;
            //cout << "$ [5] STA " << i+1 << " RR suc, idle = " << tempDoze 
            //    << ", RX = " << timeACK <<  endl;
            //updateStaState(stations[i], 0, timeACK, tempDoze, 0); 
            //updateStaStatePS(stations[i], 0, timeACK, 0, tempDoze ); 
            // reset RRsuc
            stations[i].setRRSuc(false);
            // reset contend this TFP
            stations[i].setContendThisTFP(false);
        }
    }

    // generate next Event
    newEventID = 1;
    int lastTFRtime = ((myClock%TFPeriod)?(myClock/TFPeriod):(myClock/TFPeriod-1))*TFPeriod;
    newEventTime = lastTFRtime + TFPeriod; 
    newEventDuration = timeTFR + SIFS;
    struct Event lastEvent;
    updateEvent( lastEvent, nextEvent );
    updateEvent( nextEvent, newEventID, newEventTime, newEventDuration ); 
    return lastEvent;
}

/** estimate system performance **/
double estimate( vector<STA> &stations )
{
    vector<STA>::iterator it;
    int totalSentPkt = 0;
    // display arrival average
    for ( int i = 0; i < numSta; i++ )
    {
        totalSentPkt = stations[i].getTotalSentPkt();
        cout << "total sent pkt: " << totalSentPkt << endl;
        cout << "estimated lambda: " << double(totalSentPkt)/double(endTime) << endl;
    }
    // display working state and compute energy consumption
    double totalEnergyEfficiency = 0.0;
    for ( it = stations.begin(); it != stations.end(); it++)
    {
        cout << "-------------STA " << it->getID() << "-------------" << endl;
        it->displayStaState();
        it->energyConsumption(); // compute
        it->computeEnergyEfficiency();
        cout << "Energy efficiency: " << it->getEnergyEfficiency() << endl;
    }
    for ( it = stations.begin(); it != stations.end(); it++)
    {
        totalEnergyEfficiency += it->getEnergyEfficiency();
    }
    totalEnergyEfficiency /= numSta;
    cout << endl << "total Energy Efficiency = " 
        << totalEnergyEfficiency << endl; 
    return totalEnergyEfficiency;
}

/** initialize system  **/
void initialize( struct Event &nextEvent, vector<STA> &stations )
{
    myClock = 0;
    //* set stations *//
    vector<STA>::iterator it = stations.begin();
    for (int i = 1; it != stations.end(); i++, it++)
    {
        it->setID(i); // set ID
        it->setLocalLambda(globalLambda); // set lambda
        //updateStaState(stations[i-1], 0, 0, TFPeriod, 0); // don't include the first TFP into energy consumption
        //updateStaStatePS(stations[i-1], 0, 0, 0, TFPeriod); 
    }
    updateEvent( nextEvent, 1, TFPeriod, timeTFR );
    cout << "*************************** system initialized ***********************************" << endl;
    displayNextEvent( nextEvent ); 
}

/** update next event **/
void updateEvent( struct Event &lastEvent, int newID, int newEventTime, int newEventDuration )
{
    lastEvent.eventID = newID; 
    lastEvent.eventTime = newEventTime; 
    lastEvent.eventDuration = newEventDuration;
}

void updateEvent( struct Event &lastEvent, struct Event &nextEvent )
{
    lastEvent.eventID = nextEvent.eventID;
    lastEvent.eventTime = nextEvent.eventTime;
    lastEvent.eventDuration = nextEvent.eventDuration;
}

/** displaySystemState **/
void displaySystemState( vector<STA> &stations)
{
    cout << "*********** system state top **************" << endl;
    cout << "system clock(us):    " << myClock << endl;
    cout << "simulation endtime:  " << endTime << endl;
    /* test container */
    cout << "stations ID: (0 is AP)" ;
    for(int i = 0; i < numSta; i++)
    {
        if ( !(i%20) )
            cout << endl;
        cout << setw(2) << stations[i].getID() << ' ';
    }
    cout << endl << "TF period:           " << TFPeriod << endl;
    cout << "Contend Window Size: " << contendWindow << endl; 
    cout << "system lambda:       " << globalLambda << endl;
    cout << "the Maximum UL:      " << timeMaxUL << endl;
    cout << "*********** system state end **************" << endl;
}

/** displayNextEvent **/
void displayNextEvent(const struct Event &nextEvent )
{
    cout << "+++++++++++++next event: +++++++++++++" << endl;
//    cout << "myclock:        " << myClock << endl;
    cout << "event ID:       " << nextEvent.eventID << endl;
    cout << "event time:     " << nextEvent.eventTime << endl;
    cout << "event duration: " << nextEvent.eventDuration << endl;
    cout << "+++++++++++++++++end++++++++++++++++++" << endl;
}

void displayLastEvent(const struct Event &nextEvent )
{
    cout << "-------------last event: -------------" << endl;
//    cout << "myclock:        " << myClock << endl;
    cout << "event ID:       " << nextEvent.eventID << endl;
    cout << "event time:     " << nextEvent.eventTime << endl;
    cout << "event duration: " << nextEvent.eventDuration << endl;
    cout << "-----------------end------------------" << endl;
}

