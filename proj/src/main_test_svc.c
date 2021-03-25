#include "include/board.h"

/******************************************************************
 * Handler mode code
 ******************************************************************/
/* sys_add
 *   test function
 */
int sys_add(int a, int b)
{
    return a+b;
}

int32_t svc_dispatch(uint32_t n, uint32_t args[])
{
    switch(n) {
      case 0:
          return sys_add((int)args[0], (int)args[1]);
	}
	return -1;
}

/******************************************************************
 * Thread mode code
 ******************************************************************/
/* svc interface */
int test_add(int a, int b)
{
    int val;
    __ASM volatile ("svc 0\n\tmov %0, r0" : "=r" (val));
    return val;
}

int main()
{
	int r = test_add(7,4);
	
	return 0;
}
