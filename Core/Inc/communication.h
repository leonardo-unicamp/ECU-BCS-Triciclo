//**********************************************************************//
// File:        communication.h                                         //
// Description: state machine of communication with the balance control //
//              system (used by UART e Bluetooth communication)         //
// Author:      Leonardo Rossi Leao                                     //
// Reviewer:    Rodrigo Moreira Bacurau                                 //
// Last update: 2023-10-16                                              //
//**********************************************************************//

#ifndef INC_COMMUNICATION_H_
#define INC_COMMUNICATION_H_

#include "pid.h"
#include "tim.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "sensorReadings.h"
#include "motorControl.h"

// States
#define SM_INIT    0
#define SM_READY   1
#define SM_GET     2
#define SM_SET     3
#define SM_PARAM   4
#define SM_VALUE   5
#define SM_STREAM  6
#define SM_CANCEL  7

// Communication type
#define SM_UART       0
#define SM_BLUETOOTH  1

// Boolean defines
#define SM_TRUE   1
#define SM_FALSE  0

// Enum that contain the available data types
enum smDataTypes {
	INT,
	FLOAT,
	CHAR
} ;


// Broadcast settings type
typedef struct broadcastSettings {
	uint8_t uiEnable;
	uint8_t uiInterval;
	float * pBuffer[128];
	uint16_t uiCounter;
	TIM_TypeDef *pTimer;
	TIM_HandleTypeDef *htim;
} broadcastSettings_t;

// Communication state machine data type
typedef struct stateMachineData {
	int   *iData;
	float *fData;
	char  *cData;
	uint8_t uiWritable;
	uint8_t uiReadable;
	float fUpperLimit;
	float fLowerLimit;
	enum smDataTypes eDataType;
} stateMachineData_t;

// Basic settings of the communication state machine
typedef struct communicationSettings {
	char cLastChar;
	uint8_t uiCallState;
	uint8_t uiCurrentState;
	uint8_t uiCommunicationLink;
	UART_HandleTypeDef *pUart;
	UART_HandleTypeDef *pBluetooth;
	broadcastSettings_t xBroadcast;
	uint16_t uiBufferCounter;
	stateMachineData_t smBuffer[128];
} communicationSettings_t;

//**********************************************************************//
// Method:      vCommunicationInit                                      //
// Description: initialize the communication state machine              //
// Parameters:  UART_HandleTypeDef *: address to UART                   //
//              UART_HandleTypeDef *: address to Bluetooth UART         //
// Returns:     n/a                                                     //
//**********************************************************************//
void vCommunicationInit(UART_HandleTypeDef *, UART_HandleTypeDef *);

//**********************************************************************//
// Method:      vCommunicationStateMachine                              //
// Description: main function that contain the states transitions and   //
//              and methods associates with each state                  //
// Parameters:  char: character received from serial                    //
// Returns:     n/a                                                     //
//**********************************************************************//
void vCommunicationStateMachine(char);

//**********************************************************************//
// Method:      vCommunicationStateMachineAppendData                    //
// Description: append a state machine data to the buffer               //
// Parameters:  stateMachineData_t: data that can be manipulated in the //
//                                  state machine                       //
// Returns:     n/a                                                     //
//**********************************************************************//
void vCommunicationStateMachineAppendData(stateMachineData_t);

//**********************************************************************//
// Method:      vCommunicationRead                                      //
// Description: read data from UART (wired or bluetooth)                //
// Parameters:  char *: pointer to string input variable                //
//              uint16_t: message size                                  //
// Returns:     n/a                                                     //
//**********************************************************************//
void vCommunicationRead(char *, uint16_t);

//**********************************************************************//
// Method:      vCommunicationWrite                                     //
// Description: write data to UART (wired or bluetooth)                 //
// Parameters:  unsigned char *: pointer to string input variable       //
// Returns:     n/a                                                     //
//**********************************************************************//
void vCommunicationWrite(char *);

//**********************************************************************//
// Method:      vCommunicationSetDestination                            //
// Description: change the origin/destination of the state machine,     //
//              this link can be UART or Bluetooth (previous set in     //
//              the initialization function)                            //
// Parameters:  uint8_t: machine state                                  //
// Returns:     n/a                                                     //
//**********************************************************************//
void vCommunicationSetLink(uint8_t);

//**********************************************************************//
// Method:      vCommunicationLPUART1Callback                           //
// Description: callback of UART interruption                           //
// Parameters:  n/a                                                     //
// Returns:     n/a                                                     //
//**********************************************************************//
void vCommunicationLPUART1Callback(void);

//**********************************************************************//
// Method:      vCommunicationStartBroadcast                            //
// Description: start a timer that periodically send a broadcast with   //
//              balance control system informations                     //
// Parameters:  uint32_t: broadcast interval time                       //
// Returns:     n/a                                                     //
//**********************************************************************//
void vCommunicationStartBroadcast(uint32_t);

//**********************************************************************//
// Method:      vCommunicationStopBroadcast                             //
// Description: stop the timer that periodically send a broadcast with  //
//              balance control system informations                     //
// Parameters:  n/a                                                     //
//********************************************************************* //
// Returns:     n/a                                                    *//
void vCommunicationStopBroadcast(void);

//**********************************************************************//
// Method:      vCommunicationStopBroadcast                             //
// Description: send a broadcast with all data of the balance control   //
//              system                                                  //
// Parameters:  n/a                                                     //
// Returns:     n/a                                                     //
//**********************************************************************//
void vCommunicationSendBroadcast(void);



#endif
