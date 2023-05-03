/**
 * Full project: https://github.com/LesniakM/stm32_li-ion_autotester
 * 
 * @file hw_config.h
 * @brief Header for hw_config.c file. 
 * 
 * 
 * @author Mariusz Lesniak
 * https://github.com/LesniakM
 * Contact: lesniak.mar@gmail.com
 * 
 * Distributed under licence:
 * Attribution-NonCommercial 2.5 Generic (CC BY-NC 2.5)
 * Full licence: https://creativecommons.org/licenses/by-nc/2.5/legalcode
 * 
 * ******************************************************************************
  * @attention
  * STM32 HAL libs are licensed by STMicroelectronics.
  * 
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
 */

#ifndef HW_CONFIG_H        // Include guard start.
#define HW_CONFIG_H


// extern "C"                 // Hal libs are written in C.

#ifdef __cplusplus
extern "C" {
#endif


  /* Includes ------------------------------------------------------------------*/
  #include "stm32f1xx_hal.h"

  /* Exported functions prototypes ---------------------------------------------*/
  void Error_Handler(void);

  void SysTick_Handler(void);


void hal_config();

#ifdef __cplusplus
}
#endif



#endif  // Include guard end.