/*
 * SBGC.h

 *
 *  Created on: Dec 27, 2016
 *      Author: kevin
 */

#include "Serial.h"
#include <stdint.h>
#include <string>

using namespace std;

#ifndef SBGC_H_
#define SBGC_H_

class SBGC {
public:
	struct real_time_data {
		int16_t ACC_DATA_ROLL;
		int16_t GYRO_DATA_ROLL;
		int16_t ACC_DATA_PITCH;
		int16_t GYRO_DATA_PITCH;
		int16_t ACC_DATA_YAW;
		int16_t GYRO_DATA_YAW;
		uint16_t SERIAL_ERROR_COUNT;
		string SYSTEM_ERROR;
		string SYSTEM_SUB_ERROR;
		uint32_t RESERVED;
		int16_t RC_ROLL;
		int16_t RC_PITCH;
		int16_t RC_YAW;
		int16_t RC_CMD;
		int16_t EXT_FC_ROLL;
		int16_t EXT_FC_PITCH;
		int16_t ANGLE_ROLL;
		int16_t ANGLE_PITCH;
		int16_t ANGLE_YAW;
		int16_t FRAME_IMU_ANGLE_ROLL;
		int16_t FRAME_IMU_ANGLE_PITCH;
		int16_t FRAME_IMU_ANGLE_YAW;
		int16_t RC_ANGLE_ROLL;
		int16_t RC_ANGLE_PITCH;
		int16_t RC_ANGLE_YAW;
		uint16_t CYCLE_TIME;
		uint16_t I2C_ERROR_COUNT;
		uint8_t ERROR_CODE; //deprecated, use 16bit SYSTEM_ERROR instead
		uint16_t BAT_LEVEL;
		uint8_t OTHER_FLAGS;
		uint8_t CUR_IMU;
		uint8_t CUR_PROFILE;
		uint8_t MOTOR_POWER_ROLL;
		uint8_t MOTOR_POWER_PITCH;
		uint8_t MOTOR_POWER_YAW;
	};
public:
	SBGC();
	virtual ~SBGC();
	void initialize();

	bool validate();

	real_time_data result(char msg[], Serial serial, int counter);

private:

	int16_t _readInt(char x, char y);
	uint16_t _readUInt(char x, char y);
	string _sys_error(uint16_t errors);
	string _sub_error(uint8_t suberror);
};

#endif /* SBGC_H_ */
