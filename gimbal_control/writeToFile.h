/*
 * writeToFile.h
 *
 *  Created on: Jan 24, 2017
 *      Author: kevin
 */


#include <iostream>
#include <stdint.h>
#include "SBGC.h"

using namespace std;

#ifndef WRITETOFILE_H_
#define WRITETOFILE_H_

class writeToFile {
public:
	writeToFile();
	virtual ~writeToFile();

	void initialize();


	void print(int x);
	void print(double x);
	void print(string x);

	void writeAng(double x, double y, double z);
	void writeImu(SBGC::real_time_data Real_Time_Data);

	void close();

private:

	const string currentDateTime();
};

#endif /* WRITETOFILE_H_ */
