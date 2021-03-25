/****************************************************************
 * GPIO TEST
 *
 * NUCLEO BOARD
 *
 * USR BTN : connected to pin PC13
 * USR LED : connected to pin PA5
 ****************************************************************/
#define MAIN2

#ifdef MAIN1
#include <stdlib.h>
#include "include/board.h"

// GPIO port mode register definitions
#define PIN_MODE_INPUT			0
#define PIN_MODE_OUTPUT			1
#define	PIN_MODE_ALTFUNC		2
#define	PIN_MODE_ANALOG			3

#define SET_PIN_MODE(_p,pin,mode) ((_p)->MODER = ((_p)->MODER & (~(3<<((pin)<<1)))) | ((mode)<<((pin)<<1)))

// GPIO port type register definitions
#define	PIN_OUTPUT_PUSHPULL		0
#define PIN_OUTPUT_OPENDRAIN	1

#define	SET_PIN_OUTPUT(_p,pin,type)	((_p)->OTYPER = ((_p)->OTYPER & (~(1<<(pin)))) | ((type)<<(pin)))

// GPIO port output speed register
// LOW    ~ 4MHz max
// MEDIUM ~ 25MHz max
// FAST   ~ 50MHz max
// HIGH   ~ 100MHz max
#define	PIN_OUTPUT_SPEED_LOW	0
#define	PIN_OUTPUT_SPEED_MEDIUM 1
#define	PIN_OUTPUT_SPEED_FAST	2
#define	PIN_OUTPUT_SPEED_HIGH	3

#define SET_PIN_OUTPUT_SPEED(_p,pin,speed) ((_p)->OSPEEDR = ((_p)->OSPEEDR & (~(3<<((pin)<<1)))) | ((speed)<<((pin)<<1)))

// GPIO port pull up/pull down register
#define PIN_RESISTOR_NO			0
#define PIN_RESISTOR_PULLUP		1
#define PIN_RESISTOR_PULLDOWN	2

#define SET_PIN_RESISTOR(_p,pin,resistor) ((_p)->PUPDR = ((_p)->PUPDR & (~(3<<((pin)<<1)))) | ((resistor)<<((pin)<<1)))

// LD2 control (PC13)
#define LD2_SET	(_GPIOA->BSRR = 1<<5)
#define LD2_CLR (_GPIOA->BSRR = 1<<(16+5))

// BTN
#define BTN		((~(_GPIOC)->IDR >>13) & 1)

int main()
{
	// Enable clock for GPIOA to be able to use it
	_RCC->AHB1ENR |=1;
	
	// Configure pin PA5 to control LD2
	SET_PIN_MODE(_GPIOA,5,PIN_MODE_OUTPUT);
	SET_PIN_OUTPUT(_GPIOA,5,PIN_OUTPUT_PUSHPULL);
	SET_PIN_OUTPUT_SPEED(_GPIOA,5,PIN_OUTPUT_SPEED_MEDIUM);
	SET_PIN_RESISTOR(_GPIOA,5,PIN_RESISTOR_NO);
	
	// Test the LD2 led
	LD2_SET;
	LD2_CLR;
	LD2_SET;
	LD2_CLR;
	
	// Enable clock for GPIOC to be able to use it
	_RCC->AHB1ENR |=1<<2;
	
	// Configure pin PC13 in input
	SET_PIN_MODE(_GPIOC,13,PIN_MODE_INPUT);
	SET_PIN_RESISTOR(_GPIOC,13,PIN_RESISTOR_NO);
	
	while (1) {
		if (BTN) {
			LD2_SET;
		} else {
			LD2_CLR;
		}
	}
	
	return 0;
}
#endif

#ifdef MAIN2
#include "include/board.h"
#include "lib/io.h"

volatile int state=0;

void led(int on)
{
	if (on) {
		io_set(_GPIOA, 1<<5);
	} else {
		io_clear(_GPIOA, 1<<5);
	}
}

/* interrupt callback */
void pc13_cb()
{
	state=!state;
	led(state);
}

int main()
{
	// Configure pin PA5 to control LD2
	io_configure(_GPIOA, PIN_5, PIN_MODE_OUTPUT, NULL);
	
	// Configure pin PC13 to trigger an interrupt
	io_configure(_GPIOC, PIN_13, PIN_MODE_INPUT|PIN_OPT_IRQ_EDGE_RISE, pc13_cb);
	
	led(state);
	
	while (1) {
		/* wait for an interrupt */
	}
	
}

#endif
