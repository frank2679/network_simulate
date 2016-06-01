#include <iostream>
#include "myClass.h"

void STA::generateArrival()
{
    arrivalTime.clear(); 
    int updateDuration = myClock - lastArrival; // 
    std::exponential_distribution<double> interArrivalTime(localLambda);
    std::mt19937 rng( std::random_device{}() );
    int tempDuration = int(interArrivalTime(rng));
    int tempArrivalTime = 0;
    while(tempDuration < updateDuration)
    {
        totalSentPkt++; 
        ULQueue++;
        tempArrivalTime = lastArrival + tempDuration;
        arrivalTime.push_back( tempArrivalTime );
        tempDuration += int(interArrivalTime(rng));
    }
    lastArrival = tempArrivalTime; // last arrival time, also the last element of vector arrivalTime. 
    // std::cout << std::fixed << std::setprecision(6); 
}

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

void STA::updateQueue()
{
}

void STA::validateArrival()
{
    std::cout << "totalSentPkt: " << totalSentPkt << std::endl;
    std::cout << "lastArrival:  " << lastArrival << std::endl;
    std::cout << "STA " << this->ID << " avg packet per us: " << double(totalSentPkt)/double(lastArrival) << std::endl;  
}

void STA::getArrivalTime()
{
    std::vector<int>::iterator it = arrivalTime.begin();
    std::cout << "arrivalTime from " << lastArrival << ": " ; 
    for(; it != arrivalTime.end(); it++)
        std::cout << *it << '\n';
}

void updateStaState( STA &sta, int TX, int RX, int Idle, int Doze )
{
    sta.timeTX += TX;
    sta.timeRX += RX;
    sta.timeIdle += Idle;
    sta.timeDoze += Doze;
}

void STA::displayStaState() 
{
    std::cout << "STA " << ID << " UL queue:" 
        << ULQueue << std::endl; 
    std::cout << "STA " << ID << " more bit:"
        << int(moreBit);
}
