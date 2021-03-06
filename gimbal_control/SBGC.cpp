/*
 * SBGC.cpp
 *
 *  Created on: Dec 27, 2016
 *      Author: kevin
 */

#include "SBGC.h"
#include "Serial.h"
#include <stdint.h>
#include <iostream>
#include <bitset>

using namespace std;

SBGC::real_time_data Real_Time_Data;

SBGC::SBGC(){
	initialize();
}

SBGC::~SBGC() {
}

void SBGC::initialize(){
	printf("Initialization successful\n");

}

/**
 * the char array containing the message received from the Gimbal is processed in this method,
 * in there it is decided what is to with every single entry
 */
SBGC::real_time_data SBGC::result(char msg[], int counter ){

	Real_Time_Data.ACC_DATA_X 			= _readInt(msg[4], msg[5]) * G_force;
	Real_Time_Data.GYRO_DATA_X			= _readInt(msg[6], msg[7]) * angular_speed;
	Real_Time_Data.ACC_DATA_Y 			= _readInt(msg[8], msg[9]) * G_force;
	Real_Time_Data.GYRO_DATA_Y			= _readInt(msg[10], msg[11]) * angular_speed;
	Real_Time_Data.ACC_DATA_Z 			= _readInt(msg[12], msg[13]) * G_force;
	Real_Time_Data.GYRO_DATA_Z  		= _readInt(msg[14], msg[15]) * angular_speed;
	Real_Time_Data.SERIAL_ERROR_COUNT 	= _readUInt(msg[16], msg[17]);
	Real_Time_Data.SYSTEM_ERROR 		= _sys_error(_readUInt(msg[18], msg[19]));
	Real_Time_Data.SYSTEM_SUB_ERROR 	= _sub_error(msg[20]);
	Real_Time_Data.RESERVED				= _readByteArr(msg[21],msg[22],msg[23]);
	Real_Time_Data.RC_ROLL				= _readInt(msg[24], msg[25]);
	Real_Time_Data.RC_PITCH				= _readInt(msg[26], msg[27]);
	Real_Time_Data.RC_YAW				= _readInt(msg[28], msg[29]);
	Real_Time_Data.RC_CMD				= _readInt(msg[30], msg[31]);
	Real_Time_Data.EXT_FC_ROLL			= _readInt(msg[32], msg[33]);
	Real_Time_Data.EXT_FC_PITCH			= _readInt(msg[34], msg[35]);
	Real_Time_Data.ANGLE_ROLL			= _readInt(msg[36], msg[37]) * deg;
	Real_Time_Data.ANGLE_PITCH			= _readInt(msg[38], msg[39]) * deg;
	Real_Time_Data.ANGLE_YAW			= _readInt(msg[40], msg[41]) * deg;
	Real_Time_Data.FRAME_IMU_ANGLE_ROLL	= _readInt(msg[42], msg[43]) * deg;
	Real_Time_Data.FRAME_IMU_ANGLE_PITCH= _readInt(msg[44], msg[45]) * deg;
	Real_Time_Data.FRAME_IMU_ANGLE_YAW	= _readInt(msg[46], msg[47]) * deg;
	Real_Time_Data.RC_ANGLE_ROLL  		= _readInt(msg[48], msg[49]);
	Real_Time_Data.RC_ANGLE_PITCH		= _readInt(msg[50], msg[51]);
	Real_Time_Data.RC_ANGLE_YAW		   	= _readInt(msg[52], msg[53]);
	Real_Time_Data.CYCLE_TIME			= _readInt(msg[54], msg[55]);
	Real_Time_Data.I2C_ERROR_COUNT		= _readInt(msg[56], msg[57]);
	Real_Time_Data.ERROR_CODE			= msg[58]; //deprecated, use 16bit SYSTEM_ERROR above
	Real_Time_Data.BAT_LEVEL			= _readInt(msg[59], msg[60]) * volt;
	Real_Time_Data.MOTOR_STATE			= _getMotorState(msg[61]);
	Real_Time_Data.CUR_IMU				= _getIMU(msg[62]);
	Real_Time_Data.CUR_PROFILE			= msg[63];
	Real_Time_Data.MOTOR_POWER_ROLL		= msg[64];
	Real_Time_Data.MOTOR_POWER_PITCH	= msg[65];
	Real_Time_Data.MOTOR_POWER_YAW		= msg[66];
	Real_Time_Data.CHECKSUM				= msg[67];
	return Real_Time_Data;
}

/**
 * signed numbers that consist of two array entries
 */
int16_t SBGC::_readInt(uint8_t x, uint8_t y){
	int16_t value = int16_t(y << 8) | x;
	return value;
}

/**
 * unsigned numbers that consist of two array entries
 */
uint16_t SBGC::_readUInt(uint8_t x, uint8_t y){
	uint16_t value = uint16_t(y << 8) | x;
	return value;
}

/**when I wrote this, only god and I understood what I was doing
 *now only god knows
 */
uint32_t SBGC::_readByteArr(char x, char y, char z){
	uint32_t value = (x << 8) | (y << 8) | z;
	return value;
}

/**
 * this method is used to check whether there is an error on the gimbal controller
 */
string SBGC::_sys_error(uint16_t errors){
	if(errors == 0){
		return "NO ERROR";
	}else{
		string err = "";
		int8_t size = 16;
		bool errorArr[size];
		for(uint8_t i = 0; i < size; i++){
			errorArr[i] = (errors & 0xFFFF) >> 15;
			errors <<= 1;
		}
		if(errorArr[15]){
			err.append("ERR_NO_SENSOR:");
		}else if(errorArr[14]){
			err.append("ERR_CALIB_ACC:");
		}else if(errorArr[13]){
			err.append("ERR_SET_POWER:");
		}else if(errorArr[12]){
			err.append("ERR_CALIB_POLES:");
		}else if(errorArr[11]){
			err.append("ERR_PROTECTION:");
		}else if(errorArr[10]){
			err.append("ERR_SERIAL:");
		}else if(errorArr[9]){
			err.append("ERR_LOW_BAT1:");
		}else if(errorArr[8]){
			err.append("ERR_LOW_BAT2:");
		}else if(errorArr[7]){
			err.append("ERR_GUI_VERSION:");
		}else if(errorArr[6]){
			err.append("ERR_MISS_STEPS:");
		}else if(errorArr[5]){
			err.append("ERR_SYSTEM:");
		}else if(errorArr[4]){
			err.append("ERR_EMERGENCY_STOP:");
		}
		return err;
	}
}

/**
 * if there is an emergency stop on the gimbal controller an additional suberror shows up
 */
string SBGC::_sub_error(uint8_t suberror){
	switch(suberror)
	{
		case 1:
			return "ERR_I2C_ERRORS";
		case 2:
			return "ERR_DRV_OTW";
		case 3:
			return "ERR_DRV_FAULT";
		case 4:
			return "ERR_ENCODER_IMU_ANGLE";
		case 5:
			return "ERR_CALIBRATION_FAILED";
		case 6:
			return "ERR_INTERNAL_SYSTEM_ERROR";
		case 7:
			return "ERR_ENCODER_CALIBRATION_BAD_SCALE";
		case 8:
			return "ERR_OVER_TEMPERATURE";
		case 9:
			return "ERR_BAD_MOTOR_POLES_INVERT";
		case 10:
			return "ERR_NOT_ENOUGH_MEMORY";
		case 11:
			return "ERR_IMU_SENSOR_NOT_RESPONDING";
		case 13:
			return "ERR_MOTOR_OVERHEAT_PROTECTION";
		default:
			return "NO_SUB_ERR";
	}
		return "0";
}

/**
 * shows the active IMU
 */
string SBGC::_getIMU(uint8_t x){
	switch(x)
	{
		case 1:
			return "Profile: Main IMU";
		case 2:
			return "Profile: Frame IMU";
		default:
			return "Error: Reading Profile";
	}
}

/*
 * shows whether motors are running or not
 */
bool SBGC::_getMotorState(uint8_t x){
	std::bitset<8> converted = x;
	bool state = converted.test(0);
	return state;
}

/**
 * shows active profile
 */
string SBGC::_getProfile(uint8_t x){
	return "Current Profile: " + x;
}


