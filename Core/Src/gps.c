#include "gps.h"

uint8_t uiRxByte;
uint8_t uiRxCounter;
char cRxBuffer[80];
char uiOption[5];

GPS_GPGGA_t xGPGGA = (GPS_GPGGA_t){0};
GPS_GPRMC_t xGPRMC = (GPS_GPRMC_t){0};

void vGPSUartCallback(void)
{
	HAL_UART_Receive_IT(&huart6, &uiRxByte, 1);
	if(uiRxByte == '$')
	{
		sscanf(cRxBuffer, "$%5s", uiOption);

		if(strcmp(uiOption, "GPRMC") == 0)
			vGPSParseGPRMC(cRxBuffer);
		else if(strcmp(uiOption, "GPGGA") == 0)
			vGPSParseGPGGA(cRxBuffer);

		uiRxCounter = 0;
		memset(cRxBuffer, '\0', 80);
	}
	cRxBuffer[uiRxCounter] = uiRxByte;
	uiRxCounter++;
}

void vGPSParseGPRMC(char *cBuffer)
{
	sscanf(cRxBuffer, "$GPRMC,%2d%2d%2d.%d,%c,%f,%c,%f,%c,%f,%f,%2d%2d%2d,%f,%c*%2s",
		   &xGPRMC.uiHour, &xGPRMC.uiMinutes, &xGPRMC.uiSeconds, &xGPRMC.uiMicrosseconds,
		   &xGPRMC.cStatus, &xGPRMC.fLatitude, &xGPRMC.cLatitudeRef, &xGPRMC.fLongitude,
		   &xGPRMC.cLongitudeRef, &xGPRMC.fSpeed, &xGPRMC.fAngle, &xGPRMC.uiDate,
		   &xGPRMC.uiMonth, &xGPRMC.uiYear, &xGPRMC.fMagneticVariation, &xGPRMC.cMagneticRef,
		   xGPRMC.cChecksum);

	// Convert knots to km/h
	xGPRMC.fSpeed *= 1.852;
}

void vGPSParseGPGGA(char *cBuffer)
{
	sscanf(cRxBuffer, "$GPGGA,%2d%2d%2d.%d,%f,%c,%f,%c,%d,%d,%f,%f,%c,%f,%c",
		   &xGPGGA.uiHour, &xGPGGA.uiMinutes, &xGPGGA.uiSeconds, &xGPGGA.uiMicrosseconds,
		   &xGPGGA.fLatitude, &xGPGGA.cLatitudeRef, &xGPGGA.fLongitude, &xGPGGA.cLongitudeRef,
		   &xGPGGA.uiGPSFix, &xGPGGA.uiNumberSatellites, &xGPGGA.fHorizontalDilution,
		   &xGPGGA.fAltitude, &xGPGGA.cUnitAltitude, &xGPGGA.fGeoidHeight, &xGPGGA.cUnitHeight);
}
