#include <iostream>
#include <vector>
#include "myClass.h"
#include "functionDeclaration.h"
using namespace std;

/* system parameters, global variables */
int myClock; // system clock
const double globalLambda = 0.001; // pkts/us
const int TFPeriod = 10000; // 10ms
const int numSta = 50;
const int contendWindow = 32;
const int endTime = 200000; // 1000000us = 1s 
const int timeTFR = 5; // 50us
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

    /* test generate() */
    /*
    myClock = 1000000;
    it = stations.begin();
    it->generateArrival();
    displaySystemState( stations ); // passing with reference
    stations[0].validateArrival();
    */

    vector<STA>::iterator it = stations.begin();
    struct Event nextEvent;
    initialize( nextEvent, stations );
    /* body */
    while ( myClock < endTime )
    {
        myClock = nextEvent.eventTime;
        cout << "myclock : " << myClock << endl;
        //* generate traffic *//
        generateArrival( stations );
        // display the queue of each stations
        for ( it = stations.begin(); it != stations.end(); it++ )
        {
            cout << "STA " << it->getID() << 
            " UL queue size: " <<  it->getULQueue() << endl;
        }
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
                cout << "!!!!!!!!!! Error operations. !!!!!!!!!!! " << endl; 
                break;
        }
    }
    /* system estimate */
    displaySystemState( stations ); // passing with reference
    estimate( stations );
    
    return 0;
}
