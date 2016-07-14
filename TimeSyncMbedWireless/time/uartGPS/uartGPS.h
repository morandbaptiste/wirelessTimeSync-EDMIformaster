/*
 * uartGPS.h
 *
 * Created: 16/05/2016 15:57:27
 *  Author: MORAND Baptiste
 */ 


#ifndef UARTGPS_H_
#define UARTGPS_H_
/************************************************************************/
/* INCLUDE                                                              */
/************************************************************************/
//#include <asf.h>
#include <FreeRTOS.h>
#include <queue.h>
#include <hmi.h>
/************************************************************************/
/* FUNCTION                                                             */
/************************************************************************/
/** 
@brief task to manage the data from by UART
*/
void gpsUartTask(void);



#endif /* UARTGPS_H_ */