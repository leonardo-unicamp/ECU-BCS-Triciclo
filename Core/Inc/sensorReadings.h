//**********************************************************************//
// File:        sensorReadings.h                                        //
// Description: Header file that contains the functions, methods, and   //
//              libraries used to sensor readings task                  //
// Author:      Leonardo Rossi Leao                                     //
// Reviewer:    Rodrigo Moreira Bacurau                                 //
// Last update: 2023-11-13                                              //
//**********************************************************************//

#ifndef INC_SENSORREADINGS_H_
#define INC_SENSORREADINGS_H_

#include "tim.h"
#include "gps.h"
#include "spi.h"
#include "cmsis_os.h"
#include "ODrive.h"
#include "SCHA63T.h"
#include "actuator.h"

struct xCoordinates {
	float fX;
	float fY;
	float fZ;
};

struct xIMU {
	struct xCoordinates xGyroscope;
	struct xCoordinates xAccelerometer;
	float fTemperature;
};

struct xMotor {
	float fEncoderPosition;
	float fEncoderVelocity;
	float fBusVoltage;
	float fBusCurrent;
	float fIqSetpoint;
	float fIqMeasured;
	float fTemperature;
};

struct xGPS {
	float fLatitude;
	char  cLatitudeRef;
	float fLongitude;
	char  cLongitudeRef;
	float fSpeed;
};

typedef struct SensorsReadings {
	struct xIMU   xIMU;
	struct xGPS   xGPS;
	struct xMotor xSuspensionMotor;
	struct xMotor xHandlebarsMotor;
} sensorReadings_t;


//**********************************************************************//
// Method:      vSensorReadingsInit                                     //
// Description: Start sensors readings peripherals and timers for       //
//              periodic data acquisition                               //
// Input:       none                                                    //
// Output:      none                                                    //
//**********************************************************************//
void vSensorReadingsInit(void);

//**********************************************************************//
// Method:      vSensorReadingStart                                     //
// Description: Entrypoint for sensor readings task. Use libraries to   //
//              acquisite sensor data and store into a data struct      //
// Input:       none                                                    //
// Output:      none                                                    //
//**********************************************************************//
void vSensorReadingsStart();

#endif /* INC_SENSORREADINGS_H_ */
