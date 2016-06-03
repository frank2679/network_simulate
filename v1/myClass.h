// system time unit: micro sec
extern int myClock;
#ifndef _MY_CLASS_H
#define _MY_CLASS_H
#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <iomanip>
class STA 
{
    friend void updateStaState ( STA&, int, int, int, int );
    friend void generateArrival( std::vector<STA>& );
    private:
        int ID;
        double localLambda = 0; // in-class constructor
        double Rate = 1;
        int ULQueue = 0;        // indicate whether to contend
        int lastTime = 0;
        std::vector<int> arrivalTime;
        bool RRSucIndicator = false; // in-class constructor
        bool moreBit = false;
        int totalGeneratePkt = 0;
        int totalSentPkt = 0;   
        int timeTX = 0, timeRX = 0, timeIdle = 0, timeDoze = 0;
    public:
        //* constructor *// 
        STA(){} // constructor
        STA(int n):ID(n){} // constructor
        STA(int n, double lambda):ID(n), localLambda(lambda){}  

        //* some variable uesd to help system running *//

        //* fetch property *// 
        int getID(){return ID;}
        double getLocalLambda() { return localLambda;}
        double getRate() { return Rate; }
        void getArrivalTime();
        int getULQueue() { return ULQueue; }
        bool RRSucIndicate() { return RRSucIndicator; }
        bool getMoreBit() { return moreBit; }
        int getTotalSentPkt() { return totalSentPkt; }

        //* setting *//
        void setID(int n){ ID = n;}
        void setLocalLambda(double lambda){localLambda = lambda;}
        void setRRSucIndicate(bool x) { RRSucIndicator = x; }
        void setRate(double x) { Rate = x; }
        void setMoreBit(bool x) { moreBit = x; }

        //* other operations *//
        void popULQueue() { ULQueue--;}
        void generateArrival(); // as a part of sys update 
        //void displayArrivalTime();
        void validateArrival();
        void displayStaState();
};

struct Event
{
    int eventID = 0;
    int eventTime = 0;
    int eventDuration = 0; 
};
#endif
