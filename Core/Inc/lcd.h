//**********************************************************************//
// File:        lcd.h                                                   //
// Description: Header file that contains the functions and methods for //
//              handling with the LCD hardware (2x16) by I2C            //
// Author:      Leonardo Rossi Leao                                     //
// Reviewer:    Leonardo Kenji Kobaicy                                  //
//              Matheus Meirelles Onofre Martins                        //
// Created:     19/09/2023                                              //
// Last Update: 19/09/2023                                              //
// Notes:       This code is an adaptation of Rodrigo M. Bacurau's code //
//**********************************************************************//

/*
 * Step by step to use this library:
 * 1. Enable I2C1 as I2C and NVIC interrupts/DMA Rx and Tx
 * 2. Create a message queue named lcdMessageQueue
 */

// Required libraries
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "stdio.h"
#include "string.h"
#include "i2c.h"

#ifndef INC_LCD_H_
#define INC_LCD_H_

#define LCD_CMD_INIT_CONFIG      0x28
#define LCD_CMD_LCDON_CURSOROFF  0x0C
#define LCD_CMD_CLEAR            0x01
#define LCD_CMD_RETURN_HOME      0x02
#define LCD_CMD_DECREMENT_CURSOR 0x04
#define LCD_CMD_INCREMENT_CURSOR 0x06
#define LCD_RS_DATA              1U
#define LCD_RS_CMD               0U
#define LCD_BIT_RS               01U
#define LCD_BIT_RW               0x02U
#define LCD_BIT_E                0x04U
#define LCD_BIT_BACKLIGHT        0x08U

#define LCD_LINE0      0U
#define LCD_COLUMN0    0U
#define LCD_L0C0_BASE  0x80
#define LCD_L1C0_BASE  0xC0
#define LCD_MAX_COLUMN 15U

#define LCD_YES 1
#define LCD_NO  0

// Define a struct to I2C communication by LCD
typedef struct {
	I2C_HandleTypeDef *pHandler;
	uint8_t cAddress;
} lcdI2C_t;

// Define a struct to messages on LCD
typedef struct {
	uint8_t uiFirstLineHasValue;
	uint8_t uiSecondLineHasValue;
	char cFirstLine[16];
	char cSecondLine[16];
	float *pFirstLineValue;
	float *pSecondLineValue;
	unsigned int uiTime;
} lcdMessage_t;

// Define a struct to messages carroussel
typedef struct {
	lcdMessage_t xMessages[10];
	uint8_t uiMessagesCounter;
	uint8_t uiMaxMessages;
} lcdCarroussel_t;

// Define a struct with LCD initializing settings
typedef struct {
	lcdI2C_t I2C;
	lcdCarroussel_t xCarrousel;
	osMessageQueueId_t xQueueId;
	uint8_t uiBacklightStatus;
} lcdSettings_t;


//**********************************************************************//
// Method:      vLcdStart                                               //
// Description: Entrypoint to lcd task in line follower robot           //
// Input:       void (no input parameters are required)                 //
// Output:      none (no output parameters are returned)                //
//**********************************************************************//
void vLcdStart(void *);


//**********************************************************************//
// Method:      vLcdInit                                                //
// Description: Starts LCD peripherals and I2C communication            //
// Input:       void (no input parameters are required)                 //
// Output:      none (no output parameters are returned)                //
//**********************************************************************//
void vLcdInit(void);


//**********************************************************************//
// Method:      vLcdAppendMessageToCarrousel                            //
// Description: Append a lcdMessage_t to messages carroussel            //
// Input:       lcdMessage_t: message to be appended                    //
// Output:      uint8_t: status of message append (1: ok | 0: error)    //
//**********************************************************************//
uint8_t vLcdAppendMessageToCarrousel(lcdMessage_t);

//**********************************************************************//
// Method:      vLcdAppendMessageToCarrousel                            //
// Description: Insert a message on LCD Queue                           //
// Input:       lcdMessage_t xMessage: message to show immediatelly     //
// Output:      none                                                    //
//**********************************************************************//
void vLcdShowWarningMessage(lcdMessage_t);


#endif
