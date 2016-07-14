/*
 * iddle.c
 *
 * Created: 08/07/2016 11:28:31
 *  Author: Time Syn Project
 */ 
#include <wdt_config.h>
#include <FreeRTOS.h>
#include <conf_timeProtocol.h>



portTickType saveTimeRCrash=0;
void vApplicationIdleHook( void ){
	
		if(xTaskGetTickCount()-saveTimeRCrash>(TIME_CRASH_RX/portTICK_RATE_MS)){
			saveTimeRCrash=xTaskGetTickCount();
			if(clearRCrash==true){
				NVIC_SystemReset();
			}
			else{
				clearRCrash=true;
			}
		}
		
			wdt_clear();
}