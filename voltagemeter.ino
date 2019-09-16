//用“ //-------------------------------- ”括起来的程序可以改蓝色的字

      //--------------------------------

//V1.0 10/07/2019: varification and validation ok

//V1.1 11/07/2019: tidy up code. use functions and call them in the main

//V2.1 14/08/2019: change the wiring on the hardware to get more accurate 9v measure; code has ben changed to fit this.

//V2.2 14/08/2019: added the page 2 for both 1.5v tester and 9v tester; added sleeping mode;

//V2.3 15/08/2019: added the screen off function;

//V2.4 17/08/2019 changed the welcome page; extend the time before sleeping to 1 min; 

//v2.5 19/08/2019 changed the 9v measure code

//*****************************************************************************************************************************************************************

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSans9pt7b.h>
#include <avr/sleep.h>//this AVR library contains the methods that controls the sleep modes
#include <avr/power.h>

#define OLED_RESET 4
#define interruptPin 2 //Pin we are going to use to wake up the Arduino

Adafruit_SSD1306 display(OLED_RESET);

int tinker;



//=====================================================================================================================================================================

void setup() 
{

  Serial.begin(9600);

  // clear the screen
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setFont(&FreeSans9pt7b);
  display.setTextColor(WHITE);
  display.clearDisplay();

  pinMode(LED_BUILTIN,OUTPUT);//We use the led on pin 13 to indecate when Arduino is A sleep
  pinMode(interruptPin,INPUT_PULLUP);//Set pin d2 to input using the buildin pullup resistor
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN,HIGH);//turning LED on
  tinker=0; //time before sleep
}

//========================================================================================================================================================================
//=============================================================================start of loop===============================================================================

void loop() {

  digitalWrite(LED_BUILTIN, HIGH);
  
  display.setCursor(0,12);
  int xCursor=0, yCursor=26; 
  
  // read the input on analog pin 0:
  int sensorValueAA = analogRead(A0);
  
  //int sensorValueNV = analogRead(A1);
  int sensorValueNVFst = analogRead(A1);
  int sensorValueNVSec = analogRead(A2);
  int sensorValueNVTrd = analogRead(A3);

  //int tinker; //time before sleep
  
  // transfer to realworld voltage  
  float voltageNVf = sensorValueNVFst * (5.0 / 1023.0)-0.08;
  
  float voltageAA = sensorValueAA * (5.0/1023.0);
  float voltageNV = voltageNVf*2; 

  Serial.print(voltageAA);
  Serial.print("        ");
  Serial.println(voltageNVf);

  delay(1000);
  
//............................................................................
  //switch situation
   int s, sn;
   if (voltageAA>=1.49)
    {
    s=1;
    }
    
    else if (voltageAA <= 1.48 && voltageAA >= 1.19)
    {
    s=2;  
    }
    
    else if (voltageAA <= 1.18 && voltageAA >= 0.80)
    {
     s=3;
    }  
       
    else if (voltageAA <= 0.79)
    {
     s=4;
    }
//..............................................................................

   if (voltageNV >=8.94)
    {
    sn=1;
    }

    else if (voltageNV <= 8.93 && voltageNV >= 7.20)
    {
    sn=2;  
    }

    else if (voltageNV <= 7.19 && voltageNV >= 5.40)
    {
     sn=3;
    }     

    else if (voltageNV <= 5.14)
    {
     sn=4;
    }
    
//............................................................................

  //print on screen
    // 1.5V tester
  if (voltageNV < 1.0 && voltageAA >=0.1)
  {
   printAA(voltageAA,s,xCursor,yCursor);
   delay(2000);
   printAASecondPage (s, xCursor, yCursor);
   delay (3000);
   printAAThirdPage  (s, xCursor, yCursor);
   delay (3000);
   tinker = 0;
  }
  
//................................................................................
     //9V tester
   else if(voltageNV >= 1.0)
  {
  printNV(voltageNV,sn,xCursor,yCursor);
  delay (2000);
  printNVSecondPage (sn,xCursor,yCursor);
  delay (3000);
  tinker = 0;
  }

//................................................................................
     //no battery
  else if (voltageAA < 0.1 && voltageNV < 1.0)
  {
  printNoBattery (xCursor,yCursor);
  delay (1);
  tinker++;
  Serial.println (tinker);

    if( tinker == 60)
    {
    Going_To_Sleep();
    }
    
  }

 
 

}

//=============================================================End of loop=================================================================================
//=========================================================================================================================================================

    void printAA(float voltageAA,int s,int xCursor, int yCursor)
 {
  //display.print("1.5V tester: ");
  display.print(voltageAA);
  display.print("V");
  display.setCursor(xCursor,yCursor);
  switch(s){  
  case 1:
//--------------------------------
  display.print("New battery");
//--------------------------------  
  break;

  case 2:
//--------------------------------
  display.print("Average battery");
//--------------------------------
  break;

  case 3:
//----------------------------------
  display.print("Low battery");
//----------------------------------
  break;

  case 4:
//--------------------------------
  display.print("Dead battery");
//--------------------------------
  break;
  }
    display.display();
    display.clearDisplay();
}

//=======================================================================================================================================

  void printAASecondPage (int s,int xCursor, int yCursor)
  {
    display.setCursor(0,12);
    switch(s)
    {
      case 1:
 //--------------------------------------
      display.print("Put into Good");
      display.setCursor(xCursor,yCursor);
      display.print("Battery Section");
 //----------------------------------------
     break;

     case 2:
//-----------------------------------------
      display.print("Put in Average");
      display.setCursor(xCursor,yCursor);
      display.print("Battery Section");
//-----------------------------------------
     break;

     case 3:
//-----------------------------------------
      display.print("Put into Low");
      display.setCursor(xCursor,yCursor);
      display.print("Battery Section");
//-----------------------------------------
     break;

     case 4:
//-----------------------------------------
      display.print("Please Recycle");
      display.setCursor(xCursor,yCursor);
      display.print("This Battery");
//-----------------------------------------
     break;
      }
    display.display();
    display.clearDisplay();
    }
    

//=======================================================================================================================================

void printAAThirdPage (int s,int xCursor, int yCursor)

{
    display.setCursor(0,12);
    switch(s)
    {
      case 1:
 //--------------------------------------
      display.print("For High Power");
      display.setCursor(xCursor,yCursor);
      display.print("Devices");
 //----------------------------------------
     break;

     case 2:
//-----------------------------------------
      display.print("For Medium");
      display.setCursor(xCursor,yCursor);
      display.print("Power Devices");
//-----------------------------------------
     break;

     case 3:
//-----------------------------------------
      display.print("For Low Power");
      display.setCursor(xCursor,yCursor);
      display.print("Devices");
//-----------------------------------------
     break;

     case 4:
//-----------------------------------------
      display.print("Please Recycle");
      display.setCursor(xCursor,yCursor);
      display.print("This Battery");
//-----------------------------------------
     break;
      }
    display.display();
    display.clearDisplay();
    }


//=======================================================================================================================================

  void printNV(float voltageNV, int sn, int xCursor, int yCursor)
  {
  //display.print("9V tester: ");  
  display.print(voltageNV);
  display.print("V");
  display.setCursor(xCursor,yCursor);
  switch(sn)
  {  
  case 1:
//--------------------------------
  display.print("New battery");
//--------------------------------
  break;

  case 2:
//--------------------------------
  display.print("Average battery");
//--------------------------------
  break;

  case 3:
//----------------------------------
  display.print("Low battery");
//----------------------------------
  break;

  case 4:
//--------------------------------
  display.print("Dead battery");
//--------------------------------
  break;
  }
    display.display();
    display.clearDisplay();
}

//=============================================================================================================================================

 void printNVSecondPage (int sn,int xCursor, int yCursor)
  {
    display.setCursor(0,12);
    switch(sn)
    {
      case 1:
 //--------------------------------------
      display.print("Put into Good");
      display.setCursor(xCursor,yCursor);
      display.print("Battery Storage");
 //----------------------------------------
     break;

     case 2:
//-----------------------------------------
      display.print("Put in Average");
      display.setCursor(xCursor,yCursor);
      display.print("Battery Storage");
//-----------------------------------------
     break;

     case 3:
//-----------------------------------------
      display.print("Put into Low");
      display.setCursor(xCursor,yCursor);
      display.print("Battery Storage");
//-----------------------------------------
     break;

     case 4:
//-----------------------------------------
      display.print("Please Recycle");
      display.setCursor(xCursor,yCursor);
      display.print("This Battery");
//-----------------------------------------
     break;
      }
    display.display();
    display.clearDisplay();
    }
//=============================================================================================================================================

  void printNoBattery (int xCursor, int yCursor)
{
//-------------------------------------
    display.print ("Please insert");
//-------------------------------------
    display.setCursor(xCursor,yCursor);
//--------------------------------
    display.print ("a battery");
//--------------------------------
    display.display();
    display.clearDisplay();
}

//===============================================================================================================================================

void Going_To_Sleep(){
    sleep_enable();//Enabling sleep mode
    attachInterrupt(0, wakeUp, LOW);//attaching a interrupt to pin d2
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);//Setting the sleep mode, in our case full sleep
    //power_all_disable ();
    
    digitalWrite(LED_BUILTIN, LOW);
    display.ssd1306_command(SSD1306_DISPLAYOFF);
    delay(1000);

    power_all_disable ();
    sleep_cpu();//activating sleep mode
    tinker = 0;
    
    display.ssd1306_command(SSD1306_DISPLAYON);
    display.setCursor(12,21);
    display.print("    Welcome");
    display.display();
    delay(1000);
    display.clearDisplay();
    
    digitalWrite(LED_BUILTIN,HIGH);//turning LED on
  }

//===============================================================================================================================================

void wakeUp(){
   sleep_disable();//Disable sleep mode
   power_all_enable();  
   detachInterrupt(0); //Removes the interrupt from pin 2;
}

//==============================================================================================================================================
