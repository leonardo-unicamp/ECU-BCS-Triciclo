//**********************************************************************//
// File:        communication.h                                         //
// Description: contains the declaration off all variables that are     //
//              manipulated by the communication state machine          //
// Author:      Leonardo Rossi Leao                                     //
// Reviewer:    Rodrigo Moreira Bacurau                                 //
// Last update: 2023-10-18                                              //
//**********************************************************************//

#ifndef INC_COMMUNICATION_INIT_H_
#define INC_COMMUNICATION_INIT_H_

#include "communication.h"

extern Axis Axes[MAX_AXES];
extern SCHA63TOutput SCHA63TData;
extern pid_data_type pidConfig;

extern float fLastMotorSpeed;
extern float fMotorAcceleration;
extern float fActuadorTorque;
extern float fAccelerationMeasured;
extern float fAccelerationAngle;

stateMachineData_t smGyroscopeX;
stateMachineData_t smGyroscopeY;
stateMachineData_t smGyroscopeZ;
stateMachineData_t smAccelerationX;
stateMachineData_t smAccelerationY;
stateMachineData_t smAccelerationZ;
stateMachineData_t smEncoderPosition;
stateMachineData_t smEncoderVelocity;
stateMachineData_t smIqMeasured;
stateMachineData_t smIqSetpoint;
stateMachineData_t smMotorTemperature;
stateMachineData_t smFetTemperature;
stateMachineData_t smPidKp;
stateMachineData_t smPidKi;
stateMachineData_t smPidKd;
stateMachineData_t smMotorAcceleration;
stateMachineData_t smAccelerationMeasured;
stateMachineData_t smAccelerationAngle;
stateMachineData_t smActuadorTorque;

void create(void){
	smGyroscopeX.fData      = &SCHA63TData.data_gyro[0];
	smGyroscopeX.eDataType  = FLOAT;
	smGyroscopeX.uiReadable = SM_TRUE;
	smGyroscopeX.uiWritable = SM_FALSE;

	smGyroscopeY.fData      = &SCHA63TData.data_gyro[1];
	smGyroscopeY.eDataType  = FLOAT;
	smGyroscopeY.uiReadable = SM_TRUE;
	smGyroscopeY.uiWritable = SM_FALSE;

	smGyroscopeZ.fData      = &SCHA63TData.data_gyro[2];
	smGyroscopeZ.eDataType  = FLOAT;
	smGyroscopeZ.uiReadable = SM_TRUE;
	smGyroscopeZ.uiWritable = SM_FALSE;

	smAccelerationX.fData      = &SCHA63TData.data_accl[0];
	smAccelerationX.eDataType  = FLOAT;
	smAccelerationX.uiReadable = SM_TRUE;
	smAccelerationX.uiWritable = SM_FALSE;

	smAccelerationY.fData      = &SCHA63TData.data_accl[1];
	smAccelerationY.eDataType  = FLOAT;
	smAccelerationY.uiReadable = SM_TRUE;
	smAccelerationY.uiWritable = SM_FALSE;

	smAccelerationZ.fData      = &SCHA63TData.data_accl[2];
	smAccelerationZ.eDataType  = FLOAT;
	smAccelerationZ.uiReadable = SM_TRUE;
	smAccelerationZ.uiWritable = SM_FALSE;

	smEncoderPosition.fData      = &Axes[0].AXIS_Encoder_Pos;
	smEncoderPosition.eDataType  = FLOAT;
	smEncoderPosition.uiReadable = SM_TRUE;
	smEncoderPosition.uiWritable = SM_FALSE;

	smEncoderVelocity.fData      = &Axes[0].AXIS_Encoder_Vel;
	smEncoderVelocity.eDataType  = FLOAT;
	smEncoderVelocity.uiReadable = SM_TRUE;
	smEncoderVelocity.uiWritable = SM_FALSE;

	smIqMeasured.fData      = &Axes[0].AXIS_Iq_Measured;
	smIqMeasured.eDataType  = FLOAT;
	smIqMeasured.uiReadable = SM_TRUE;
	smIqMeasured.uiWritable = SM_FALSE;

	smIqSetpoint.fData      = &Axes[0].AXIS_Iq_Setpoint;
	smIqSetpoint.eDataType  = FLOAT;
	smIqSetpoint.uiReadable = SM_TRUE;
	smIqSetpoint.uiWritable = SM_FALSE;

	smMotorTemperature.fData      = &Axes[0].AXIS_Motor_Temperature;
	smMotorTemperature.eDataType  = FLOAT;
	smMotorTemperature.uiReadable = SM_TRUE;
	smMotorTemperature.uiWritable = SM_FALSE;

	smFetTemperature.fData      = &Axes[0].AXIS_FET_Temperature;
	smFetTemperature.eDataType  = FLOAT;
	smFetTemperature.uiReadable = SM_TRUE;
	smFetTemperature.uiWritable = SM_FALSE;

	smPidKp.fData      = &pidConfig.fKp;
	smPidKp.eDataType  = FLOAT;
	smPidKp.uiReadable = SM_TRUE;
	smPidKp.uiWritable = SM_TRUE;
	smPidKp.fLowerLimit = -100;
	smPidKp.fUpperLimit = +100;

	smPidKi.fData      = &pidConfig.fKp;
	smPidKi.eDataType  = FLOAT;
	smPidKi.uiReadable = SM_TRUE;
	smPidKi.uiWritable = SM_TRUE;
	smPidKi.fLowerLimit = -100;
	smPidKi.fUpperLimit = +100;

	smPidKd.fData      = &pidConfig.fKp;
	smPidKd.eDataType  = FLOAT;
	smPidKd.uiReadable = SM_TRUE;
	smPidKd.uiWritable = SM_TRUE;
	smPidKd.fLowerLimit = -100;
	smPidKd.fUpperLimit = +100;

	smMotorAcceleration.fData      = &fMotorAcceleration;
	smMotorAcceleration.eDataType  = FLOAT;
	smMotorAcceleration.uiReadable = SM_TRUE;
	smMotorAcceleration.uiWritable = SM_FALSE;

	smAccelerationMeasured.fData      = &fAccelerationMeasured;
	smAccelerationMeasured.eDataType  = FLOAT;
	smAccelerationMeasured.uiReadable = SM_TRUE;
	smAccelerationMeasured.uiWritable = SM_FALSE;

	smAccelerationAngle.fData      = &fAccelerationAngle;
	smAccelerationAngle.eDataType  = FLOAT;
	smAccelerationAngle.uiReadable = SM_TRUE;
	smAccelerationAngle.uiWritable = SM_FALSE;

	smActuadorTorque.fData      = &fActuadorTorque;
	smActuadorTorque.eDataType  = FLOAT;
	smActuadorTorque.uiReadable = SM_TRUE;
	smActuadorTorque.uiWritable = SM_FALSE;
}

#endif /* INC_COMMUNICATION_INIT_H_ */
