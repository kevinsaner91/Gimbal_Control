#include "Serial.h"
#include "SBGC.h"
#include "writeToFile.h"
#include <iostream>

using namespace std;

Serial serial;

const char character = 0x3E;
char command_ID = 0x17;
char data_size = 0x00;
char header_checksum = (command_ID + data_size) % 256;
char data = 0x00;
const int minCycleTime = 1000;

/**
 * passes the received char array (serial.msg) to the class specific msg array
 * and sets the values of the char array back to 0
 */
void readMSG(char* msg, int counter){
	for (int i = 0; i < counter; i++) {
		//prints the received data for debugging
		//printf("Character: %i   ::   0x%x\n", i, serial.msg[i]);
		msg[i] = serial.msg[i];
		serial.msg[i] = 0;
	}
}
/**
 * writes the char array to the serial port
 * prints an error message if it fails
 */
int writeBytes(char* request){

	int bytesWritten = 0;
	bytesWritten = serial.write_message(request);
	if (bytesWritten == 5) {
		printf("Request sent -> ");
	} else {
		printf("Error occurred writing bytes\n");
	}
	return bytesWritten;
}

/**
 * The cycle time is set in milliseconds. Due to the fact that the board
 * can only handle requests slower than 100Hz, the default minimum frequency is
 * set to 160Hz and thus 6000us. This value added to the time all other methods
 * need roughly comes to 100Hz.
 */
void setCycleTime(int cycletime){
	cycletime *= 1000;
	if(cycletime < minCycleTime){
		usleep(minCycleTime);
	}else{
		usleep(cycletime);
	}
}

/**
 * in the main method, first everything is instantiated (initialized).
 * After the method goes in a while loop to cyclically write and receive
 * data from the board.
 */
int main() {

	SBGC sbgc;
	SBGC::real_time_data Real_Time_Data;
	writeToFile Filewriter;

	serial.initialize_defaults();
	serial.start();

	char request[5] = { character, command_ID, data_size, header_checksum, data };

	for(int i = 0; true; i++) {

		//cycle time is set 50Hz
		setCycleTime(10);

		writeBytes(request);

		usleep(1000);

		if (serial.read_message()) {
			printf("Received response\n");
			int counter = serial.result;
			char response[counter];
			readMSG(response, counter);


			printf("\n");
			Real_Time_Data = sbgc.result(response, counter);
			printf("ACC-Sensor data: %f :: %f :: %f :: %f :: %f :: %f",
					Real_Time_Data.ANGLE_ROLL, Real_Time_Data.ANGLE_PITCH,
					Real_Time_Data.ANGLE_YAW,
					Real_Time_Data.FRAME_IMU_ANGLE_ROLL,
					Real_Time_Data.FRAME_IMU_ANGLE_PITCH,
					Real_Time_Data.FRAME_IMU_ANGLE_YAW);
			printf("\n");

			Filewriter.writeAng(Real_Time_Data.ANGLE_ROLL, Real_Time_Data.ANGLE_PITCH, Real_Time_Data.ANGLE_YAW);

			if(i%50==0){
				Filewriter.writeImu(Real_Time_Data);
			}
		}else{
			printf("No response received\n");
			//If no response is available the angles are all 0
			Filewriter.writeAng(0,0,0);
			//If there is not data to read available, the serial port is restarted. The
			//program then waits for 0.01 seconds, until everything is established. Until
			//now it is unknown how much the performance is affected by the restart. If
			serial.start();
			usleep(10000);
		}
	}
	printf("\n");
	Filewriter.close();
	serial.stop();
}



