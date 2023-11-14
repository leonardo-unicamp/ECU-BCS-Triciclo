//**********************************************************************//
// File:        systemInit.h                                            //
// Description: Header file that contains the functions and methods for //
//              system initializing task                                //
// Author:      Leonardo Rossi Leao                                     //
// Reviewer:    Rodrigo Moreira Bacurau                                 //
// Last update: 2023-11-13                                              //
//**********************************************************************//

#ifndef INC_SYSTEMINIT_H_
#define INC_SYSTEMINIT_H_

// FreeRTOS settings
#include "cmsis_os.h"

// STM32 Default Includes
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "eth.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "math.h"
#include "gpio.h"
#include "stdio.h"
#include "usart.h"
#include "fdcan.h"
#include "string.h"
#include "usb_otg.h"

// System Libraries
#include "pid.h"
#include "ODrive.h"
#include "SCHA63T.h"
#include "actuator.h"
#include "IIRfilter.h"
#include "communication.h"
#include "sensorReadings.h"

#define gravity                           9.80665
#define degToRad(angleInDegrees)          ((angleInDegrees) * M_PI / 180.0)
#define radToDeg(angleInRadians)          ((angleInRadians) * 180.0 / M_PI)
#define angleToMotorTurns(angleInDegrees) (angleInDegrees / 7.2)
#define motorTurnsToAngle(MotorTurns)     (MotorTurns * 7.2)
#define iqToTorqueNM(iq)                  ((50 * 8.27 / 150) * iq)

//**********************************************************************//
// Method:      vSystemInitStart                                        //
// Description: Entrypoint to line follower robot initializing task     //
// Input:       none                                                    //
// Output:      none                                                    //
//**********************************************************************//
void vSystemInitStart();


#endif /* INC_SYSTEMINIT_H_ */
