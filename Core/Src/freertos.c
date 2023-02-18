/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "mb.h"
#include "uart.h"
#include "tim.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for Hello */
osThreadId_t HelloHandle;
const osThreadAttr_t Hello_attributes = {
  .name = "Hello",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for BlinkLed */
osThreadId_t BlinkLedHandle;
const osThreadAttr_t BlinkLed_attributes = {
  .name = "BlinkLed",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for MasterTask */
osThreadId_t MasterTaskHandle;
const osThreadAttr_t MasterTask_attributes = {
  .name = "MasterTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for SlaveTask */
osThreadId_t SlaveTaskHandle;
const osThreadAttr_t SlaveTask_attributes = {
  .name = "SlaveTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for SysTask */
osThreadId_t SysTaskHandle;
const osThreadAttr_t SysTask_attributes = {
  .name = "SysTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void start_hello(void *argument);
void start_blink(void *argument);
void start_master_task(void *argument);
void start_slave_task(void *argument);
void start_sys_task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  HAL_TIM_Base_Start(&htim7);
  //eMBMasterInit(MB_RTU, 3, 115200, MB_PAR_NONE);
  //eMBMasterEnable();
	log_info("before eMBInit\n");
  eMBInit(MB_RTU, 0x01, 2, 115200, MB_PAR_NONE);
  eMBEnable();
  log_info("after eMBInit\n");
  //HelloHandle = osThreadNew(start_hello, NULL, &Hello_attributes);
  BlinkLedHandle = osThreadNew(start_blink, NULL, &BlinkLed_attributes);
  printk("debug\n");
  //SysTaskHandle = osThreadNew(start_sys_task, NULL, &SysTask_attributes);
  MasterTaskHandle = osThreadNew(start_master_task, NULL, &MasterTask_attributes);
  //SlaveTaskHandle = osThreadNew(start_slave_task, NULL, &SlaveTask_attributes);
  /* USER CODE END Init */
/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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

/**
  * @}
  */

/**
  * @}
  */
}

/* USER CODE BEGIN Header_start_hello */
/**
  * @brief  Function implementing the Hello thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_start_hello */
void start_hello(void *argument)
{
  /* USER CODE BEGIN start_hello */
  /* Infinite loop */
  for(;;)
  {
    printk("Hello\n");
    osDelay(1000);
  }
  /* USER CODE END start_hello */
}

/* USER CODE BEGIN Header_start_blink */
/**
* @brief Function implementing the BlinkLed thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_start_blink */
void start_blink(void *argument)
{
  /* USER CODE BEGIN start_blink */
  /* Infinite loop */
  for(;;)
  {
    HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
    printk("blink\n");
    osDelay(1000);
  }
  /* USER CODE END start_blink */
}

/* USER CODE BEGIN Header_start_master_task */
/**
* @brief Function implementing the MasterTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_start_master_task */
void start_master_task(void *argument)
{
  /* USER CODE BEGIN start_master_task */
  /* Infinite loop */
  for(;;)
  {
    eMBMasterPoll();
  }
  /* USER CODE END start_master_task */
}

/* USER CODE BEGIN Header_start_slave_task */
/**
* @brief Function implementing the SlaveTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_start_slave_task */
void start_slave_task(void *argument)
{
  /* USER CODE BEGIN start_slave_task */
  /* Infinite loop */
  for(;;)
  {
    eMBPoll();
  }
  /* USER CODE END start_slave_task */
}

/* USER CODE BEGIN Header_start_sys_task */
/**
* @brief Function implementing the SysTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_start_sys_task */
void start_sys_task(void *argument)
{
  /* USER CODE BEGIN start_sys_task */
  uint16_t data[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  /* Infinite loop */
  for (;;)
  {
    //eMBMasterReqReadHoldingRegister(1, 0, 10, 100);
    eMBMasterReqWriteMultipleHoldingRegister(1, 0, 10, data, 100);
    HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
    for(uint8_t i=0;i<sizeof(data)/sizeof(uint16_t);i++)
      data[i]++;
    osDelay(100);
  }
  /* USER CODE END start_sys_task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

