#include <iostream>
#include <vector>
#include "myClass.h"
#include "functionDeclaration.h"
using namespace std;

/* system parameters, global variables */
int myClock; // system clock
const double globalLambda = 0.00001; // pkts/us
const int TFPeriod = 10000; // 10ms
const int numSta = 5;
const int contendWindow = 32;
const int endTime = 10000000; // 1000000us = 1s 
const int timeTFR = 50; // 50us
const int timeTF = 50;
const int timeContending = 0; // didn't consider it originally
const int timeRR = 50;
const int timePkt = 1000;
const int timeACK = 50;
const int SIFS = 0;
const int nch = 10;

int main()
{
    /* parameters */
    /* stochastical variable */
    int energyConsumption_PS = 0;
    int energyConsumption_NPS = 0;
    double PSEfficiency = 0;
    int avgPacketDelay = 0;

    /* system state */
    
    /* build nodes */
    vector<STA> stations(numSta);
    STA ap(0);
    stations.push_back(ap); // last one is AP. 

    /* test generate() */
    /*
    vector<STA>::iterator it = stations.begin();
    myClock = 1000000;
    it = stations.begin();
    it->generateArrival();
    displaySystemState(); // passing with reference
    stations[0].validateArrival();
    stations[0].getArrivalTime();
    cout << "last arrival time: " << stations[0].getLastArrival() << endl;
    */

    struct Event nextEvent;
    initialize( nextEvent, stations );
    displayNextEvent( nextEvent ); 
    /* body */
    while ( myClock < endTime )
    {
        myClock = nextEvent.eventTime;
        //cout << "myclock : " << myClock << endl;
        switch ( nextEvent.eventID )
        {
            case 1: 
                apSendTriggerRandom( nextEvent, stations );
                break;
            case 2: 
                RR( nextEvent, stations );
                break;
            case 3: 
                triggerAllocation( nextEvent, stations );
                break;
            case 4: 
                ULTransmit( nextEvent, stations );
                break;
            case 5: 
                triggerAck( nextEvent, stations );
                break;
            default: 
                break;
        }
    }
    /* system estimate */
    displaySystemState( stations ); // passing with reference
    estimate( stations );
    
    return 0;
}
