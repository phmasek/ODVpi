#include <iostream>
#include <cerrno>

// SERIAL
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>    /* For O_RDWR */
#include <unistd.h>   /* For open(), creat() */
#include <cstring>
// SERIAL END

using namespace std;

int setUpSerialPort(const char* port_location) {

    int PORT = open( port_location, O_RDWR| O_NOCTTY );

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
    cfsetospeed (&tty, (speed_t)B9600);
    cfsetispeed (&tty, (speed_t)B9600);

    /* Setting other Port Stuff */
    tty.c_cflag     &=  ~PARENB;            // Make 8n1
    tty.c_cflag     &=  ~CSTOPB;
    tty.c_cflag     &=  ~CSIZE;
    tty.c_cflag     |=  CS8;

    tty.c_cflag     &=  ~CRTSCTS;           // no flow control
    tty.c_cc[VMIN]   =  1;                  // read doesn't block
    tty.c_cc[VTIME]  =  5;                  // 0.5 seconds read timeout
    tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines

    /* Make raw */
    cfmakeraw(&tty);

    /* Flush Port, then applies attributes */
    tcflush( PORT, TCIFLUSH );
    if ( tcsetattr ( PORT, TCSANOW, &tty ) != 0) {
       std::cout << "Error " << errno << " from tcsetattr" << std::endl;
    }

    return PORT;
}

int writeToPort(const string msg, int PORT) {
    unsigned char *val=new unsigned char[msg.length()+1];
    strcpy((char *)val,msg.c_str());

    int n_written = 0,
        spot = 0;

    do {
        n_written = write( PORT, &val[spot], 1 );
        spot += n_written;
        if (spot == (int) msg.length()) {
            write( PORT, "\r", 1 );
        }
    } while (spot < msg.length() && n_written > 0);

    return spot;
}


int32_t main(int32_t argc, char **argv) {
    int port = setUpSerialPort("/dev/ttyS0");
    cout << port << endl;

    while (true) {
        cout << writeToPort("Hello", port) << endl;
        usleep(1000000);
    }

}