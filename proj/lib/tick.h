#ifndef _TICK_H_
#define _TICK_H_

#ifdef __cplusplus
extern "C" {
#endif 

#include "include/board.h"

typedef void (*OnTick)(void);

/* tick_init
 *   setup systick timer
 */
int tick(uint32_t ms, OnTick cb);

/* tick_start
 *   start the systick timer
 */
void tick_start();

/* tick_stop
 *   stop the systick timer
 */
void tick_stop();

#ifdef __cplusplus
}
#endif

#endif
