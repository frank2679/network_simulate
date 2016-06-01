#include "myClass.h"
/* system configuring */ // all parameters will be set the first time they are set. 
extern int myClock; 
extern double const globalLambda;
extern int const TFPeriod;
extern int const numSta;
extern int const contendWindow;
extern int const endTime;
extern int const timeTFR;
extern int const timeTF; 
extern int const timeContending;
extern int const timeRR;
extern int const timePkt; // given unit data rate
extern int const timeACK;
extern int const SIFS;
extern int const nch;

/* 5 operations */
void apSendTriggerRandom( struct Event &, std::vector<STA> & );
void RR( struct Event &, std::vector<STA> &);
void triggerAllocation( struct Event &, std::vector<STA> &);
void ULTransmit( struct Event &, std::vector<STA> &);
void triggerAck( struct Event &, std::vector<STA> &);
/* others */
void displaySystemState( std::vector<STA> &);
void initialize( struct Event &, std::vector<STA> &); 
void updateNextEvent( struct Event &, int , int, int);
void displayNextEvent( const struct Event &);
void estimate( std::vector<STA> & );
