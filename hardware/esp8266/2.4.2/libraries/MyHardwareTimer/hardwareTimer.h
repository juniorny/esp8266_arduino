#ifndef __HARDWARETIMER_H__
#define __HARDWARETIMER_H__

#include "Arduino.h"
#include "stdint.h"
#include "esp8266_peri.h"

class HardwareTimer {
public:
	HardwareTimer(uint8_t timer_id);
	
	void enable(uint8_t divider, uint8_t int_type, uint8_t reload);
	void ICACHE_RAM_ATTR disable();
	void ICACHE_RAM_ATTR write(uint32_t ticks);
	void ICACHE_RAM_ATTR isr_init();

	void attachInterrupt(timercallback userFunc) {
		user_cb = userFunc;
		if (id == 1)
			ETS_FRC1_INTR_ENABLE();
	}

    void detachInterrupt() {
		user_cb = 0;  
		if (id == 1)
		{
			TEIE &= ~TEIE1;		//edge int disable    
			ETS_FRC1_INTR_DISABLE();
		}
	};
	
	
	uint8_t id;
    volatile timercallback user_cb;
};

extern HardwareTimer Timer1;

#endif
