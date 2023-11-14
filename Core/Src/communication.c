//**********************************************************************//
// File:        communication.h                                         //
// Description: state machine of communication with the balance control //
//              system (used by UART e Bluetooth communication)         //
// Author:      Leonardo Rossi Leao                                     //
// Reviewer:    Rodrigo Moreira Bacurau                                 //
// Last update: 2023-10-16                                              //
//**********************************************************************//

#include "communication.h"
#include "communication_init.h"

uint8_t uiId[2];
uint8_t uiIdCounter;
communicationSettings_t xComSettings;

char cRxChar;
uint8_t uiOptionId;
char cValueBuffer[32];
uint8_t uiValueCounter;

void vCommunicationStateMachineReady(char);
void vCommunicationStateMachineParam(char);
void vCommunicationStateMachineGet(char);
void vCommunicationStateMachineSet(char);
void vCommunicationStateMachineBroadcast(char);

//**********************************************************************//
// Method:      vCommunicationInit                                      //
// Description: initialize the communication state machine              //
// Parameters:  UART_HandleTypeDef *: address to UART                   //
//              UART_HandleTypeDef *: address to Bluetooth UART         //
// Returns:     n/a                                                     //
//**********************************************************************//
void vCommunicationInit(UART_HandleTypeDef *pUart, UART_HandleTypeDef *pBluetooth)
{
	xComSettings.pUart = pUart;
	xComSettings.pBluetooth = pBluetooth;
	xComSettings.uiCommunicationLink = SM_UART;
	xComSettings.xBroadcast.uiEnable = SM_FALSE;
	xComSettings.xBroadcast.uiInterval = 100;

	// Broadcast settings
	xComSettings.xBroadcast.htim = &htim6;
	xComSettings.xBroadcast.pTimer = TIM6;

	// Creating Buffer
	create();
	vCommunicationStateMachineAppendData(smGyroscopeX);
	vCommunicationStateMachineAppendData(smGyroscopeY);
	vCommunicationStateMachineAppendData(smGyroscopeZ);
	vCommunicationStateMachineAppendData(smAccelerationX);
	vCommunicationStateMachineAppendData(smAccelerationY);
	vCommunicationStateMachineAppendData(smAccelerationZ);
	vCommunicationStateMachineAppendData(smEncoderPosition);
	vCommunicationStateMachineAppendData(smEncoderVelocity);
	vCommunicationStateMachineAppendData(smIqMeasured);
	vCommunicationStateMachineAppendData(smIqSetpoint);
	vCommunicationStateMachineAppendData(smMotorTemperature);
	vCommunicationStateMachineAppendData(smFetTemperature);
	vCommunicationStateMachineAppendData(smPidKp);
	vCommunicationStateMachineAppendData(smPidKi);
	vCommunicationStateMachineAppendData(smPidKd);
	vCommunicationStateMachineAppendData(smMotorAcceleration);
	vCommunicationStateMachineAppendData(smAccelerationMeasured);
	vCommunicationStateMachineAppendData(smAccelerationAngle);
	vCommunicationStateMachineAppendData(smActuadorTorque);

	// Initial state of machine
	xComSettings.uiCurrentState = SM_INIT;

	// Request to read the first character
	HAL_UART_Receive_IT(xComSettings.pUart, (uint8_t *)&cRxChar, 1);
}

//**********************************************************************//
// Method:      vCommunicationSetDestination                            //
// Description: change the origin/destination of the state machine,     //
//              this link can be UART or Bluetooth (previous set in     //
//              the initialization function)                            //
// Parameters:  uiLink (uint8_t): machine state                         //
// Returns:     n/a                                                     //
//**********************************************************************//
void vCommunicationSetLink(uint8_t uiLink)
{
	xComSettings.uiCommunicationLink = uiLink;
}

//**********************************************************************//
// Method:      vCommunicationRead                                      //
// Description: read data from UART (wired or bluetooth)                //
// Parameters:  ucOutput (unsigned char *): pointer to string output    //
//              uiMessageSize (uint16_t): message size                  //
// Returns:     n/a                                                     //
//**********************************************************************//
void vCommunicationRead(char *cOutput, uint16_t uiSize)
{
	UART_HandleTypeDef *huart;

	if(xComSettings.uiCommunicationLink == SM_UART)
		huart = xComSettings.pUart;
	else if(xComSettings.uiCommunicationLink == SM_BLUETOOTH)
		huart = xComSettings.pBluetooth;

	HAL_UART_Receive_IT(huart, (uint8_t *)cOutput, uiSize);
}

//**********************************************************************//
// Method:      vCommunicationWrite                                     //
// Description: write data to UART (wired or bluetooth)                 //
// Parameters:  ucBuffer (unsigned char *): pointer to string           //
// Returns:     n/a                                                     //
//**********************************************************************//
void vCommunicationWrite(char *cBuffer)
{
	UART_HandleTypeDef *huart;

	if(xComSettings.uiCommunicationLink == SM_UART)
		huart = xComSettings.pUart;
	else if(xComSettings.uiCommunicationLink == SM_BLUETOOTH)
		huart = xComSettings.pBluetooth;

	HAL_UART_Transmit_IT(huart, (uint8_t *)cBuffer, (uint16_t)strlen(cBuffer));
}

//**********************************************************************//
// Method:      vCommunicationStateMachineAppendData                    //
// Description: append a state machine data to the buffer               //
// Parameters:  stateMachineData_t: data that can be manipulated in the //
//                                  state machine                       //
// Returns:     n/a                                                     //
//**********************************************************************//
void vCommunicationStateMachineAppendData(stateMachineData_t smData)
{
	xComSettings.smBuffer[xComSettings.uiBufferCounter] = smData;
	xComSettings.uiBufferCounter++;
}

//**********************************************************************//
// Method:      vCommunicationStateMachine                              //
// Description: main function that contain the states transitions and   //
//              and methods associates with each state                  //
// Parameters:  ucChar (unsigned char): character received from serial  //
// Returns:     n/a                                                     //
//**********************************************************************//
void vCommunicationStateMachine(char cChar)
{
	char cBuffer[4];

	switch(xComSettings.uiCurrentState)
	{
		case SM_INIT:
			if(cChar == '#')
				xComSettings.uiCurrentState = SM_READY;
			break;

		case SM_READY:
			vCommunicationStateMachineReady(cChar);
			break;

		case SM_GET:
			vCommunicationStateMachineGet(cChar);
			break;

		case SM_PARAM:
			vCommunicationStateMachineParam(cChar);
			break;

		case SM_CANCEL:
			sprintf(cBuffer, "#e;");
			vCommunicationWrite(cBuffer);
			xComSettings.uiCurrentState = SM_INIT;
			break;

	}

	xComSettings.cLastChar = cChar;
}

//**********************************************************************//
// Method:      vCommunicationStateMachineReady                         //
// Description: redirects to the respective functionalities             //
// Parameters:  cChar (char): character received by UART                //
// Returns:     n/a                                                     //
//**********************************************************************//
void vCommunicationStateMachineReady(char cChar)
{
	if(cChar == 'g')
	{
		xComSettings.uiCallState = SM_GET;
		xComSettings.uiCurrentState = SM_PARAM;
	}
	else if(cChar == 's')
	{
		xComSettings.uiCallState = SM_SET;
		xComSettings.uiCurrentState = SM_PARAM;
	}
	else if(cChar == 'b')
	{
		xComSettings.uiCallState = SM_STREAM;
		xComSettings.uiCurrentState = SM_PARAM;
	}
	else
		xComSettings.uiCurrentState = SM_CANCEL;
}

//**********************************************************************//
// Method:      vCommunicationStateMachineParam                         //
// Description: get parameter associated with the command               //
// Parameters:  cChar (char): character received by UART                //
// Returns:     n/a                                                     //
//**********************************************************************//
void vCommunicationStateMachineParam(char cChar)
{
	if(uiIdCounter < 2)
	{
		uiId[uiIdCounter] = (uint8_t) cChar - 48;
		uiIdCounter++;
	}
	if(uiIdCounter >= 2)
	{
		uiOptionId = uiId[0]*10 + uiId[1];
		if(uiOptionId < xComSettings.uiBufferCounter)
			xComSettings.uiCurrentState = xComSettings.uiCallState;
		else
			xComSettings.uiCurrentState = SM_CANCEL;
		uiIdCounter = 0;
	}
}

//**********************************************************************//
// Method:      vCommunicationStateMachineGet                           //
// Description: redirects to get the value of the variable of interest  //
// Parameters:  cChar (char): character received by UART                //
// Returns:     n/a                                                     //
//**********************************************************************//
void vCommunicationStateMachineGet(char cChar)
{
	int   iValueGet;
	float fValueGet;
	char  *cValueGet;
	char  cBuffer[64];

	if(cChar == ';')
	{
		switch(xComSettings.smBuffer[uiOptionId].eDataType)
		{
			case INT:
				iValueGet = *xComSettings.smBuffer[uiOptionId].iData;
				sprintf(cBuffer, "#a%.2dv%d;\n", uiOptionId, iValueGet);
				break;

			case FLOAT:
				fValueGet = *xComSettings.smBuffer[uiOptionId].fData;
				sprintf(cBuffer, "#a%.2dv%.5f;\n", uiOptionId, fValueGet);
				break;

			case CHAR:
				cValueGet = xComSettings.smBuffer[uiOptionId].cData;
				sprintf(cBuffer, "#a%.2dv%s;\n", uiOptionId, cValueGet);
		}
		vCommunicationWrite(cBuffer);
		xComSettings.uiCurrentState = SM_INIT;
	}
	else
	{
		uiIdCounter = 0;
		xComSettings.uiCurrentState = SM_CANCEL;
		vCommunicationStateMachine(';');
	}
}


//**********************************************************************//
// Method:      vCommunicationStateMachineSet                           //
// Description: get value from UART message and apply to the specified  //
//              parameter                                               //
// Parameters:  cChar (char): character received by UART                //
// Returns:     n/a                                                     //
//**********************************************************************//
void vCommunicationStateMachineSet(char cChar)
{
	int   iReceivedNumber;
	float fReceivedNumber;
	float fLowerLimit = xComSettings.smBuffer[uiOptionId].fLowerLimit;
	float fUpperLimit = xComSettings.smBuffer[uiOptionId].fUpperLimit;

	if(cChar == ';')
	{
		cValueBuffer[uiValueCounter] = '\0';

		if(xComSettings.smBuffer[uiOptionId].uiWritable)
		{
			switch(xComSettings.smBuffer[uiOptionId].eDataType)
			{
				case INT:
					iReceivedNumber = atoi(cValueBuffer);
					if ((int)fLowerLimit <= iReceivedNumber && (int)fUpperLimit >= iReceivedNumber)
					{
						*xComSettings.smBuffer[uiOptionId].iData = iReceivedNumber;
						xComSettings.uiCurrentState = SM_INIT;
					}
					else
					{
						xComSettings.uiCurrentState = SM_CANCEL;
						vCommunicationStateMachine(';');
					}
					break;

				case FLOAT:
					fReceivedNumber = atof(cValueBuffer);
					if (fLowerLimit <= fReceivedNumber && fUpperLimit >= fReceivedNumber)
					{
						*xComSettings.smBuffer[uiOptionId].fData = fReceivedNumber;
						xComSettings.uiCurrentState = SM_INIT;
					}
					else
					{
						xComSettings.uiCurrentState = SM_CANCEL;
						vCommunicationStateMachine(';');
					}
					break;

				case CHAR:
					strcpy(xComSettings.smBuffer[uiOptionId].cData, cValueBuffer);
					break;
			}
		}

	}
	else if((cChar >= '0' && cChar <= '9') || cChar == ',' || cChar == '.')
	{
		cValueBuffer[uiValueCounter] = cChar;
		uiValueCounter++;
	}
	else
	{
		xComSettings.uiCurrentState = SM_CANCEL;
		vCommunicationStateMachine(';');
	}
}

//**********************************************************************//
// Method:      vCommunicationStartBroadcast                            //
// Description: start a timer that periodically send a broadcast with   //
//              balance control system informations                     //
// Parameters:  uiInterval (uint32_t): broadcast interval time          //
// Returns:     n/a                                                     //
//**********************************************************************//
void vCommunicationStartBroadcast(uint32_t uiInterval)
{
	xComSettings.xBroadcast.pTimer->ARR = uiInterval;
	xComSettings.xBroadcast.uiEnable = SM_TRUE;
	HAL_TIM_Base_Start_IT(xComSettings.xBroadcast.htim);
}

//**********************************************************************//
// Method:      vCommunicationStopBroadcast                             //
// Description: stop the timer that periodically send a broadcast with  //
//              balance control system informations                     //
// Parameters:  n/a                                                     //
// Returns:     n/a                                                    *//
//********************************************************************* //
void vCommunicationStopBroadcast(void)
{
	HAL_TIM_Base_Stop_IT(xComSettings.xBroadcast.htim);
}

//**********************************************************************//
// Method:      vCommunicationStopBroadcast                             //
// Description: send a broadcast with all data of the balance control   //
//              system                                                  //
// Parameters:  n/a                                                     //
// Returns:     n/a                                                     //
//**********************************************************************//
void vCommunicationSendBroadcast(void)
{
	char cTemp[16];
	char cBuffer[256] = "#b";

	uint8_t uiCounter;
	uint8_t uiMaxCounter = xComSettings.xBroadcast.uiCounter;

	for(uiCounter = 0; uiCounter < uiMaxCounter; uiCounter++)
	{
		sprintf(cTemp, "%.2f;", *xComSettings.xBroadcast.pBuffer[uiCounter]);
		strcat(cBuffer, cTemp);
	}

	strcat(cBuffer, "\n\r");
	vCommunicationWrite(cBuffer);
}

//**********************************************************************//
// Method:      vCommunicationLPUART1Callback                           //
// Description: callback of UART interruption                           //
// Parameters:  n/a                                                     //
// Returns:     n/a                                                     //
//**********************************************************************//
void vCommunicationLPUART1Callback(void)
{
	vCommunicationRead(&cRxChar, 1);
	vCommunicationStateMachine(cRxChar);
}

