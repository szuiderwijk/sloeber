#ifdef __IN_ECLIPSE__
//This is a automatic generated file
//Please do not modify this file
//If you touch this file your change will be overwritten during the next build
//This file has been generated on 2018-11-18 13:32:27

#include "Arduino.h"
#include "Arduino.h"
#include <LiquidCrystal.h>
#include "pitches.h"
#include "buzzer.h"
#include "EmulatedVal.h"

void setup() ;
void startTimer() ;
void handleInitState() ;
void timerBuzz(int timeStamp) ;
void checkTimerBuzz(long milliseconds) ;
String formatOutput(int hours, int minutes, int seconds, int tenmils) ;
void printVal(int val) ;
int readVal(int socket) ;
boolean checkWire(int val, const EmulatedVal* ev) ;
boolean checkWires() ;
void done() ;
void explode() ;
void dismantle() ;
void handleRunningState() ;
void loop() ;


#include "time_bomb.ino"

#endif
