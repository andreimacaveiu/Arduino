#include "Ultrasonic.h"
Ultrasonic ultra1(45,47);//trigger , echo
Ultrasonic ultra2(43,41);
#include <QTRSensors.h>
QTRSensorsRC qtr((unsigned char[]) {2,3,4,5,6,7,8,9}, 8);
int max[8]={NULL},min[8]={9999,9999,9999,9999,9999,9999,9999,9999} ; //minumum and maximum values for each sensor , used for calibration
int cal[8];//used for storing the calibrated value
int R=255;
int L=255;

void setup()
{
  Serial.begin(9600);
  Serial.println("test");
  calibrate();
  pinMode( 10, OUTPUT );
  pinMode( 11, OUTPUT );
  pinMode( 12, OUTPUT );
  pinMode( 13, OUTPUT );
  analogWrite(10, 255);
  analogWrite(11, 255);
  analogWrite(12, 255);
  analogWrite(13, 255);
}

void compensate(int&r ,int& l,bool& brake)
{
  brake=false;
  unsigned int pos[8];
  qtr.read(pos);
  if(pos[7]>cal[7] && pos[0]>cal[0] && pos[3]>cal[3] && pos[4]>cal[4])
                      brake=true;
  else if(pos[3]>cal[3] || pos[4]>cal[4])
                 {r=255;l=255;}
  else if(pos[0]>cal[0])
        {r=255;l=0;}
    else if(pos[1]>cal[1])
          {r=200;l=0;}
     else if(pos[2]>cal[2])
           {r=150;l=0;}
      else if(pos[5]>cal[5])
           {r=0;l=150;}
       else if(pos[6]>cal[6])
           {r=0;l=200;}
        else if(pos[7]>cal[7])
             {r=0;l=255;}
}

void calibrate()
{
 
 for (int d = 0; d<250; d++)  // make the calibration take about 5 seconds
{
  unsigned int pos[8];
  qtr.read(pos);
  for(int i=0;i<8;i++)
  {
    if(pos[i]<min[i])
      min[i]=pos[i];
    if(pos[i]>max[i])
      max[i]=pos[i];
  }
     delay(20);
  }
  for(int i=0;i<8;i++)
    cal[i]=(3*max[i]-min[i])/4;
}

void stop_vehicle()
{
    analogWrite(10, 180);
    analogWrite(11, 0);
    delay(250);
    analogWrite(10, 255);
    analogWrite(11, 255);
    analogWrite(12, 255);
    analogWrite(13, 255);
    
}
void loop()
{
  
  bool brake;
  int n=ultra1.Ranging(CM);
  int m=ultra2.Ranging(CM);
  if(n<15 || m<15)
    stop_vehicle();
  else
  {
  compensate(R,L,brake);
  if(brake==true)
  {
    stop_vehicle();
    delay(10000);
  }
  else
  {
  analogWrite(12,R);
  analogWrite(13,L);
  analogWrite(11, 100);
  analogWrite(10, 0);
  }
  }
}


