/*
 * writeToFile.cpp
 *
 *  Created on: Jan 24, 2017
 *      Author: kevin
 */

#include "writeToFile.h"
#include "SBGC.h"
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <string>
#include <ctime>
#include <sys/time.h>

using namespace std;

#define FILE_LOCATION_POS "/home/ang.csv"
#define FILE_LOCATION_IMU "/home/imu_gimbal.csv"
//#define _BSD_SOURCE

FILE *csv_fp;
FILE *csv_imu;
ofstream angFile;
ofstream imuFile;

//Constructor
writeToFile::writeToFile() {
	initialize();
}
//Destructor
writeToFile::~writeToFile() {
	// TODO Auto-generated destructor stub
}
/*
 * initializes the two files to write to
 */
void writeToFile::initialize(){

		csv_fp = fopen(FILE_LOCATION_POS, "w+");
		csv_imu = fopen(FILE_LOCATION_IMU, "w+");
		if (csv_fp == NULL || csv_imu == NULL) {
			//printf("csv fopen failed");
			perror("csv fopen failed");
		}
		fprintf(csv_fp, "init \n");
		fprintf(csv_imu, "init \n");

		angFile.open(FILE_LOCATION_POS);
		imuFile.open(FILE_LOCATION_IMU);
		angFile << "init\n";
		angFile << "timestamp : Roll : Pitch : Yaw\n";
		imuFile << "init\n";
}

/**
 * writes the angles of the gimbal
 */
void writeToFile::writeAng(double x, double y, double z) {
	angFile << currentDateTime();
	angFile << " : ";
	angFile << x;
	angFile << " : ";
	angFile << y;
	angFile << " : ";
	angFile << z;
	angFile << " : ";
	angFile << "\n";
}

/**
 * writes the additional imu data
 */
void writeToFile::writeImu(SBGC::real_time_data Real_Time_Data){
	imuFile << currentDateTime();
	imuFile << " : ";
	imuFile << Real_Time_Data.ACC_DATA_X;
	imuFile << " : ";
	imuFile << Real_Time_Data.ACC_DATA_Y;
	imuFile << " : ";
	imuFile << Real_Time_Data.ACC_DATA_Z;
	imuFile << " : ";
	imuFile << Real_Time_Data.GYRO_DATA_X;
	imuFile << " : ";
	imuFile << Real_Time_Data.GYRO_DATA_Y;
	imuFile << " : ";
	imuFile << Real_Time_Data.GYRO_DATA_Z;
	imuFile << " : ";
	imuFile << Real_Time_Data.SYSTEM_ERROR;
	imuFile << " : ";
	imuFile << Real_Time_Data.SYSTEM_SUB_ERROR;
	imuFile << " : ";
	imuFile << Real_Time_Data.RC_ROLL;
	imuFile << " : ";
	imuFile << Real_Time_Data.RC_PITCH;
	imuFile << " : ";
	imuFile << Real_Time_Data.RC_YAW;
	imuFile << " : ";
	imuFile << Real_Time_Data.RC_CMD;
	imuFile << " : ";
	imuFile << Real_Time_Data.EXT_FC_ROLL;
	imuFile << " : ";
	imuFile << Real_Time_Data.EXT_FC_PITCH;
	imuFile << " : ";
	imuFile << Real_Time_Data.RC_ANGLE_ROLL;
	imuFile << "\n";

}

/**
 * gets the current system (phytec) time in microseconds
 */
const string writeToFile::currentDateTime(){
	char fmt[64], buf[64];
	struct timeval tv;
	struct tm *tm;

	gettimeofday(&tv, NULL);
	tm = localtime(&tv.tv_sec);


	strftime(fmt, sizeof fmt, "%Y-%m-%d %H:%M:%S.%%06u", tm);
	snprintf(buf, sizeof buf, fmt, tv.tv_usec);
	return buf;
}

/**
 * closes the files
 */
void writeToFile::close() {
	fclose(csv_fp);
	fclose(csv_imu);
	angFile.close();
	imuFile.close();
}

