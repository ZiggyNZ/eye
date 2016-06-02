//Test code for AVC wheel functions
#include <stdio.>
#include <time.h>

//External imports
extern "C" int init_hardware();
extern "C" int init(int d_lev);

extern "C" int take_picture();
extern "C" char get_pixel(int row, int col, int color);
extern "C" void set_pixel(int col, int row, char red,char green,char blue);

//extern "C" int open_screen_stream();
//extern "C" int close_screen_stream();
//extern "C" int update_screen();
//extern "C" void GetLine(int row,int threshold);
//extern "C" int display_picture(int delay_sec,int delay_usec);
//extern "C" int save_picture(char filename[5]);

extern "C" int set_motor(int motor,int speed); //Motor 1 = right wheel

extern "C" int read_analog(int ch_adc);
extern "C" int Sleep(int sec, int usec);
extern "C" int select_IO(int chan, int direct);
extern "C" int write_digital(int chan,char level);
extern "C" int read_digital(int chan);
extern "C" int set_PWM(int chan, int value);

extern "C" int init(int d_lev);
extern "C" int connect_to_server( char server_addr[15],int port);
extern "C" int send_to_server(char message[24]);
extern "C" int receive_from_server(char message[24]);

int main(){
	init_hardware(); //Initialise the hardware

	init(1); //Sets up the RPi hardware
   	connect_to_server("130.195.6.196", 1024); // connects to the server
   	send_to_server("Please"); // sends a request for the password to the server
   	char message[24]; //receives the password from the server
   	recieve_from_server(message);
   	prinf("%s", message); // prints out the password that is recieved
   	send_to_server(message); // sends the password back to the server to open the gate
   	Sleep(5, 0); //Wait 5 seconds

   	set_motor(1, 100);
   	set_motor(2, -100);
      Sleep(5, 0);
      set_motor(1, 0);
      set_motor(2, 0);
return 0;}