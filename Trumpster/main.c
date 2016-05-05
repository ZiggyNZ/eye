#include <stdio.h>
#include <stdlib.h>
extern "C" int  InitHardware();
extern "C" int  ReadAnalog(int  ch_adc);
extern "C" int  Sleep(int sec, int  usec);
int  main()
{
    InitHardware();
    //We  declare  an  integer  variable  to  store  the  ADC  data
    int  adc_reading;
    //Reads  from  Analog  Pin 0 (A0)  through  A7
    adc_reading = ReadAnalog(0);
    //Prints  read  analog  value
    printf("%d\n",adc_reading);
    //Waits  for  0.5  seconds  (500000  microseconds)
    Sleep(0,500000);
    int number = 120;
    char character;
    double kill;
    long af;
    printf("%d n", sizeof(number));
    printf("%d n", sizeof(character));
    printf("%d n", sizeof(kill));
    printf("%d", sizeof(af));
    return 0;
}