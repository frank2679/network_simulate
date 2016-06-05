#include <iostream>
#include <vector>
#include <time.h>
#include "myClass.h"
#include "functionDeclaration.h"
using namespace std; 
/** event 1 **/
void apSendTriggerRandom( struct Event &nextEvent, vector<STA> &stations )
{
    std::cout << "Operation [1]: ap send TFR" << std::endl;
    //* decide who will RR *//
    vector<STA>::iterator it;
    int newEventID = 0;
    int newEventTime = 0;
    int newEventDuration = 0;
    bool flag = true; // no sta will UL
    for ( it = stations.begin(); it != stations.end(); it++ )
        if ( it->getULQueue() )
        {
            it->setContendSucIndicate(true); // set according to ULQueue
            flag = false;
        }

    if ( !flag )
    {
        // for test, display the RR STAs
        cout << "RR Staions ID (both suc or not): ";
        for ( it = stations.begin(); it != stations.end(); it++ )
            if ( it->contendSucIndicate() )
                cout << it->getID() << ' ' ;
        cout << endl;
        //* update stations state *// //check later 
        for ( int i = 0; i < numSta; i++ )
            updateStaState ( stations[i], 0, timeTFR, SIFS, 0);
        //* update next event *//
        newEventID = 2;
        newEventTime = myClock + nextEvent.eventDuration; // the endtime of this event
        newEventDuration = timeRR + timeContending + SIFS;
    }
    else // no sta UL
    {
        cout << "No stations will UL. " << endl;
        //+ update stations state //check later 
        //+ update next event 
        newEventID = 1;
        newEventTime = (myClock/TFPeriod+1) * TFPeriod  ; // the endtime of this event
        newEventDuration = timeTFR + SIFS;
    }
    // update next event
    updateNextEvent( nextEvent, newEventID, newEventTime, newEventDuration ); 
    displayNextEvent(nextEvent);
}

/** event 2 **/
void RR( struct Event &nextEvent, vector<STA> &stations )
{
    std::cout << "Operation [2]: Resource Request" << std::endl;
    vector<STA>::iterator it;
    int newEventID = 0;
    int newEventTime = 0;
    int newEventDuration = 0;
    srand ( time(NULL));
    vector<int> contendSta(numSta, 0);
    for ( int i = 0; i < numSta; i++ )
    {
        contendSta[i] = rand() % contendWindow;
        //cout << "STA " << i + 1 << " backoff is " << contendSta[i] << endl;
    }

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
    //** test contend **//
    cout << "suc contend STAs(ID): " ;
    for ( it = stations.begin(); it != stations.end(); it++ )
        if ( it->contendSucIndicate() )
        {
            cout << it->getID() << ' ';
        }
    cout << endl;
    //* update stations state *// 
    for ( it = stations.begin(); it != stations.end(); it++ )
        if ( it->contendSucIndicate() )
            updateStaState ( *it, timeRR, 0, SIFS, 0);
    //* next event *//
    newEventID = 3;
    newEventTime = myClock + nextEvent.eventDuration; // the endtime of this event
    newEventDuration = timeTF + SIFS;
    updateNextEvent( nextEvent, newEventID, newEventTime, newEventDuration );// ID = 2, means RR 
    displayNextEvent(nextEvent);
}

/** event 3 **/
void triggerAllocation( struct Event &nextEvent, vector<STA> &stations )
{
    std::cout << "Operation [3]: Trigger Allocation" << std::endl;
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
    // come from event [2]
    vector<int> contendSucSta; // contain the STA ID  
    // compute ns 
    for ( it = stations.begin(); it != stations.end(); it++ )
    {
        if ( it->contendSucIndicate() )
        {
            it->setContendSucIndicate(true);
            contendSucSta.push_back(it->getID());
            ns++;
            flagLast_2 = true;
        }
        if ( it->getMoreBit() )
        {
            flagLast_4 = true;
            it->setRRSucIndicate(true);
            // get the number of STAs who has more UL packets.
            Ns++;
            // reset morebit
            it->setMoreBit(false);
        }
    }
    // come from event [4]
    if ( flagLast_4 )
    {
        flagLast_4 = false;
        cout << "last event is event [4]. " << endl;
    }

    //++ unselect extra stations ++//
    if ( flagLast_2 )
    {
        flagLast_2 = false;
        cout << "last event is event [2]. " << endl;
        srand ( time(NULL));
        int index = 0;
        Ns = ns;
        if ( ns > nch )
        {
            index = rand() % nch;
            int numToUnselect = ns - nch; 
            //    cout << "num to unselect: " << numToUnselect << endl;
            for ( int i = 0; i < numToUnselect; i++ )
            {
                stations[contendSucSta[index]].setRRSucIndicate(false);
                index = (index+1)%ns;
            }
            Ns = nch;
        }else 
            for ( it = stations.begin(); it != stations.end(); it++ )
                if ( it->contendSucIndicate() )
                    it->setRRSucIndicate(true);
        // reset contend indicate
        for ( it = stations.begin(); it != stations.end(); it++ )
            it->setContendSucIndicate(false);
        //++ test RR ++//
        cout << "suc RR STAs(ID): " ;
        for ( it = stations.begin(); it != stations.end(); it++ )
            if ( it->RRSucIndicate() )
            {
                cout << it->getID() << ' ';
            }
        cout << endl;
    }

    //++ set Rate ++//
    for ( it = stations.begin(); it != stations.end(); it++ )
        if ( it->RRSucIndicate() )
        {
            //+++ set data rate +++//
            it->setRate(double(nch)/double(Ns));
            dataRate = it->getRate();
            //cout << "STA " << stations[i].getID() << " data rate : " << dataRate  << endl;
        }

    //+ update next event +//
    newEventID = 4;
    newEventTime = myClock + nextEvent.eventDuration; // the endtime of this event
    newEventDuration = double(timePkt)/double(dataRate) + SIFS; // datarate
    updateNextEvent( nextEvent, newEventID, newEventTime, newEventDuration ); 
    displayNextEvent(nextEvent);
}

/** event 4 **/
void ULTransmit( struct Event &nextEvent, vector<STA> &stations )
{
    std::cout << "Operation [4]: UL Transmit" << std::endl;
    vector<STA>::iterator it;
    int newEventID = 0;
    int newEventTime = 0;
    int newEventDuration = 0;
    double dataRate = 0;
    for ( it = stations.begin(); it != stations.end(); it++ )
        if ( it->RRSucIndicate() ) // stasions who will up packet
        {
            cout << "Pop mow+++++++++++++++++++++++++" << endl;
            //+ update queue +//
            it->popULQueue();
            // reset RRSucIndicate
            it->setRRSucIndicate(false);
            // set more bit
            if ( it->getULQueue() ) // all packets have been sent.
                it->setMoreBit(true);
            // reset data rate
            it->setRate(1.0);
        }

    // display the UL queue
    cout << "after pop-queue: " << endl;
    for (it = stations.begin(); it != stations.end(); it++ )
        cout << it->getULQueue() << ' ';  
    cout << endl;
    // update next event //
    newEventID = 5;
    for ( int i = 0; i < numSta; i++ )
        if ( stations[i].getMoreBit() )
            newEventID = 3;
    newEventTime = myClock + nextEvent.eventDuration; // the endtime of this event
    newEventDuration = timeACK;
    updateNextEvent( nextEvent, newEventID, newEventTime, newEventDuration ); 
    displayNextEvent(nextEvent);
}

/** event 5 **/
void triggerAck( struct Event &nextEvent, vector<STA> &stations )
{
    std::cout << "Operation [5]: trigger ACK" << std::endl;
    vector<STA>::iterator it;
    int newEventID = 0;
    int newEventTime = 0;
    int newEventDuration = 0;
    newEventID = 1;
    newEventTime = (myClock/TFPeriod+1) * TFPeriod  ; // the endtime of this event
    newEventDuration = timeTFR + SIFS;
    updateNextEvent( nextEvent, newEventID, newEventTime, newEventDuration ); 
    displayNextEvent(nextEvent);
}


/** initialize system  **/
void initialize( struct Event &nextEvent, vector<STA> &stations )
{
    //* set stations *//
    vector<STA>::iterator it = stations.begin();
    for (int i = 1; it != stations.end(); i++, it++)
    {
        it->setID(i); // set ID
        it->setLocalLambda(globalLambda); // set lambda
    }
    updateNextEvent( nextEvent, 1, TFPeriod, timeTFR );
    cout << "*********** system initialized **************" << endl;
    displayNextEvent( nextEvent ); 
}

/** update next event **/
void updateNextEvent( struct Event &lastEvent, int newID, int newEventTime, int newEventDuration )
{
    lastEvent.eventID = newID; 
    lastEvent.eventTime = newEventTime; 
    lastEvent.eventDuration = newEventDuration;
}

/* help functions */
/** displaySystemState **/
void displaySystemState( vector<STA> &stations)
{
    cout << "*********** system state top **************" << endl;
    cout << "system clock(us): " << myClock << endl;
    cout << "simulation endtime: " << endTime << endl;
    /* test container */
    cout << "stations ID: " ;
    for(int i = 0; i < numSta; i++)
    {
        if ( !(i%20) )
            cout << endl;
        cout << setw(2) << stations[i].getID() << ' ';
    }
    cout << "(0 is AP)" <<  endl;
    cout << "TF period: " << TFPeriod << endl;
    cout << "system lambda: " << globalLambda << endl;
    cout << "*********** system state end **************" << endl;
}

/** displayNextEvent **/
void displayNextEvent(const struct Event &nextEvent )
{
    cout << "-------------next event: -------------" << endl;
//    cout << "myclock:        " << myClock << endl;
    cout << "event ID:       " << nextEvent.eventID << endl;
    cout << "event time:     " << nextEvent.eventTime << endl;
    cout << "event duration: " << nextEvent.eventDuration << endl;
    cout << "-----------------end------------------" << endl;
}

/** estimate system performance **/
void estimate( vector<STA> &stations )
{
    int totalSentPkt = 0;
    for ( int i = 0; i < numSta; i++ )
    {
        //* validate the arrival generate *//
        totalSentPkt = stations[i].getTotalSentPkt();
        cout << "estimated lambda: " << double(totalSentPkt)/double(endTime) << endl;
    }
}
