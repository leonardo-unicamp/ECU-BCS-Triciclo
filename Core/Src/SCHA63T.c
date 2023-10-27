/*
 * SCHA63T.c
 *
 *  Created on: Sep 1, 2022
 *      Author: rodri
 */

#include "SCHA63T.h"
#include "spi.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"


#define DEV_NAME     "SCHA63T-K03" // device name
#define FW_VER       "1.0.0"  // F/W version

#define CS_DUE       0        // SPI CS DUE
#define CS_UNO       1        // SPI CS UNO

#define G            0        // g
#define M_S    		 1        // m/s
#define ACCL_UNIT    M_S      // SCHA63T register DUE Rate Z

#define NUMBER_SAMPLES_OFFSET_CALCULATION       1000


#define REG_DUE_RZ   0x01     // SCHA63T register DUE Rate Z
#define REG_DUE_RY   0x03     // SCHA63T register UNO Rate Y
#define REG_DUE_RZ2  0x0B     // SCHA63T register UNO Rate Z2
#define REG_DUE_RY2  0x0D     // SCHA63T register UNO Rate Y2
#define REG_UNO_RX   0x01     // SCHA63T register DUE Rate X
#define REG_UNO_AX   0x04     // SCHA63T register DUE Acc X
#define REG_UNO_AY   0x05     // SCHA63T register DUE Acc Y
#define REG_UNO_AZ   0x06     // SCHA63T register DUE Acc Z
#define REG_UNO_RX2  0x0B     // SCHA63T register DUE Rate X2
#define REG_TEMP     0x07     // SCHA63T register temperature
#define REG_SUMSTAT  0x0E     // SCHA63T register summary status
#define REG_FILT_G   0x16     // SCHA63T register gyro filter control
#define REG_SYS_TEST 0x17     // SCHA63T register SYS_TEST
#define REG_RESET    0x18     // SCHA63T register reset control
#define REG_MODE     0x19     // SCHA63T register mode control
#define REG_FILT_A   0x1A     // SCHA63T register ACC filter control
#define REG_CID      0x1B     // SCHA63T register component ID
#define REG_TR2      0x1C     // SCHA63T register traceability 2
#define REG_TR0      0x1D     // SCHA63T register traceability 0
#define REG_TR1      0x1E     // SCHA63T register traceability 1
#define REG_SELBNK   0x1F     // SCHA63T register select bank


#define SCALE_GYRO (1.0/80)   // SCHA63T gyro scale (deg/s/LSB) (nominal)
#define SCALE_ACCL (1.0/4905) // SCHA63T accelerometer scale (g/LSB) (nominal)
#define SCALE_TEMP (1.0/30)   // SCHA63T temperature scale (C/LSB)


#define MAX_TRY_INIT 3        // max try of device initialization


#define BYTE_L(x)    ((x) & 0xFF)
#define BYTE_H(x)    (((x) >> 8) & 0xFF)
#define ROUND(x)     (long)((x) + 0.5)


// -----------------------------------------------------------------------------

GPIO_TypeDef *SCHA63TGPIOUnoCs_Port, *SCHA63TGPIODueCs_Port;
uint16_t SCHA63TGPIOUnoCs_Pin, SCHA63TGPIODueCs_Pin;
SPI_HandleTypeDef *SCHA63TUnoDueSPI;

volatile uint8_t samp_enable; // sampling enable (0:disable,1:enable)
volatile uint8_t rang_sel[] = {0, 0}; // dynamic range selection gyro/accel: 0=13Hz, 1=20Hz, 2=46Hz
volatile uint8_t filt_sel[] = {0, 0}; // filter selection gyro/accel: 0=13Hz, 1=20Hz, 2=46Hz

// cross-axis compensation table ([1] 2.6)
float Cx[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}; // gyro
float Bx[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}; // accel

// Stores the gyro offsets
float gyrosOffsets[3] = {0.0716127455, -0.104887523, -0.0191749521};

char cSCHA63TInit(GPIO_TypeDef *GPIOUnoCs_Port, uint16_t GPIOUnoCs_Pin,
				  GPIO_TypeDef *GPIODueCs_Port, uint16_t GPIODueCs_Pin,
				  SPI_HandleTypeDef *UnoDueSPI) {

  // Store the Pin and Ports of CS and SPI
  SCHA63TGPIOUnoCs_Port = GPIOUnoCs_Port;
  SCHA63TGPIOUnoCs_Pin = GPIOUnoCs_Pin;
  SCHA63TGPIODueCs_Port = GPIODueCs_Port;
  SCHA63TGPIODueCs_Pin = GPIODueCs_Pin;
  SCHA63TUnoDueSPI = UnoDueSPI;

  // Put in high chip select of UNO e DUE
  HAL_GPIO_WritePin(SCHA63TGPIOUnoCs_Port, SCHA63TGPIOUnoCs_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(SCHA63TGPIODueCs_Port, SCHA63TGPIODueCs_Pin, GPIO_PIN_SET);

  HAL_Delay(25);

  // set operation mode on
  write_reg(CS_DUE, REG_MODE, 0);
  write_reg(CS_DUE, REG_MODE, 0);
  write_reg(CS_UNO, REG_MODE, 0);
  HAL_Delay(70);

  // set UNO configuration
  set_rang_filt(CS_UNO);

  // reset DUE
  write_reg(CS_DUE, REG_RESET, 1);
  HAL_Delay(25);

  // set DUE operation mode on
  write_reg(CS_DUE, REG_MODE, 0);
  write_reg(CS_DUE, REG_MODE, 0);
  HAL_Delay(1);

  // set DUE configuration
  set_rang_filt(CS_DUE);

  for (int i = 0; i < MAX_TRY_INIT; i++) {
	HAL_Delay(525);

	// set EOI = 1
	write_reg(CS_UNO, REG_RESET, 2);
	write_reg(CS_DUE, REG_RESET, 2);

	if (test_init_end(CS_UNO) && test_init_end(CS_DUE)) {
	  // Enable
	  samp_enable = 1;
	  return 1;
	}

    write_reg(CS_UNO, REG_RESET, 1); // reset UNO/DUE
    write_reg(CS_DUE, REG_RESET, 1);
    HAL_Delay(25);

    // set operation mode on
	write_reg(CS_DUE, REG_MODE, 0);
	write_reg(CS_DUE, REG_MODE, 0);
	write_reg(CS_UNO, REG_MODE, 0);
	HAL_Delay(50);

	// set UNO configuration
	set_rang_filt(CS_UNO);

    // set DUE configuration
    set_rang_filt(CS_DUE);

    HAL_Delay(45);
  }

  return 0;
}

// read sensor data ------------------------------------------------------------
void cSCHA63TRead(SCHA63TOutput* SCHA63TData) {
  static const uint8_t reg1[] = {REG_DUE_RZ, REG_DUE_RY};
  static const uint8_t reg2[] = {REG_UNO_RX, REG_UNO_AX, REG_UNO_AY, REG_UNO_AZ};
  uint16_t data1[2], data2[4];

  //time_ms = (time_ms + 1000 / SAMP_RATE) % 86400000;

  if (!samp_enable) return;

  // read sensor registers
  if (!read_reg(CS_DUE, reg1, 2, data1) || !read_reg(CS_UNO, reg2, 4, data2))  {
    return;
  }
  float gyro[3], accl[3];
  gyro[0] = (int16_t)data2[0] * SCALE_GYRO * (rang_sel[0] ? 0.5  : 1.0);
  gyro[1] = (int16_t)data1[1] * SCALE_GYRO * (rang_sel[0] ? 0.5  : 1.0);
  gyro[2] = (int16_t)data1[0] * SCALE_GYRO * (rang_sel[0] ? 0.5  : 1.0);
  accl[0] = (int16_t)data2[1] * SCALE_ACCL * (rang_sel[1] ? 0.25 : 1.0) * (ACCL_UNIT ? 9.81 : 1.0);
  accl[1] = (int16_t)data2[2] * SCALE_ACCL * (rang_sel[1] ? 0.25 : 1.0) * (ACCL_UNIT ? 9.81 : 1.0);
  accl[2] = (int16_t)data2[3] * SCALE_ACCL * (rang_sel[1] ? 0.25 : 1.0) * (ACCL_UNIT ? 9.81 : 1.0);

  // cross-axis compensation ([1] 2.6)
  SCHA63TData->data_gyro[0] = Cx[0][0] * gyro[0] + Cx[0][1] * gyro[1] + Cx[0][2] * gyro[2] - gyrosOffsets[0];
  SCHA63TData->data_gyro[1] = Cx[1][0] * gyro[0] + Cx[1][1] * gyro[1] + Cx[1][2] * gyro[2] - gyrosOffsets[1];
  SCHA63TData->data_gyro[2] = Cx[2][0] * gyro[0] + Cx[2][1] * gyro[1] + Cx[2][2] * gyro[2] - gyrosOffsets[2];
  SCHA63TData->data_accl[0] = Bx[0][0] * accl[0] + Bx[0][1] * accl[1] + Bx[0][2] * accl[2];
  SCHA63TData->data_accl[1] = Bx[1][0] * accl[0] + Bx[1][1] * accl[1] + Bx[1][2] * accl[2];
  SCHA63TData->data_accl[2] = Bx[2][0] * accl[0] + Bx[2][1] * accl[1] + Bx[2][2] * accl[2];
  SCHA63TData->status =  read_stat(&SCHA63TData->temperature);

}

void cSCHA63TOffsetCalculation()
{
	unsigned int iCount;
	float gyrosOffsetsAccum[3] = {0, 0, 0};
	SCHA63TOutput SCHA63TData;

	gyrosOffsets[0] = 0;
	gyrosOffsets[1] = 0;
	gyrosOffsets[2] = 0;

	for(iCount = 0; iCount < NUMBER_SAMPLES_OFFSET_CALCULATION; iCount++)
	{
		cSCHA63TRead(&SCHA63TData);
		gyrosOffsetsAccum[0] += SCHA63TData.data_gyro[0];
		gyrosOffsetsAccum[1] += SCHA63TData.data_gyro[1];
		gyrosOffsetsAccum[2] += SCHA63TData.data_gyro[2];
	}

	gyrosOffsets[0] = gyrosOffsetsAccum[0] / NUMBER_SAMPLES_OFFSET_CALCULATION;
	gyrosOffsets[1] = gyrosOffsetsAccum[1] / NUMBER_SAMPLES_OFFSET_CALCULATION;
	gyrosOffsets[2] = gyrosOffsetsAccum[2] / NUMBER_SAMPLES_OFFSET_CALCULATION;
}

// read cross-axis compensation table ([1] 2.6) --------------------------------
void read_axis_table() {
  static const uint8_t reg_axis[] = // cross-axis compensation registers
    {0x0B, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x1B, 0x1C};
  uint8_t reg_mode = REG_MODE;
  uint16_t data1[1], data2[9];

  //trace(3, "read_axis_table\r\n");

  // enter test mode
  write_reg(CS_DUE, reg_mode, 0x10); // unlock code 010
  write_reg(CS_DUE, reg_mode, 0x08); // unlock code 001
  write_reg(CS_DUE, reg_mode, 0x20); // unlock code 100
  if (!read_reg(CS_DUE, &reg_mode, 1, data1) || (data1[0] & 0x7) != 0x7) {
    //trace(2, "test mode enter error\r\n");
    return;
  }
  write_reg(CS_DUE, REG_SELBNK, 5); // change bank to 5

  if (read_reg(CS_DUE, reg_axis, 9, data2)) {
    Cx[0][0] = (int8_t)BYTE_L(data2[0]) / 4096.0f + 1.0f; // Cxx
    Cx[0][1] = (int8_t)BYTE_H(data2[0]) / 4096.0f;        // Cxy
    Cx[0][2] = (int8_t)BYTE_L(data2[1]) / 4096.0f;        // Cxz
    Cx[1][0] = (int8_t)BYTE_H(data2[1]) / 4096.0f;        // Cyx
    Cx[1][1] = (int8_t)BYTE_L(data2[2]) / 4096.0f + 1.0f; // Cyy
    Cx[1][2] = (int8_t)BYTE_H(data2[2]) / 4096.0f;        // Cyz
    Cx[2][0] = (int8_t)BYTE_L(data2[3]) / 4096.0f;        // Czx
    Cx[2][1] = (int8_t)BYTE_H(data2[3]) / 4096.0f;        // Czy
    Cx[2][2] = (int8_t)BYTE_L(data2[4]) / 4096.0f + 1.0f; // Czz
    Bx[0][0] = (int8_t)BYTE_H(data2[4]) / 4096.0f + 1.0f; // bxx
    Bx[0][1] = (int8_t)BYTE_L(data2[5]) / 4096.0f;        // bxy
    Bx[0][2] = (int8_t)BYTE_H(data2[5]) / 4096.0f;        // bxz
    Bx[1][0] = (int8_t)BYTE_L(data2[6]) / 4096.0f;        // byx
    Bx[1][1] = (int8_t)BYTE_H(data2[6]) / 4096.0f + 1.0f; // byy
    Bx[1][2] = (int8_t)BYTE_L(data2[7]) / 4096.0f;        // byz
    Bx[2][0] = (int8_t)BYTE_H(data2[7]) / 4096.0f;        // bzx
    Bx[2][1] = (int8_t)BYTE_L(data2[8]) / 4096.0f;        // bzy
    Bx[2][2] = (int8_t)BYTE_H(data2[8]) / 4096.0f + 1.0f; // bzz

    //trace(3,"axis_reg=%04X %04X %04X %04X %04X %04X %04X %04X %04X\r\n",
    //  data2[0], data2[1], data2[2], data2[3], data2[4], data2[5], data2[6],
    //  data2[7], data2[8]);
  }
  write_reg(CS_DUE, REG_SELBNK, 0); // restore bank to 0
  write_reg(CS_DUE, REG_RESET, 1);  // reset DUE
  //HAL_Delay(25);
}

// set sensor dynamic ranges and filters ---------------------------------------
void set_rang_filt(uint8_t cs) {

  if(cs == CS_DUE) {
	  write_reg(CS_DUE, REG_FILT_G, // DUE: Rz2_DYN,Rz2_FILT,Ry2_DYN,Ry2_FILT
	              ((uint16_t)rang_sel[0] << 14) | ((uint16_t)filt_sel[0] << 11) | ((uint16_t)filt_sel[0] <<  8) |
	              ((uint16_t)rang_sel[0] <<  6) | ((uint16_t)filt_sel[0] <<  3) | ((uint16_t)filt_sel[0] <<  0));
  }
  else if(cs == CS_UNO) {
	  write_reg(CS_UNO, REG_FILT_G, // UNO: Rx2_DYN,Rx2_FILT
	              ((uint16_t)rang_sel[0] << 14) | ((uint16_t)filt_sel[0] << 11) | ((uint16_t)filt_sel[0] << 8));
	  write_reg(CS_UNO, REG_FILT_A, // UNO: Ax_DYN,Ax_FILT,Ay_DYN,Ay_FILT,Az_DYN,Az_FILT
			  ((uint16_t)rang_sel[1] << 11) | ((uint16_t)filt_sel[1] << 8) |
			  ((uint16_t)rang_sel[1] <<  7) | ((uint16_t)filt_sel[1] << 4) |
			  ((uint16_t)rang_sel[1] <<  3) | ((uint16_t)filt_sel[1] << 0));
  }
}

// test initialization completed -----------------------------------------------
char test_init_end(uint8_t cs) {
  uint8_t req[4] = {0}, rsp[4] = {0};

  // read summary status register
  req[0] = REG_SUMSTAT << 2;
  req[3] = crc_spi(req);
  write_read_spi(cs, req, rsp);
  write_read_spi(cs, req, rsp);
  HAL_Delay(3);
  write_read_spi(cs, req, rsp);
  return (rsp[0] & 0x3) == 0x1; // RS (return status) == 01
}



// read sensor status ----------------------------------------------------------
uint8_t read_stat(float *temp) {
  static const uint8_t reg1[] = {REG_TEMP, REG_SUMSTAT};
  static const uint8_t reg2[] = {REG_TEMP, REG_SUMSTAT};
  uint16_t data1[2], data2[2];

  // read status registers
  if (!read_reg(CS_DUE, reg1, 2, data1) || !read_reg(CS_UNO, reg2, 2, data2)) {
    return 0xFF; // error
  }
  temp[0] = 25.0 + (int16_t)data1[0] * SCALE_TEMP;
  temp[1] = 25.0 + (int16_t)data2[0] * SCALE_TEMP;

  // sensor summary status (0x00: all OK)
  return (uint8_t)(((~data2[1] >> 11) & 0x1) << 5 | // S_OK_Rx_F
                   ((~data1[1] >> 13) & 0x1) << 4 | // S_OK_Ry_F
                   ((~data1[1] >> 11) & 0x1) << 3 | // S_OK_Rz_F
                   ((~data2[1] >>  9) & 0x1) << 2 | // S_OK_Ax_F
                   ((~data2[1] >>  6) & 0x1) << 1 | // S_OK_Ay_F
                   ((~data2[1] >>  3) & 0x1) << 0); // S_OK_Az_F
}

// read registers of SCHA63T ---------------------------------------------------
char read_reg(uint8_t cs, const uint8_t *reg, int n, uint16_t *data) {
  uint8_t req[4] = {0}, rsp[4];

  HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);

  for (int i = 0; i <= n; i++) {
    if (i < n) {
      req[0] = (reg[i] & 0x1F) << 2; // reg read operation
      req[3] = crc_spi(req);
    }
    write_read_spi(cs, req, rsp);

    if (i > 0) {
      if (crc_spi(rsp) != rsp[3]) { // CRC error
        //trace(2, "read_reg CRC error: cs=%d reg=%02X\r\n", cs, reg[i]);
        return 0;
      }
      data[i-1] = ((uint16_t)rsp[1] << 8) | rsp[2];
    }
  }

  return 1;
}

// write register of SCHA63T ---------------------------------------------------
char write_reg(uint8_t cs, uint8_t reg, uint16_t data) {
  uint8_t req[4], rsp[4];

  req[0] = (uint8_t)0x80 | ((reg & 0x1F) << 2); // reg write operation
  req[1] = (uint8_t)BYTE_H(data);
  req[2] = (uint8_t)BYTE_L(data);
  req[3] = crc_spi(req);
  write_read_spi(cs, req, rsp);

  req[0] = (reg & 0x1F) << 2; // reg read operation
  req[1] = req[2] = 0;
  req[3] = crc_spi(req);
  write_read_spi(cs, req, rsp);

  if (crc_spi(rsp) != rsp[3]) {
    //trace(2, "write_reg CRC error: cs=%d reg=%02X\r\n", cs, reg);
    return 0;
  }
  return 1;
}


// write and read 32 bit SPI frame ---------------------------------------------
void write_read_spi(uint8_t cs, uint8_t *req, uint8_t *rsp) {

  if(cs == CS_UNO)
  	  HAL_GPIO_WritePin(SCHA63TGPIOUnoCs_Port, SCHA63TGPIOUnoCs_Pin, GPIO_PIN_RESET);
    else if(cs == CS_DUE)
  	  HAL_GPIO_WritePin(SCHA63TGPIODueCs_Port, SCHA63TGPIODueCs_Pin, GPIO_PIN_RESET);

  HAL_SPI_TransmitReceive(SCHA63TUnoDueSPI, req, rsp, 4, 100);

  if(cs == CS_UNO)
	  HAL_GPIO_WritePin(SCHA63TGPIOUnoCs_Port, SCHA63TGPIOUnoCs_Pin, GPIO_PIN_SET);
  else if(cs == CS_DUE)
	  HAL_GPIO_WritePin(SCHA63TGPIODueCs_Port, SCHA63TGPIODueCs_Pin, GPIO_PIN_SET);
}

// SPI frame CRC of SCHA63T ([1] 5.1.6) ----------------------------------------
uint8_t crc_spi(const uint8_t *data) {
  uint8_t crc = 0xFF;

  for (int i = 0; i < 3; i++) {
    for (int j = 7; j >= 0; j--) {
      uint8_t bit = crc >> 7;
      crc <<= 1;
      if (bit ^ ((data[i] >> j) & 0x01)) crc ^= 0x1D;
    }
  }
  return (uint8_t)~crc;
}

// NMEA checksum ---------------------------------------------------------------
uint8_t cs_nmea(const char *buff, size_t len) {
  uint8_t sum = 0;

  for (int i = 1; i < len; i++) {
    sum ^= buff[i];
  }
  return sum;
}
