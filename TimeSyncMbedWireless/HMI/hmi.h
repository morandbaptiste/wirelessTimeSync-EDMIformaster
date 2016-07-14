/*
 * uartHMI.h
 *
 * Created: 12/05/2016 22:12:20
 *  Author: MORAND Baptiste
 */


#ifndef UARTHMI_H_
#define UARTHMI_H_
/************************************************************************/
/* include directories                                                  */
/************************************************************************/
#include <mbed.h>
#include <FreeRTOS.h>
#include <task.h>
//#include <board.h>
#include <queue.h>
#include <timeManager.h>
#include <usart.h>
#include <usart_interrupt.h>
#include <timeProtocol.h>
/************************************************************************/
/* MACRO                                                                */
/************************************************************************/
#define LENGTH_QUEUE 50

/************************************************************************/
/* GLOBAL VAR                                                           */
/************************************************************************/

extern xQueueHandle uartQueue;
extern Serial hmi;
/************************************************************************/
/* FUNCTION                                                             */
/************************************************************************/
/**
 @brief  configure peripheral UART from the debugger, debugger is connected to c stdio function
*/
void configureHMI(void);
/**
 @brief	Task of HMI, use to communicate via UART to EDBG
*/
void HMITask(void);
/**
@brief add the char* to the queue of the uartQueue.
@param send : char* add in the uartQueue*
@return true if the char is add to the queue
*/
bool sendHMI(const char send[]);




#endif /* UARTHMI_H_ */