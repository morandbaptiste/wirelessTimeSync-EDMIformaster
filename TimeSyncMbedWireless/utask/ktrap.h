/*
 * ktrap.h
 *
 * Created: 13/05/2016 16:36:05
 *  Author: MORAND Baptiste
 */ 


#ifndef KTRAP_H_
#define KTRAP_H_
/************************************************************************/
/* INCLUDE                                                              */
/************************************************************************/
#include <FreeRTOS.h>
#include <task.h>
#include <hmi.h>


/************************************************************************/
/* FUNCTION                                                             */
/************************************************************************/
/**
@fn vApplicationStackOverflowHook
@brief kernel hook here if stack overflow
*/
//void vApplicationStackOverflowHook( xTaskHandle xTask, signed char *pcTaskName );

/**
@fn vApplicationMallocFailedHook
@brief kernel hook here if malloc allocation failed detection
*/
//void vApplicationMallocFailedHook(void);


#endif /* KTRAP_H_ */