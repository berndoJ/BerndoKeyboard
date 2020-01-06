#include "sys_usart.h"
#include "error.h"
#include <stdlib.h>

UART_HandleTypeDef *hw_uart1_handle;
SYS_InitStatus_t hw_uart1_init_status;

static UART_HandleTypeDef _hw_uart1_handle;

static void _SYS_USART1_MspInitCallback(UART_HandleTypeDef *handle);
static void _SYS_USART1_MspDeInitCallback(UART_HandleTypeDef *handle);

void SYS_USART_Init(void)
{
    hw_uart1_handle = &_hw_uart1_handle;

    hw_uart1_handle->Instance = USART1;
    hw_uart1_handle->Init.BaudRate = SYS_UART1_BAUD_RATE;
    hw_uart1_handle->Init.WordLength = UART_WORDLENGTH_8B;
    hw_uart1_handle->Init.StopBits = UART_STOPBITS_1;
    hw_uart1_handle->Init.Parity = UART_PARITY_NONE;
    hw_uart1_handle->Init.Mode = UART_MODE_TX_RX;
    hw_uart1_handle->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    hw_uart1_handle->Init.OverSampling = UART_OVERSAMPLING_16;
    hw_uart1_handle->MspInitCallback = _SYS_USART1_MspInitCallback;
    hw_uart1_handle->MspDeInitCallback = _SYS_USART1_MspDeInitCallback;

    if (HAL_UART_Init(hw_uart1_handle) != HAL_OK)
        SYS_ThrowError(SYSERR_FATAL_ERROR, __LINE__, __FILE__, "UART1 INIT FAIL");
    else
        hw_uart1_init_status = SYS_INIT_STATUS_INITIALISED;
}

static void _SYS_USART1_MspInitCallback(UART_HandleTypeDef *handle)
{
    GPIO_InitTypeDef gpio_init = {0};

    // Enable clocks.
    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // Configure TX pin.
    gpio_init.Pin = GPIO_PIN_9;
    gpio_init.Mode = GPIO_MODE_AF_PP;
    gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &gpio_init);

    // Configure RX pin.
    gpio_init.Pin = GPIO_PIN_10;
    gpio_init.Mode = GPIO_MODE_INPUT;
    gpio_init.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &gpio_init);
}

static void _SYS_USART1_MspDeInitCallback(UART_HandleTypeDef *handle)
{
    // Disable clock.
    __HAL_RCC_USART1_CLK_DISABLE();

    // Deinit GPIOs.
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_10);
}