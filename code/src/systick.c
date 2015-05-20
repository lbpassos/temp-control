/**
 * @file systick.c
 * @author Lénio Passos - 22164
 * @date 16 Mai 2015
 * @brief Temporização
 *
 */
#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif
#include <stdint.h>
#include "systick.h"

static const uint32_t MAX_TIMER_32bit = 0xFFFFFFFF;

static volatile uint32_t sysTicks;

/** @brief Interrupt systick handler
 *
 */
void SysTick_Handler(void)
{
	sysTicks++;
}

//delayTime em multiplos de 10 ms
inline void delay(uint32_t delayTime)
{
	volatile uint32_t currentValueSysTick;

	currentValueSysTick = sysTicks;
	while ((sysTicks - currentValueSysTick) < delayTime);
}

void initSysTick_Config()
{
	if( SysTick_Config( SystemCoreClock / 100) ) //cclk=100 [MHz]
	{
		while (1);
	}
}

uint8_t chrono_timeout(uint32_t initial, uint32_t delta){
	uint32_t t = sysTicks;

	if( (t-initial) >= delta ) return 1;
	else
		if( (t-initial)<0 ){
			t += (MAX_TIMER_32bit - initial) + 1;
			if( t >= delta )
				return 1;
		}
	return 0;
}

uint32_t chrono_start(){ return sysTicks; }
