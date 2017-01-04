#include "Serial.h"
#include "SBGC.h"
#include <iostream>

using namespace std;

int main(){
	//char array to debug
	char debugbuf[] = {0x3E, 0x17, 0x00, 0x17, 0x00};

	Serial serial;
	SBGC sbgc;
	SBGC::real_time_data Real_Time_Data;


	serial.initialize_defaults();
	int success = serial.start();
	printf("\n");
	printf("Start: %d\n", success);
	printf("\n");

	while(true){
		sleep(1);
		int bytesWritten = serial.write_message(debugbuf);
		if(bytesWritten == 5){
			printf("Bytes written: %i\n\n", bytesWritten);
		}else{
			printf("Error occurred writing bytes\n\n");
		}

		usleep(50);



		//
		if(serial.read_message()){
			char msg[serial.result];
			int counter = serial.result;
			for (int i = 0; i < counter; i++) {
				printf("Character: %i   ::   0x%x\n", i, serial.msg[i]);
				msg[i] = serial.msg[i];
				serial.msg[i] = 0;
			}
			printf("\n");
			Real_Time_Data = sbgc.result(msg, serial, counter);
			printf("The received data is: %i :: %i :: %i\n",Real_Time_Data.ACC_DATA_PITCH,
			Real_Time_Data.ACC_DATA_ROLL,
			Real_Time_Data.ACC_DATA_YAW);
			printf("\n");
			cout << Real_Time_Data.SYSTEM_ERROR;
			cout << Real_Time_Data.SYSTEM_SUB_ERROR;
			printf("\n");
		}
	}

	printf("\n");
	serial.stop();

}
