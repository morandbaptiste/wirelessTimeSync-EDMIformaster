/*
 * utask.c
 *
 * Created: 13/05/2016 16:34:52
 *  Author: MORAND Baptiste
 */ 
#include "utask.h"

void kernelConfig(void){

	hmi.printf("kernel config\r\n");
	// tasks creations
	xTaskCreate((pdTASK_CODE)HMITask,(const signed char*)"HMI",configMINIMAL_STACK_SIZE,NULL, tskIDLE_PRIORITY +1, NULL);
	xTaskCreate((pdTASK_CODE)timeProtocolTask,(const signed char*)"time",configMINIMAL_STACK_SIZE+600,NULL, tskIDLE_PRIORITY +3, NULL);
	//xTaskCreate((pdTASK_CODE)ledBlinkTask,(const signed char*)"led",configMINIMAL_STACK_SIZE,NULL, tskIDLE_PRIORITY +4, NULL);
	//xTaskCreate((pdTASK_CODE)gpsUartTask,(const signed char*)"gps",configMINIMAL_STACK_SIZE,NULL, tskIDLE_PRIORITY +2, NULL);
}
