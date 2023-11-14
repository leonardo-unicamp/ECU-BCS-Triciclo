/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "sensorReadings.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
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
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for sensorReadings */
osThreadId_t sensorReadingsHandle;
uint32_t sensorReadingsBuffer[ 512 ];
osStaticThreadDef_t sensorReadingsControlBlock;
const osThreadAttr_t sensorReadings_attributes = {
  .name = "sensorReadings",
  .cb_mem = &sensorReadingsControlBlock,
  .cb_size = sizeof(sensorReadingsControlBlock),
  .stack_mem = &sensorReadingsBuffer[0],
  .stack_size = sizeof(sensorReadingsBuffer),
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for systemInit */
osThreadId_t systemInitHandle;
uint32_t systemInitBuffer[ 1024 ];
osStaticThreadDef_t systemInitControlBlock;
const osThreadAttr_t systemInit_attributes = {
  .name = "systemInit",
  .cb_mem = &systemInitControlBlock,
  .cb_size = sizeof(systemInitControlBlock),
  .stack_mem = &systemInitBuffer[0],
  .stack_size = sizeof(systemInitBuffer),
  .priority = (osPriority_t) osPriorityRealtime1,
};
/* Definitions for systemControl */
osThreadId_t systemControlHandle;
uint32_t systemControlBuffer[ 1024 ];
osStaticThreadDef_t systemControlControlBlock;
const osThreadAttr_t systemControl_attributes = {
  .name = "systemControl",
  .cb_mem = &systemControlControlBlock,
  .cb_size = sizeof(systemControlControlBlock),
  .stack_mem = &systemControlBuffer[0],
  .stack_size = sizeof(systemControlBuffer),
  .priority = (osPriority_t) osPriorityRealtime,
};
/* Definitions for motorControl */
osThreadId_t motorControlHandle;
uint32_t motorControlBuffer[ 1024 ];
osStaticThreadDef_t motorControlControlBlock;
const osThreadAttr_t motorControl_attributes = {
  .name = "motorControl",
  .cb_mem = &motorControlControlBlock,
  .cb_size = sizeof(motorControlControlBlock),
  .stack_mem = &motorControlBuffer[0],
  .stack_size = sizeof(motorControlBuffer),
  .priority = (osPriority_t) osPriorityRealtime2,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
extern void vSensorReadingsStart(void *argument);
extern void vSystemInitStart(void *argument);
extern void vSystemControlStart(void *argument);
extern void vMotorControlStart(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of sensorReadings */
  sensorReadingsHandle = osThreadNew(vSensorReadingsStart, NULL, &sensorReadings_attributes);

  /* creation of systemInit */
  systemInitHandle = osThreadNew(vSystemInitStart, NULL, &systemInit_attributes);

  /* creation of systemControl */
  systemControlHandle = osThreadNew(vSystemControlStart, NULL, &systemControl_attributes);

  /* creation of motorControl */
  motorControlHandle = osThreadNew(vMotorControlStart, NULL, &motorControl_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
__weak void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

