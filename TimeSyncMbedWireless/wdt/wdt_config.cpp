/*
 * wdt_config.c
 *
 * Created: 08/07/2016 10:52:22
 *  Author: Time Syn Project
 */ 
#include "wdt_config.h"
uint8_t clearRCrash;
void wdt_config(void){
	//GCLK->CLKCTRL.reg=0x4204;
	/* Create a new configuration structure for the Watchdog settings and fill
	 * with the default module settings. */
	//! [setup_1]
	struct wdt_conf config_wdt;
	//! [setup_1]
	//! [setup_2]
	wdt_get_config_defaults(&config_wdt);
	//! [setup_2]

	/* Set the Watchdog configuration settings */
	//! [setup_3]
	config_wdt.always_on            = false;
#if !((SAML21) || (SAMC21) || (SAML22))
	config_wdt.clock_source         = GCLK_GENERATOR_2;
#endif
	config_wdt.timeout_period       = WDT_PERIOD_16384CLK;
	//! [setup_3]

	/* Initialize and enable the Watchdog with the user settings */
	//! [setup_4]
	
	//wdt_set_config(&config_wdt);
	
	//! [setup_4]	
	//hmi.printf("config wdt\r\n");
	
	
	
}
void wdt_clear(void){
	wdt_reset_count();
	
}