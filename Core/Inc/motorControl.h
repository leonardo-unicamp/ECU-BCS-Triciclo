//**********************************************************************//
// File:        motorControl.c                                          //
// Description: Source file that contains the functions and methods     //
//              implementations for motor control task                  //
// Author:      Leonardo Rossi Leao                                     //
// Reviewer:    Rodrigo Moreira Bacurau                                 //
// Last Update: 2023-11-13                                              //
//**********************************************************************//

#ifndef INC_MOTORCONTROL_H_
#define INC_MOTORCONTROL_H_

#include "fdcan.h"
#include "pid.h"
#include "cmsis_os.h"
#include "ODrive.h"
#include "sensorReadings.h"

#define ACCELERATION_TARGET 0.0

enum MotorType {
	SUSPENSION,
	HANDLEBARS
};

enum MotorModes {
	MM_FREE,
	MM_POSITION_CONTROL,
	MM_TORQUE_CONTROL,
	MM_ACCELERATION_CONTROL,
	MM_EMERGENCY,
	MM_SIN_MOVEMENT,
	MM_CHIRP_MOVEMENT,
};

typedef struct MotorSettings {
	enum MotorType xMotorType;
	enum MotorModes xOperationMode;
	Control_Mode xControlMode;
	float fPositionTarget;
	float fTorqueTarget;
} motorSettings_t;


//**********************************************************************//
// Method:      vMotorControlInit                                       //
// Description: Initialize motor control variables and peripherals      //
// Input:       none                                                    //
// Output:      none                                                    //
//**********************************************************************//
void vMotorControlInit(void);

//**********************************************************************//
// Method:      vMotorControlStart                                      //
// Description: Entrypoint to motor control task in line follower robot //
// Input:       none                                                    //
// Output:      none                                                    //
//**********************************************************************//
void vMotorControlStart();

//**********************************************************************//
// Method:      vMotorControlSetLeftDuty                                //
// Description: Set a position to suspension motor                      //
// Input:       float: position to be setted in radians                 //
// Output:      none                                                    //
//**********************************************************************//
void vMotorControlSetSuspensionPosition(float);

//**********************************************************************//
// Method:      vMotorControlSetLeftDuty                                //
// Description: Set a torque to suspension motor                        //
// Input:       float fTorque: torque to be setted in Newton            //
// Output:      none                                                    //
//**********************************************************************//
void vMotorControlSetSuspensionTorque(float);

//**********************************************************************//
// Method:      vMotorControlSetMode                                    //
// Description: Set a new operation mode to motor control               //
// Input:       enum MotorModes xMode: desired motor control mode       //
// Output:      none                                                    //
//**********************************************************************//
void vMotorControlSetSuspensionMode(enum MotorModes);
#endif /* INC_MOTORCONTROL_H_ */
