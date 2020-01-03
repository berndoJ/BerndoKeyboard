#include "main.h"
#include "sys_clock.h"

int main(void)
{
    HAL_Init();

    SYS_Clock_Init();

    while (1)
    {
        
    }

    return 0;
}