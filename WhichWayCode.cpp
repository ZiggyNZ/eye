//Test code for AVC wheel functions
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
		int motorChange = 0;
		int pixelColour; //Black pixel = 0, white = 1
		int sum = 0; //Will determine whether the line is on the left or right side of the image (or even there at all)

		for(x=0, x<320, x++){ //Camera is 320 long x 240 high, this will scan the centre line
			int r = get_pixel(x, 120, 1); //Red pixel values had the biggest difference between the white/black (found this from testing)
			if(r > 100){ //Values ranged from around 0 to 160 (rather than expected 0 to 255)
				pixelColour = 1;}
			else{ //If white pixel is not detected, consider it a black pixel
				pixelColour = 0;}
			int value = (x - 160) * pixelColour; //If white pixel on left, will be negative, if on right, will be positive
			sum += value;}} //Negative = line on left of centre, positive = line on right of centre (magnitude of number represents the distance away from centre)

		sum /= 320; //Average the sum value and we should get around the pixel at the centre of the line
		int proportion = sum / 45; //Highest testing value was 41 ish so 45 adds some leway for error
		motorChange = 122 * proportion;
		set_motor(1, 122 + motorChange);
		set_motor(2, -122 + motorChange);
		Sleep(0, 100000000); //SLeep for 0.1 of a second
	}
set_motor(1, 0);
set_motor(2, 0);
return 0;}























