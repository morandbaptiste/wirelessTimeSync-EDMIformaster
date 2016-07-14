#include "mbed.h"


 




#include <utask.h>
#include <hmi.h>
#include <ktrap.h>
#include <internalClock.h>
#include <ppsGPS.h>
#include <network.h>
#include <wdt_config.h>

int main (void ){
	 system_interrupt_disable_global();
    //configure HMI
    configureHMI();
    //RTC configure
   configureInternalClock();
    //configure external intrerrupt for pps
    configurationPPS();
    //configure the network of the time SYNCHRONISATION
	
    configurationNetwork();
	wdt_config();
    // Initialize the demo..
    kernelConfig();
    //demask global interrupt
    system_interrupt_enable_global();
    hmi.printf("\r\n\r\n**********************************************************************\r\n");
    hmi.printf("tick time: %lu ms\r\n",portTICK_RATE_MS);   
    #ifdef MASTERMODE
        hmi.printf("Master clock\r\n");
    #else
        hmi.printf("Slave Clock \r\n");
    #endif
    hmi.printf("end config application begin\r\n");
    hmi.printf("\r\n**********************************************************************\r\n\r\n");
    //start the scheduler
    vTaskStartScheduler();
    while(1){//trap
		hmi.printf("trap!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\r\n");
    }

}
