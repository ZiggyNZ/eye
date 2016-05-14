//Test code for direction to turn
#include <stdio.>
#include <time.h>

//External imports
extern "C" int init_hardware();
//extern "C" int init(int d_lev);

extern "C" int take_picture();
extern "C" char get_pixel(int row, int col, int color);
extern "C" void set_pixel(int col, int row, char red,char green,char blue);

//extern "C" int open_screen_stream();
//extern "C" int close_screen_stream();
//extern "C" int update_screen();
//extern "C" void GetLine(int row,int threshold);
//extern "C" int display_picture(int delay_sec,int delay_usec);
//extern "C" int save_picture(char filename[5]);

extern "C" int set_motor(int motor,int speed);

extern "C" int read_analog(int ch_adc);
extern "C" int Sleep(int sec, int usec);
extern "C" int select_IO(int chan, int direct);
extern "C" int write_digital(int chan,char level);
extern "C" int read_digital(int chan);
extern "C" int set_PWM(int chan, int value);

//extern "C" int connect_to_server( char server_addr[15],int port);
//extern "C" int send_to_server(char message[24]);
//extern "C" int receive_from_server(char message[24]);

int main(){
	init_hardware(); //Initialise the hardware
	while(true){ //Repeat until told otherwise
		take_picture(); //AVC takes a picture!
		int pixelColour; //Black pixel = 0, white = 1
		int sum = 0; //Will determine whether the line is on the left or right side of the image (or even there at all)
		for(x=0, x<320, x++){ //Camera is 320 long x 240 high, this will scan the centre line
			int r = get_pixel(120, x, 1); //Scan pixel for red value
			int g = get_pixel(120, x, 2); //green pixel
			int b = get_pixel(120, x, 3); //blue pixel
			if(r > 150 && g > 150 && b > 150){ //If a white pixel is detected (for now assuming white = anything over 150)
				pixelColour = 1;
			}
			else{ //If white pixel is not detected, consider it a black pixel
				pixelColour = 0;
			}
			int value = (x - 160) * pixelColour; //If white pixel on left, will be negative, if on right, will be positive
			sum += value; //Negative = line on left of centre, positive = line on right of centre (magnitude of number represents the distance away from centre)		
		}
		motor_v = 0; // if between 200 &-200 then the speed will default to 127
		if(sum > 200){ //If line is farily to the left (will have to test line size and tinker with '200' value)
			//motor_v++; // increases speed of the left wheel and lowers the speed of the right
			motor_v = motor_v + .4*(sum);
		}
		else if(sum < -200){ //If line is on the left
			//motor_v--;// increases right wheel and decreases left
			motor_v = motor_v + .4*(sum); // is =ve bc of the -ve sum
		} // Current code may cause the pi to zig zag around the line aslong as there's white on the camera
		// Will integrate PID soon.
        set_motor(1, 127 + motor_v);
        set_motor(2, 127 - motor_v);
        Sleep(100000000, 0); //SLeep for 0.1 of a second
	}

return 0;}
