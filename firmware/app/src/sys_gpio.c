#include "sys_gpio.h"
#include "stm32f1xx_hal.h"

SYS_InitStatus_t hw_gpio_init_status = SYS_INIT_STATUS_NOT_INITIALISED;

static void _SYS_GPIO_InitPortA(void);
static void _SYS_GPIO_InitPortB(void);
static void _SYS_GPIO_InitPortC(void);
static void _SYS_GPIO_InitPortD(void);

void SYS_GPIO_Init(void)
{
    _SYS_GPIO_InitPortA();
    _SYS_GPIO_InitPortB();
    _SYS_GPIO_InitPortC();
    _SYS_GPIO_InitPortD();

    hw_gpio_init_status = SYS_INIT_STATUS_INITIALISED;
}

static void _SYS_GPIO_InitPortA(void)
{
    GPIO_InitTypeDef port_init = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();

    port_init.Pin = GPIO_ERROR_LED_PIN;
    port_init.Mode = GPIO_MODE_OUTPUT_PP;
    port_init.Pull = GPIO_NOPULL;
    port_init.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &port_init);

    port_init.Pin = GPIO_HWBIT_0_PIN;
    port_init.Mode = GPIO_MODE_INPUT;
    port_init.Pull = GPIO_NOPULL;
    port_init.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &port_init);
}

static void _SYS_GPIO_InitPortB(void)
{
    GPIO_InitTypeDef port_init = {0};

    __HAL_RCC_GPIOB_CLK_ENABLE();

    port_init.Pin = GPIO_USER_LED_0_PIN | GPIO_USER_LED_1_PIN | GPIO_USER_LED_2_PIN | GPIO_EEPROM_WP_PIN;
    port_init.Mode = GPIO_MODE_OUTPUT_PP;
    port_init.Pull = GPIO_NOPULL;
    port_init.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &port_init);

    port_init.Pin = GPIO_HWBIT_1_PIN | GPIO_HWBIT_2_PIN;
    port_init.Mode = GPIO_MODE_INPUT;
    port_init.Pull = GPIO_NOPULL;
    port_init.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &port_init);
}

static void _SYS_GPIO_InitPortC(void)
{
    GPIO_InitTypeDef port_init = {0};

    __HAL_RCC_GPIOC_CLK_ENABLE();

    //! TODO: Configure PC7 (LED_DATA) for PWM...

    port_init.Pin = GPIO_KBM_DATA_0_PIN | GPIO_KBM_DATA_1_PIN | GPIO_KBM_DATA_2_PIN | GPIO_KBM_DATA_3_PIN |
                    GPIO_KBM_DATA_4_PIN | GPIO_KBM_DATA_5_PIN | GPIO_KBM_DATA_6_PIN | GPIO_HWBIT_4_PIN;
    port_init.Mode = GPIO_MODE_INPUT;
    port_init.Pull = GPIO_NOPULL;
    port_init.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(GPIOC, &port_init);
}

static void _SYS_GPIO_InitPortD(void)
{
    GPIO_InitTypeDef port_init = {0};

    __HAL_RCC_GPIOD_CLK_ENABLE();

    //! TODO: Configure PC7 (LED_DATA) for PWM...

    port_init.Pin = GPIO_PORTEX01_INT_PIN;
    port_init.Mode = GPIO_MODE_INPUT;
    port_init.Pull = GPIO_NOPULL;
    port_init.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(GPIOD, &port_init);
}