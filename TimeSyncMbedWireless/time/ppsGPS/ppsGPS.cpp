/*
 * ppsGPS.c
 *
 * Created: 13/05/2016 11:23:06
 *  Author: MORAND Baptiste
 */ 
#include "ppsGPS.h"
//InterruptIn pps(PA28);

char messageHMIError[100];
/** Callback function for the EXTINT driver, called when an external interrupt
 *  detection occurs.
 */

void ppsISR(void)
{	
	wdt_clear();
	Disable_global_interrupt();
	Clock timeCopy={0,0,true};
	//Disable_global_interrupt();
	Clock timeMaster={0,0,true};
	readClock(&timeCopy);
	//Enable_global_interrupt();
	timeMaster.second=timeCopy.second;
	timeMaster.halfmillis=timeCopy.halfmillis;
	if(timeMaster.halfmillis>=RTC_FREQ/2){//for the accurate 500ms
		timeMaster.second++;
	}
	timeMaster.halfmillis=0;
	/*if((timeMaster.halfmillis>RTC_FREQ)||(timeMaster.halfmillis<0)){
			printf("timeMasterpps");
			printfClock(timeMaster);
	}
	if((timeCopy.halfmillis>RTC_FREQ)||(timeCopy.halfmillis<0)){
		printf("timeCopypps:'");
		printfClock(timeCopy);
	}
	*/
	timeProt.offset=subClock(timeMaster,timeCopy);
	//timeProt.offset.second=0;
	#ifdef MASTERMODE
		updateClock();
	#else
		if(timeProt.offset.sign==true){
			sprintf(messageHMIError,"		error pps: %lus %u	t:%lus",timeProt.offset.second,(unsigned int)(timeProt.offset.halfmillis/2),timeMaster.second);
		}
		else{
			sprintf(messageHMIError,"		error pps: -%lus %u	t:%lus",timeProt.offset.second,(unsigned int)(timeProt.offset.halfmillis/2),timeMaster.second);
		}
		sendHMI(messageHMIError);
	#endif
		Enable_global_interrupt();
}

void configurationPPS(void){
		hmi.printf("confif pps\r\n");
	//	pps.rise(ppsISR);
		struct extint_chan_conf eint_chan_conf;
		extint_chan_get_config_defaults(&eint_chan_conf);
		// put the button as EXTINT
		eint_chan_conf.gpio_pin           = PIN_PA28A_EIC_EXTINT8;
		eint_chan_conf.gpio_pin_mux       = MUX_PA28A_EIC_EXTINT8;
		eint_chan_conf.detection_criteria = EXTINT_DETECT_RISING;
		eint_chan_conf.filter_input_signal = false;
		
		extint_chan_set_config(8, &eint_chan_conf);
		//configuration callback
		extint_register_callback(&ppsISR,8,EXTINT_CALLBACK_TYPE_DETECT);
		//InterruptManager::get()->add_handler(ppsISR, EIC_IRQn);
		//ppsIsrr.attach(&ppsISR);
		//activation callback
		extint_chan_enable_callback(8,EXTINT_CALLBACK_TYPE_DETECT);		
		
		
}