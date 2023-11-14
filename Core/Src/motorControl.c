//**********************************************************************//
// File:        motorControl.c                                          //
// Description: Source file that contains the functions and methods     //
//              implementations for motor control task                  //
// Author:      Leonardo Rossi Leao                                     //
// Reviewer:    Rodrigo Moreira Bacurau                                 //
// Last Update: 2023-11-13                                              //
//**********************************************************************//

#include "motorControl.h"

#define ACCELERATION_TARGET 0.0

Axis xAxes[2];
motorSettings_t xSuspensionMotor;
motorSettings_t xHandlebarsMotor;
extern sensorReadings_t xSensorReadings;

float fSuspensionPIDTorque;

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

    // TODO: This previous work only for one motor
    CAN_Setup(xAxes, 1);
    pid_init(64, 37, 2.0, 400, 200);

    //Initialize Motor Control Driver (ODrive)
    Set_Input_Pos(xAxes[0], 0, 0, 0);
    Reboot_ODrive(xAxes[0]);
    osDelay(3000);
    Set_Limits(xAxes[0], 96, 120); //96 turns/s, 120 A
    osDelay(10);
    Set_Controller_Modes(xAxes[0], TORQUE_CONTROL, PASSTHROUGH);
    osDelay(10);
    Set_Axis_Requested_State(xAxes[0], CLOSED_LOOP_CONTROL);
    osDelay(10);
}

//**********************************************************************//
// Method:      vMotorControlStart                                      //
// Description: Entrypoint to motor control task in line follower robot //
// Input:       none                                                    //
// Output:      none                                                    //
//**********************************************************************//
void vMotorControlStart()
{
    while(1)
    {
        osThreadFlagsWait(RUN_MOTOR_CONTROL, osFlagsWaitAll, osWaitForever);

        switch(xSuspensionMotor.xOperationMode)
        {
            case MM_FREE:
                Set_Input_Torque(xAxes[SUSPENSION], 0);
                break;

            case MM_POSITION_CONTROL:
                Set_Input_Pos(xAxes[SUSPENSION], xSuspensionMotor.fPositionTarget, 96, 3.86);
                break;

            case MM_TORQUE_CONTROL:
                Set_Input_Torque(xAxes[SUSPENSION], xSuspensionMotor.fTorqueTarget);
                break;

            case MM_ACCELERATION_CONTROL:
                xSuspensionMotor.fTorqueTarget = pidUpdateData(
                    xSensorReadings.fSuspensionMotorAcceleration,
                    ACCELERATION_TARGET
                );
                Set_Input_Torque(xAxes[SUSPENSION], xSuspensionMotor.fTorqueTarget);
                break;

            case MM_EMERGENCY:
                // TODO: Execute a procedure to go smoothly to a safe position
                break;
                
            default:
                break;
        }
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

