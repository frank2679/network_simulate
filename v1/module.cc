#include <iostream>
#include <vector>
#include <time.h>
#include "myClass.h"
#include "functionDeclaration.h"
using namespace std; 
/** event 1 **/
void apSendTriggerRandom( struct Event &nextEvent, vector<STA> &stations )
{
//    std::cout << "Operation [1]: ap send TFR" << std::endl;
    //* update variables *//
    //* generate traffic *//
    for ( int i = 0; i < numSta; i++ )
    {
        stations[i].generateArrival();
        //stations[i].displayArrivalTime();
    }
    //* decide who will RR *//
    for (int i = 0; i < numSta; i++ )
        if ( stations[i].getULQueue() )
            stations[i].setRRSucIndicate(true); 

    // for test, display the RR STAs
    /*
    cout << "RR Staions (both suc or not): ";
    for (auto it = stations.begin(); it != stations.end(); it++ )
        if ( it->RRSucIndicate() )
            cout << it->getID() << ' ' ;
    cout << endl;
    */
    //* update stations state *// check later 
    for ( int i = 0; i < numSta; i++ )
        updateStaState ( stations[i], 0, timeTFR, SIFS, 0);
    //* update next event *//
    int newEventTime = 0;
    int newEventDuration = 0;
    newEventTime = myClock + nextEvent.eventDuration; // the endtime of this event
    newEventDuration = timeRR + timeContending + SIFS;
    updateNextEvent( nextEvent, 2, newEventTime, newEventDuration );// ID = 2, means RR 
//    displayNextEvent(nextEvent);
}

/** event 2 **/
void RR( struct Event &nextEvent, vector<STA> &stations )
{
 //   std::cout << "Operation [2]: Resource Request" << std::endl;
    //* update variables *//
    //* generate traffic *// // no need to generate traffic
    //* only contend *//
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
        if ( stations[i].RRSucIndicate() )
        {
            for ( int j = i+1; j < numSta; j++ )
            {
                if ( contendSta[i] == contendSta[j] )
                    if ( stations[i].RRSucIndicate() )
                        if ( stations[j].RRSucIndicate() )
                        {
                            stations[j].setRRSucIndicate(false);// an pitfall here. 
                            flag = 1;
                        }
            }
            if ( flag )
            {
                stations[i].setRRSucIndicate(false); // when more than 2 stas with the same value, only two will be shutdown. 
                flag = 0;
            }
        }
    }
    //** test contend **//
    /*
    cout << "suc contend stations: " ;
    for ( int i = 0; i < numSta; i++ )
        if ( stations[i].RRSucIndicate() )
        {
            cout << stations[i].getID() << ' ';
        }
    cout << endl;
    */
    //* update stations state *// 
    for ( int i = 0; i < numSta; i++ )
        if ( stations[i].RRSucIndicate() )
            updateStaState ( stations[i], timeRR, 0, SIFS, 0);
    //* next event *//
    int newEventTime = 0;
    int newEventDuration = 0;
    newEventTime = myClock + nextEvent.eventDuration; // the endtime of this event
    newEventDuration = timeTF + SIFS;
    updateNextEvent( nextEvent, 3, newEventTime, newEventDuration );// ID = 2, means RR 
//    displayNextEvent(nextEvent);
}

/** event 3 **/
void triggerAllocation( struct Event &nextEvent, vector<STA> &stations )
{
//    std::cout << "Operation [3]: Trigger Allocation" << std::endl;
    //* RR procedure *//
    int ns = 0; // num of suc contending stations 
    vector<int> contendSucSta; // contain the STA ID  
    for ( int i = 0; i < numSta; i++ )
        if ( stations[i].RRSucIndicate() )
        {
            contendSucSta.push_back(i);
            ns++;
        }
    srand ( time(NULL));
    int index = 0;
    int Ns = ns;
    //** unselect extra stations **//
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
    }
    //** test RR **//
    /*
    cout << "suc RR stations: " ;
    for ( int i = 0; i < numSta; i++ )
        if ( stations[i].RRSucIndicate() )
        {
            cout << stations[i].getID() << ' ';
        }
    cout << endl;
    */
    //** set Rate and more bit **//
    double dataRate = 1.0;
    for ( int i = 0; i < numSta; i++ )
        if ( stations[i].RRSucIndicate() )
        {
            //*** set data rate ***//
            stations[i].setRate(double(nch)/double(Ns));
            dataRate = stations[i].getRate();
            //cout << "STA " << stations[i].getID() << " data rate : " << dataRate  << endl;
            //*** set more bit ***//
            if ( stations[i].getULQueue() > 1 )
                stations[i].setMoreBit(true);  // set more bit
        }

    //* update next event *//
    int newEventTime = 0;
    int newEventDuration = 0;
    newEventTime = myClock + nextEvent.eventDuration; // the endtime of this event
    newEventDuration = double(timePkt)/double(dataRate) + SIFS; // datarate
    updateNextEvent( nextEvent, 4, newEventTime, newEventDuration ); 
//    displayNextEvent(nextEvent);
}

/** event 4 **/
void ULTransmit( struct Event &nextEvent, vector<STA> &stations )
{
//    std::cout << "Operation [4]: UL Transmit" << std::endl;
    //* update data rate *//
    double dataRate = 0;
    for ( int i = 0; i < numSta; i++ )
        if ( stations[i].RRSucIndicate() ) // stasions who will up packet
        {
            //* update queue *//
            stations[i].popULQueue();
            if ( !stations[i].getULQueue() ) // all packets have been sent.
            {
                // reset more bit, i.e. false
                stations[i].setMoreBit(false);
                // reset RRSucIndicate
                stations[i].setRRSucIndicate(false);
            }
            // reset data rate
            stations[i].setRate(1.0);
        }

    //* update next event *//
    int newEventID = 5;
    for ( int i = 0; i < numSta; i++ )
        if ( stations[i].getMoreBit() )
            newEventID = 3;
    int newEventTime = 0;
    int newEventDuration = 0;
    newEventTime = myClock + nextEvent.eventDuration; // the endtime of this event
    // problem to determine the rate;
    newEventDuration = timeACK;
    updateNextEvent( nextEvent, newEventID, newEventTime, newEventDuration ); 
 //   displayNextEvent(nextEvent);
}

/** event 5 **/
void triggerAck( struct Event &nextEvent, vector<STA> &stations )
{
//    std::cout << "Operation [5]: trigger ACK" << std::endl;
    //* update next event *//
    int newEventTime = 0;
    int newEventDuration = 0;
    newEventTime = (myClock/TFPeriod+1) * TFPeriod  ; // the endtime of this event
    newEventDuration = timeTFR + SIFS;
    updateNextEvent( nextEvent, 1, newEventTime, newEventDuration ); 
 //   displayNextEvent(nextEvent);
}


/** initialize system  **/
void initialize( struct Event &nextEvent, vector<STA> &stations )
{
    //* set stations *//
    for (int i = 1; i < numSta+1; i++)
    {
        stations[i-1].setID(i); // set ID
        stations[i-1].setLocalLambda(globalLambda); // set lambda
    }
    updateNextEvent( nextEvent, 1, TFPeriod, timeTFR );
    cout << "*********** system initialized **************" << endl;
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
