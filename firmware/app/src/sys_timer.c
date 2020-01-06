#include "sys_timer.h"
#include "error.h"
#include "neopixel32.h"

TIM_HandleTypeDef *hw_timer3_handle;
DMA_HandleTypeDef *hw_timer3_dma_handle;
SYS_InitStatus_t hw_timer3_init_status = SYS_INIT_STATUS_NOT_INITIALISED;

static TIM_HandleTypeDef _hw_timer3_handle;
static DMA_HandleTypeDef _hw_timer3_dma_handle;

static void _SYS_Timer3_PWM_MspInit_Callback(TIM_HandleTypeDef *handle);
static void _SYS_Timer3_PWM_MspDeInit_Callback(TIM_HandleTypeDef *handle);

void SYS_Timer_Init(void)
{
    TIM_MasterConfigTypeDef master_cfg = {0};
    TIM_OC_InitTypeDef oc_cfg = {0};
    GPIO_InitTypeDef gpio = {0};

    // Init Timer 3.
    hw_timer3_handle = &_hw_timer3_handle;
    hw_timer3_dma_handle = &_hw_timer3_dma_handle;

    hw_timer3_handle->Instance = TIM3;
    hw_timer3_handle->Init.Prescaler = 0;
    hw_timer3_handle->Init.CounterMode = TIM_COUNTERMODE_UP;
    hw_timer3_handle->Init.Period = NP32_WS2812_TIM_PERIOD;
    hw_timer3_handle->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    hw_timer3_handle->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    hw_timer3_handle->PWM_MspInitCallback = _SYS_Timer3_PWM_MspInit_Callback;
    hw_timer3_handle->PWM_MspDeInitCallback = _SYS_Timer3_PWM_MspDeInit_Callback;

    if (HAL_TIM_PWM_Init(hw_timer3_handle) != HAL_OK)
        SYS_ThrowError(SYSERR_FATAL_ERROR, __LINE__, __FILE__, "TIM INIT ERR");

    master_cfg.MasterOutputTrigger = TIM_TRGO_RESET;
    master_cfg.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

    if (HAL_TIMEx_MasterConfigSynchronization(hw_timer3_handle, &master_cfg) != HAL_OK)
        SYS_ThrowError(SYSERR_FATAL_ERROR, __LINE__, __FILE__, "TIM MCS INIT ERR");

    oc_cfg.OCMode = TIM_OCMODE_PWM1;
    oc_cfg.Pulse = 0;
    oc_cfg.OCPolarity = TIM_OCPOLARITY_HIGH;
    oc_cfg.OCFastMode = TIM_OCFAST_DISABLE;

    if (HAL_TIM_PWM_ConfigChannel(hw_timer3_handle, &oc_cfg, TIM_CHANNEL_2) != HAL_OK)
        SYS_ThrowError(SYSERR_FATAL_ERROR, __LINE__, __FILE__, "TIM OCC INIT ERR");

    __HAL_RCC_GPIOC_CLK_ENABLE();

    gpio.Pin = GPIO_PIN_7;
    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &gpio);

    __HAL_AFIO_REMAP_TIM3_ENABLE(); // Remaps Timer3-Ch2 to PC7.

    hw_timer3_init_status = SYS_INIT_STATUS_INITIALISED;
}

static void _SYS_Timer3_PWM_MspInit_Callback(TIM_HandleTypeDef *handle)
{
    __HAL_RCC_TIM3_CLK_ENABLE();

    // Init DMA for TIM3-CH2
    hw_timer3_dma_handle->Instance = DMA1_Channel3;
    hw_timer3_dma_handle->Init.Direction = DMA_MEMORY_TO_PERIPH;
    hw_timer3_dma_handle->Init.PeriphInc = DMA_PINC_DISABLE;
    hw_timer3_dma_handle->Init.MemInc = DMA_MINC_ENABLE;
    hw_timer3_dma_handle->Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hw_timer3_dma_handle->Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hw_timer3_dma_handle->Init.Mode = DMA_NORMAL;
    hw_timer3_dma_handle->Init.Priority = DMA_PRIORITY_MEDIUM;

    if (HAL_DMA_Init(hw_timer3_dma_handle) != HAL_OK)
        SYS_ThrowError(SYSERR_FATAL_ERROR, __LINE__, __FILE__, "TIM DMA INIT ERR");
    
    __HAL_LINKDMA(handle, hdma[TIM_DMA_ID_CC4], *hw_timer3_dma_handle);
    __HAL_LINKDMA(handle, hdma[TIM_DMA_ID_UPDATE], *hw_timer3_dma_handle);
}

static void _SYS_Timer3_PWM_MspDeInit_Callback(TIM_HandleTypeDef *handle)
{
    __HAL_RCC_TIM3_CLK_DISABLE();

    HAL_DMA_DeInit(handle->hdma[TIM_DMA_ID_CC4]);
    HAL_DMA_DeInit(handle->hdma[TIM_DMA_ID_UPDATE]);
}