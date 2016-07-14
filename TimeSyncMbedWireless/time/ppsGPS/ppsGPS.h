/*
 * ppsGPS.h
 *
 * Created: 13/05/2016 11:22:31
 *  Author: MORAND Baptiste
 */ 


#ifndef PPSGPS_H_
#define PPSGPS_H_
/************************************************************************/
/* INCLUDE                                                              */
/************************************************************************/

#include <timeManager.h>
#include <hmi.h>
#include <string.h>
#include <extint.h>
#include <wdt_config.h>
#include "InterruptManager.h"
#include "mbed.h"
#include <timeProtocol.h>
/************************************************************************/
/* FUNCTION                                                             */
/************************************************************************/
/**
 @brief : configure the PPS signal
*/
void configurationPPS(void);
/**
 @brief function call when EXT_ISR is arrived
 */
void ppsISR(void);



#endif /* PPSGPS_H_ */