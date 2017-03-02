#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

RH_ASK driver;

void setup()
{
    Serial.begin(9600);    // Debugging only
    if (!driver.init())
         Serial.println("init failed");
}

void loop()
{
    
    float amp=analogRead(A6); //reads analog value from current sensor at analog pin 6
    float vol=analogRead(A0); 
    
    for(int i=0;i<=5;i++) //makes an average of the values
    {
     
      amp=(amp+analogRead(A6))/2;
      vol=(vol+analogRead(A0))/2;
       delay(150);
    }
    int q;
    //char *current;
    amp= {fabs((((amp-511.99)*5)/1023)/0.0645)}; //((((value-511)*5)/1023)/0.0645
    vol=fabs(vol/44.5);
    
    char info[12]={NULL};
    dtostrf(vol,2,2,info);
    char aux [5]={NULL};
    dtostrf(amp,2,2,aux);
    
    int n=strlen(info);   
    int m=strlen(aux);

    info[n]=' ';
    
    for(int i=n+1;i<=n+m;i++)
      info[i]=aux[i-n-1];

    info[n+m+1]=' ';
    
    Serial.println(info);
    
    driver.send((uint8_t*)info, strlen(info));
    driver.waitPacketSent();
}
