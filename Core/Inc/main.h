/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define UPDATE_RATE_MS 10
#define IMU1_SPI_SCK_Pin GPIO_PIN_2
#define IMU1_SPI_SCK_GPIO_Port GPIOE
#define IMU1_SPI_MISO_Pin GPIO_PIN_5
#define IMU1_SPI_MISO_GPIO_Port GPIOE
#define IMU1_SPI_MOSI_Pin GPIO_PIN_6
#define IMU1_SPI_MOSI_GPIO_Port GPIOE
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define SENS_LIN_FRE_AD_Pin GPIO_PIN_5
#define SENS_LIN_FRE_AD_GPIO_Port GPIOF
#define BLUETOOTH_TX_Pin GPIO_PIN_6
#define BLUETOOTH_TX_GPIO_Port GPIOF
#define SH1_DIS_AD_Pin GPIO_PIN_7
#define SH1_DIS_AD_GPIO_Port GPIOF
#define IMU2_CS_UNO_Pin GPIO_PIN_8
#define IMU2_CS_UNO_GPIO_Port GPIOF
#define SH2_FOR_AD_Pin GPIO_PIN_9
#define SH2_FOR_AD_GPIO_Port GPIOF
#define IMU2_CS_DUE_Pin GPIO_PIN_0
#define IMU2_CS_DUE_GPIO_Port GPIOC
#define IMU2_SPI_MISO_Pin GPIO_PIN_2
#define IMU2_SPI_MISO_GPIO_Port GPIOC
#define IMU2_SPI_MOSI_Pin GPIO_PIN_3
#define IMU2_SPI_MOSI_GPIO_Port GPIOC
#define MOTOR_BCS_RX_Pin GPIO_PIN_0
#define MOTOR_BCS_RX_GPIO_Port GPIOA
#define SPEED_TIM_Pin GPIO_PIN_3
#define SPEED_TIM_GPIO_Port GPIOA
#define CTRL_SHA1_DA_Pin GPIO_PIN_4
#define CTRL_SHA1_DA_GPIO_Port GPIOA
#define CTRL_SHA2_DA_Pin GPIO_PIN_5
#define CTRL_SHA2_DA_GPIO_Port GPIOA
#define SENS_LIN_ESQ_AD_Pin GPIO_PIN_6
#define SENS_LIN_ESQ_AD_GPIO_Port GPIOA
#define LD1_Pin GPIO_PIN_0
#define LD1_GPIO_Port GPIOB
#define SENS_LIN_DIR_AD_Pin GPIO_PIN_1
#define SENS_LIN_DIR_AD_GPIO_Port GPIOB
#define SH1_FOR_AD_Pin GPIO_PIN_11
#define SH1_FOR_AD_GPIO_Port GPIOF
#define SH2_DIS_AD_Pin GPIO_PIN_13
#define SH2_DIS_AD_GPIO_Port GPIOF
#define BLUETOOTH_RX_Pin GPIO_PIN_8
#define BLUETOOTH_RX_GPIO_Port GPIOE
#define IMU2_SPI_SCK_Pin GPIO_PIN_10
#define IMU2_SPI_SCK_GPIO_Port GPIOB
#define MOTOR_DIR_CAN_RX_Pin GPIO_PIN_12
#define MOTOR_DIR_CAN_RX_GPIO_Port GPIOB
#define LD3_Pin GPIO_PIN_14
#define LD3_GPIO_Port GPIOB
#define STLINK_RX_Pin GPIO_PIN_8
#define STLINK_RX_GPIO_Port GPIOD
#define STLINK_TX_Pin GPIO_PIN_9
#define STLINK_TX_GPIO_Port GPIOD
#define USB_OTG_FS_PWR_EN_Pin GPIO_PIN_10
#define USB_OTG_FS_PWR_EN_GPIO_Port GPIOD
#define LED_R_Pin GPIO_PIN_12
#define LED_R_GPIO_Port GPIOD
#define LED_G_Pin GPIO_PIN_13
#define LED_G_GPIO_Port GPIOD
#define LED_B_Pin GPIO_PIN_14
#define LED_B_GPIO_Port GPIOD
#define USB_OTG_FS_OVCR_Pin GPIO_PIN_7
#define USB_OTG_FS_OVCR_GPIO_Port GPIOG
#define GPS_RX_Pin GPIO_PIN_6
#define GPS_RX_GPIO_Port GPIOC
#define GPS_TX_Pin GPIO_PIN_7
#define GPS_TX_GPIO_Port GPIOC
#define MOTOR_BCS_TX_Pin GPIO_PIN_11
#define MOTOR_BCS_TX_GPIO_Port GPIOC
#define MOTOR_DIR_RX_Pin GPIO_PIN_12
#define MOTOR_DIR_RX_GPIO_Port GPIOC
#define MOTOR_BCS_CAN_RX_Pin GPIO_PIN_0
#define MOTOR_BCS_CAN_RX_GPIO_Port GPIOD
#define MOTOR_BCS_CAN_TX_Pin GPIO_PIN_1
#define MOTOR_BCS_CAN_TX_GPIO_Port GPIOD
#define MOTOR_DIR_TX_Pin GPIO_PIN_2
#define MOTOR_DIR_TX_GPIO_Port GPIOD
#define IMU1_CS_UNO_Pin GPIO_PIN_5
#define IMU1_CS_UNO_GPIO_Port GPIOD
#define IMU1_CS_DUE_Pin GPIO_PIN_6
#define IMU1_CS_DUE_GPIO_Port GPIOD
#define MOTOR_DIR_CAN_TX_Pin GPIO_PIN_6
#define MOTOR_DIR_CAN_TX_GPIO_Port GPIOB
#define DISPLAY_SDA_Pin GPIO_PIN_7
#define DISPLAY_SDA_GPIO_Port GPIOB
#define DISPLAY_SCL_Pin GPIO_PIN_8
#define DISPLAY_SCL_GPIO_Port GPIOB
#define LD2_Pin GPIO_PIN_1
#define LD2_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

#define UPDATE_RATE			(UPDATE_RATE_MS/1000.0)					  // in sec
#define SIN_MAGNITUDE_DEG	2.0    							          // in +-deg (of the motor)
#define SIN_MAGNITUDE  		(SIN_MAGNITUDE_DEG/7.2)   				  // in turns (of the motor - reduction 50:1)
#define SIN_PERIOD   		3.0						                  // in sec
#define SIN_FREQUENCY   	(1.0/5.0)   							  // in herts
#define MAX_TABLE_SIZE      2000									  // one period
#define PI                  3.14159265358979323846

#define IMU_HEIGHT 0.47

#define USE_FILTER

#define FILTER 				LP_IIR_13_00_HZ_4_TAPS


#define LP_IIR_46_00_HZ_16_TAPS 1
#define LP_IIR_20_00_HZ_16_TAPS 2
#define LP_IIR_13_00_HZ_16_TAPS 3
#define LP_IIR_4_00_HZ_16_TAPS  4
#define LP_IIR_2_00_HZ_16_TAPS  5
#define LP_IIR_1_00_HZ_16_TAPS  6
#define LP_IIR_0_50_HZ_16_TAPS  7
#define LP_IIR_0_10_HZ_16_TAPS  8
#define LP_IIR_0_01_HZ_16_TAPS  9

#define LP_IIR_46_00_HZ_8_TAPS 10
#define LP_IIR_20_00_HZ_8_TAPS 11
#define LP_IIR_13_00_HZ_8_TAPS 12
#define LP_IIR_4_00_HZ_8_TAPS  13
#define LP_IIR_2_00_HZ_8_TAPS  14
#define LP_IIR_1_00_HZ_8_TAPS  15
#define LP_IIR_0_50_HZ_8_TAPS  16
#define LP_IIR_0_10_HZ_8_TAPS  17
#define LP_IIR_0_01_HZ_8_TAPS  18

#define LP_IIR_46_00_HZ_4_TAPS 19
#define LP_IIR_20_00_HZ_4_TAPS 20
#define LP_IIR_13_00_HZ_4_TAPS 21
#define LP_IIR_4_00_HZ_4_TAPS  22
#define LP_IIR_2_00_HZ_4_TAPS  23
#define LP_IIR_1_00_HZ_4_TAPS  24
#define LP_IIR_0_50_HZ_4_TAPS  25
#define LP_IIR_0_10_HZ_4_TAPS  26
#define LP_IIR_0_01_HZ_4_TAPS  27

#define LP_IIR_46_00_HZ_2_TAPS 28
#define LP_IIR_20_00_HZ_2_TAPS 29
#define LP_IIR_13_00_HZ_2_TAPS 30
#define LP_IIR_4_00_HZ_2_TAPS  31
#define LP_IIR_2_00_HZ_2_TAPS  32
#define LP_IIR_1_00_HZ_2_TAPS  33
#define LP_IIR_0_50_HZ_2_TAPS  34
#define LP_IIR_0_10_HZ_2_TAPS  35
#define LP_IIR_0_01_HZ_2_TAPS  36

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
