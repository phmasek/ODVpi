/**
 * ODVpi - Benchmark code extending OpenDaVinci 
 * (https://github.com/se-research/OpenDaVINCI)
 * Copyright (C) 2016 Philip Masek, Magnus Thulin
 *
 * This code is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This code is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <csignal>
#include <sstream>
#include <iostream>
#include <cerrno>

// WRITE TO FILE
#include <fstream>
// WRITE TO FILE END

// SERIAL
#include <termios.h>
#include <fcntl.h>    /* For O_RDWR */
#include <unistd.h>   /* For open(), creat() */
#include <cstring>
// SERIAL END

using namespace std;

timespec start, end;

int setUpSerialPort(const char* port_location, int32_t baud) {

    int PORT = open( port_location, O_RDWR| O_NOCTTY| O_NONBLOCK );

    struct termios tty;
    struct termios tty_old;
    memset (&tty, 0, sizeof tty);

    /* Error Handling */
    if ( tcgetattr ( PORT, &tty ) != 0 ) {
       std::cout << "Error " << errno << " from tcgetattr: " << strerror(errno) << std::endl;
    }

    /* Save old tty parameters */
    tty_old = tty;

    /* Set Baud Rate */
    cfsetospeed (&tty, (speed_t)B115200);
    cfsetispeed (&tty, (speed_t)B115200);

    /* Setting other Port Stuff */
    tty.c_cflag     &=  ~PARENB;            // Make 8n1
    tty.c_cflag     &=  ~CSTOPB;
    tty.c_cflag     &=  ~CSIZE;
    tty.c_cflag     |=  CS8;

    tty.c_cflag     &=  ~CRTSCTS;           // no flow control
    tty.c_cc[VMIN]   =  1;                  // read doesn't block
    tty.c_cc[VTIME]  =  5;                  // 0.5 seconds read timeout
    tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines

//    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

    /* Make raw */
    cfmakeraw(&tty);

    /* Flush Port, then applies attributes */
    tcflush( PORT, TCIFLUSH );
    if ( tcsetattr ( PORT, TCSANOW, &tty ) != 0) {
       std::cout << "Error " << errno << " from tcsetattr" << std::endl;
    }

    return PORT;
}

const char* readFromPort(int PORT) {
    int n = 0,
        spot = 0;
    char buf = '\0';

    /* Whole response*/
    char response[1024];
    memset(response, '\0', sizeof response);
    int i = 0;

    do {
        n = read( PORT, &buf, 1 );
        sprintf( &response[spot], "%c", buf );
        spot += n;
        clock_gettime(CLOCK_REALTIME, &end);
    } while( buf != '\r' && n > 0 );


    if (n < 0) {
        //std::cout << "Error reading: " << strerror(errno) << std::endl;
    } else if (n == 0) {
        //std::cout << "Read nothing!" << std::endl;
    } else {
        clock_gettime(CLOCK_REALTIME, &start);
        stringstream ss;
        ss << response;
        return ss.str().c_str();
    }

    return "";
}

int32_t main(int32_t argc, char **argv) {

    int32_t BAUD_RATE = B9600;
    const char* device_location = NULL;
    const char* baud = "9600";
    const char* output_file_path = NULL;

    for (int args=0;args<argc;args++){
        if (string(argv[args])=="-b" || string(argv[args])=="--baud") {
            baud = argv[args+1];
            if (baud == "2400") {
                BAUD_RATE = B2400;
            } else if (baud == "4800") {
                BAUD_RATE = B4800;
            } else if (baud == "9600") {
                BAUD_RATE = B9600;
            } else if (baud == "19200") {
                BAUD_RATE = B19200;
            } else if (baud == "38400") {
                BAUD_RATE = B38400;
            } else if (baud == "115200") {
                BAUD_RATE = B115200;
            }
        } else if (string(argv[args])=="-d" || string(argv[args])=="--device") {
            device_location = argv[args+1];
        } else if (string(argv[args])=="-o" || string(argv[args])=="--output") {
            output_file_path = argv[args+1];
        }
    }

    
    int port = setUpSerialPort(device_location, BAUD_RATE);
    if (!device_location || port < 0) {
        return -1;
    } else {
        cout << "Listening to:\t\t" << device_location << endl;
        cout << "Baud rate:\t\t" << baud << endl;
        cout << "Writing to:\t\t" << (output_file_path ? output_file_path : "no output file") << endl;
    }

    int i = 0;
    ofstream output_file;
    clock_gettime(CLOCK_REALTIME, &start);

    if (output_file_path) {
        output_file.open(output_file_path);
        while (true) {
	    const char* msg = readFromPort(port);
            output_file << msg;
	    if (end.tv_sec-start.tv_sec>=5)
		break;
        }  
    } else {
        while (true) {
            cout << readFromPort(port) << endl;
        }
    }    

    output_file.close();
    return 0;

}
