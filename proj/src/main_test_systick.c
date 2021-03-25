#include "include/board.h"
#include "lib/tick.h"
#include "lib/io.h"

void led(int on)
{
	if (on) {
		io_set(_GPIOA, 1<<5);
	} else {
		io_clear(_GPIOA, 1<<5);
	}
}

/* tick timer callback */
void on_tick_cb()
{
	static uint32_t state=0;
	state=!state;
	led(state);
}

int main()
{
	// Configure pin PA5 to control LD2
	io_configure(_GPIOA, PIN_5, PIN_MODE_OUTPUT, NULL);

	led(0);
	
	/* setup tick event (time in ms) */
	tick(1000, on_tick_cb);
	tick_start();
	
	while (1) {
	}
	
	return 0;
}
