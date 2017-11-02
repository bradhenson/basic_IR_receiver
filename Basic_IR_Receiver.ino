/********************************************************************************
Basic_IR_Receiver.c
Author: Bradford Henson
Date: 10/30/2017
Version: 0.0.0.1
License: USE AT YOUR OWN RISK

Description: 
------------
TODO

Hardware connections are as followed:
-------------------------------------
TODO

Additional considerations:
-------------------------------------
- A backpack is used with the LCD to provide connectivity with fewer pins. It also uses the I2C.
  The library used for the backpack in the prototype is from the following website. This effectively
  replaces (extends functionality) the standard LiquidCrystal library. 
  https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads/NewliquidCrystal_1.3.4.zip
  A good tutorial for using this type of backpack is at:
  http://arduino-info.wikispaces.com/LCD-Blue-I2C

**********************************************************************************/
#define F_CPU 16000000UL    // define it now as 16 MHz unsigned long

#include <avr/io.h>       // included in all avr projects
#include <util/delay.h>     // add this to use the delay function
#include <avr/interrupt.h>    // add this to use the interrupt function
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

uint8_t inputBuffer[44];    // creates a buffer array for storing input values
uint8_t readingOne[10];
uint8_t readingTwo[10];
uint8_t readingThree[10];
uint8_t totalReadings = 0;    // keeps track of how many readings have been received
uint8_t k = 0;
uint8_t i = 0;
uint8_t finalRoute = 0;
uint8_t headerGood = 1;     // flag
uint8_t readingMatch = 1;   // flag
uint8_t compareStatus = 0;    // flag
uint8_t readyStatus = 0;    // flag
int convertedRoute = 0;

void compareinputbuffer();
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); // A new LCD library has to be installed
  
void setup()
{  
  
  /* PIN Registers */
  DDRD &= ~(1 << 2);      // clear DDRD bit 2, sets PD2 (pin 4) for input
  PORTD |= (1 << 2);      // set PD2/INT0 (pin 4) internal pull-up resistor
  DDRB |= (1 << 6 );
  PORTD &= ~(1 << 6);
  
  /* Interrupt Registers */
  EIMSK = 0b00000001;     // enable External Interrupt Request for INIT0
  EICRA = 0b00000011;     // enables interrupt on rising edge of INIT0 
  sei();            // enables all interrupts
  

  lcd.begin(16,2);                // Set up the LCD's number of columns and rows
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(F("  Standing By   "));
  lcd.setCursor(0, 1);
  lcd.print(F("               "));
}
  void loop() {

    if (totalReadings == 3)
    {
      EIMSK &= ~(1 << 0);
      i = 0;
      while(i < 10)
      {       
        if (readingOne[i] == readingTwo[i] && readingTwo[i] == readingThree[i])
        {
          readingMatch = 1;
        }
        else
        {
          for (i = 0; i < 10; i++) readingOne[i] = 0;
          for (i = 0; i < 10; i++) readingTwo[i] = 0;
          for (i = 0; i < 10; i++) readingThree[i] = 0;
          readingMatch = 0;
          totalReadings = 0;
          compareStatus = 0;
          readyStatus = 0;
          EIMSK |= (1 << 0);
          break;
        }
        i++;
      }
      if(readingMatch == 1 && i == 10) 
      {
        for (k = 0; k < 16; k++);
        {
          if (readingOne[k] == 0) convertedRoute &= ~(1 << k);
          else convertedRoute |= (1 << k);
        }
        switch (convertedRoute)
        {
        case 0b0000000001010101:
          finalRoute = 0b00000000;  // route 1 - 000
          compareStatus = 1;
          break;
        case 0b0000000010101001:
          finalRoute = 0b00000001;  // route 2 - 001
          compareStatus = 1;
          break;
        case 0b0000000010100101:
          finalRoute = 0b00000010;  // route 3 - 010
          compareStatus = 1;
          break;
        case 0b0000000101001001:
          finalRoute = 0b00000011;  // route 4 - 011
          compareStatus = 1;
          break;
        case 0b0000000010010101:
          finalRoute = 0b00000100;  // route 5 - 100
          compareStatus = 1;
          break;
        case 0b0000000100101001:
          finalRoute = 0b00000101;  // route 6 - 101
          compareStatus = 1;
          break;
        case 0b0000000100100101:
          finalRoute = 0b00000110;  // route 7 - 110
          compareStatus = 1;
          break;
        case 0b0000001001001001:
          finalRoute = 0b00000111;  // route 8 - 111
          compareStatus = 1;
          break;
        }
        if (compareStatus == 1)
        {
          readyStatus = 1;
          switch (finalRoute)
          {
          case 0:
              lcd.setCursor(0, 0);
              lcd.print(F("RECEIVED READING"));
              lcd.setCursor(0, 1);
              lcd.print(F(" ROUTE 1 - 000  "));
            break;
          case 1:
            lcd.setCursor(0, 0);
            lcd.print(F("RECEIVED READING"));
            lcd.setCursor(0, 1);
            lcd.print(F(" ROUTE 2 - 001  "));
            break;
          case 2:
            lcd.setCursor(0, 0);
            lcd.print(F("RECEIVED READING"));
            lcd.setCursor(0, 1);
            lcd.print(F(" ROUTE 3 - 010  "));
            break;
          case 3:
            lcd.setCursor(0, 0);
            lcd.print(F("RECEIVED READING"));
            lcd.setCursor(0, 1);
            lcd.print(F(" ROUTE 4 - 011  "));
            break;
          case 4:
            lcd.setCursor(0, 0);
            lcd.print(F("RECEIVED READING"));
            lcd.setCursor(0, 1);
            lcd.print(F(" ROUTE 5 - 100  "));
            break;
          case 5:
            lcd.setCursor(0, 0);
            lcd.print(F("RECEIVED READING"));
            lcd.setCursor(0, 1);
            lcd.print(F(" ROUTE 6 - 101  "));
            break;
          case 6:
            lcd.setCursor(0, 0);
            lcd.print(F("RECEIVED READING"));
            lcd.setCursor(0, 1);
            lcd.print(F(" ROUTE 7 - 110  "));
            break;
          case 7:
            lcd.setCursor(0, 0);
            lcd.print(F("RECEIVED READING"));
            lcd.setCursor(0, 1);
            lcd.print(F(" ROUTE 8 - 111  "));
            break;                                                                        
          }
        } 
        else
        {
          for (i = 0; i < 10; i++) readingOne[i] = 0;
          for (i = 0; i < 10; i++) readingTwo[i] = 0;
          for (i = 0; i < 10; i++) readingThree[i] = 0;
          totalReadings = 0;
          compareStatus = 0;
          readyStatus = 0;
          readingMatch = 0;
          EIMSK |= (1 << 0);
        }        
      }
    }
  /////////////////////////////////////////////////////////////////////////
  //
  //      Finally, the vehicle is ready to start
  //
  /////////////////////////////////////////////////////////////////////////
  
  // add code here to start moving the vehicle
  
  
  
  
  } // end of loop
 
/////////////////////////////////////////////////////////////////////////////

  ISR(INT0_vect)
  {
    _delay_us(230);
    inputBuffer[0] = PIND2;
        
    for (i = 1; i < 44; i++)
    {
      _delay_us(560);
      inputBuffer[i] = PIND2;
    }
    for (i = 0; i < 16; i++)
    {
      if (inputBuffer[i] == 1) headerGood = 1;
      else
      {
        headerGood = 0;
        break;
      }
    }
    
    for (i = 15; i < 24; i++)
    {
      if (headerGood == 0) break;
      if (inputBuffer[i] == 0) headerGood = 1;
      else headerGood = 0;
    }   
    
    i = 24;
    
    while(headerGood == 1 && i < 36)
    {
       switch (i)
       {
         case 0:
          if (inputBuffer[i] == 1) headerGood = 1;
          else headerGood = 0;         
          break;         
         case 1:
          if (inputBuffer[i] == 0) headerGood = 1;
          else headerGood = 0;
          break;
         case 2:
          if (inputBuffer[i] == 1) headerGood = 1;
          else headerGood = 0;         
          break;
         case 3:
          if (inputBuffer[i] == 0) headerGood = 1;
          else headerGood = 0;         
          break;
         case 4:
          if (inputBuffer[i] == 1) headerGood = 1;
          else headerGood = 0;         
          break;
         case 5:
          if (inputBuffer[i] == 0) headerGood = 1;
          else headerGood = 0;         
          break;  
         case 6:
          if (inputBuffer[i] == 1) headerGood = 1;
          else headerGood = 0;         
          break;
         case 7:
          if (inputBuffer[i] == 0) headerGood = 1;
          else headerGood = 0;         
          break;
         case 8:
          if (inputBuffer[i] == 1) headerGood = 1;
          else headerGood = 0;       
          break;
         case 9:
          if (inputBuffer[i] == 0) headerGood = 1;
          else headerGood = 0;       
          break;
         case 10:
          for (i = 35; i < 45; i++)
          {
            switch(totalReadings)
            {
            case 0:
              readingOne[i-35] = inputBuffer[i];
              if(i == 45) totalReadings = 1;
              break;
            case 1:
              readingTwo[i-35] = inputBuffer[i];
              if(i == 45) totalReadings = 2;
              break;
            case 2:
              readingThree[i-35] = inputBuffer[i];
              if(i == 45) totalReadings = 3;
              break;  
            }
          }
          break;
      }       
      i++;
    
    }
  }
 
