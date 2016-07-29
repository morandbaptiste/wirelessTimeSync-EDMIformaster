/*
 * internalClock.c
 *
 * Created: 13/05/2016 13:59:02
 *  Author: MORAND Baptiste
 */ 


#include "internalClock.h"
volatile unsigned int count_set_period=VALUE_TIMER;
volatile int valueCor;
volatile int timeCorrection;
volatile int timeCorr;
volatile bool stateLed=false;
DigitalOut led(LED1);
struct rtc_module rtc_instance;

uint32_t  readClock(Clock* timeClock){
	//rtc_count_disable(&rtc_instance);
	Disable_global_interrupt();
	uint32_t timeCounter=0;
	RTC->MODE0.CTRL.reg &=  0b1111111111111101;//disable timer
	while(RTC->MODE0.STATUS.bit.SYNCBUSY==1);//wait timer disable
	
	timeCounter=RTC->MODE0.COUNT.reg; 
	timeClock->second=timeManage.second;	
	timeClock->halfmillis=timeManage.halfmillis;	
	//timeCounter=0;
	

	//Enable_global_interrupt();
	timeClock->sign=true;	

	timeClock->halfmillis+=timeCounter;
	timeClock->second+=timeClock->halfmillis/RTC_FREQ;
	timeClock->halfmillis=timeClock->halfmillis%RTC_FREQ;
	RTC->MODE0.CTRL.reg |= RTC_MODE0_CTRL_ENABLE; //enable timer
	Enable_global_interrupt();
	return timeCounter;
}
void writeClock(Clock clk){
	//rtc_count_disable(&rtc_instance);
	//Disable_global_interrupt();

	RTC->MODE0.CTRL.reg &=  0b1111111111111101;//stop timer
	
	while(RTC->MODE0.STATUS.bit.SYNCBUSY==1);
	RTC->MODE0.COUNT.reg=clk.halfmillis%VALUE_TIMER;
	timeManage.second=clk.second;
	timeManage.halfmillis=(int)(clk.halfmillis/VALUE_TIMER)*VALUE_TIMER;
	timeManage.second+=timeManage.halfmillis/RTC_FREQ;
	timeManage.halfmillis=timeManage.halfmillis%RTC_FREQ;
	RTC->MODE0.CTRL.reg |= RTC_MODE0_CTRL_ENABLE; //enable timer

	

	
	
}
void isrInternalClok(void){
		RTC->MODE0.CTRL.reg &=  0b1111111111111101;//stop timer
			Disable_global_interrupt();
			//printf("+");
			static bool state;
			state=stateLed;
			

			timeManage.halfmillis+=VALUE_TIMER;

		
			if(timeManage.halfmillis<(RTC_FREQ/2)){//all 500ms
				stateLed=LOW;
			}
			else{
				stateLed=HIGH;
			}
			if(state!=stateLed){
				led = stateLed;
				//	if(synchroLed!=NULL){
				//		xSemaphoreGiveFromISR( synchroLed,NULL);
				//	}
			}

			
			timeManage.second+=timeManage.halfmillis/RTC_FREQ;
			timeManage.halfmillis=timeManage.halfmillis%RTC_FREQ;
			if((valueCor!=0)){
				if (timeCorr==0){
					timeCorr=timeCorrection;
					Clock off;
					off.second=(unsigned int )abs(valueCor)/RTC_FREQ;
					off.halfmillis=(unsigned int)abs(valueCor)%RTC_FREQ;
					if(valueCor<0){
						off.sign=false;
					}
					else{
						off.sign=true;
					}
					//change the time
					timeManage=sumClock(timeManage,off);
					//change the correction offset
					sumOffset=sumClock(sumOffset,off);//ok
				}
				else{
					timeCorr--;
				}	
				sumOffset.second+=sumOffset.halfmillis/RTC_FREQ;
				sumOffset.halfmillis=sumOffset.halfmillis%RTC_FREQ;
				timeManage.second+=timeManage.halfmillis/RTC_FREQ;
				timeManage.halfmillis=timeManage.halfmillis%RTC_FREQ;
			
			}
			

			
	Enable_global_interrupt();
	RTC->MODE0.CTRL.reg |= RTC_MODE0_CTRL_ENABLE; //enable timer
}
void RTC_Handler( void ){
	//pc.printf("ISR");
	RTC->MODE0.INTFLAG.bit.CMP0=1;		//disable the flag COMPARE
	RTC->MODE0.INTFLAG.bit.OVF=1;		//disable the flag COMPARE
	isrInternalClok();
}

void configureInternalClock(void){
	hmi.printf("internal clock config\r\n");
	//gENERIC CLOCK GENRERATOR
	GCLK->GENCTRL.reg=0x00010402;
	//Generic Clock Generator Division
	GCLK->GENDIV.reg=0x00000102;
	//Generic Clock Control
	GCLK->CLKCTRL.reg=0x4204;
	//peripheral RTC configuration
	
	RTC->MODE0.CTRL.bit.PRESCALER=0x0;	//prescaller at 0
	RTC->MODE0.CTRL.bit.MODE=0x0;			//mode 0 : 32 bit counter
	RTC->MODE0.CTRL.bit.SWRST=0;			//no software reset
	RTC->MODE0.CTRL.bit.MATCHCLR=1;			//automatic reset counter
	
	//event configuration
	RTC->MODE0.INTENSET.bit.OVF=1;		//enable the event overflow
	RTC->MODE0.INTENSET.bit.CMP0=1;		//enable the event compare
	RTC->MODE0.INTFLAG.bit.CMP0=1;		//disable the flag COMPARE
	RTC->MODE0.INTFLAG.bit.OVF=1;		//disable the flag COMPARE
	RTC->MODE0.COUNT.bit.COUNT=0;		//set the RTC value
	RTC->MODE0.COMP[0].reg = count_set_period;//set the value of the period

	RTC->MODE0.CTRL.reg |= RTC_MODE0_CTRL_ENABLE;//enable the timer

	NVIC_SetPriority (RTC_IRQn, (1<<__NVIC_PRIO_BITS) - 1);//configure interrupt
	NVIC_EnableIRQ(RTC_IRQn);//enable interrrupt
	
}