/********************************************************************************
Author: Bradford Henson
Date: 11/22/2017
Version: 0.0.0.2
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
#define F_CPU 16000000UL          // define CPU as 16 MHz unsigned long
#define TRUE 1                    // define boolean value for TRUE = 1
#define FALSE 0                   // define boolean value for FALSE = 0

#include <avr/io.h>               // included in all avr projects
#include <util/delay.h>           // add this to use the delay function
#include <avr/interrupt.h>        // add this to use the interrupt function
#include <LiquidCrystal_I2C.h>    // new LCD Arduino library for use with the backpack
#include <Wire.h>                 // add this to use I2C functions in Arduino

uint8_t inputBuffer[44];          // creates a buffer array for storing input values
uint8_t readingOne[10];           // buffer for the first reading
uint8_t readingTwo[10];           // buffer for the second reading
uint8_t readingThree[10];         // buffer for the third reading
uint8_t totalReadings = 0;        // keeps track of how many readings have been received
uint8_t i = 0;                    // temp variable used in most arrays and count tracking
uint8_t headerGood = TRUE;        // flag 
uint8_t readingMatch = FALSE;     // flag
int finalRoute = 0;               // the final route after all of the parsing and verifying

/*Function Prototypes:*/
void clearAll();
void receievedReading();

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

/********************************************************************************

                            SETUP SECTION

********************************************************************************/ 
void setup()
{  
  
  /* PIN Registers */
  DDRD &= ~(1 << 2);              // clear DDRD bit 2, sets PD2 for input
  PORTD |= (1 << 2);              // set PD2/INT0 internal pull-up resistor
  
  /* Interrupt Registers */
  EIMSK = 0b00000001;             // enable External Interrupt Request for INIT0
  EICRA = 0b00000010;             // enables interrupt on falling edge of INIT0 
  sei();                          // enables all interrupts globally
  
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
    * The main loop does nothing until the ISR function
    * has recorded 3 readings, we can then compare the three 
    * readings against one another just to 
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
          readingMatch = TRUE;
        }
        else
        {
          clearAll();
          break;
        }
        i++;
      }
      
       /***************************************************
       * Now that it is confirmed that the three readings 
       * are in fact the same, the following (for) loop
       * converts the reading array into an integer variable
       * type. Then the (switch) statement compares the integer
       * to the known list of 8 choices. if it finds a match,
       * it will set finalRoute and display it on an LCD.
       ***************************************************/
      if(readingMatch == TRUE) 
      {
        for (i = 0; i < 16; i++);
        {
          /***********************************************
          *The array is being cast as an int to make it easier 
          *to pass the value to the switch case.
          ***********************************************/
          if (readingOne[i] == 0) finalRoute &= ~(1 << i);
          else if (readingOne[i]== 1) finalRoute |= (1 << i);
        }
        switch (finalRoute)
        {
        case 0b0000000001010101:
          // route 1 - 000
            receievedReading();
            lcd.print(F(" ROUTE 1 - 000  "));
            while(1){

               //Code for route 1 goes here
            }
          break;
        case 0b0000000010101001:
          // route 2 - 001
            receievedReading();
            lcd.print(F(" ROUTE 2 - 001  "));
            while(1){

               //Code for route 2 goes here
            }
          break;
        case 0b0000000010100101:
          // route 3 - 010
            receievedReading();
            lcd.print(F(" ROUTE 3 - 010  "));
            while(1){

               //Code for route 3 goes here
            }
          break;
        case 0b0000000101001001:
          // route 4 - 011
            receievedReading();
            lcd.print(F(" ROUTE 4 - 011  "));
            while(1){

               //Code for route 4 goes here
            }
          break;
        case 0b0000000010010101:
          // route 5 - 100
            receievedReading();
            lcd.print(F(" ROUTE 5 - 100  "));
            while(1){

               //Code for route 5 goes here
            }
          break;
        case 0b0000000100101001:
          // route 6 - 101
            receievedReading();
            lcd.print(F(" ROUTE 6 - 101  "));
            while(1){

               //Code for route 6 goes here
            }
          break;
        case 0b0000000100100101:
          // route 7 - 110
            receievedReading();
            lcd.print(F(" ROUTE 7 - 110  "));
            while(1){

               //Code for route 7 goes here
            }
          break;
        case 0b0000001001001001:
          // route 8 - 111
            receievedReading();
            lcd.print(F(" ROUTE 8 - 111  "));
            while(1){

               //Code for route 8 goes here
            }
          break;
         }      
       } 
       else
       {
         clearAll();
       }        
      }
 } // end of event loop
 
/********************************************************************************

                            FUNCTIONS SECTION

********************************************************************************/ 

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
  ISR(INT0_vect)
  {
    //LCD commands are for trouble shooting, they would indicate an ISR event
    //lcd.clear();
    //lcd.setCursor(0,1);
    //lcd.print("ISR TRIGGERED");
    
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
      if (inputBuffer[i] == 0) headerGood = TRUE;
      else
      {
        headerGood = FALSE;
        break;
      }
    }
    
    for (i = 16; i < 24; i++)
    {
      if (headerGood == FALSE) break;
      if (inputBuffer[i] == 1) headerGood = TRUE;
      else headerGood = FALSE;
    }   
    i = 24; // Technically this isn't needed, the count is 24
    while(headerGood == TRUE && i < 35)
    {
       /* Compare the 5 logical 0's */
       switch (i)
       {
         case 24:
          if (inputBuffer[i] == 0) headerGood = TRUE;
          else headerGood = FALSE;         
          break;         
         case 25:
          if (inputBuffer[i] == 1) headerGood = TRUE;
          else headerGood = FALSE;
          break;
         case 26:
          if (inputBuffer[i] == 0) headerGood = TRUE;
          else headerGood = FALSE;         
          break;
         case 27:
          if (inputBuffer[i] == 1) headerGood = TRUE;
          else headerGood = FALSE;         
          break;
         case 28:
          if (inputBuffer[i] == 0) headerGood = TRUE;
          else headerGood = FALSE;         
          break;
         case 29:
          if (inputBuffer[i] == 1) headerGood = TRUE;
          else headerGood = FALSE;         
          break;  
         case 30:
          if (inputBuffer[i] == 0) headerGood = TRUE;
          else headerGood = FALSE;         
          break;
         case 31:
          if (inputBuffer[i] == 1) headerGood = TRUE;
          else headerGood = FALSE;         
          break;
         case 32:
          if (inputBuffer[i] == 0) headerGood = TRUE;
          else headerGood = FALSE;       
          break;
         case 33:
          if (inputBuffer[i] == 1) headerGood = TRUE;
          else headerGood = FALSE;       
          break;
         case 34:
         /* Store the last portion of the signal as a reading */
          for (i = 34; i < 44; i++)
          {
            switch(totalReadings)
            {
            case 0:
              readingOne[i-34] = inputBuffer[i];
              if(i == 43) totalReadings = 1;
              break;
            case 1:
              readingTwo[i-34] = inputBuffer[i];
              if(i == 43) totalReadings = 2;
              break;
            case 2:
              readingThree[i-34] = inputBuffer[i];
              if(i == 43) totalReadings = 3;
              break;  
            }
          }
          break;
      }       
      i++;
    
    }
  }

  void receievedReading(){
       lcd.setCursor(0, 0);
       lcd.print(F("RECEIVED READING"));
       lcd.setCursor(0, 1);
  }

  void clearAll(){
          /* Clears all the data and starts over */
          for (i = 0; i < 10; i++) readingOne[i] = 0;
          for (i = 0; i < 10; i++) readingTwo[i] = 0;
          for (i = 0; i < 10; i++) readingThree[i] = 0;
          totalReadings = FALSE;
          readingMatch = FALSE;
          sei();                  // enables all interrupts
  }

