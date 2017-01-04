/*
 * Serial.h
 *
 *  Created on: Dec 7, 2016
 *      Author: Kevin Saner
 */
// ------------------------------------------------------------------------------
//   Includes
// ------------------------------------------------------------------------------

#include <cstdlib>
#include <stdio.h>   // Standard input/output definitions
#include <unistd.h>  // UNIX standard function definitions
#include <fcntl.h>   // File control definitions
#include <termios.h> // POSIX terminal control definitions
#include <pthread.h> // This uses POSIX Threads
#include <signal.h>

#ifndef SERIAL_H_
#define SERIAL_H_

#define SERIAL_PORT_OPEN   1;
#define SERIAL_PORT_CLOSED 0;
#define SERIAL_PORT_ERROR -1;
#define DEFAULT_COMMAND_SIZE 5;

class Serial {
public:
	Serial(const char *uart_name_ , int baudrate_);
	void initialize_defaults();
	Serial();
	virtual ~Serial();

	bool debug;
	const char *uart_name;
	int  baudrate;
	int  status;
	char msg[53];
	int result;

	int read_message();
	int write_message(char buf[]);

	int start();
	void stop();

	int open_serial();
	void close_serial();

	void handle_quit( int sig );

	void delay();

	int validate();

private:
	pthread_mutex_t  lock;

	int  fd;

	int  _open_port(const char* port);
	bool _setup_port(int baud, int data_bits, int stop_bits, bool parity, bool hardware_control);
	int  _read_port();
	int _write_port(char *debugbuf, unsigned len);
	int _validate();
	int _header_checksum(char cmd_id, char data_size);
	int _body_checksum();

};

#endif /* SERIAL_H_ */
