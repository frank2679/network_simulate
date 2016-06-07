#include <iostream>
#include "myClass.h"
#include "functionDeclaration.h"

using namespace std;

/* friend function */

void updateStaState( STA &sta, int TX, int RX, int Idle, int Doze )
{
    sta.timeTX += TX;
    sta.timeRX += RX;
    sta.timeIdle += Idle;
    sta.timeDoze += Doze;
}

void updateStaStatePS( STA &sta, int TX, int RX, int Idle, int Doze )
{
    sta.timePSTX += TX;
    sta.timePSRX += RX;
    sta.timePSIdle += Idle;
    sta.timePSDoze += Doze;
}

void generateArrival( vector<STA> &stations )
{
    vector<STA>::iterator it = stations.begin();
    for ( ; it != stations.end(); it++ )
    {
        (it->arrivalTime).clear();
        int randomDuration = myClock - it->lastTime; // 
        // random setting //
        std::exponential_distribution<double> interArrivalTime(it->localLambda);
        std::mt19937 rng( std::random_device{}() );

        int tempDuration = int(interArrivalTime(rng));
        int tempArrivalTime = 0;
        int tempPkt = 0;
        while(tempDuration < randomDuration)
        {
            ++(it->totalGeneratePkt); // used to validate the simulation 
            ++(it->totalSentPkt);
            ++(it->ULQueue);
            tempPkt++;
            tempArrivalTime = it->lastTime + tempDuration;
            (it->arrivalTime).push_back( tempArrivalTime );
            tempDuration += int(interArrivalTime(rng));
        }
        // display arrival
        /*
        if (tempPkt)
        {
            cout << "New " << tempPkt << " packets generated: " ;
            for ( const int &i: it->arrivalTime)
                cout << i << ' ';
            cout << endl;
            cout << "STA " << it->getID() << " queue size: "
                << it->ULQueue << endl;
        }else
        { 
            cout << "No packets generatd. " << endl;
        }
        */
        it->lastTime = myClock;
    }
}

void STA::validateArrival()
{
    std::cout << "totalSentPkt: " << totalSentPkt << std::endl;
    std::cout << "STA " << this->ID << " avg packet per us: " << double(totalSentPkt)/double(myClock) << std::endl;  
}

void STA::getArrivalTime()
{
    std::vector<int>::iterator it = arrivalTime.begin();
    std::cout << "arrivalTime from: " ; 
    for(; it != arrivalTime.end(); it++)
        std::cout << *it << '\n';
}

void STA::displayStaState() 
{
    cout << "Total sent Pkts: " << setw(8) << this->totalSentPkt << endl;
    cout << "            " << setw(8) << "non-PS" << setw(8) << "PS" << endl;
    cout << "TX time:    " << setw(8) << timeTX << ", "
        << setw(8) << timePSTX << endl;
    cout << "RX time:    " << setw(8) << timeRX << ", " 
        << setw(8) << timePSRX << endl;
    cout << "idle time:  " << setw(8) << timeIdle << ", " 
        << setw(8) << timePSIdle << endl;
    cout << "doze time:  " << setw(8) << timeDoze << ", "
        << setw(8) << timePSDoze << endl;
    cout << "total time: " << setw(8) << timeTX + timeRX + timeIdle + timeDoze 
        << ", " << setw(8) << timePSTX + timePSRX + timePSIdle + timePSDoze << endl;
}

void displayULQueue( vector<STA> &stations )
{
    vector<STA>::iterator it = stations.begin(); 
    cout << "stations' UL queue size: " ; 
    for (; it != stations.end(); it++)
        cout << it->ULQueue << ' ' ;
    cout << endl;
}

void displayRRSucIndicate( vector<STA> &stations)
{
    vector<STA>::iterator it = stations.begin(); 
    //++ test RR ++//
    cout << "suc RR STAs(ID): " ;
    for ( it = stations.begin(); it != stations.end(); it++ )
        if ( it->RRSucIndicate() )
        {
            cout << it->getID() << ' ';
        }
    cout << endl;
}

void displayMoreBit( vector<STA> &stations)
{
    vector<STA>::iterator it = stations.begin();
    cout << "Morebit set STAs(ID): ";
    for ( ; it != stations.end(); it++ )
        if ( it->moreBit )
            cout << it->getID() << ' ' ;
    cout << endl;
}

void STA::energyConsumption()
{
    EnergyConsumption = timeTX*powerTX + timeRX*powerRX + timeIdle*powerIdle + timeDoze*powerDoze;
    EnergyConsumptionPS = timePSTX*powerTX + timePSRX*powerRX + timePSIdle*powerIdle + timePSDoze*powerDoze;
}

void STA::computeEnergyEfficiency()
{
    double energyNoPS = this->getEnergyConsumption();
    double energyPS = this->getEnergyConsumptionPS();
    energyEfficiency = (energyNoPS - energyPS)/energyNoPS;
}
