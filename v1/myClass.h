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
    friend void updateStaStatePS ( STA&, int, int, int, int );
    friend void generateArrival( std::vector<STA>& );
    friend void displayULQueue( std::vector<STA> &stations ); 
    friend void displayRRSucIndicate( std::vector<STA> &stations); 
    friend void displayMoreBit( std::vector<STA> &stations); 

    private:
        int ID;
        double localLambda = 0; // in-class constructor
        double Rate = 1.0;
        int ULQueue = 0;        // indicate whether to contend
        int lastTime = 0;
        std::vector<int> arrivalTime;
        bool contendThisTFP = false;// for compute working time
        bool RRSuc = false;// for compute working time
        bool contendSucIndicator = false; // for running the system 
        bool RRSucIndicator = false;  
        bool moreBit = false;
        int totalGeneratePkt = 0;
        int totalSentPkt = 0;   
        int timeTX = 0, timeRX = 0, timeIdle = 0, timeDoze = 0;
        int timePSTX = 0, timePSRX = 0, timePSIdle = 0, timePSDoze = 0;
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
        bool getContendThisTFP() { return contendThisTFP; }
        bool getRRSuc() { return RRSuc; }
        bool contendSucIndicate() { return contendSucIndicator; }
        bool RRSucIndicate() { return RRSucIndicator; }
        bool getMoreBit() { return moreBit; }
        int getTotalSentPkt() { return totalSentPkt; }

        //* setting *//
        void setID(int n){ ID = n;}
        void setLocalLambda(double lambda) {localLambda = lambda;}
        void setContendThisTFP(bool x) { contendThisTFP = x; } 
        void setRRSuc(bool x) { RRSuc = x; } 
        void setContendSucIndicate(bool x) { contendSucIndicator = x; }
        void setRRSucIndicate(bool x) { RRSucIndicator = x; }
        void setRate(double x) { Rate = x; }
        void setMoreBit(bool x) { moreBit = x; }

        //* other operations *//
        void popULQueue() { --ULQueue;}
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
