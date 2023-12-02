//**********************************************************************//
// File:        BCS_main.h                                              //
// Description: bare metal code that execute that PID of the balance    //
//              control system and signal acquisition of the cycle-car  //
// Author:      Leonardo Rossi Leao                                     //
// Reviewer:    Rodrigo Moreira Bacurau                                 //
// Last update: 2023-10-17                                              //
//**********************************************************************//

#ifndef INC_BCS_MAIN_H_
#define INC_BCS_MAIN_H_

#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "eth.h"
#include "fdcan.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_otg.h"
#include "gpio.h"

#include "string.h"
#include "stdio.h"
#include "SCHA63T.h"
#include "ODrive.h"
#include "math.h"
#include "pid.h"
#include "communication.h"
#include "actuator.h"

#define gravity                           9.80665
#define degToRad(angleInDegrees)          ((angleInDegrees) * M_PI / 180.0)
#define radToDeg(angleInRadians)          ((angleInRadians) * 180.0 / M_PI)
#define angleToMotorTurns(angleInDegrees) (angleInDegrees/7.2)
#define motorTurnsToAngle(MotorTurns)     (MotorTurns*7.2)
#define iqToTorqueNM(iq)                  ((50*8.27/150)*iq)

typedef struct timerFlags {
	uint8_t uiCounter;
	uint8_t ui10ms;
	uint8_t ui100ms;
	uint8_t ui1000ms;
} timerFlags_t;

//**********************************************************************//
// Method:      vBCSInit                                                //
// Description: Initializes the peripherals necessary for the balance   //
//              control system to function                              //
// Parameters:  n/a                                                     //
// Returns:     n/a                                                     //
//**********************************************************************//
void vBCSInit(void);

//**********************************************************************//
// Method:      vBCSMain                                                //
// Description: Get data and control the balance of cycle-car for each  //
//              10ms by applying a PID control                          //
// Parameters:  n/a                                                     //
// Returns:     n/a                                                     //
//**********************************************************************//
void vBCSMain(void);

#endif /* INC_BCS_MAIN_H_ */
