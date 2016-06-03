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
        it->lastTime = myClock;
    }
}

/*
   void STA::generateArrival()
   {
   arrivalTime.clear(); 
   int updateDuration = TFPeriod; // 
   std::exponential_distribution<double> interArrivalTime(localLambda);
   std::mt19937 rng( std::random_device{}() );
   int tempDuration = int(interArrivalTime(rng));
   int tempArrivalTime = 0;
   while(tempDuration < updateDuration)
   {
   totalSentPkt++; 
   ULQueue++;
   tempArrivalTime = lastTime + tempDuration;
   arrivalTime.push_back( tempArrivalTime );
   tempDuration += int(interArrivalTime(rng));
   }
   lastTime = (myClock/TFPeriod) * TFPeriod ;  
//std::cout << "last time: " << lastTime << std::endl;
}
*/

/*
   void STA::displayArrivalTime()
   {
   std::vector<int>::iterator it;
   if ( this->ULQueue > 0 )
   {
   std::cout <<"STA " << this->getID() << " arrival time : ";
   for(it = arrivalTime.begin(); it != arrivalTime.end(); it++ )
   std::cout << *it << ' ';
   std::cout << '\n';
   }else
   {
   std::cout << "STA " << this->getID() << " has no UL pkt."
   << std::endl;
   }
   }
   */

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
    std::cout << "STA " << ID << " UL queue:" 
        << ULQueue << std::endl; 
    std::cout << "STA " << ID << " more bit:"
        << int(moreBit);
}
