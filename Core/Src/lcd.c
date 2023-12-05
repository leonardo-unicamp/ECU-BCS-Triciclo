//**********************************************************************//
// File:        lcd.c                                                   //
// Description: Source file that contains the functions and methods     //
//              implementations for handling with the LCD hardware      //
//              (2x16) by I2C                                           //
// Author:      Leonardo Rossi Leao                                     //
// Reviewer:    Leonardo Kenji Kobaicy                                  //
//              Matheus Meirelles Onofre Martins                        //
// Created:     19/09/2023                                              //
// Last Update: 19/09/2023                                              //
// Notes:       This code is an adaptation of Rodrigo M. Bacurau's code //
//**********************************************************************//

#include "lcd.h"

// Functions with local scope
static void vLcdSendNibble(unsigned char);
static void vLcdSendByte(unsigned char);
static void vLcdWrite2Lcd(unsigned char, unsigned char);
static void vLcdSendCommand(unsigned char);
static void vLcdWriteData(unsigned char);
static void vLcdSetCursor(uint8_t, uint8_t);
static void vLcdWriteString(const char*);
static void vLcdShowMessage(lcdMessage_t *);


// Global variables
lcdSettings_t xLcdSettings;
extern osMessageQueueId_t lcdMessageQueueHandle;

//**********************************************************************//
// Method:      vLcdInit                                                //
// Description: Starts LCD peripherals and I2C communication            //
// Input:       t_lcdI2C: LCD boot settings                             //
// Output:      none (no output parameters are returned)                //
//**********************************************************************//
void vLcdInit()
{
	// Define some LCD settings and initialize
	xLcdSettings.I2C.pHandler = &hi2c2;
	xLcdSettings.I2C.cAddress = 0x27;
	//xLcdSettings.I2C.cAddress = 0x3F;
	xLcdSettings.xCarrousel.uiMaxMessages = 10;
	xLcdSettings.xQueueId = lcdMessageQueueHandle;
	xLcdSettings.uiBacklightStatus = 1;

	// Initialize display
	osDelay(50);
	vLcdSendNibble(0x30);
	osDelay(5);
	vLcdSendNibble(0x30);
	osDelay(1);
	vLcdSendNibble(0x30);
	osDelay(10);
	vLcdSendNibble(0x20);
	osDelay(2);

	vLcdSendCommand(LCD_CMD_INIT_CONFIG);
	vLcdSendCommand(LCD_CMD_LCDON_CURSOROFF);
	vLcdSendCommand(LCD_CMD_CLEAR);
	vLcdSetCursor(LCD_LINE0, LCD_COLUMN0);
}

//**********************************************************************//
// Method:      vLcdStart                                               //
// Description: Entrypoint to lcd task in line follower robot           //
// Input:       void (no input parameters are required)                 //
// Output:      none (no output parameters are returned)                //
//**********************************************************************//
void vLcdStart(void *args)
{
	uint32_t uiQueueCount;
	lcdMessage_t xMessage;
	uint8_t uiCarrouselCounter, uiNumberOfMessages;

	// Infinite loop
	while(1)
	{
		// Show all messages on message queue
		uiQueueCount = osMessageQueueGetCount(xLcdSettings.xQueueId);
		while(uiQueueCount > 0)
		{
			osMessageQueueGet(xLcdSettings.xQueueId, &xMessage, 0, osWaitForever);
			vLcdShowMessage(&xMessage);
			osDelay(xMessage.uiTime);
			vLcdSendCommand(LCD_CMD_CLEAR);
			uiQueueCount--;
		}

		// Reset counter if its necessary
		uiNumberOfMessages = xLcdSettings.xCarrousel.uiMessagesCounter;
		if(uiCarrouselCounter >= uiNumberOfMessages)
			uiCarrouselCounter = 0;

		// Show messages on carroussel
		if(uiNumberOfMessages > 0)
		{
			xMessage = xLcdSettings.xCarrousel.xMessages[uiCarrouselCounter];
			vLcdShowMessage(&xMessage);
			osDelay(xMessage.uiTime);
			uiCarrouselCounter++;
		}
	}
}

//**********************************************************************//
// Method:      vLcdAppendMessageToCarrousel                            //
// Description: Append a lcdMessage_t to messages carroussel            //
// Input:       lcdMessage_t: message to be appended                    //
// Output:      uint8_t: status of message append (1: ok | 0: error)    //
//**********************************************************************//
uint8_t vLcdAppendMessageToCarrousel(lcdMessage_t xMessage)
{
	uint8_t uiMaxMessages = xLcdSettings.xCarrousel.uiMaxMessages;
	uint8_t uiMessagesCounter = xLcdSettings.xCarrousel.uiMessagesCounter;

	// Check if it is possible append a new message to carroussel
	if(uiMessagesCounter < uiMaxMessages)
	{
		xLcdSettings.xCarrousel.xMessages[uiMessagesCounter] = xMessage;
		xLcdSettings.xCarrousel.uiMessagesCounter++;
		return(1);
	}
	return(0);
}

//**********************************************************************//
// Method:      vLcdSendCommand                                         //
// Description: Sends a command to the LCD via I2C                      //
// Input:       char: command (it is recommended to use LCD_CMD_...)    //
// Output:      none (no output parameters are returned)                //
//**********************************************************************//
void vLcdSendCommand(unsigned char ucCommand)
{
	// Just a relay to send command
	vLcdWrite2Lcd(ucCommand, LCD_RS_CMD);
}

//**********************************************************************//
// Method:      vLcdWriteData                                           //
// Description: Sends data to the LCD via I2C                           //
// Input:       char: command (it is recommended to use LCD_CMD_...)    //
// Output:      none (no output parameters are returned)                //
//**********************************************************************//
void vLcdWriteData(unsigned char ucData)
{
	// Just a relay to send data
	vLcdWrite2Lcd(ucData, LCD_RS_DATA);
}

//**********************************************************************//
// Method:      vLcdSetCursor                                           //
// Description: Sets a position for the cursor on the LCD               //
// Input:       uint8_t: line position (0 to 1)                         //
//              uint8_t: column position (0 to 15)                      //
// Output:      none (no output parameters are returned)                //
//**********************************************************************//
void vLcdSetCursor(uint8_t uiLine, uint8_t uiColumn)
{
	char cCommand;

	if(uiLine == LCD_LINE0)
		cCommand = LCD_L0C0_BASE;
	else
	    cCommand = LCD_L1C0_BASE;

	cCommand += (uiColumn & LCD_MAX_COLUMN);
	vLcdSendCommand(cCommand);
}

//**********************************************************************//
// Method:      vLcdWriteString                                         //
// Description: Writes a string message on the LCD                      //
// Input:       const char*: characters buffer containing the message   //
// Output:      none (no output parameters are returned)                //
//**********************************************************************//
void vLcdWriteString(const char *cBuffer)
{
	while(*cBuffer)
		vLcdWriteData(*cBuffer++);
}


//**********************************************************************//
// Method:      vLcdShowMessage                                         //
// Description: Writes a text message on the LCD using a lcdMessage_t   //
// Input:       lcdMessage_t: message to be showed on the LCD           //
// Output:      none (no output parameters are returned)                //
//**********************************************************************//
void vLcdShowMessage(lcdMessage_t *pMessage)
{
	char cFirstLine[16], cSecondLine[16];

	if(pMessage->uiFirstLineHasValue == LCD_YES)
		sprintf(cFirstLine, pMessage->cFirstLine, *pMessage->pFirstLineValue);
	else if(pMessage->uiFirstLineHasValue == LCD_NO)
		sprintf(cFirstLine, pMessage->cFirstLine);

	vLcdSendCommand(LCD_CMD_CLEAR);
	vLcdSetCursor(0,0);
	vLcdWriteString(cFirstLine);

	if(pMessage->uiSecondLineHasValue == LCD_YES)
		sprintf(cSecondLine, pMessage->cSecondLine,*pMessage->pSecondLineValue);
	else if(pMessage->uiSecondLineHasValue == LCD_NO)
		sprintf(cSecondLine, pMessage->cSecondLine);

    vLcdSetCursor(1,0);
	vLcdWriteString(cSecondLine);
}

//**********************************************************************//
// Method:      vLcdSendNibble                                          //
// Description: Sends a partial command or data to the LCD              //
// Input:       unsigned char: four bit message                         //
// Output:      none (no output parameters are returned)                //
//**********************************************************************//
void vLcdSendNibble(unsigned char ucNibble)
{
	uint8_t data[] = {ucNibble | LCD_BIT_E, ucNibble};
	HAL_I2C_Master_Transmit(
		xLcdSettings.I2C.pHandler,
		xLcdSettings.I2C.cAddress << 1,
		data,
		sizeof(data),
		50
	);
}

//**********************************************************************//
// Method:      vLcdSendByte                                            //
// Description: Sends a byte to the LCD by I2C                          //
// Input:       uint8_t: byte to be send                                //
// Output:      none (no output parameters are returned)                //
//**********************************************************************//
void vLcdSendByte(uint8_t uiByte)
{
	uint8_t data[] = {uiByte, uiByte, uiByte | LCD_BIT_E, uiByte | LCD_BIT_E, uiByte};
	HAL_I2C_Master_Transmit(
		xLcdSettings.I2C.pHandler,
		xLcdSettings.I2C.cAddress << 1,
		data,
		sizeof(data),
		50
	);
}

//**********************************************************************//
// Method:      vLcdWrite2Lcd                                           //
// Description: Sends a command or data to the LCD                      //
// Input:       unsigned char: byte to be send                          //
//              unsigned char: command LCD_RS_CMD or data LCD_RS_DATA   //
// Output:      none (no output parameters are returned)                //
//**********************************************************************//
void vLcdWrite2Lcd(unsigned char ucBuffer, unsigned char ucDataType)
{
	uint8_t uiLowNibble  = (ucBuffer << 4) & 0xF0;
	uint8_t uiHighNibble = ucBuffer & 0xF0;
	uint8_t uiBacklight  = xLcdSettings.uiBacklightStatus;
	uint8_t uiFormat = (LCD_BIT_RS*ucDataType) | (LCD_BIT_BACKLIGHT*uiBacklight);

	vLcdSendByte(uiHighNibble | uiFormat);
	vLcdSendByte(uiLowNibble | uiFormat);
}

//**********************************************************************//
// Method:      vLcdAppendMessageToCarrousel                            //
// Description: Insert a message on LCD Queue                           //
// Input:       lcdMessage_t xMessage: message to show immediatelly     //
// Output:      none                                                    //
//**********************************************************************//
void vLcdShowWarningMessage(lcdMessage_t xMessage)
{
	osMessageQueuePut(xLcdSettings.xQueueId, &xMessage, 0, 10);
}
