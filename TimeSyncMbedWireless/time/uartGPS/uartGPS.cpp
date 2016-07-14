/*
 * uartGPS.c
 *
 * Created: 16/05/2016 15:57:41
 *  Author: MORAND Baptiste
 */ 
#include "uartGPS.h"
void gpsUartTask(void){
		while(1){
			sendHMI("gps Uart task.");
			vTaskDelay(100000/portTICK_RATE_MS);
		}
}