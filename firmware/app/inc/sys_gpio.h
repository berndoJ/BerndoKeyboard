/*******************************************************************************
* \file sys_gpio.h
* \date 03.01.2020
********************************************************************************
* \author Johannes Berndorfer (berndoJ)
* \copyright Copyright (c) 2020 by Johannes Berndorfer (berndoJ)
********************************************************************************
* \brief GPIO configuration and initialisation module.
*******************************************************************************/

#if !defined(__SYS_GPIO_H)
#define __SYS_GPIO_H

#include "sys_def.h"

#define GPIO_KBM_DATA_0_PORT        GPIOC
#define GPIO_KBM_DATA_0_PIN         GPIO_PIN_12
#define GPIO_KBM_DATA_1_PORT        GPIOC
#define GPIO_KBM_DATA_1_PIN         GPIO_PIN_11
#define GPIO_KBM_DATA_2_PORT        GPIOC
#define GPIO_KBM_DATA_2_PIN         GPIO_PIN_10
#define GPIO_KBM_DATA_3_PORT        GPIOC
#define GPIO_KBM_DATA_3_PIN         GPIO_PIN_9
#define GPIO_KBM_DATA_4_PORT        GPIOC
#define GPIO_KBM_DATA_4_PIN         GPIO_PIN_8
#define GPIO_KBM_DATA_5_PORT        GPIOC
#define GPIO_KBM_DATA_5_PIN         GPIO_PIN_4
#define GPIO_KBM_DATA_6_PORT        GPIOC
#define GPIO_KBM_DATA_6_PIN         GPIO_PIN_5

#define GPIO_LED_DATA_PORT          GPIOA
#define GPIO_LED_DATA_PIN           GPIO_PIN_2

#define GPIO_PORTEX01_INT_PORT      GPIOD
#define GPIO_PORTEX01_INT_PIN       GPIO_PIN_2

#define GPIO_USER_LED_0_PORT        GPIOB
#define GPIO_USER_LED_0_PIN         GPIO_PIN_0
#define GPIO_USER_LED_1_PORT        GPIOB
#define GPIO_USER_LED_1_PIN         GPIO_PIN_1
#define GPIO_USER_LED_2_PORT        GPIOB
#define GPIO_USER_LED_2_PIN         GPIO_PIN_2
#define GPIO_ERROR_LED_PORT         GPIOA
#define GPIO_ERROR_LED_PIN          GPIO_PIN_4

#define GPIO_HWBIT_0_PORT           GPIOA
#define GPIO_HWBIT_0_PIN            GPIO_PIN_8
#define GPIO_HWBIT_1_PORT           GPIOB
#define GPIO_HWBIT_1_PIN            GPIO_PIN_8
#define GPIO_HWBIT_2_PORT           GPIOB
#define GPIO_HWBIT_2_PIN            GPIO_PIN_9
#define GPIO_HWBIT_3_PORT           GPIOB
#define GPIO_HWBIT_3_PIN            GPIO_PIN_12
#define GPIO_HWBIT_4_PORT           GPIOC
#define GPIO_HWBIT_4_PIN            GPIO_PIN_6

#define GPIO_EEPROM_WP_PORT         GPIOB
#define GPIO_EEPROM_WP_PIN          GPIO_PIN_5

/**
 * \brief Initialisation status for the GPIO pins.
 */
extern SYS_InitStatus_t hw_gpio_init_status;

/**
 * \brief Initialises the GPIO registers of the microcontroller to match the
 *        platform hardware.
 */
void SYS_GPIO_Init(void);

#endif // __SYS_GPIO_H