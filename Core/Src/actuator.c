//**********************************************************************//
// File:        actuator.h                                              //
// Description: properties, typdefs e methods declarations for motor    //
//              actuator in the balance control system                  //
// Author:      Leonardo Rossi Leao                                     //
// Reviewer:    Rodrigo Moreira Bacurau                                 //
// Last update: 2023-10-19                                              //
//**********************************************************************//

#include "actuator.h"

#define FALSE  0
#define TRUE   1
#define MAX_SIGNAL_BUFFER_SIZE   2048
#define RAD2DEG(angleInRadians)  ((angleInRadians) * 180.0 / M_PI)

actuatorSettings_t xActSettings;
extern float fActuadorTorque;
extern float fAccelerationMeasured;

unsigned int uiCounter;
unsigned int uiMaxBufferCounter;
float fActuatorPosition;
float fSinBuffer[MAX_SIGNAL_BUFFER_SIZE];
float fChirpBuffer[MAX_SIGNAL_BUFFER_SIZE];

uint8_t uiAllowPositionControl;

//**********************************************************************//
// Method:      vActuatorInit                                           //
// Description: Initialize actuator mode                                //
// Parameters:  Axis: motor which control will be aplied                //
// Returns:     n/a                                                     //
//**********************************************************************//
void vActuatorInit(Axis xMotor)
{
	xActSettings.xMotor = xMotor;
}

//**********************************************************************//
// Method:      vActuatorConfigSin                                      //
// Description: Configure a sine function to be applyed by the actuator //
// Parameters:  fMagnitude (float): sine magnitude                      //
//              fFrequency (float): sine frequency                      //
//              fPhase     (float): sine horizontal phase               //
//              fMeanValue (float): sine vertical phase                 //
// Returns:     n/a                                                     //
//**********************************************************************//
void vActuatorConfigSin(float fMagnitude, float fFrequency, float fPhase, float fMeanValue)
{
	int iCounter;
	float fTime;

	if((fMagnitude + fMeanValue) <= MAX_MAGNITUDE_RAD && fFrequency <= MAX_FREQUENCY_HZ)
	{
		xActSettings.sin.fFrequency = fFrequency;
		xActSettings.sin.fMagnitude = fMagnitude;
		xActSettings.sin.fPhase     = fPhase;
		xActSettings.sin.fMeanValue = fMeanValue;

		uiMaxBufferCounter = 1/(UPDATE_RATE*fFrequency);
		for(iCounter = 0; iCounter < uiMaxBufferCounter; iCounter++)
		{
			fTime = iCounter*UPDATE_RATE;
			fSinBuffer[iCounter] = fMagnitude*sin(2*M_PI*fFrequency*(fTime + fPhase)) + fMeanValue;
		}
		uiAllowPositionControl = TRUE;
	}
	else
		uiAllowPositionControl = FALSE;
}

//**********************************************************************//
// Method:      vActuatorConfigChirp                                    //
// Description: Configure a sine function to be applyed by the actuator //
// Parameters:  fAlpha     (float): chirp alpha constant                //
//              fBeta      (float): chirp beta constant                 //
//              fMagnitude (float): chirp magnitude                     //
//              fPhase     (float): chirp horizontal phase              //
//              fMeanValue (float): chirp vertical phase                //
//              fMaxFreq   (float): chirp maximum frequency             //
// Returns:     n/a                                                     //
//**********************************************************************//
void vActuatorConfigChirp(float fAlpha, float fBeta, float fMagnitude, float fPhase,
		float fMeanValue, float fMaxFreq)
{
	int iCounter;
	float fTime, fSignalFreq;

	xActSettings.chirp.fAlpha     = fAlpha;
	xActSettings.chirp.fBeta      = fBeta;
	xActSettings.chirp.fMagnitude = fMagnitude;
	xActSettings.chirp.fPhase     = fPhase;
	xActSettings.chirp.fMeanValue = fMeanValue;
	xActSettings.chirp.fMaxFreq   = fMaxFreq;

	while(iCounter < MAX_SIGNAL_BUFFER_SIZE && fSignalFreq <= MAX_FREQUENCY_HZ &&
			fSignalFreq <= fMaxFreq)
	{
		fTime = iCounter*UPDATE_RATE;
		fSignalFreq = fAlpha*fTime + fBeta;
		fSinBuffer[iCounter] = fMagnitude*(2*M_PI*fSignalFreq*(fTime + fPhase)) + fMeanValue;
		iCounter++;
	}

	// Go back to zero
	if(iCounter + 1 < MAX_SIGNAL_BUFFER_SIZE)
		fSinBuffer[iCounter] = 0;
}

//**********************************************************************//
// Method:      vActuatorPositionControlStart                           //
// Description: Start to control motor position with the motion type    //
//              choosed by the user                                     //
// Parameters:  actMotionTypes_t: actuator motion type (SIN/CHIRP)      //
// Returns:     n/a                                                     //
//**********************************************************************//
void vActuatorPositionControlStart(actMotionTypes_t eMotionType)
{
	if(uiAllowPositionControl)
	{
		Set_Controller_Modes(xActSettings.xMotor, POSITION_CONTROL, PASSTHROUGH);
		xActSettings.uiControlType = ACT_POSITION_CONTROL;
		xActSettings.uiMotionType = eMotionType;
		uiCounter = 0;
	}
}

//**********************************************************************//
// Method:      vActuatorPositionControlStop                            //
// Description: Stop to control motor position with the motion type     //
//              choosed by the user and go back to torque control       //
// Parameters:  n/a                                                     //
// Returns:     n/a                                                     //
//**********************************************************************//
void vActuatorPositionControlStop(void)
{
	// Ask to bacurau about this function
}

//**********************************************************************//
// Method:      vActuatorTorqueControlStart                             //
// Description: Start to control motor torque using acceleration        //
//              feedback control                                        //
// Parameters:  n/a                                                     //
// Returns:     n/a                                                     //
//**********************************************************************//
void vActuatorTorqueControlStart(void)
{
	Set_Controller_Modes(xActSettings.xMotor, TORQUE_CONTROL, PASSTHROUGH);
	xActSettings.uiControlType = ACT_TORQUE_CONTROL;
}

//**********************************************************************//
// Method:      vActuatorTorqueControlStart                             //
// Description: Stop to control motor torque using acceleration         //
//              feedback control                                        //
// Parameters:  n/a                                                     //
// Returns:     n/a                                                     //
//**********************************************************************//
void vActuatorTorqueControlStop(void)
{
	// Ask to bacurau about this function
}

//**********************************************************************//
// Method:      vActuadorRun                                            //
// Description: Manage the type of engine actuation (position or torque //
//              control)                                                //
// Parameters:  n/a                                                     //
// Returns:     n/a                                                     //
//**********************************************************************//
void vActuadorRun(void)
{
	switch(xActSettings.uiControlType)
	{
		case ACT_TORQUE_CONTROL:
			fActuadorTorque = pidUpdateData(fAccelerationMeasured, 0);
			Set_Input_Torque(xActSettings.xMotor, fActuadorTorque/50);
			break;

		case ACT_POSITION_CONTROL:
			if(uiCounter < uiMaxBufferCounter)
				fActuatorPosition = fSinBuffer[uiCounter++];
			else
				uiCounter = 0;
			//Set_Input_Pos(xActSettings.xMotor, fActuatorPosition, 96, 3.86);
			break;
	}
}
