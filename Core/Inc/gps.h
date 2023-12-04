/*
 * gps.h
 *
 *  Created on: Dec 3, 2023
 *      Author: leonardo
 */

#ifndef INC_GPS_H_
#define INC_GPS_H_

#include "stdint.h"
#include "usart.h"
#include "stdio.h"
#include "string.h"

typedef struct GPRMC {
	int   uiHour;
	int   uiMinutes;
	int   uiSeconds;
	int   uiMicrosseconds;
	int   uiDate;
	int   uiMonth;
	int   uiYear;
	char  cStatus;
	float fLatitude;
	char  cLatitudeRef;
	float fLongitude;
	char  cLongitudeRef;
	float fSpeed;
	float fAngle;
	float fMagneticVariation;
	char  cMagneticRef;
	char  cChecksum[3];
} GPS_GPRMC_t;

typedef struct GPGGA {
	int   uiHour;
	int   uiMinutes;
	int   uiSeconds;
	int   uiMicrosseconds;
	float fLatitude;
	char  cLatitudeRef;
	float fLongitude;
	char  cLongitudeRef;
	int   uiGPSFix;
	int   uiNumberSatellites;
	float fHorizontalDilution;
	float fAltitude;
	char  cUnitAltitude;
	float fGeoidHeight;
	char  cUnitHeight;
	char  cChecksum[3];
} GPS_GPGGA_t;


void vGPSUartCallback(void);
void vGPSParseGPRMC(char *);
void vGPSParseGPGGA(char *);


#endif /* INC_GPS_H_ */
