/*

 * uart.h
 *
 * Created: 18/05/2016 18:24:55
 *  Author: MORAND Baptiste
 */ 


#ifndef UART_H_
#define UART_H_
/************************************************************************/
/* INCLUDE                                                              */
/************************************************************************/

#include <timeManager.h>
#include <usart.h>
#include <usart_interrupt.h>
#include <timeProtocol.h>
#include <MxRadio.h>

/************************************************************************/
/* MACRO                                                                */
/************************************************************************/
#define CIRCULAR_BUFFER_LENGTH 50
#define MAX_RX_BUFFER_LENGTH   12
#define RADIO_CHANNEL 12
#define RADIO

/************************************************************************/
/* STRUCTURE                                                            */
/************************************************************************/
typedef struct structCircularBuffer{
	volatile uint8_t buffer[CIRCULAR_BUFFER_LENGTH];
	volatile uint8_t indice;
	uint8_t currentIndice;
}circularBuffer;

/************************************************************************/
/* GLOBAL                                                               */
/************************************************************************/
extern  uint16_t rx;
extern volatile circularBuffer buffer;
extern volatile uint8_t rx_buffer[MAX_RX_BUFFER_LENGTH];
/************************************************************************/
/* FUNCTION                                                             */
/************************************************************************/
/** 
@brief when a unint_8 is comming
*/
void ISRNetworkReception(void);
/** 
@brief When you have send the transmission buffer
*/
void ISRNetworkTransmission(void);

/** 
@brief when you send a data via network
*/
void networkTx(const uint8_t send[],const uint8_t length);
/** 
@brief call to check if there is data to receive
*/
uint8_t networkRead(void);
/** 
@brief Use to configure the networl
*/
void configurationNetwork(void);
/** 
@brief say if the network have receive a data
@return  true if there are data not read else false
*/
bool networkAvailable(void);
/** 
@brief free the reception buffer
*/
void networkFlush(void);

#endif /* UART_H_ */