/*SKELETON CODES FOR AVC */
/*  A.Roberts, 5 Apr 2016*/
#include <stdio.h>

//#include <pthread.h>
#include <time.h>
#include <string.h>

// sudo gcc -Wall
extern "C" int init_hardware();
extern "C" int init(int d_lev);

extern "C" int take_picture();
extern "C" char get_pixel(int row, int col, int color);
extern "C" void set_pixel(int col, int row, char red,char green,char blue);

extern "C" int open_screen_stream();
extern "C" int close_screen_stream();
extern "C" int update_screen();
//extern "C" void GetLine(int row,int threshold);
extern "C" int display_picture(int delay_sec,int delay_usec);
extern "C" int save_picture(char filename[5]);

extern "C" int set_motor(int motor,int speed);

extern "C" int read_analog(int ch_adc);
extern "C" int Sleep(int sec, int usec);
extern "C" int select_IO(int chan, int direct);
extern "C" int write_digital(int chan,char level);
extern "C" int read_digital(int chan);
extern "C" int set_PWM(int chan, int value);

extern "C" int connect_to_server( char server_addr[15],int port);
extern "C" int send_to_server(char message[24]);
extern "C" int receive_from_server(char message[24]);


int v_left = 0;
int v_right = 0;

int main()
{
    int i;
    init(0);
    // connect camera to the screen
    open_screen_stream();
    // set all didgital outputs to +5V
    for (i = 0; i < 8; i++)
    {
      // set all digital channels as outputs
      select_IO(i,0);
      write_digital(i,1);
    }
    while(1)
    {
       take_picture();      // take camera shot
       // draw some line
       set_pixel(100, 55 ,255,0,0);
       set_pixel(101, 55 ,255,0,0);
       set_pixel(102, 55 ,255,0,0);
       set_pixel(103, 55 ,255,0,0);
       // display picture
       update_screen();

       // check motors
       v_left = -135;
       v_right = -135;
       set_motor(1,v_right);
       set_motor(2,v_left);
       Sleep(1,0);
       v_left = 135;
       v_right = 135;
       set_motor(1,v_right);
       set_motor(2,v_left);
       Sleep(1,0);
       for (i = 0 ; i < 8; i++)
       {
        int av = read_analog(i);
        printf("ai=%d av=%d\n",i,av);
    }


     }

   // terminate hardware
    close_screen_stream();
    set_motor(1,0);
    set_motor(2,0);
  
    return 0;


}
