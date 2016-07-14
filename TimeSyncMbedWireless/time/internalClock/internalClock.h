/*
 * internalClock.h
 *
 * Created: 13/05/2016 13:59:35
 *  Author: MORAND Baptiste
 */ 


#ifndef INTERNALCLOCK_H_
#define INTERNALCLOCK_H_
/************************************************************************/
/*INCLUDE                                                               */
/************************************************************************/
#include <FreeRTOS.h>
#include <semphr.h>
//#include <timeManager.h>
#include <timeProtocol.h>
#include <rtc_count_interrupt.h>
#include <rtc_count.h>
#include <rtc_tamper.h>
#include "mbed.h"

/************************************************************************/
/* MACRO                                                                */
/************************************************************************/
#define FEATURE_RTC_CONTINUOUSLY_UPDATED
#define OSC32K	0x1
#define GCLKGEN2 0x02
#define GCLK_RTC 0x04

/************************************************************************/
/* GLOBAL                                                               */
/************************************************************************/

extern volatile unsigned int count_set_period;
extern volatile int timeCorrection;
extern volatile int timeCorr;
extern volatile int valueCor;
extern struct rtc_module rtc_instance;
/************************************************************************/
/* function                                                             */
/************************************************************************/
/** 
@brief read  the current clock
@return the Current Clock
*/
uint32_t readClock(Clock* timeClock);
/** 
@brief write the current Clock
@param clk : the clock to set
*/
void writeClock(Clock clk);
/** 
@brief configure rtc for making an internal clock
*/
void configureInternalClock(void);
/** 
@brief call when the ISR is comming
*/
void isrInternalClok(void );
#endif /* INTERNALCLOCK_H_ */