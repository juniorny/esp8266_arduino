#include "hardwareTimer.h"

void ICACHE_RAM_ATTR timer1_isr_handler(void *para);

HardwareTimer::HardwareTimer(uint8_t timer_id)
	:id(timer_id)
{

}

void HardwareTimer::enable(uint8_t divider, uint8_t int_type, uint8_t reload)
{
	if (id == 1)
	{
		T1C = (1 << TCTE) | ((divider & 3) << TCPD) | ((int_type & 1) << TCIT) | ((reload & 1) << TCAR);
		T1I = 0;
	}
}

void ICACHE_RAM_ATTR HardwareTimer::disable()
{
	if (id == 1)
	{
		T1C = 0;	
		T1I = 0;
	}
    	
}


void ICACHE_RAM_ATTR HardwareTimer::write(uint32_t ticks)
{
	if (id == 1)
	{
		T1L = ((ticks)& 0x7FFFFF);    
		if ((T1C & (1 << TCIT)) == 0) 
			TEIE |= TEIE1;//edge int enable
	}

}

void ICACHE_RAM_ATTR HardwareTimer::isr_init()
{    
	if (id == 1)
		ETS_FRC_TIMER1_INTR_ATTACH(timer1_isr_handler, this);
}

void ICACHE_RAM_ATTR timer1_isr_handler(void *para) 
{   
	HardwareTimer *t1 = (HardwareTimer *)para;
	if ((T1C & ((1 << TCAR) | (1 << TCIT))) == 0) 
		TEIE &= ~TEIE1;//edge int disable    
	T1I = 0;    
	if (t1->user_cb) 
	{		
		// to make ISR compatible to Arduino AVR model where interrupts are disabled		
		// we disable them before we call the client ISR		
		uint32_t savedPS = xt_rsil(15); // stop other interrupts		
		t1->user_cb();		 
		xt_wsr_ps(savedPS);	
	}

}

HardwareTimer Timer1(1);


