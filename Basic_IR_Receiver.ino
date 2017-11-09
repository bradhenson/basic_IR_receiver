/********************************************************************************
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

********************************************************************************/
#define F_CPU 16000000UL          // define it now as 16 MHz unsigned long

#include <avr/io.h>               // included in all avr projects
#include <util/delay.h>           // add this to use the delay function
#include <avr/interrupt.h>        // add this to use the interrupt function
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

uint8_t inputBuffer[44];          // creates a buffer array for storing input values
uint8_t readingOne[10];           // buffer for the first reading
uint8_t readingTwo[10];           // buffer for the second reading
uint8_t readingThree[10];         // buffer for the third reading
uint8_t totalReadings = 0;        // keeps track of how many readings have been received
uint8_t k = 0;                    // temp variable used to control a loop
uint8_t i = 0;                    // temp variable used to control a loop
uint8_t finalRoute;               // the final route after all of the parsing and verifying
uint8_t headerGood = 1;           // flag 
uint8_t readingMatch = 1;         // flag
uint8_t compareStatus = 0;        // flag
uint8_t readyStatus = 0;          // flag
int convertedRoute = 0;           // temp variable compared against the known 8 routes

void compareinputbuffer();
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

/********************************************************************************

                            SETUP SECTION

********************************************************************************/ 
void setup()
{  
  
  /* PIN Registers */
  DDRD &= ~(1 << 3);              // clear DDRD bit 3, sets PD3 for input
  PORTD |= (1 << 3);              // set PD2/INT0 internal pull-up resistor
  
  /* Interrupt Registers */
  EIMSK = 0b00000001;             // enable External Interrupt Request for INIT0
  EICRA = 0b00000001;             // enables interrupt on rising edge of INIT0 
  sei();                          // enables all interrupts
  
  /* Setup Display */
  lcd.begin(16,2);                // Set up the LCD's number of columns and rows
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(F("  Standing By   "));
  lcd.setCursor(0, 1);
  lcd.print(F("                "));
}

/********************************************************************************

                                EVENT LOOP

********************************************************************************/ 
  void loop() {
    
    /***************************************************
    * Up til now, the main loop has just been looking for
    * changes to the interrupt pin and firing off the ISR function.
    * When the ISR function has recorded 3 readings, we can
    * compare the three readings against one another just to 
    * make sure they are the same three readings. If they are
    * not the same readings, we clear everything and start over.
    * It seems like a drastic measure at this point, but 
    * we don't want the wrong values getting set before the robot
    * starts moving.
    ***************************************************/
    if (totalReadings == 3)
    {
      cli();                      // disables all interrupts
      i = 0;
      while(i < 10)
      {       
        if (readingOne[i] == readingTwo[i] && readingTwo[i] == readingThree[i])
        {
          readingMatch = 1;
        }
        else
        {
          /* Clears all the data and starts over */
          for (i = 0; i < 10; i++) readingOne[i] = 0;
          for (i = 0; i < 10; i++) readingTwo[i] = 0;
          for (i = 0; i < 10; i++) readingThree[i] = 0;
          readingMatch = 0;
          totalReadings = 0;
          compareStatus = 0;
          readyStatus = 0;
          sei();                  // enables all interrupts
          break;
        }
        i++;
      }
      
       /***************************************************
       * Now that it is confirmed that the three readings 
       * are in fact the same, the following (for) loop
       * converts the readingX array into an integer variable
       * type. Then the (switch) statement compares the integer
       * to the known list of 8 choices. if it finds a match,
       * it will set finalRoute equal to the convertedRoute and 
       * sets the compareStatus flag to 1. We keep the convertedRoute
       * and finalRoute seperate to allow for this final check.
       ***************************************************/
      if(readingMatch == 1) 
      {
        for (i = 0; i < 16; i++);
        {
          if (readingOne[i] == 0) convertedRoute &= ~(1 << i);
          else if (readingOne[i]== 1) convertedRoute |= (1 << i);
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
        
        /***************************************************
        * The value stored in the convertedRoute variable has
        * been confirmed to be one of the 8 possible choices.
        * The next if statement sets the readyStatus flag to 
        * 1 and and writes the finalRoute variable to the 
        * LCD screen. 
        ***************************************************/
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
          /* Clears all the data and starts over */
          for (i = 0; i < 10; i++) readingOne[i] = 0;
          for (i = 0; i < 10; i++) readingTwo[i] = 0;
          for (i = 0; i < 10; i++) readingThree[i] = 0;
          totalReadings = 0;
          compareStatus = 0;
          readyStatus = 0;
          readingMatch = 0;
          sei();                  // enables all interrupts
        }        
      }
    }
/********************************************************************************
 *
 *     Finally, the vehicle is ready to start
 *
********************************************************************************/ 
  while(readyStatus == 1){
  
  // add code here to start moving the vehicle


    
  }
  
  
  
  } // end of loop
 
/********************************************************************************

                            FUNCTIONS SECTION

********************************************************************************/ 

  ISR(INT0_vect)
  /****************************************************************
  * Interrupt Service Routine vector fires when the INIT0 pin senses a
  * state change. The micro will stop what it is doing and execute this
  * function. The first thing that happens is a total of 44 measurements 
  * are taken at 560us. This is off set by 230us on the first reading.
  * Taking measurement at this interval will put the measurement in the
  * middle of a signal (HIGH or LOW). Once the buffer is full, this function 
  * compares the first 35 position of the array to what is expected, this 
  * is the same for all 8 route signals. Then the final 10 positions are 
  * stored as reading. Each time a reading is complete, it increments the 
  * reading counter. This function will have to complete three times 
  * before the data can be used in the main loop. If at any point the data 
  * being compared in the first 35 positions isn't what is expected, the 
  * function is exited without storing a reading.
  *****************************************************************/
  {
    _delay_us(230);
    inputBuffer[0] = PIND2;

    /* Capturing the data */    
    for (i = 1; i < 44; i++)
    {
      _delay_us(560);
      inputBuffer[i] = PIND2;
    }
    /* Compare the header portion of the signal */
    for (i = 0; i < 16; i++)
    {
      if (inputBuffer[i] == 0) headerGood = 1;
      else
      {
        headerGood = 0;
        break;
      }
    }
    
    for (i = 15; i < 24; i++)
    {
      if (headerGood == 0) break;
      if (inputBuffer[i] == 1) headerGood = 1;
      else headerGood = 0;
    }   
    
    i = 24;
    
    while(headerGood == 1 && i < 36)
    {
       /* Compare the 5 logical 0's */
       switch (i)
       {
         case 0:
          if (inputBuffer[i] == 0) headerGood = 1;
          else headerGood = 0;         
          break;         
         case 1:
          if (inputBuffer[i] == 1) headerGood = 1;
          else headerGood = 0;
          break;
         case 2:
          if (inputBuffer[i] == 0) headerGood = 1;
          else headerGood = 0;         
          break;
         case 3:
          if (inputBuffer[i] == 1) headerGood = 1;
          else headerGood = 0;         
          break;
         case 4:
          if (inputBuffer[i] == 0) headerGood = 1;
          else headerGood = 0;         
          break;
         case 5:
          if (inputBuffer[i] == 1) headerGood = 1;
          else headerGood = 0;         
          break;  
         case 6:
          if (inputBuffer[i] == 0) headerGood = 1;
          else headerGood = 0;         
          break;
         case 7:
          if (inputBuffer[i] == 1) headerGood = 1;
          else headerGood = 0;         
          break;
         case 8:
          if (inputBuffer[i] == 0) headerGood = 1;
          else headerGood = 0;       
          break;
         case 9:
          if (inputBuffer[i] == 1) headerGood = 1;
          else headerGood = 0;       
          break;
         case 10:
         /* Store the last portion of the signal as a reading */
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
 
