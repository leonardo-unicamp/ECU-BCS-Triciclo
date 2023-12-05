//**********************************************************************//
// File:        motorControl.c                                          //
// Description: Source file that contains the functions and methods     //
//              implementations for motor control task                  //
// Author:      Leonardo Rossi Leao                                     //
// Reviewer:    Rodrigo Moreira Bacurau                                 //
// Last Update: 2023-11-13                                              //
//**********************************************************************//

#include "actuator.h"
#include "motorControl.h"
#include "communication.h"

#define FALSE  0
#define TRUE   1
#define MAX_SIGNAL_BUFFER_SIZE   2048
#define RAD2DEG(angleInRadians)  ((angleInRadians) * 180.0 / M_PI)

#define ACCELERATION_TARGET 0.0
#define RUN_MOTOR_CONTROL   0b01

Axis xAxes[2];
motorSettings_t xSuspensionMotor;
motorSettings_t xHandlebarsMotor;
actuatorSettings_t xActSettings;

float fSuspensionPIDTorque;
float fActuatorPosition;
float fSinBuffer[MAX_SIGNAL_BUFFER_SIZE];
float fChirpBuffer[MAX_SIGNAL_BUFFER_SIZE];
unsigned int uiCounter;
unsigned int uiMaxBufferCounter;
uint8_t uiAllowPositionControl;
enum MotorModes xLastMode;

int iSMChangeMotorMode;
int streamCounter = 0;

extern sensorReadings_t xSensorReadings;


//**********************************************************************//
// Method:      vMotorControlInit                                       //
// Description: Initialize motor control variables and peripherals      //
// Input:       none                                                    //
// Output:      none                                                    //
//**********************************************************************//
void vMotorControlInit(void)
{
    xAxes[SUSPENSION].AxisID        = 2;
    xAxes[SUSPENSION].CanInstance   = &hfdcan1;
    xSuspensionMotor.xMotorType     = SUSPENSION;
    xSuspensionMotor.xOperationMode = MM_FREE;
    xSuspensionMotor.xControlMode   = TORQUE_CONTROL;

    xAxes[HANDLEBARS].AxisID = 3;
    xAxes[HANDLEBARS].CanInstance = &hfdcan1;
    xHandlebarsMotor.xMotorType     = HANDLEBARS;
    xHandlebarsMotor.xOperationMode = MM_FREE;
    xHandlebarsMotor.xControlMode   = TORQUE_CONTROL;

    CAN_Setup(xAxes, 1);
    pid_init(64, 37, 2.0, 400, 200);

    //Initialize Motor Control Driver (ODrive)
    Set_Input_Pos(xAxes[SUSPENSION], 0, 0, 0);
    Reboot_ODrive(xAxes[SUSPENSION]);
    osDelay(3000);
    Set_Limits(xAxes[SUSPENSION], 96, 120); //96 turns/s, 120 A
    osDelay(10);
    Set_Controller_Modes(xAxes[SUSPENSION], TORQUE_CONTROL, PASSTHROUGH);
    osDelay(10);
    Set_Axis_Requested_State(xAxes[SUSPENSION], CLOSED_LOOP_CONTROL);
    osDelay(10);

    iSMChangeMotorMode = MM_FREE;
}

//**********************************************************************//
// Method:      vMotorControlStart                                      //
// Description: Entrypoint to motor control task in line follower robot //
// Input:       none                                                    //
// Output:      none                                                    //
//**********************************************************************//
void vMotorControlStart()
{
	unsigned int uiSinMovementCounter;

    while(1)
    {
    	// Changing operation mode by State Machine
    	if(iSMChangeMotorMode != xSuspensionMotor.xOperationMode)
    	{
    		xSuspensionMotor.xOperationMode = (enum MotorModes)iSMChangeMotorMode;
    		if(iSMChangeMotorMode == MM_FREE || iSMChangeMotorMode == MM_TORQUE_CONTROL ||
    		   iSMChangeMotorMode == MM_ACCELERATION_CONTROL)
    		{
    		    Set_Controller_Modes(xAxes[SUSPENSION], TORQUE_CONTROL, PASSTHROUGH);
    		}
    		else if(iSMChangeMotorMode == MM_POSITION_CONTROL || iSMChangeMotorMode == MM_SIN_MOVEMENT)
    		{
    		    Set_Controller_Modes(xAxes[SUSPENSION], POSITION_CONTROL, PASSTHROUGH);
    		}
    	}

        switch(xSuspensionMotor.xOperationMode)
        {
            case MM_FREE:
                Set_Input_Torque(xAxes[SUSPENSION], 0);
                xLastMode = MM_FREE;
                break;

            case MM_POSITION_CONTROL:
                Set_Input_Pos(xAxes[SUSPENSION], xSuspensionMotor.fPositionTarget, 96, 3.86);
                xLastMode = MM_POSITION_CONTROL;
                break;

            case MM_TORQUE_CONTROL:
                Set_Input_Torque(xAxes[SUSPENSION], xSuspensionMotor.fTorqueTarget);
                xLastMode = MM_TORQUE_CONTROL;
                break;

            case MM_ACCELERATION_CONTROL:
                xSuspensionMotor.fTorqueTarget = pidUpdateData(0, ACCELERATION_TARGET);
                Set_Input_Torque(xAxes[SUSPENSION], xSuspensionMotor.fTorqueTarget);
                xLastMode = MM_ACCELERATION_CONTROL;
                break;

            case MM_SIN_MOVEMENT:
            	if(xLastMode != MM_SIN_MOVEMENT)
            	{
            		uiSinMovementCounter = 0;
            		vActuatorConfigSin(
                		xActSettings.sin.fMagnitude,
            			xActSettings.sin.fFrequency,
            			xActSettings.sin.fPhase,
            			xActSettings.sin.fMeanValue
            		);
            	}

            	if(uiAllowPositionControl)
            	{
            		xSuspensionMotor.fPositionTarget = fSinBuffer[uiSinMovementCounter];
            		Set_Input_Pos(xAxes[SUSPENSION], xSuspensionMotor.fPositionTarget, 96, 3.86);
            		uiSinMovementCounter++;

            		if(uiSinMovementCounter >= uiMaxBufferCounter)
            			uiSinMovementCounter = 0;
            	}
            	xLastMode = MM_SIN_MOVEMENT;
            	break;

            default:
                break;
        }

        osDelay(10);
        vCommunicationSendBroadcast();
    }
}

//**********************************************************************//
// Method:      vMotorControlSetLeftDuty                                //
// Description: Set a position to suspension motor                      //
// Input:       float: position to be setted in radians                 //
// Output:      none                                                    //
//**********************************************************************//
void vMotorControlSetSuspensionPosition(float fPosition)
{
    xSuspensionMotor.fPositionTarget = fPosition;
}

//**********************************************************************//
// Method:      vMotorControlSetLeftDuty                                //
// Description: Set a torque to suspension motor                        //
// Input:       float fTorque: torque to be setted in Newton            //
// Output:      none                                                    //
//**********************************************************************//
void vMotorControlSetSuspensionTorque(float fTorque)
{
	xSuspensionMotor.fTorqueTarget = fTorque;
}

//**********************************************************************//
// Method:      vMotorControlSetMode                                    //
// Description: Set a new operation mode to motor control               //
// Input:       enum MotorModes xMode: desired motor control mode       //
// Output:      none                                                    //
//**********************************************************************//
void vMotorControlSetSuspensionMode(enum MotorModes xMode)
{
	xSuspensionMotor.xOperationMode = xMode;
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
