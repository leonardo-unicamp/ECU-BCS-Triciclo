//**********************************************************************//
// File:        sensorReadings.c                                        //
// Description: Source file that contains the functions and methods     //
//              implementations for handling with sensor readings task  //
// Author:      Leonardo Rossi Leao                                     //
// Reviewer:    Rodrigo Moreira Bacurau                                 //
// Last update: 2023-11-13                                              //
//**********************************************************************//

#include "sensorReadings.h"

extern UART_HandleTypeDef huart6;
extern Axis xAxes[2];
extern GPS_GPRMC_t xGPRMC;

SCHA63TOutput SCHA63TData;
sensorReadings_t xSensorReadings;

void vSensorReadingsInit(void){

	// initialize IMU device
	char cImuInit = cSCHA63TInit(
		IMU1_CS_UNO_GPIO_Port,
		IMU1_CS_UNO_Pin,
		IMU1_CS_DUE_GPIO_Port,
		IMU1_CS_DUE_Pin,
		&hspi4
	);

	if (!cImuInit)
		HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_SET);

	// Update gyroscopes offset
	cSCHA63TOffsetCalculation();
}

//**********************************************************************//
// Method:      vSensorReadingsStart                                    //
// Description: Entrypoint for sensor readings task. Use libraries to   //
//              acquisite sensor data and store into a data struct      //
// Input:       none                                                    //
// Output:      none                                                    //
//**********************************************************************//
void vSensorReadingsStart()
{
	while (1)
	{
		// Get data from motor
		Get_Encoder_Estimates(xAxes[0]);
		xSensorReadings.xSuspensionMotor.fEncoderPosition = xAxes[0].AXIS_Encoder_Pos;
		xSensorReadings.xSuspensionMotor.fEncoderVelocity = xAxes[0].AXIS_Encoder_Vel;

		Get_IQ(xAxes[0]);
		xSensorReadings.xSuspensionMotor.fIqMeasured = xAxes[0].AXIS_Iq_Measured;
		xSensorReadings.xSuspensionMotor.fIqSetpoint = xAxes[0].AXIS_Iq_Setpoint;

		Get_Temperature(xAxes[0]);
		xSensorReadings.xSuspensionMotor.fIqSetpoint = xAxes[0].AXIS_Motor_Temperature;

		Get_Bus_Voltage_Current(xAxes[0]);
		xSensorReadings.xSuspensionMotor.fBusCurrent = xAxes[0].AXIS_Bus_Current;
		xSensorReadings.xSuspensionMotor.fBusVoltage = xAxes[0].AXIS_Bus_Voltage;

		// Get data from IMU
		cSCHA63TRead(&SCHA63TData);
		xSensorReadings.xIMU.xAccelerometer.fX = SCHA63TData.data_accl[0];
		xSensorReadings.xIMU.xAccelerometer.fY = SCHA63TData.data_accl[1];
		xSensorReadings.xIMU.xAccelerometer.fZ = SCHA63TData.data_accl[2];
		xSensorReadings.xIMU.xGyroscope.fX = SCHA63TData.data_gyro[0];
		xSensorReadings.xIMU.xGyroscope.fY = SCHA63TData.data_gyro[1];
		xSensorReadings.xIMU.xGyroscope.fZ = SCHA63TData.data_gyro[2];
		xSensorReadings.xIMU.fTemperature = SCHA63TData.temperature;

		// Get data from GPS
		xSensorReadings.xGPS.fLatitude     = xGPRMC.fLatitude;
		xSensorReadings.xGPS.cLatitudeRef  = xGPRMC.cLatitudeRef;
		xSensorReadings.xGPS.fLongitude    = xGPRMC.fLongitude;
		xSensorReadings.xGPS.cLongitudeRef = xGPRMC.cLongitudeRef;
		xSensorReadings.xGPS.fSpeed        = xGPRMC.fSpeed;

		osDelay(10);
	}
}
