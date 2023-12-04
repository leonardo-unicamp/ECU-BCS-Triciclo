//**********************************************************************//
// File:        systemControl.c                                         //
// Description: Source file that contains the functions and methods     //
//              implementations for system control task                 //
// Author:      Leonardo Rossi Leao                                     //
// Reviewer:    Rodrigo Moreira Bacurau                                 //
// Last Update: 2023-11-13                                              //
//**********************************************************************//

#include "systemControl.h"

systemControlSettings_t xSystemControl;

//**********************************************************************//
// Method:      vSystemControlInit                                      //
// Description: Initialize system control variables and peripherals     //
// Input:       none                                                    //
// Output:      none                                                    //
//**********************************************************************//
void vSystemControlInit(void)
{
	xSystemControl.xOperationMode = Free;
	osThreadFlagsWait(sysBootAllowed, osFlagsWaitAll, osWaitForever);
}

//**********************************************************************//
// Method:      vSystemControlStart                                     //
// Description: Entrypoint to System Control task. Manage the system    //
//              modes of operation and it's an interface between all    //
//              system tasks                                            //
// Input:       none                                                    //
// Output:      none                                                    //
//**********************************************************************//
void vSystemControlStart()
{
	while(1)
	{
		osDelay(1000);
	}
}

//**********************************************************************//
// Method:      vSystemControlSetMode                                   //
// Description: Set a new operation mode to system control              //
// Input:       enum SystemControlModes: mode to be setted to system    //
// Output:      none                                                    //
//**********************************************************************//
void vSystemControlSetMode(enum SystemControlModes xMode)
{
	xSystemControl.xOperationMode = xMode;

	switch(xMode)
	{
		case Iddle:
			break;

		case Free:
			break;

		case Control:
			break;

		case Excited:
			break;

		case Emergency:
			break;
	}
}
