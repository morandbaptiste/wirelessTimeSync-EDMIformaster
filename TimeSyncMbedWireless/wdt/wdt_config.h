/*
 * wdt_config.h
 *
 * Created: 08/07/2016 10:52:41
 *  Author: Time Syn Project
 */ 


#ifndef WDT_CONFIG_H_
#define WDT_CONFIG_H_

#include <wdt.h>
//#include <hmi.h>
//#include <mbed.h>

extern uint8_t clearRCrash;

void wdt_config(void);
#ifdef __cplusplus
extern "C" {
#endif
	 void wdt_clear(void);
#ifdef __cplusplus
}
#endif

#endif /* WDT_CONFIG_H_ */