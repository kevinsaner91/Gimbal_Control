/*
 * SBGC.cpp
 *
 *  Created on: Dec 27, 2016
 *      Author: kevin
 */

#include "SBGC.h"
#include "Serial.h"
#include <stdint.h>

using namespace std;

#define COMMAND 0x17;



SBGC::SBGC(){
	initialize();
}

SBGC::~SBGC() {
}

void SBGC::initialize(){
	printf("Initialization successful\n");
}

SBGC::real_time_data SBGC::result(char msg[], Serial serial, int counter ){
	SBGC::real_time_data Real_Time_Data;
	int i = 4;
	Real_Time_Data.ACC_DATA_PITCH 		= _readInt(msg[i], msg[i++]);
	Real_Time_Data.GYRO_DATA_PITCH		= _readInt(msg[i++], msg[i++]);
	Real_Time_Data.ACC_DATA_ROLL 		= _readInt(msg[i++], msg[i++]);
	Real_Time_Data.GYRO_DATA_ROLL		= _readInt(msg[i++], msg[i++]);
	Real_Time_Data.ACC_DATA_YAW 		= _readInt(msg[i++], msg[i++]);
	Real_Time_Data.GYRO_DATA_YAW  		= _readInt(msg[i++], msg[i++]);
	Real_Time_Data.SERIAL_ERROR_COUNT 	= _readUInt(msg[i++], msg[i++]);
	Real_Time_Data.SYSTEM_ERROR 		= _sys_error(_readUInt(msg[i++], msg[i++]));
	Real_Time_Data.SYSTEM_SUB_ERROR 	= _sub_error(msg[i++]);
	return Real_Time_Data;
}

int16_t SBGC::_readInt(char x, char y){
	int16_t value = (x << 8) | y;
	return value;
}

uint16_t SBGC::_readUInt(char x, char y){
	uint16_t value = (x << 8) | y;
	return value;
}

string SBGC::_sys_error(uint16_t errors){
	if(errors == 0){
		return "NO ERROR";
	}else{
		string err = "";
		bool errorArr[16];
		for(int i = 0; i < sizeof(int16_t); i++){
			errorArr[i] = (errors & 0xFFFF) >> 15;
			errors <<= 1;
		}
		if(errorArr[0]){
			err.append("ERR_NO_SENSOR:");
		}else if(errorArr[1]){
			err.append("ERR_CALIB_ACC:");
		}else if(errorArr[2]){
			err.append("ERR_SET_POWER:");
		}else if(errorArr[3]){
			err.append("ERR_CALIB_POLES:");
		}else if(errorArr[4]){
			err.append("ERR_PROTECTION:");
		}else if(errorArr[5]){
			err.append("ERR_SERIAL:");
		}else if(errorArr[6]){
			err.append("ERR_LOW_BAT1:");
		}else if(errorArr[7]){
			err.append("ERR_LOW_BAT2:");
		}else if(errorArr[8]){
			err.append("ERR_GUI_VERSION:");
		}else if(errorArr[9]){
			err.append("ERR_MISS_STEPS:");
		}else if(errorArr[10]){
			err.append("ERR_SYSTEM:");
		}else if(errorArr[11]){
			err.append("ERR_EMERGENCY_STOP:");
		}
		return err;
	}
}


string SBGC::_sub_error(uint8_t suberror){
	switch(suberror)
	{
		case 1:
			return "ERR_I2C_ERRORS";
		case 2:
			return "ERR_DRV_OTW";
		case 3:
			return "ERR_DRV_FAULT";
	}
		return "0";
}
