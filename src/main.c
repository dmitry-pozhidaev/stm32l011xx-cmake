/*
*
* <h2><center>&copy; Copyright(c) 2016 STMicroelectronics.
* All rights reserved.</center></h2>
*
* This software component is licensed by ST under BSD 3-Clause license,
* the "License"; You may not use this file except in compliance with the
* License. You may obtain a copy of the License at: opensource.org/licenses/BSD-3-Clause
*
*/

#include "stm32l0xx_hal.h"

int main(void)
{
    HAL_Init();
    while (1)
    {
        /* GPIO toggle. Pin initialized in stm32l0xx_hal_msp.c */
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
        HAL_Delay(100);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
        HAL_Delay(100);
    }
}
