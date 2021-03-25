#include "tick.h"

static OnTick callback=NULL;

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	_SysTick->CTRL;
	if (callback) callback();
}


/* tick_init
 *   setup systick timer
 */
int tick(uint32_t ms, OnTick cb)
{
	uint32_t tmp = ms*(sysclks.ahb_freq/1000);
	if (tmp <= (1<<24)) {	/*fck=84MHz ==> tick max = 200ms */
		_SysTick->LOAD = tmp-1;
		_SysTick->CTRL = 0x4;
	} else {				/*fck=84MHz ==> tick max = 1600ms */
		tmp = ms*(sysclks.ahb_freq/1000/8);
		if (tmp <= (1<<24)) {
			_SysTick->LOAD = tmp-1;
			_SysTick->CTRL = 0;
		} else {
			return -1;
		}
	}
	if (cb) {
		callback=cb;
		NVIC_SetPriority(SysTick_IRQn,SYSTICK_PRIORITY);
		_SysTick->CTRL |= 0x2;
	} else {
		_SysTick->VAL = 0;
		while ((_SysTick->CTRL & (1<<16))!=0) ;
	}
	return 0;
}

/* tick_start
 *   start the systick timer
 */
void tick_start()
{
	_SysTick->VAL = 0;
	_SysTick->CTRL |= 1;
}

/* tick_stop
 *   stop the systick timer
 */
void tick_stop()
{
	_SysTick->CTRL &= ~1;
}
