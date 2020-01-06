#include "sys_ws2812.h"
#include "error.h"
#include <stdint.h>

NP32_Instance_t *hw_ws2812_handle;
SYS_InitStatus_t hw_ws2812_init_status = SYS_INIT_STATUS_NOT_INITIALISED;

static NP32_Instance_t _hw_ws2812_handle = {0};

static int8_t _NP32_StartDMA_Call(uint16_t *buf, uint16_t len);

void SYS_WS2812_Init(void)
{
    hw_ws2812_handle = &_hw_ws2812_handle;

    hw_ws2812_handle->LED_Count = 11U;
    hw_ws2812_handle->LED_Disable_Flag = 0;
    hw_ws2812_handle->StartDMA_Call = _NP32_StartDMA_Call;

    if (NP32_Init(hw_ws2812_handle) != 0)
        SYS_ThrowError(SYSERR_FATAL_ERROR, __LINE__, __FILE__, "NP32 INIT ERR");

    hw_ws2812_init_status = SYS_INIT_STATUS_INITIALISED;
}

int8_t _NP32_StartDMA_Call(uint16_t *buf, uint16_t len)
{
    return 0;
}