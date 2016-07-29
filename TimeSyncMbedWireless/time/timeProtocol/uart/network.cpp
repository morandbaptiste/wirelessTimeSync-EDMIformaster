/*
 * uart.c
 *
 * Created: 18/05/2016 18:24:38
 *  Author: MORAND Baptiste
 */
#include "network.h"
/************************************************************************/
/* GLOBAL                                                               */
/************************************************************************/
#ifdef RADIO
cMxRadio radio(PB30,PC19,PC18,PB31,PB15,PA20,PB00);
#endif
volatile circularBuffer buffer;
extern portTickType saveTimeRCrash;
/************************************************************************/
/* GLOBAL FUNCTION                                                      */
/************************************************************************/
void ISRNetworkReception(uint8_t e){
	 uint16_t rx;
	saveTimeRCrash=xTaskGetTickCount();
	if(((e & TRX_IRQ_TRX_END) == TRX_IRQ_TRX_END)||((e & TRX_IRQ_RX_START) == TRX_IRQ_RX_START))
	{
		
		#ifdef RADIO
		uint8_t available=radio.available();
		 for(uint8_t i=0;i<available;i++){

			//Disable_global_interrupt();
			//pc.printf("mimi\r\n");
			//synchro semaphore with a task

			rx=radio.read();
			buffer.buffer[buffer.indice++]=rx;
			//pc.printf("rx:%d\r\n",rx);
			if(buffer.indice>=CIRCULAR_BUFFER_LENGTH){
				buffer.indice=0;
			}

			if(rx==HEADER){
				if(timeProt.synchroTime!=NULL){
					xSemaphoreGiveFromISR(timeProt.synchroTime,NULL);
				}
				timeProt.waitIdentifier=true;

				#ifdef MASTERMODE
					Clock save;
					save.second=timeProt.saveTime[0].second;
					save.halfmillis=timeProt.saveTime[0].halfmillis;
					save.sign=timeProt.saveTime[0].sign;
					readClock(&save);
					timeProt.saveTime[0].second=save.second;
					timeProt.saveTime[0].halfmillis=save.halfmillis;
					timeProt.saveTime[0].sign=save.sign;
		
				#else
					readClock(&timeProt.rx);
				#endif
				continue;
				//return;
			}
			
			#ifdef MASTERMODE
			if(timeProt.waitIdentifier==true){
				timeProt.waitIdentifier=false;
				timeProt.waitType=true;
				if(rx<MAX_SLAVE_CLOCK){
					timeProt.saveTime[rx].second=timeProt.saveTime[0].second;
					timeProt.saveTime[rx].halfmillis=timeProt.saveTime[0].halfmillis;
				}
				continue;
			}
			
	
			#else
			if(timeProt.waitIdentifier==true){
				timeProt.waitIdentifier=false;
				if(rx==0){
					timeProt.waitType=true;
				}
			}
			if(timeProt.waitType==true){
				if(rx==DELAYRESPONSE){
					timeProt.rxDelay.second=timeProt.rx.second;
					timeProt.rxDelay.halfmillis=timeProt.rx.halfmillis;
				}
				else{
					if(rx==SYNC){
						timeProt.rxSync.second=timeProt.rx.second;
						timeProt.rxSync.halfmillis=timeProt.rx.halfmillis;
					}
				}
		
			}
			
			#endif
		
			//Enable_global_interrupt();
		 }
		
		 #endif
		  radio.flush();
	 }
	 
	 
}

void ISRNetworkTransmission(void){
	//puts("t\r\n");
	
}

bool networkAvailable(void){


	if(buffer.currentIndice!=buffer.indice){
		//puts("network available");
		return true;	
	}
	else{
			
		return false;
	}

}
void networkFlush(void){
	//usart_disable_callback(&usart_instance, USART_CALLBACK_BUFFER_RECEIVED);//momo
	buffer.currentIndice=buffer.indice;
	//usart_enable_callback(&usart_instance, USART_CALLBACK_BUFFER_RECEIVED); //momo
}
void networkTx(const uint8_t send[],const uint8_t length){
	#ifdef RADIO
		uint8_t i;
		radio.beginTransmission();
		for(i=0;i<length;i++){
			
			radio.write(send[i]);
			
		}
		radio.endTransmission();
	#endif
}
uint8_t networkRead(void){
	uint8_t i =networkAvailable();
	if(networkAvailable()){
		uint8_t recu;
		recu=buffer.buffer[buffer.currentIndice];
		buffer.currentIndice++;
		if(buffer.currentIndice>=CIRCULAR_BUFFER_LENGTH){
			buffer.currentIndice=0;
		}
		return recu; 
	}
	else{
		return 0;
	}

	
}

void configurationNetwork(){
	hmi.printf("network config\r\n");
	buffer.currentIndice=0;
	buffer.indice=0;
	#ifdef RADIO
	radio.attachIrq(ISRNetworkReception);
    radio.begin(RADIO_CHANNEL);
	#endif
	

}
