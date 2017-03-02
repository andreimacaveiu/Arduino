#include <RH_ASK.h>
#include <SPI.h> // Not actualy used but needed to compile
#include <LiquidCrystal.h>
//#include <String>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 10, 5, 4, 3, 2);

RH_ASK driver;

int n=1;

void setup() {
 Serial.begin(9600);  // Debugging only
   if (!driver.init())
       Serial.println("init failed");
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
  // Print a message to the LCD.
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0,0);
  lcd.print("Info(sensor#1)");
    uint8_t buf[20];
    uint8_t buflen =sizeof(buf);
    if (driver.recv(buf, &buflen)) // Non-blocking
    {
     String c=(char*)buf;//convert "buf" to char

    //string to char array
    char info[20];
    char *volt,*amp;
    strncpy(info, c.c_str(), sizeof(info));
    info[sizeof(info) - 1] = 0;
    //lcd.print(info);   
    volt = strtok(info,"  ");
    amp = strtok(NULL," ");
    double power=atof(volt)*atof(amp);
     
    lcd.setCursor(0, 1);
    lcd.print("Voltage: ");
    lcd.print(volt);
    lcd.print(" [V]");

    lcd.setCursor(0,2);
    lcd.print("Current: ");
    lcd.print(amp);
    lcd.print(" [A]");

    lcd.setCursor(0,3);
    lcd.print("Power:   ");
    lcd.print(power);
    lcd.print(" [W]");
     
    } 
    
    delay(500);

//set screen-reset every 15 loops in order to avoid display bugs/errors
    if(n%15==0)
    {
      lcd.clear();
      n=1;
    }
    n++;
    
          
}

