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

		int16_t ACC_DATA_X;			// [G] (=9.81m/s²)
		int16_t GYRO_DATA_X;		// [°/s]
		int16_t ACC_DATA_Z;			// [G] (=9.81m/s²)
		int16_t GYRO_DATA_Z;		// [°/s]
		int16_t ACC_DATA_Y;			// [G] (=9.81m/s²)
		int16_t GYRO_DATA_Y;		// [°/s]
		uint16_t SERIAL_ERROR_COUNT;// unused
		string SYSTEM_ERROR;		// contains all system errors
		string SYSTEM_SUB_ERROR;	// reason of emergency stop
		uint32_t RESERVED;			// unused
		int16_t RC_ROLL;
		int16_t RC_PITCH;
		int16_t RC_YAW;
		int16_t RC_CMD;
		int16_t EXT_FC_ROLL;
		int16_t EXT_FC_PITCH;
		double ANGLE_ROLL;			// [°]
		double ANGLE_PITCH;			// [°]
		double ANGLE_YAW;			// [°]
		double FRAME_IMU_ANGLE_ROLL;
		double FRAME_IMU_ANGLE_PITCH;
		double FRAME_IMU_ANGLE_YAW;
		int16_t RC_ANGLE_ROLL;
		int16_t RC_ANGLE_PITCH;
		int16_t RC_ANGLE_YAW;
		uint16_t CYCLE_TIME;		// cycle time of the sbgc board
		uint16_t I2C_ERROR_COUNT;	// number of registered errors on I2C bus
		uint8_t ERROR_CODE; 		// deprecated, use 16bit SYSTEM_ERROR instead
		double BAT_LEVEL;			// [V]
		bool MOTOR_STATE;			// 1 = ON/ 0 = OFF
		string CUR_IMU;				// shows which IMU is currently used
		uint8_t CUR_PROFILE;		// shows the currently active profile
		uint8_t MOTOR_POWER_ROLL;	// in the current firmware version of the board not included
		uint8_t MOTOR_POWER_PITCH;	// in the current firmware version of the board not included
		uint8_t MOTOR_POWER_YAW;	// in the current firmware version of the board not included
		uint8_t CHECKSUM;
	};


	const static double deg = 0.0219725625;
	const static double G_force = 1/512;
	const static double angular_speed = 0.06103701895;
	const static double volt = 0.01;



	SBGC();
	virtual ~SBGC();
	void initialize();

	bool validate();
	real_time_data result(char msg[], int counter);
	void print(SBGC::real_time_data Real_Time_Data);

private:


	int16_t _readInt(uint8_t x, uint8_t y);
	uint16_t _readUInt(uint8_t x, uint8_t y);
	string _sys_error(uint16_t errors);
	string _sub_error(uint8_t suberror);
	uint32_t _readByteArr(char x, char y, char z);
	string _getProfile(uint8_t x);
	string _getIMU(uint8_t x);
	double _getVoltage(uint16_t x);
	bool _getMotorState(uint8_t x);
	char getCommand();

};

#endif /* SBGC_H_ */
