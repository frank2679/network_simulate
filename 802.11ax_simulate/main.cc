#include <iostream>
#include <vector>
#include <fstream>
#include "myClass.h"
#include "functionDeclaration.h"
using namespace std;

/* system parameters, global variables */
int myClock; // system clock
int numSta = 1;
double globalLambda = 0.00006; // pkts/us
const int TFPeriod = 10000; // 10ms
const int contendWindow = 32;
const int endTime = 4100000; // 1000000us = 1s 
const int timeTFR = 50; // 50us
const int timeTF = 50;
const int timeContending = 0; // didn't consider it originally
const int timeRR = 50;
const int timePkt = 1000;
const int timeACK = 50;
const int SIFS = 0;
const int nch = 10;
const int powerTX = 1000;
const int powerRX = 600;
const int powerIdle = 300;
const int powerDoze = 150;
const int timeMaxUL = 10000;
bool tooManyUL = false;

int main()
{
    /* parameters */
    double energyEfficiency = 0.0;

    // file output
    ofstream myfile;
    myfile.open("./data/output_1.dat", ofstream::app);

    for (; globalLambda < 0.0003; globalLambda += 0.00003)
    {
        energyEfficiency = simulate();
        // fileout
        myfile << globalLambda << ' ' << energyEfficiency << endl; 
    }

    myfile.close();
    return 0;
}
double simulate()
{
    double energyEfficiency = 0.0;
    vector<STA> stations(numSta);
    STA ap(0);

    vector<STA>::iterator it = stations.begin();
    struct Event nextEvent;
    struct Event lastEvent;
    initialize( nextEvent, stations );
    /* body */
    while ( myClock < endTime )
    {
        myClock = nextEvent.eventTime;
        //* generate traffic *//
        generateArrival( stations );
        switch ( nextEvent.eventID )
        {
            case 1: 
                lastEvent = apSendTriggerRandom( nextEvent, stations );
                //displayLastEvent(lastEvent);
                //displayNextEvent(nextEvent);
                //cout << endl;
                break;
            case 2: 
                lastEvent = RR( nextEvent, stations );
                //displayLastEvent(lastEvent);
                //displayNextEvent(nextEvent);
                //cout << endl;
                break;
            case 3: 
                lastEvent = triggerAllocation( nextEvent, stations );
                //displayLastEvent(lastEvent);
                //displayNextEvent(nextEvent);
                //cout << endl;
                break;
            case 4: 
                lastEvent = ULTransmit( nextEvent, stations );
                //displayLastEvent(lastEvent);
                //displayNextEvent(nextEvent);
                //cout << endl;
                break;
            case 5: 
                lastEvent = triggerAck( nextEvent, stations );
                //displayLastEvent(lastEvent);
                //displayNextEvent(nextEvent);
                //cout << endl;
                break;
            default: 
                cout << "!!!!!!!!!! Error operations. !!!!!!!!!!! " << endl; 
                break;
        }
    }
    /* system estimate */
    cout << "******************** simulation end, Report here ********************" << endl;
    displaySystemState( stations ); 
    energyEfficiency = estimate( stations );
    if ( tooManyUL )
        cout << "Too many UL " <<  endl;
    return energyEfficiency;
}
