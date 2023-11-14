//**********************************************************************//
// File:        systemInit.h                                            //
// Description: Header file that contains the functions and methods for //
//              system control task                                     //
// Author:      Leonardo Rossi Leao                                     //
// Reviewer:    Rodrigo Moreira Bacurau                                 //
// Last update: 2023-11-13                                              //
//**********************************************************************//

#ifndef INC_SYSTEMCONTROL_H_
#define INC_SYSTEMCONTROL_H_

#include "cmsis_os.h"
#include "actuator.h"

#define sysBootAllowed 0b1

/*
 * Description of system operation modes:
 *     Iddle:
 *     Control:
 *     Excited:
 *     Emergency:
 *     Free:
 */
enum SystemControlModes {
	Iddle,
	Control,
	Excited,
	Emergency,
	Free,
};

typedef struct SystemControlSettings {
	enum SystemControlModes xOperationMode;
} systemControlSettings_t;

//**********************************************************************//
// Method:      vSystemControlInit                                      //
// Description: Initialize system control variables and peripherals     //
// Input:       none                                                    //
// Output:      none                                                    //
//**********************************************************************//
void vSystemControlInit(void);

//**********************************************************************//
// Method:      vSystemControlStart                                     //
// Description: Entrypoint to System Control task. Manage the system    //
//              modes of operation and it's an interface between all    //
//              system tasks                                            //
// Input:       none                                                    //
// Output:      none                                                    //
//**********************************************************************//
void vSystemControlStart(void);

//**********************************************************************//
// Method:      vSystemControlSetMode                                   //
// Description: Set a new operation mode to system control              //
// Input:       enum SystemControlModes: mode to be setted to system    //
// Output:      none                                                    //
//**********************************************************************//
void vSystemControlSetMode(enum SystemControlModes);


#endif /* INC_SYSTEMCONTROL_H_ */
