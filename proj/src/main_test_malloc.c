/******************************************************************
 * Memory allocator Test
 ******************************************************************/

#include "stdlib.h"

void *mems[10];

int main(void)
{
    mems[0] = malloc(11);
    mems[1] = malloc(22);
    mems[2] = malloc(33);
    mems[3] = malloc(65);
    mems[4] = malloc(55);
    mems[5] = malloc(72);
    mems[6] = malloc(0);
    
    free(mems[0]); mems[0]=NULL;
    free(mems[1]); mems[1]=NULL;
    free(mems[3]); mems[3]=NULL;
    free(mems[2]); mems[2]=NULL;
    free(mems[5]); mems[5]=NULL;

    return(0);
}
