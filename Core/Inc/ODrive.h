/*
 * ODrive.h
 *
 *  Created on: 07-Jul-2022
 *      Author: sidiyer27
 */

#ifndef ODRIVE_ODRIVE_H_
#define ODRIVE_ODRIVE_H_

#include "main.h"

#define MAX_AXES 3

#define TORQUE_CONSTANT (8.27 / 150) // Nm/A

//COMMAND ID
#define ODRIVE_HEARTBEAT_MESSAGE		0x001
#define ODRIVE_GET_ERROR				0x003
#define SET_AXIS_NODE_ID				0x006
#define SET_AXIS_REQUESTED_STATE 		0x007
#define GET_ENCODER_ESTIMATES			0x009
#define SET_CONTROLLER_MODES			0x00B
#define SET_INPUT_POS					0x00C
#define SET_INPUT_VEL					0x00D
#define SET_INPUT_TORQUE				0x00E
#define SET_LIMITS						0x00F
#define SET_TRAJ_VEL_LIMIT				0x011
#define SET_TRAJ_ACCEL_LIMIT			0x012    ///
#define SET_TRAJ_INERTIA				0x013    ///
#define GET_IQ							0x014
#define GET_TEMPERATURE					0x015    ///
#define REBOOT_ODRIVE					0x016
#define GET_BUS_VOLTAGE_CURRENT			0x017
#define CLEAR_ERRORS					0x018
#define SET_ABSOLUTE_POSITION			0x019    ///
#define SET_POSITION_GAIN				0x01A
#define SET_VEL_GAINS					0x01B


//Axis Parameters
typedef struct Axis
{
	FDCAN_HandleTypeDef *CanInstance;
	int AxisID;
	float AXIS_Encoder_Pos;
	float AXIS_Encoder_Vel;
	float AXIS_Bus_Voltage;
	float AXIS_Bus_Current;
	float AXIS_Iq_Setpoint;
	float AXIS_Iq_Measured;
	float AXIS_Motor_Temperature;
	float AXIS_FET_Temperature;
	uint32_t AXIS_Error;
	uint8_t AXIS_Current_State;
	uint8_t Controller_Status;
}Axis;

//Axis States
typedef enum{
	UNDEFINED = 0x0,
	IDLE = 0x1,
	STARTUP_SEQUENCE = 0x2,
	FULL_CALIBRATION_SEQUENCE = 0x3,
	MOTOR_CALIBRATION = 0x4,
	ENCODER_INDEX_SEARCH = 0x6,
	ENCODER_OFFSET_CALIBRATION = 0x7,
	CLOSED_LOOP_CONTROL = 0x8,
	LOCKIN_SPIN = 0x9,
	ENCODER_DIR_FIND = 0xA,
	HOMING = 0xB,
	ENCODER_HALL_POLARITY_CALIBRATION = 0xC,
	ENCODER_HALL_PHASE_CALIBRATION = 0xD
} Axis_State;

//Control Modes
typedef enum{
	VOLTAGE_CONTROL = 0x0,
	TORQUE_CONTROL = 0x1,
	VELOCITY_CONTROL = 0x2,
	POSITION_CONTROL = 0x3
} Control_Mode;

//Input Modes
typedef enum{
	INACTIVE = 0x0,
	PASSTHROUGH = 0x1,
	VEL_RAMP = 0x2,
	POS_FILTER = 0x3,
	MIX_CHANNELS = 0x4,
	TRAP_TRAJ = 0x5,
	TORQUE_RAMP = 0x6,
	MIRROR = 0x7,
	TUNING = 0x8
} Input_Mode;

void CAN_Setup(Axis *Axes, unsigned char ucNumAxes);

void Set_TX_Param(FDCAN_TxHeaderTypeDef *pTxHeader, int AXIS_ID, int COMMAND_ID, int id_type, int frame_type, int data_length);

void CAN_Send_Packet(FDCAN_HandleTypeDef *CAN_INSTANCE, FDCAN_TxHeaderTypeDef *TxHeader, uint8_t *pTxData);

void Set_Axis_Requested_State(Axis Axis, Axis_State state);

void Set_Input_Vel(Axis Axis, float vel, float torqueff);

void Set_Controller_Modes(Axis Axis, Control_Mode ControlMode, Input_Mode InputMode);

void Set_Input_Pos(Axis Axis, float Input_Pos, int Vel_FF, int Torque_FF);

void Set_Input_Torque(Axis Axis, float torque);

void Set_Position_Gain(Axis Axis, float pos_gain);

void Set_Vel_Gains(Axis Axis, float Vel_Gain, float Vel_Int_Gain);

void Set_Axis_Node_ID(Axis Axis, uint32_t node_id);

void Set_Limits(Axis Axis, float vel_lim, float curr_lim);

void Clear_Errors(Axis Axis);

void Reboot_ODrive(Axis AXIS_ID);

void ODrive_RX_CallBack(Axis *Axes, FDCAN_RxHeaderTypeDef *RxHeader, uint8_t * RxData);

void Get_Bus_Voltage_Current(Axis Axis);

void Get_IQ(Axis Axis);

void Get_Temperature(Axis Axis);

void Get_Encoder_Estimates(Axis Axis);


#endif /* ODRIVE_ODRIVE_H_ */
