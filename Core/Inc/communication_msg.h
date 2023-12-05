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
extern actuatorSettings_t xActSettings;

void create(void){

	// Message ID: 00
	stateMachineData_t smAccelerometerX = {
		.fData      = &xSensorReadings.xIMU.xAccelerometer.fX,
		.eDataType  = FLOAT,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_FALSE
	};
	vCommunicationStateMachineAppendData(smAccelerometerX);

	// Message ID: 01
	stateMachineData_t smAccelerometerY = {
		.fData      = &xSensorReadings.xIMU.xAccelerometer.fY,
		.eDataType  = FLOAT,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_FALSE
	};
	vCommunicationStateMachineAppendData(smAccelerometerY);

	// Message ID: 02
	stateMachineData_t smAccelerometerZ = {
		.fData      = &xSensorReadings.xIMU.xAccelerometer.fZ,
		.eDataType  = FLOAT,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_FALSE
	};
	vCommunicationStateMachineAppendData(smAccelerometerZ);

	// Message ID: 03
	stateMachineData_t smGyroscopeX = {
		.fData      = &xSensorReadings.xIMU.xGyroscope.fX,
		.eDataType  = FLOAT,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_FALSE
	};
	vCommunicationStateMachineAppendData(smGyroscopeX);

	// Message ID: 04
	stateMachineData_t smGyroscopeY = {
		.fData      = &xSensorReadings.xIMU.xGyroscope.fY,
		.eDataType  = FLOAT,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_FALSE
	};
	vCommunicationStateMachineAppendData(smGyroscopeY);

	// Message ID: 05
	stateMachineData_t smGyroscopeZ = {
		.fData      = &xSensorReadings.xIMU.xGyroscope.fZ,
		.eDataType  = FLOAT,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_FALSE
	};
	vCommunicationStateMachineAppendData(smGyroscopeZ);

	// Message ID: 06
	stateMachineData_t smEncoderPosition = {
		.fData      = &xSensorReadings.xSuspensionMotor.fEncoderPosition,
		.eDataType  = FLOAT,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_FALSE
	};
	vCommunicationStateMachineAppendData(smEncoderPosition);

	// Message ID: 07
	stateMachineData_t smEncoderVelocity = {
		.fData      = &xSensorReadings.xSuspensionMotor.fEncoderVelocity,
		.eDataType  = FLOAT,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_FALSE
	};
	vCommunicationStateMachineAppendData(smEncoderVelocity);

	// Message ID: 08
	stateMachineData_t smBusCurrent = {
		.fData      = &xSensorReadings.xSuspensionMotor.fBusCurrent,
		.eDataType  = FLOAT,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_FALSE
	};
	vCommunicationStateMachineAppendData(smBusCurrent);

	// Message ID: 09
	stateMachineData_t smBusVoltage = {
		.fData      = &xSensorReadings.xSuspensionMotor.fBusVoltage,
		.eDataType  = FLOAT,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_FALSE
	};
	vCommunicationStateMachineAppendData(smBusVoltage);

	// Message ID: 10
	stateMachineData_t smIqMeasured = {
		.fData      = &xSensorReadings.xSuspensionMotor.fIqMeasured,
		.eDataType  = FLOAT,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_FALSE
	};
	vCommunicationStateMachineAppendData(smIqMeasured);

	// Message ID: 11
	stateMachineData_t smIqSetpoint = {
		.fData      = &xSensorReadings.xSuspensionMotor.fIqSetpoint,
		.eDataType  = FLOAT,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_FALSE
	};
	vCommunicationStateMachineAppendData(smIqSetpoint);

	// Message ID: 12
	stateMachineData_t smMotorTemperature = {
		.fData      = &xSensorReadings.xSuspensionMotor.fTemperature,
		.eDataType  = FLOAT,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_FALSE
	};
	vCommunicationStateMachineAppendData(smMotorTemperature);

	// Message ID: 13
	stateMachineData_t smMotorControlPositionTarget = {
		.fData      = &xSuspensionMotor.fPositionTarget,
		.eDataType  = FLOAT,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_TRUE,
		.fLowerLimit = -10000,
		.fUpperLimit =  10000,
	};
	vCommunicationStateMachineAppendData(smMotorControlPositionTarget);

	// Message ID: 14
	stateMachineData_t smMotorControlTorqueTarget = {
		.fData      = &xSuspensionMotor.fTorqueTarget,
		.eDataType  = FLOAT,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_TRUE,
		.fLowerLimit = -200,
		.fUpperLimit =  200,
	};
	vCommunicationStateMachineAppendData(smMotorControlTorqueTarget);

	// Message ID: 15
	stateMachineData_t smMotorControlOperationMode = {
		.iData      = &iSMChangeMotorMode,
		.eDataType  = INT,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_TRUE,
		.fLowerLimit = 0,
		.fUpperLimit = 6,
	};
	vCommunicationStateMachineAppendData(smMotorControlOperationMode);

	// Message ID: 16
	stateMachineData_t smLatitude = {
		.fData      = &xSensorReadings.xGPS.fLatitude,
		.eDataType  = FLOAT,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_FALSE
	};
	vCommunicationStateMachineAppendData(smLatitude);

	// Message ID: 17
	stateMachineData_t smLatitudeRef = {
		.cData      = &xSensorReadings.xGPS.cLatitudeRef,
		.eDataType  = CHAR,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_FALSE
	};
	vCommunicationStateMachineAppendData(smLatitudeRef);

	// Message ID: 18
	stateMachineData_t smLongitude = {
		.fData      = &xSensorReadings.xGPS.fLongitude,
		.eDataType  = FLOAT,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_FALSE
	};
	vCommunicationStateMachineAppendData(smLongitude);

	// Message ID: 19
	stateMachineData_t smLongitudeRef = {
		.cData      = &xSensorReadings.xGPS.cLongitudeRef,
		.eDataType  = CHAR,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_FALSE
	};
	vCommunicationStateMachineAppendData(smLongitudeRef);

	// Message ID: 20
	stateMachineData_t smGPSSpeed = {
		.fData      = &xSensorReadings.xGPS.fSpeed,
		.eDataType  = FLOAT,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_FALSE
	};
	vCommunicationStateMachineAppendData(smGPSSpeed);

	// Message ID: 21
	stateMachineData_t smActuatorSinFrequency = {
		.fData      = &xActSettings.sin.fFrequency,
		.eDataType  = FLOAT,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_TRUE,
		.fLowerLimit = 0,
		.fUpperLimit = MAX_FREQUENCY_HZ,
	};
	vCommunicationStateMachineAppendData(smActuatorSinFrequency);

	// Message ID: 22
	stateMachineData_t smActuatorSinAmplitude = {
		.fData      = &xActSettings.sin.fMagnitude,
		.eDataType  = FLOAT,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_TRUE,
		.fLowerLimit = 0,
		.fUpperLimit = MAX_MAGNITUDE_RAD,
	};
	vCommunicationStateMachineAppendData(smActuatorSinAmplitude);

	// Message ID: 23
	stateMachineData_t smActuatorSinOffset = {
		.fData      = &xActSettings.sin.fMeanValue,
		.eDataType  = FLOAT,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_TRUE,
		.fLowerLimit = 0,
		.fUpperLimit = MAX_MAGNITUDE_RAD,
	};
	vCommunicationStateMachineAppendData(smActuatorSinOffset);

	// Message ID: 24
	stateMachineData_t smActuatorSinPhase = {
		.fData      = &xActSettings.sin.fPhase,
		.eDataType  = FLOAT,
		.uiReadable = SM_TRUE,
		.uiWritable = SM_TRUE,
		.fLowerLimit = -6.28,
		.fUpperLimit =  6.28,
	};
	vCommunicationStateMachineAppendData(smActuatorSinPhase);

}

#endif /* INC_COMMUNICATION_MSG_H_ */
