/*
 * clock.h
 *
 * Created: 16/05/2016 17:44:17
 *  Author: MORAND Baptiste
 */ 


#ifndef CLOCK_H_
#define CLOCK_H_
/************************************************************************/
/* INCLUDE                                                              */
/************************************************************************/
#include <FreeRTOS.h>
#include <conf_timeProtocol.h>
/************************************************************************/
/* STRUCTURE                                                            */
/************************************************************************/
typedef struct structClock{
	 unsigned long int volatile second;
	unsigned long int volatile halfmillis;
	 bool volatile sign;
} Clock;
/************************************************************************/
/* GLOBAL                                                               */
/************************************************************************/
extern Clock timeManage;
/************************************************************************/
/* FUNCTION                                                             */
/************************************************************************/

/** 
@brief say if abs(clock1)>abs(clock2) (don't care about the sign)
@param clock1 clock1
@param clock2 clock2
@return true if abs(clock1)>abs(clock2) else false
*/
bool sup(Clock clock1,Clock clock2);
/**
@brief return abs(clock1)+abs(clock2) 
@param clock1 clock1
@param clock2 clock2
@return abs(clock1)+abs(clock2) 
*/
Clock sumClockPos(Clock clock1,Clock clock2 );
/**
@brief return clock1+clock2
@param clock1 clock1
@param clock2 clock2
@return  clock1+clock2
*/
Clock sumClock(Clock clock1,Clock clock2 );

/**
@brief return abs(clock1)-abs(clock2)
@param clock1 clock1
@param clock2 clock2
@return abs(clock1)-abs(clock2)
*/
Clock subClock(Clock clock1,Clock clock2 );
/**
@brief return abs(clock1)-abs(clock2) with  abs(clock1)> abs(clock2)
@param clock1 clock1 (>clock2)
@param clock2 clock2 (<clock1)
@return abs(clock1)-abs(clock2) abs(clock1)> abs(clock2)
*/
Clock subClockPos(Clock clock1,Clock clock2 );

#endif /* CLOCK_H_ */