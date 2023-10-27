//**********************************************************************//
// File:        actuator.h                                              //
// Description: properties, typdefs e methods declarations for motor    //
//              actuator in the balance control system                  //
// Author:      Leonardo Rossi Leao                                     //
// Reviewer:    Rodrigo Moreira Bacurau                                 //
// Last update: 2023-10-19                                              //
//**********************************************************************//

#ifndef INC_ACTUATOR_H_
#define INC_ACTUATOR_H_

#include "ODrive.h"
#include "math.h"
#include "pid.h"

#define ACT_TORQUE_CONTROL    0
#define ACT_POSITION_CONTROL  1

#define MAX_MAGNITUDE_RAD  8*M_PI/180
#define MAX_FREQUENCY_HZ   1

// Enum that contains all motions type
typedef enum actMotionTypes {
	SIN,
	CHIRP,
} actMotionTypes_t;

typedef struct sinMovement {
	// Equation: magnitude*sin(period*(t+phase)) + shift
	float fFrequency;
	float fMagnitude;
	float fPhase;
	float fMeanValue;
} sinMovement_t;

typedef struct chirpMovement {
	// Equation: magnitude*cos(alpha*t^2 + beta*t + phase) + shift
	float fAlpha;
	float fBeta;
	float fMagnitude;
	float fPhase;
	float fMaxFreq;
	float fMeanValue;
} chirpMovement_t;

typedef struct actuatorSettings {
	Axis xMotor;
	uint8_t uiControlType;
	actMotionTypes_t uiMotionType;
	sinMovement_t sin;
	chirpMovement_t chirp;
} actuatorSettings_t;

//**********************************************************************//
// Method:      vActuatorInit                                           //
// Description: Initialize actuator mode                                //
// Parameters:  Axis: motor which control will be aplied                //
// Returns:     n/a                                                     //
//**********************************************************************//
void vActuatorInit(Axis);

//**********************************************************************//
// Method:      vActuatorConfigSin                                      //
// Description: Configure a sine function to be applyed by the actuator //
// Parameters:  float: sine frequency                                   //
//              float: sine magnitude                                   //
//              float: sine horizontal phase                            //
//              float: sine vertical phase                              //
// Returns:     n/a                                                     //
//**********************************************************************//
void vActuatorConfigSin(float, float, float, float);

//**********************************************************************//
// Method:      vActuatorConfigChirp                                    //
// Description: Configure a sine function to be applyed by the actuator //
// Parameters:  float: chirp alpha constant                             //
//              float: chirp beta constant                              //
//              float: chirp magnitude                                  //
//              float: chirp horizontal phase                           //
//              float: chirp vertical phase                             //
//              float: chirp maximum reachead frequency                 //
// Returns:     n/a                                                     //
//**********************************************************************//
void vActuatorConfigChirp(float, float, float, float, float, float);

//**********************************************************************//
// Method:      vActuatorPositionControlStart                           //
// Description: Start to control motor position with the motion type    //
//              choosed by the user                                     //
// Parameters:  actMotionTypes_t: actuator motion type (SIN/CHIRP)      //
// Returns:     n/a                                                     //
//**********************************************************************//
void vActuatorPositionControlStart(actMotionTypes_t);

//**********************************************************************//
// Method:      vActuatorPositionControlStop                            //
// Description: Stop to control motor position with the motion type     //
//              choosed by the user and go back to torque control       //
// Parameters:  n/a                                                     //
// Returns:     n/a                                                     //
//**********************************************************************//
void vActuatorPositionControlStop(void);

//**********************************************************************//
// Method:      vActuatorTorqueControlStart                             //
// Description: Start to control motor torque using acceleration        //
//              feedback control                                        //
// Parameters:  n/a                                                     //
// Returns:     n/a                                                     //
//**********************************************************************//
void vActuatorTorqueControlStart(void);

//**********************************************************************//
// Method:      vActuatorTorqueControlStart                             //
// Description: Stop to control motor torque using acceleration         //
//              feedback control                                        //
// Parameters:  n/a                                                     //
// Returns:     n/a                                                     //
//**********************************************************************//
void vActuatorTorqueControlStop(void);

//**********************************************************************//
// Method:      vActuadorRun                                            //
// Description: Manage the type of engine actuation (position or torque //
//              control)                                                //
// Parameters:  n/a                                                     //
// Returns:     n/a                                                     //
//**********************************************************************//
void vActuadorRun(void);

#endif /* INC_ACTUATOR_H_ */
