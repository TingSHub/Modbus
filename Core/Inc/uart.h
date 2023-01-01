/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : uart.h
  * @brief          : Header for uart.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UART_H
#define __UART_H

#ifdef __cplusplus
extern "C" {
#endif
	
void printk(char *format, ...);
	
#ifdef __cplusplus
}
#endif

#endif /* __UART_H */
