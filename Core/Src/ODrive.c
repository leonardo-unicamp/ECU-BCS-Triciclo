/*
 * ODrive.c
 *
 *  Created on: 07-Jul-2022
 *      Author: sidiyer27
 */


#include "ODrive.h"
#include "fdcan.h"

unsigned char ucNumAxesConfigured = 0; // Start with no axes included


void CAN_Setup(Axis *Axes, unsigned char ucNumAxes)
{
	FDCAN_FilterTypeDef sFilterConfig1, sFilterConfig2;
	unsigned char uCont;

	// Limit the number of axes to MAX_AXES
	if(ucNumAxes > MAX_AXES)
		ucNumAxes = MAX_AXES;

	ucNumAxesConfigured = ucNumAxes;

	//Configure the FDCAN of each AXIS
	for(uCont = 0; uCont < ucNumAxes; uCont++)
	{
		if(Axes[uCont].CanInstance == &hfdcan1)
		{
			/* Configure standard ID reception filter to Rx buffer 0 */
			sFilterConfig1.IdType = FDCAN_STANDARD_ID;
			sFilterConfig1.FilterIndex = 0;
			sFilterConfig1.FilterType = FDCAN_FILTER_MASK;
			sFilterConfig1.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
			sFilterConfig1.FilterID1 = Axes[uCont].AxisID<<5;
			sFilterConfig1.FilterID2 = Axes[uCont].AxisID<<5;
			sFilterConfig1.RxBufferIndex = 0;
			if(HAL_FDCAN_ConfigFilter(Axes[uCont].CanInstance, &sFilterConfig1) != HAL_OK)
			{
				/* Filter configuration Error */
				Error_Handler();
			}

			/* Start the FDCAN module */
			if(HAL_FDCAN_Start(Axes[uCont].CanInstance)!= HAL_OK)
			{
			  Error_Handler();
			}
			/* Enable the Rx Interrupt in RX_FIFO0*/
			if (HAL_FDCAN_ActivateNotification(Axes[uCont].CanInstance, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
			{
			  /* Notification Error */
			  Error_Handler();
			}
		}
		else if(Axes[uCont].CanInstance == &hfdcan2)
		{
			/* Configure standard ID reception filter to Rx buffer 0 */
			sFilterConfig2.IdType = FDCAN_STANDARD_ID;
			sFilterConfig2.FilterIndex = 0;
			sFilterConfig2.FilterType = FDCAN_FILTER_MASK;
			sFilterConfig2.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
			sFilterConfig2.FilterID1 = sFilterConfig2.FilterID1 & Axes[uCont].AxisID<<5;
			sFilterConfig2.FilterID2 = sFilterConfig2.FilterID1 &Axes[uCont].AxisID<<5;
			sFilterConfig2.RxBufferIndex = 0;
			if(HAL_FDCAN_ConfigFilter(Axes[uCont].CanInstance, &sFilterConfig2) != HAL_OK)
			{
				/* Filter configuration Error */
				Error_Handler();
			}

			/* Start the FDCAN module */
			if(HAL_FDCAN_Start(Axes[uCont].CanInstance)!= HAL_OK)
			{
			  Error_Handler();
			}
			/* Enable the Rx Interrupt in RX_FIFO0*/
			if (HAL_FDCAN_ActivateNotification(Axes[uCont].CanInstance, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
			{
			  /* Notification Error */
			  Error_Handler();
			}
		}
	}
}

void Set_TX_Param(FDCAN_TxHeaderTypeDef *pTxHeader, int AxisID, int COMMAND_ID, int id_type, int frame_type, int data_length)
{
	/* Configure Tx buffer message */
	pTxHeader->Identifier = (AxisID << 5) | COMMAND_ID;
	pTxHeader->IdType = FDCAN_STANDARD_ID;
	pTxHeader->TxFrameType = FDCAN_DATA_FRAME;
	// data_length: up to eight bytes of data
	pTxHeader->DataLength = data_length << 16;
	pTxHeader->ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	pTxHeader->BitRateSwitch = FDCAN_BRS_OFF;
	pTxHeader->FDFormat = FDCAN_CLASSIC_CAN;
	pTxHeader->TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	pTxHeader->MessageMarker = 0x00; // Ignore because FDCAN_NO_TX_EVENTS
}

void CAN_Send_Packet(FDCAN_HandleTypeDef *pCanInstance, FDCAN_TxHeaderTypeDef *pTxHeader, uint8_t *pTxData)
{
	//HAL_FDCAN_AddMessageToTxBuffer(pCanInstance, pTxHeader, pTxData, FDCAN_TX_BUFFER0);
	HAL_FDCAN_AddMessageToTxFifoQ(pCanInstance, pTxHeader, pTxData);
}

void Set_Axis_Requested_State(Axis Axis, Axis_State state)
{
	FDCAN_TxHeaderTypeDef TxHeader;
	uint8_t TxData[4];

	unsigned int Requested_State = state;

	Set_TX_Param(&TxHeader, Axis.AxisID, SET_AXIS_REQUESTED_STATE, FDCAN_STANDARD_ID, FDCAN_DATA_FRAME, 4);

	uint8_t *ptrToFloat;
	ptrToFloat = (uint8_t *)&Requested_State;

	TxData[0] = ptrToFloat[0];
	TxData[1] = ptrToFloat[1];
	TxData[2] = ptrToFloat[2];
	TxData[3] = ptrToFloat[3];

	CAN_Send_Packet(Axis.CanInstance, &TxHeader, TxData);
}

void Set_Input_Vel(Axis Axis, float vel, float torqueff)
{
	FDCAN_TxHeaderTypeDef TxHeader;
	uint8_t TxData[8];

	uint8_t *ptrVel;
	uint8_t *ptrTor;

	Set_TX_Param(&TxHeader, Axis.AxisID, SET_INPUT_VEL, FDCAN_STANDARD_ID, FDCAN_DATA_FRAME, 8);

	ptrVel = (uint8_t *)&vel;
	ptrTor = (uint8_t *)&torqueff;

	TxData[0] = ptrVel[0];
	TxData[1] = ptrVel[1];
	TxData[2] = ptrVel[2];
	TxData[3] = ptrVel[3];
	TxData[4] = ptrTor[0];
	TxData[5] = ptrTor[1];
	TxData[6] = ptrTor[2];
	TxData[7] = ptrTor[3];

	CAN_Send_Packet(Axis.CanInstance, &TxHeader, TxData);
}

void Set_Controller_Modes(Axis Axis, Control_Mode ControlMode, Input_Mode InputMode)
{
	FDCAN_TxHeaderTypeDef TxHeader;
	uint8_t TxData[8];

	int Control = ControlMode;
	int Input = InputMode;
	uint8_t *ptrControl;
	uint8_t *ptrInput;

	Set_TX_Param(&TxHeader, Axis.AxisID, SET_CONTROLLER_MODES, FDCAN_STANDARD_ID, FDCAN_DATA_FRAME, 8);

	ptrControl = (uint8_t *)&Control;
	ptrInput = (uint8_t *)&Input;

	TxData[0] = ptrControl[0];
	TxData[1] = ptrControl[1];
	TxData[2] = ptrControl[2];
	TxData[3] = ptrControl[3];
	TxData[4] = ptrInput[0];
	TxData[5] = ptrInput[1];
	TxData[6] = ptrInput[2];
	TxData[7] = ptrInput[3];

	CAN_Send_Packet(Axis.CanInstance, &TxHeader, TxData);
}

void Set_Input_Pos(Axis Axis, float Input_Pos, int Vel_FF, int Torque_FF)
{
	FDCAN_TxHeaderTypeDef TxHeader;
	uint8_t TxData[8];

	uint8_t *ptrPos;
	uint8_t *ptrVel;
	uint8_t *ptrTor;

	Vel_FF *= 0.001;
	Torque_FF *= 0.001;

	Set_TX_Param(&TxHeader, Axis.AxisID, SET_INPUT_POS, FDCAN_STANDARD_ID, FDCAN_DATA_FRAME, 8);

	ptrPos = (uint8_t *)&Input_Pos;
	ptrVel = (uint8_t *)&Vel_FF;
	ptrTor = (uint8_t *)&Torque_FF;

	TxData[0] = ptrPos[0];
	TxData[1] = ptrPos[1];
	TxData[2] = ptrPos[2];
	TxData[3] = ptrPos[3];
	TxData[4] = ptrVel[0];
	TxData[5] = ptrVel[1];
	TxData[6] = ptrTor[0];
	TxData[7] = ptrTor[1];

	CAN_Send_Packet(Axis.CanInstance, &TxHeader, TxData);
}


void Set_Input_Torque(Axis Axis, float torque)
{
	FDCAN_TxHeaderTypeDef TxHeader;
	uint8_t TxData[4];

	uint8_t *ptrTor;

	Set_TX_Param(&TxHeader, Axis.AxisID, SET_INPUT_TORQUE, FDCAN_STANDARD_ID, FDCAN_DATA_FRAME, 4);

	ptrTor = (uint8_t *)&torque;

	TxData[0] = ptrTor[0];
	TxData[1] = ptrTor[1];
	TxData[2] = ptrTor[2];
	TxData[3] = ptrTor[3];

	CAN_Send_Packet(Axis.CanInstance, &TxHeader, TxData);
}

void Set_Position_Gain(Axis Axis, float pos_gain)
{
	FDCAN_TxHeaderTypeDef TxHeader;
	uint8_t TxData[4];

	uint8_t *ptrPos;

	Set_TX_Param(&TxHeader, Axis.AxisID, SET_POSITION_GAIN, FDCAN_STANDARD_ID, FDCAN_DATA_FRAME, 4);

	ptrPos = (uint8_t *)&pos_gain;

	TxData[0] = ptrPos[0];
	TxData[1] = ptrPos[1];
	TxData[2] = ptrPos[2];
	TxData[3] = ptrPos[3];

	CAN_Send_Packet(Axis.CanInstance, &TxHeader, TxData);
}

void Set_Vel_Gains(Axis Axis, float Vel_Gain, float Vel_Int_Gain)
{
	FDCAN_TxHeaderTypeDef TxHeader;
	uint8_t TxData[8];

	uint8_t *ptrVelGain;
	uint8_t *ptrVelIntGain;

	Set_TX_Param(&TxHeader, Axis.AxisID, SET_VEL_GAINS, FDCAN_STANDARD_ID, FDCAN_DATA_FRAME, 8);


	ptrVelGain = (uint8_t *)&Vel_Gain;
	ptrVelIntGain = (uint8_t *)&Vel_Int_Gain;

	TxData[0] = ptrVelGain[0];
	TxData[1] = ptrVelGain[1];
	TxData[2] = ptrVelGain[2];
	TxData[3] = ptrVelGain[3];
	TxData[4] = ptrVelIntGain[0];
	TxData[5] = ptrVelIntGain[1];
	TxData[6] = ptrVelIntGain[2];
	TxData[7] = ptrVelIntGain[3];

	CAN_Send_Packet(Axis.CanInstance, &TxHeader, TxData);
}

void Set_Axis_Node_ID(Axis Axis, uint32_t node_id)
{
	FDCAN_TxHeaderTypeDef TxHeader;
	uint8_t TxData[4];

	uint8_t *ptrNodeId;

	Set_TX_Param(&TxHeader, Axis.AxisID, SET_AXIS_NODE_ID, FDCAN_STANDARD_ID, FDCAN_DATA_FRAME, 4);

	ptrNodeId = (uint8_t *)&node_id;

	TxData[0] = ptrNodeId[0];
	TxData[1] = ptrNodeId[1];
	TxData[2] = ptrNodeId[2];
	TxData[3] = ptrNodeId[3];

	CAN_Send_Packet(Axis.CanInstance, &TxHeader, TxData);
}

void Set_Limits(Axis Axis, float vel_lim, float curr_lim)
{
	FDCAN_TxHeaderTypeDef TxHeader;
	uint8_t TxData[8];

	uint8_t *ptrVelLim;
	uint8_t *ptrCurrLim;

	Set_TX_Param(&TxHeader, Axis.AxisID, SET_LIMITS, FDCAN_STANDARD_ID, FDCAN_DATA_FRAME, 8);

	ptrVelLim = (uint8_t *)&vel_lim;
	ptrCurrLim = (uint8_t *)&curr_lim;

	TxData[0] = ptrVelLim[0];
	TxData[1] = ptrVelLim[1];
	TxData[2] = ptrVelLim[2];
	TxData[3] = ptrVelLim[3];
	TxData[4] = ptrCurrLim[0];
	TxData[5] = ptrCurrLim[1];
	TxData[6] = ptrCurrLim[2];
	TxData[7] = ptrCurrLim[3];

	CAN_Send_Packet(Axis.CanInstance, &TxHeader, TxData);
}

void Get_Bus_Voltage_Current(Axis Axis)
{
	FDCAN_TxHeaderTypeDef TxHeader;

	Set_TX_Param(&TxHeader, Axis.AxisID, GET_BUS_VOLTAGE_CURRENT, FDCAN_STANDARD_ID, FDCAN_REMOTE_FRAME, 0);
	CAN_Send_Packet(Axis.CanInstance, &TxHeader, NULL);
}

void Get_IQ(Axis Axis)
{
	FDCAN_TxHeaderTypeDef TxHeader;

	Set_TX_Param(&TxHeader, Axis.AxisID, GET_IQ, FDCAN_STANDARD_ID, FDCAN_REMOTE_FRAME, 0);
	CAN_Send_Packet(Axis.CanInstance, &TxHeader, NULL);
}

void Get_Encoder_Estimates(Axis Axis)
{
	FDCAN_TxHeaderTypeDef TxHeader;

	Set_TX_Param(&TxHeader, Axis.AxisID, GET_ENCODER_ESTIMATES, FDCAN_STANDARD_ID, FDCAN_REMOTE_FRAME, 0);
	CAN_Send_Packet(Axis.CanInstance, &TxHeader, NULL);
}

void Get_Temperature(Axis Axis)
{
	FDCAN_TxHeaderTypeDef TxHeader;

	Set_TX_Param(&TxHeader, Axis.AxisID, GET_TEMPERATURE, FDCAN_STANDARD_ID, FDCAN_REMOTE_FRAME, 0);
	CAN_Send_Packet(Axis.CanInstance, &TxHeader, NULL);
}



void Clear_Errors(Axis Axis)
{
	FDCAN_TxHeaderTypeDef TxHeader;

	Set_TX_Param(&TxHeader, Axis.AxisID, CLEAR_ERRORS, FDCAN_STANDARD_ID, FDCAN_DATA_FRAME, 0);
	CAN_Send_Packet(Axis.CanInstance, &TxHeader, NULL);
}

void Reboot_ODrive(Axis Axis)
{
	FDCAN_TxHeaderTypeDef TxHeader;

	Set_TX_Param(&TxHeader, Axis.AxisID, REBOOT_ODRIVE, FDCAN_STANDARD_ID, FDCAN_DATA_FRAME, 0);
	CAN_Send_Packet(Axis.CanInstance, &TxHeader, NULL);
}

void ODrive_RX_CallBack(Axis *Axes, FDCAN_RxHeaderTypeDef *RxHeader, uint8_t * RxData)
{
	unsigned char uCont;

	// Check if the message is from a configured node
	for(uCont = 0; uCont < ucNumAxesConfigured; uCont++)
	{
		if(RxHeader->Identifier>>5 == Axes[uCont].AxisID)
		{
			int32_t CMD_ID = (RxHeader->Identifier & 0x01F);

			switch(CMD_ID)
			{
				uint32_t *ptrEncPos;
				uint32_t *ptrEncVel;
				uint32_t *ptrBusV;
				uint32_t *ptrBusI;
				uint32_t *ptrIqSet;
				uint32_t *ptrIqMsr;
				uint32_t *ptrFETTemperature;
				uint32_t *ptrMotorTemperature;

				case ODRIVE_HEARTBEAT_MESSAGE:
					Axes[uCont].AXIS_Error = (RxData[0] | RxData[1]<<8 | RxData[2]<<16 | RxData[3]<<24);
					Axes[uCont].AXIS_Current_State = RxData[4];
					Axes[uCont].Controller_Status = RxData[5];
					break;

				case GET_ENCODER_ESTIMATES:
					ptrEncPos = (uint32_t *)&(Axes[uCont].AXIS_Encoder_Pos);
					*ptrEncPos = (RxData[0] + (RxData[1]<<8) + (RxData[2]<<16) + (RxData[3]<<24));
					ptrEncVel = (uint32_t *)&(Axes[uCont].AXIS_Encoder_Vel);
					*ptrEncVel = (RxData[4] + (RxData[5]<<8) + (RxData[6]<<16) + (RxData[7]<<24));
					break;

				case GET_BUS_VOLTAGE_CURRENT:
					ptrBusV = (uint32_t *)&(Axes[uCont].AXIS_Bus_Voltage);
					*ptrBusV = (RxData[0] + (RxData[1]<<8) + (RxData[2]<<16) + (RxData[3]<<24));
					ptrBusI = (uint32_t *)&(Axes[uCont].AXIS_Bus_Current);
					*ptrBusI = (RxData[4] + (RxData[5]<<8) + (RxData[6]<<16) + (RxData[7]<<24));
					break;

				case GET_IQ:
					ptrIqSet = (uint32_t *)&(Axes[uCont].AXIS_Iq_Setpoint);
					*ptrIqSet = (RxData[0] + (RxData[1]<<8) + (RxData[2]<<16) + (RxData[3]<<24));
					ptrIqMsr = (uint32_t *)&(Axes[uCont].AXIS_Iq_Measured);
					*ptrIqMsr = (RxData[4] + (RxData[5]<<8) + (RxData[6]<<16) + (RxData[7]<<24));
					break;

				case GET_TEMPERATURE:
					ptrFETTemperature = (uint32_t *)&(Axes[uCont].AXIS_FET_Temperature);
					*ptrFETTemperature = (RxData[0] + (RxData[1]<<8) + (RxData[2]<<16) + (RxData[3]<<24));
					ptrMotorTemperature = (uint32_t *)&(Axes[uCont].AXIS_Motor_Temperature);
					*ptrMotorTemperature = (RxData[4] + (RxData[5]<<8) + (RxData[6]<<16) + (RxData[7]<<24));
					break;
			}

		}
	}
}
