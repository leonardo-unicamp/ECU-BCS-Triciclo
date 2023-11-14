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

#include "cmsis_os.h"


typedef struct SensorsReadings {
	float fMotorAcceleration;
	float fActuatorTorque;
	float fIMUAcceleration;
	float fIMUAccelerationAngle;
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
