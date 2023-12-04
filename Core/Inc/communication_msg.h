//**********************************************************************//
// File:        communication.h                                         //
// Description: contains the declaration off all variables that are     //
//              manipulated by the communication state machine          //
// Author:      Leonardo Rossi Leao                                     //
// Reviewer:    Rodrigo Moreira Bacurau                                 //
// Last update: 2023-10-18                                              //
//**********************************************************************//

#ifndef INC_COMMUNICATION_MSG_H_
#define INC_COMMUNICATION_MSG_H_

#include "communication.h"

extern int iSMChangeMotorMode;
extern sensorReadings_t xSensorReadings;
extern motorSettings_t xSuspensionMotor;
extern motorSettings_t xHandlebarsMotor;

void create(void){

	stateMachineData_t smAccelerometerX = {
		.fData      = &xSensorReadings.xIMU.xAccelerometer.fX,
		.eDataType  = FLOAT,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_FALSE
	};
	vCommunicationStateMachineAppendData(smAccelerometerX);

	stateMachineData_t smAccelerometerY = {
		.fData      = &xSensorReadings.xIMU.xAccelerometer.fY,
		.eDataType  = FLOAT,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_FALSE
	};
	vCommunicationStateMachineAppendData(smAccelerometerY);

	stateMachineData_t smAccelerometerZ = {
		.fData      = &xSensorReadings.xIMU.xAccelerometer.fZ,
		.eDataType  = FLOAT,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_FALSE
	};
	vCommunicationStateMachineAppendData(smAccelerometerZ);

	stateMachineData_t smGyroscopeX = {
		.fData      = &xSensorReadings.xIMU.xGyroscope.fX,
		.eDataType  = FLOAT,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_FALSE
	};
	vCommunicationStateMachineAppendData(smGyroscopeX);

	stateMachineData_t smGyroscopeY = {
		.fData      = &xSensorReadings.xIMU.xGyroscope.fY,
		.eDataType  = FLOAT,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_FALSE
	};
	vCommunicationStateMachineAppendData(smGyroscopeY);

	stateMachineData_t smGyroscopeZ = {
		.fData      = &xSensorReadings.xIMU.xGyroscope.fZ,
		.eDataType  = FLOAT,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_FALSE
	};
	vCommunicationStateMachineAppendData(smGyroscopeZ);

	stateMachineData_t smEncoderPosition = {
		.fData      = &xSensorReadings.xSuspensionMotor.fEncoderPosition,
		.eDataType  = FLOAT,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_FALSE
	};
	vCommunicationStateMachineAppendData(smEncoderPosition);

	stateMachineData_t smEncoderVelocity = {
		.fData      = &xSensorReadings.xSuspensionMotor.fEncoderVelocity,
		.eDataType  = FLOAT,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_FALSE
	};
	vCommunicationStateMachineAppendData(smEncoderVelocity);

	stateMachineData_t smBusCurrent = {
		.fData      = &xSensorReadings.xSuspensionMotor.fBusCurrent,
		.eDataType  = FLOAT,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_FALSE
	};
	vCommunicationStateMachineAppendData(smBusCurrent);

	stateMachineData_t smBusVoltage = {
		.fData      = &xSensorReadings.xSuspensionMotor.fBusVoltage,
		.eDataType  = FLOAT,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_FALSE
	};
	vCommunicationStateMachineAppendData(smBusVoltage);

	stateMachineData_t smIqMeasured = {
		.fData      = &xSensorReadings.xSuspensionMotor.fIqMeasured,
		.eDataType  = FLOAT,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_FALSE
	};
	vCommunicationStateMachineAppendData(smIqMeasured);

	stateMachineData_t smIqSetpoint = {
		.fData      = &xSensorReadings.xSuspensionMotor.fIqSetpoint,
		.eDataType  = FLOAT,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_FALSE
	};
	vCommunicationStateMachineAppendData(smIqSetpoint);

	stateMachineData_t smMotorTemperature = {
		.fData      = &xSensorReadings.xSuspensionMotor.fTemperature,
		.eDataType  = FLOAT,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_FALSE
	};
	vCommunicationStateMachineAppendData(smMotorTemperature);

	stateMachineData_t smMotorControlPositionTarget = {
		.fData      = &xSuspensionMotor.fPositionTarget,
		.eDataType  = FLOAT,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_TRUE,
		.fLowerLimit = -10000,
		.fUpperLimit =  10000,
	};
	vCommunicationStateMachineAppendData(smMotorControlPositionTarget);

	stateMachineData_t smMotorControlTorqueTarget = {
		.fData      = &xSuspensionMotor.fTorqueTarget,
		.eDataType  = FLOAT,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_TRUE,
		.fLowerLimit = -200,
		.fUpperLimit =  200,
	};
	vCommunicationStateMachineAppendData(smMotorControlTorqueTarget);

	stateMachineData_t smMotorControlOperationMode = {
		.iData      = &iSMChangeMotorMode,
		.eDataType  = INT,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_TRUE,
		.fLowerLimit = 0,
		.fUpperLimit = 6,
	};
	vCommunicationStateMachineAppendData(smMotorControlOperationMode);

	stateMachineData_t smLatitude = {
		.fData      = &xSensorReadings.xGPS.fLatitude,
		.eDataType  = FLOAT,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_FALSE
	};
	vCommunicationStateMachineAppendData(smLatitude);

	stateMachineData_t smLatitudeRef = {
		.cData      = &xSensorReadings.xGPS.cLatitudeRef,
		.eDataType  = CHAR,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_FALSE
	};
	vCommunicationStateMachineAppendData(smLatitudeRef);

	stateMachineData_t smLongitude = {
		.fData      = &xSensorReadings.xGPS.fLongitude,
		.eDataType  = FLOAT,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_FALSE
	};
	vCommunicationStateMachineAppendData(smLongitude);

	stateMachineData_t smLongitudeRef = {
		.cData      = &xSensorReadings.xGPS.cLongitudeRef,
		.eDataType  = CHAR,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_FALSE
	};
	vCommunicationStateMachineAppendData(smLongitudeRef);

	stateMachineData_t smGPSSpeed = {
		.fData      = &xSensorReadings.xGPS.fSpeed,
		.eDataType  = CHAR,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_FALSE
	};
	vCommunicationStateMachineAppendData(smGPSSpeed);



}

#endif /* INC_COMMUNICATION_MSG_H_ */
