//Test code for AVC wheel functions
#include <stdio.>
#include <time.h>

//Universal imports
extern "C" int init_hardware();
extern "C" int Sleep(int sec, int usec);

//Camera and movement imports
extern "C" int take_picture();
extern "C" char get_pixel(int row, int col, int color);
extern "C" int set_motor(int motor,int speed); //Motor 1 = right wheel

//Networking imports
extern "C" int init(int d_lev);
extern "C" int connect_to_server( char server_addr[15],int port);
extern "C" int send_to_server(char message[24]);
extern "C" int receive_from_server(char message[24]);

//Unused / testing imports
//extern "C" int open_screen_stream();
//extern "C" int close_screen_stream();
//extern "C" int update_screen();
//extern "C" void GetLine(int row,int threshold);
//extern "C" int display_picture(int delay_sec,int delay_usec);
//extern "C" int save_picture(char filename[5]);
//extern "C" void set_pixel(int col, int row, char red,char green,char blue);
//extern "C" int select_IO(int chan, int direct);
//extern "C" int write_digital(int chan,char level);
//extern "C" int read_digital(int chan);
//extern "C" int set_PWM(int chan, int value);
//extern "C" int read_analog(int ch_adc);

int main(){
	init_hardware(); //Initialise the hardware
	double lineX; //Will keep track of the centre of the line (regardless of where it is)
	double oldLineX = 0; //Keep track of where the line was previously
	double changeDifferential; //Difference between pveious line position and current
	//Constants defined up here makes for easy tinkering
	int forwardTimeUsec = 500000;//How long to move forwards for (before turning) in microseconds
	int forwardTimeSec = 0;//Same as above except seconds
	int turnTimeUsec = 0;//Time to turn corners for in microseconds
	int turnTimeSec = 1;//Same as above except seconds

	init(1); //Sets up the RPi hardware
   	connect_to_server("130.195.6.196", 1024); //Connects to the given IP
   	send_to_server("Please"); //Sends a request for the password to the server
   	char message[24]; //Receives the password from the server
   	recieve_from_server(message);//^
   	prinf("Password is: %s", message); // prints out the password that is recieved
   	send_to_server(message); //Sends the password back to the server to open the gate
   	Sleep(5, 0); //Wait 5 seconds (give time for the gate to open)

	while(true){ //Repeat forever!
		//update_screen(); //Visual representation of camera (for testing puroses)
		take_picture(); //AVC takes a picture!
		double changeProportion = 0; //Defualt motor change = no change
		double lineWidth = 0; //Keep track of how many pixels wide the line is
		double sum = 0; //Will determine whether the line is on the left or right side of the image (or even there at all)
		int pixelColour = 0; //Essentially bool value, 0 = Black pixel, 1 = White pixel
		int pixelLeft = 0;//Same as pixelColour except for scanning left turns
   		int pixelRight = 0;//Same as pixelColor except for scanning right turns
   		int pixelLeftTotal = 0;//Number of white pixels detected on left side
   		int pixelRightTotal = 0;//Number of white pixels detected on right side
   		bool leftTurn = false;//Whether to turn left at end of code or not
   		bool rightTurn = false;//Same as above (except right)

		for(int x=0, x<320, x++){ //Camera is 320 long x 240 high, this will scan the centre line
			int r = get_pixel(x, 120, 1); //Red pixel values had the biggest difference between the white/black (found this from testing)
			if(r > 100){ //Values ranged from around 0 to 160 (rather than expected 0 to 255)
				pixelColour = 1;}//If white pixel is detected then pixelColour = 1

			lineWidth += pixelColour; //A running total of the lines width (in pixels)
			int value = (x - 160) * pixelColour; //If white pixel on left, will be negative, if on right, will be positive
			sum += value;} //Negative = line on left of centre, positive = line on right of centre (magnitude of number represents the distance away from centre)

		lineX = sum/lineWidth; //Will give centre of line x co-ordinate (between -160 and 160)
		changeProportion = lineX/160; //Line pixel position (which cannot be over 160 pixels) divided by 160 (the max possible value)
		changeDifferential = lineX - oldLineX;//Differential currently unused

		if(lineWidth < 5){ //If no lines are detected
			set_motor(1, -100);//Do U-turn in the right direction
			set_motor(2, -100);//As we are prioritising left turns, right turns means there isnt the possibility of us going back the way we came
		}
		else{ //If there is a line in view
		set_motor(1, 100 - (changeProportion*100));
		set_motor(2, -100 - (changeProportion*100));
		oldLineX = lineX;}	

		if(lineX < 50 && lineX > -50){//If we are on line (or close enough) then scan for alternate paths
			for(int y=0, y<60, y++){//IMPORTANT NOTE: values are inverted to scan 'bottom' as camera is tehnically upsidedown
				int right = get_pixel(60 , 180 + y, 1); //Scan line (like in concept diagram)
				int left = get_pixel(260 , 180 + y, 1);
				if(right > 100){
					pixelRight = 1;} //Same ideas as the line following code
				if(left > 100){
					pixelLeft = 1;}
				pixelLeftTotal += pixelLeft;
				pixelRightTotal += pixelRight;
			}
			if(pixelLeftTotal > 20){//If left side is detecting a line
				leftTurn = true; //Turn left

			}
			else if(pixelRightTotal > 20 && pixelLeftTotal < 5){//If line is detected on the right and there is no potential to turn left
				rightTurn = true;
			}
			if(leftTurn){
				set_motor(1, 100);//Move forwards before turning
				set_motor(1, -100);
				Sleep(forwardTimeSec, forwardTimeUsec); //How long to move forwards for (using defined constants)

				set_motor(1, 100);//Turn left
				set_motor(2, 100);
				Sleep(turnTimeSec, turnTimeUsec);//How long to turn for (again using constants)
			}
			if(rightTurn){
				set_motor(1, 100);//Same as above
				set_motor(1, -100);
				Sleep(forwardTimeSec, forwardTimeUsec);

				set_motor(1, -100);
				set_motor(2, -100);
				Sleep(turnTimeSec, turnTimeUsec);//Turn right instead
			}	
		}
		Sleep(0, 100000); //SLeep for 0.1 of a second
	}
return 0;}
