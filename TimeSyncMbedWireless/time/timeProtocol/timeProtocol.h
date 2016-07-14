/*
 * timeProtocol.h
 *
 * Created: 16/05/2016 15:54:42
 *  Author: MORAND Baptiste
 */ 


#ifndef TIMEPROTOCOL_H_
#define TIMEPROTOCOL_H_
/************************************************************************/
/* INCLUDE                                                              */
/************************************************************************/
#include <FreeRTOS.h>
#include <hmi.h>
#include <conf_timeProtocol.h>
#include <network.h>//to change if it's an other type of network
#include <timeManager.h>
#include <string.h>
#include <internalClock.h>
#include <clp_api.h>
/************************************************************************/
/* MACRO                                                                */
/************************************************************************/
#define HEADER 0x55
#ifdef SLAVEMODE
	#define IDDEVICE SLAVEMODE
#else
	#define IDDEVICE 0
#endif

/************************************************************************/
/* ENUM                                                                 */
/************************************************************************/
typedef enum enumType
{
	SYNC = 1,
	DELAYREQUEST = 2,
	DELAYRESPONSE = 3,
	REP=4
	  
}Type;
/************************************************************************/
/* STRUCTURE                                                            */
/************************************************************************/

/** 
@brief structure of the data who is sending or receiving 
@param id id of te slave or master(0)
@param length length of the data
@param data : data
@param crc : crc 
*/
typedef struct structData{
	Type type;
	uint8_t id;
	uint8_t length;
	uint8_t data[30];//maximum of 30 data can be send
	uint8_t crc;
}Data;

typedef struct structCorrection{
	//Clock sumOffset;
	unsigned int nbCorrection;
	//volatile unsigned int timeSoftCor;
	volatile bool valueCor;
	Clock previoustimeOffset;
	Clock currentTimeOffsetSync;
	Clock sumTime;
	float moySoftCor[MOYCORLENGTH][2];
	uint8_t indiceMoySoftCor;
	uint8_t indiceFull;
	bool previousSignOffset;

}Correction;
/** 
@brief structure to mange the timeSynchronisation protocole
@param SaveTime table to save the time to acess to the timeRxof the id 2 write saveTime[1]
@param waitIdentifier
@param delay delay of transmition in halfmillisecond
@param syncroTime s�maphore of synchronisation
*/

typedef struct strucTimeProt{
	#ifdef MASTERMODE
	volatile Clock saveTime[MAX_SLAVE_CLOCK+1];
	#endif
	#ifdef SLAVEMODE
	Clock tx;
	Clock rxSync;
	Clock rxDelay;
	Clock rx;
	#endif
	bool waitIdentifier;
	bool waitType;
	Clock delay;
	xSemaphoreHandle synchroTime;
	Clock offset;
	Correction correction;
	
	
	
	
}TimeProt;



/************************************************************************/
/* GLOBAL                                                               */
/************************************************************************/
extern TimeProt timeProt;
/** 
@brief send char * to hmi 
*/
extern char messageHMI[200];
extern char messageHMICor[200];
extern char messageHMIDelay[200];
extern char messageHMIOffset[200];
extern Clock sumOffset;

/************************************************************************/
/* FUNCTION                                                             */
/************************************************************************/
/** 
@brief gove the pgcd value
@return the pgcd value
*/
int pgcd( int a ,int b);
/** 
@brief add 1 to the sum of the offset
*/
void addOffset(int valueCorrection);
/** 
@sub 1 to the sub offset
*/
void subOffset(int valueCorrection);
/** 
@brief reset the software correction parametter
*/
void resetSofftwareCorrection(void);
/** 
@brief use sofwareCorrection and hardCorrection
*/
void correction(void);
/** 
@brief use to correct the system when the hardware can't
*/
void sofwareCorrection(void);
/** 
@brief change the frequency to adapt to the master
@return true if a correction as been effected done else false
*/
bool hardwareCorrection(void);
/** 
@brief update the clock whith the offset and the delay
*/
void updateClock(void);
/** 
@brief task to manage the time
*/
void timeProtocolTask(void);
#ifdef MASTERMODE
/** 
@brief send a sync cmd
*/
void sync(void);
#endif
/** 
@brief use to manage the data from uart
*/
void receiver(void);
/** 
@brief use to send the data to the network
@param type : type of the cmd
@param length: length of the data
@param data : data to send
*/
void send(const Type type,const uint8_t length,const uint8_t* data);
/**
@brief calcul the CRC value with your id
@param type : type of the cmd
@param length: length of the data
@param data : data to send
@return crc value
*/
uint8_t calculCRC(const Type type,const uint8_t length,const uint8_t* data);
/** 
@brief calcul the CRC value with a defined id
@param type : type of the cmd
@param length: length of the data
@param data : data to send
@param id : id of the payload 
@return crc value
*/
uint8_t calculCRCID(const Type type,const uint8_t length,const uint8_t* data,const uint8_t id);
/**
@brief use to manage the data from uart
*/
void sender(void);
/** 
@brief send a delay response to the id 
@param id of the receiver of the delay response
*/
void delayResponse(uint8_t id);
/** 
@brief send a delay request via network
*/
void delayRequest(void);

void printfClock(Clock clock);



#endif /* TIMEPROTOCOL_H_ */