#include "myClass.h"
/* system configuring */ // all parameters will be set the first time they are set. 
extern int myClock; 
extern double globalLambda;
extern int numSta;
extern int const TFPeriod;
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
extern int const timeMaxUL;
extern double const powerTX;
extern double const powerRX;
extern double const powerIdle;
extern double const powerDoze;
extern bool tooManyUL;

/* 5 operations */
struct Event apSendTriggerRandom( struct Event &, std::vector<STA> & );
struct Event RR( struct Event &, std::vector<STA> &);
struct Event triggerAllocation( struct Event &, std::vector<STA> &);
struct Event ULTransmit( struct Event &, std::vector<STA> &);
struct Event triggerAck( struct Event &, std::vector<STA> &);
/* others */
void displaySystemState( std::vector<STA> &);
void initialize( struct Event &, std::vector<STA> &); 
void updateEvent( struct Event &, int , int, int);
void updateEvent( struct Event &, struct Event & );
void displayNextEvent( const struct Event & );
void displayLastEvent( const struct Event & );
double estimate( std::vector<STA> & );
void calculateWorkTime( struct Event &, struct Event &, std::vector<STA> & );
double simulate();
