/*
 * SCHA63T.h
 *
 *  Created on: Sep 1, 2022
 *      Author: rodri
 */

#ifndef INC_SCHA63T_H_
#define INC_SCHA63T_H_

#include "main.h"

typedef struct{
	float data_gyro[3];
	float data_accl[3];
	float temperature;
	unsigned char status;
}SCHA63TOutput;

char cSCHA63TInit(GPIO_TypeDef *GPIOUnoCs_Port, uint16_t GPIOUnoCs_Pin,
		  GPIO_TypeDef *GPIODueCs_Port, uint16_t GPIODueCs_Pin,
		  SPI_HandleTypeDef *UnoDueSPI);

void cSCHA63TRead(SCHA63TOutput*);

void cSCHA63TOffsetCalculation();

void read_axis_table();

void set_rang_filt(uint8_t cs);

char test_init_end(uint8_t cs);

uint8_t read_stat(float *temp);

char read_reg(uint8_t cs, const uint8_t *reg, int n, uint16_t *data);

char write_reg(uint8_t cs, uint8_t reg, uint16_t data);

void write_read_spi(uint8_t cs, uint8_t *req, uint8_t *rsp);

uint8_t crc_spi(const uint8_t *data);

uint8_t cs_nmea(const char *buff, size_t len);

#endif /* INC_SCHA63T_H_ */
