/********************************************************************************
Author: 
Date: 11/24/2017
Version: 0.0.0.3
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
  The library used for the backpack is from the following website. This effectively
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

uint8_t inputBuffer[47];          // creates a buffer array for storing input values
uint8_t readingOne[13];           // buffer for the first reading
uint8_t totalReadings = 0;        // keeps track of how many readings have been received
uint8_t i = 0;                    // temp variable used in most arrays and count tracking
uint8_t headerGood = TRUE;        // flag 
uint8_t isrTrigger = 0;           // flag for the interrupt event
uint16_t finalRoute = 0;          // the final route after all of the parsing and verifying

/*Function Prototypes:*/
void _captureData(void);
void _processData(void);

// The default I2C address for most of these chinese LCD backpacks is 0x27
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 
/********************************************************************************

                            SETUP SECTION

********************************************************************************/ 
void setup()
{  

  Serial.begin(9600);             // setup Arduino Serial Monitor
  while (!Serial);                // wait until Serial is ready
  
  /* PIN Registers */
  DDRD &= ~(1 << PD2);            // clear DDRD bit 2, sets PD2 for input
  PORTD |= (1 << PD2);            // set PD2/INT0 internal pull-up resistor
   
  /* Interrupt Registers */
  EIMSK |= (1 << INT0);           // enable External Interrupt Request for INIT0
  EICRA &= ~(1 << ISC00);         // enables interrupt on falling edge of INIT0 
  EICRA |= (1 << ISC01);
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

      if (isrTrigger == 1) _captureData();
      if (isrTrigger == 1) _processData();

      if (totalReadings == 1) 
      {
        totalReadings = 0;
        _convertReading();
        
        switch (finalRoute)
        {
        case 5440: //0b0000000001010101
          // route 1 - 000
            lcd.setCursor(0, 0);
            lcd.print(F("RECEIVED READING"));
            lcd.setCursor(0, 1);
            lcd.print(F(" ROUTE 1 - 000  "));
            while(1){

               /////Code for route 1 goes here/////
            }
          break;
        case 5392:
          // route 2 - 001
            lcd.setCursor(0, 0);
            lcd.print(F("RECEIVED READING"));
            lcd.setCursor(0, 1);
            lcd.print(F(" ROUTE 2 - 001  "));
            while(1){

               /////Code for route 2 goes here/////
            }
          break;
        case 5200:
          // route 3 - 010
            lcd.setCursor(0, 0);
            lcd.print(F("RECEIVED READING"));
            lcd.setCursor(0, 1);
            lcd.print(F(" ROUTE 3 - 010  "));
            while(1){

               /////Code for route 3 goes here/////
            }
          break;
        case 5188:
          // route 4 - 011
            lcd.setCursor(0, 0);
            lcd.print(F("RECEIVED READING"));
            lcd.setCursor(0, 1);
            lcd.print(F(" ROUTE 4 - 011  "));
            while(1){

               /////Code for route 4 goes here/////
            }
          break;
        case 4432:
          // route 5 - 100
            lcd.setCursor(0, 0);
            lcd.print(F("RECEIVED READING"));
            lcd.setCursor(0, 1);
            lcd.print(F(" ROUTE 5 - 100  "));
            while(1){

               /////Code for route 5 goes here/////
            }
          break;
        case 4420:
          // route 6 - 101
            lcd.setCursor(0, 0);
            lcd.print(F("RECEIVED READING"));
            lcd.setCursor(0, 1);
            lcd.print(F(" ROUTE 6 - 101  "));
            while(1){

               /////Code for route 6 goes here/////
            }
          break;
        case 4372:
          // route 7 - 110
            lcd.setCursor(0, 0);
            lcd.print(F("RECEIVED READING"));
            lcd.setCursor(0, 1);
            lcd.print(F(" ROUTE 7 - 110  "));
            while(1){

               /////Code for route 7 goes here/////
            }
          break;
        case 4369: 
          // route 8 - 111
            lcd.setCursor(0, 0);
            lcd.print(F("RECEIVED READING"));
            lcd.setCursor(0, 1);
            lcd.print(F(" ROUTE 8 - 111  "));
            while(1){

               /////Code for route 8 goes here/////
            }
          break;
         }      
       } 
       else
       {
          //clear reading and reading count to start over
          for (i = 0; i < 10; i++) readingOne[i] = 0;
          totalReadings = 0;
       }        
      
 } // end of event loop
 
/********************************************************************************

                            FUNCTIONS SECTION

********************************************************************************/ 

 /*******************************************************************************
 * Function Name:
 * 
 * Author: 
 * 
 * Description: 
 *******************************************************************************/ 
 ISR(INT0_vect)
 {
    isrTrigger = 1;
 }

 /*******************************************************************************
 * Function Name: _dataCapture()
 * 
 * Author: 
 * 
 * Description: after the ISR sets the captureData variable to 1, a total of 44 
 * measurements are taken at 560us. This is off set by 230us on the first reading.
 * Taking measurement at this interval will put the measurement in the
 * middle of a signal (HIGH or LOW). Once the buffer is full, this function 
 * compares the first 34 position of the array to what is expected, this 
 * is the same for all 8 route signals. Then the final 13 positions are 
 * stored as reading. Each time a reading is complete, it increments the 
 * reading counter. 
 *******************************************************************************/
 void _captureData(void)
 { 
      _delay_us(230);
      if((PIND & (1 << 2)) == 0b00000100)inputBuffer[0] = 0;
      else inputBuffer[0] = 1;
      
      for (i = 1; i < 47; i++)
      {
        _delay_us(560);
        if((PIND & (1 << 2)) == 0b00000100)inputBuffer[i] = 0;
        else inputBuffer[i] = 1;
      }
    
    //for (i = 0; i < 47; i++)
    //{
    //  Serial.print(inputBuffer[i]);
    //}
    //      Serial.print("\n");
 }
 /*******************************************************************************
 * Function Name: _processData()
 * 
 * Author: 
 * 
 * Description: 
 *******************************************************************************/ 
 void _processData(void)
 {
    for (i = 0; i < 16; i++)
    {
      if (inputBuffer[i] == 1)headerGood = TRUE;
      else
      {
        headerGood = FALSE;
        break;
      }
    }
    for (i = 16; i < 24; i++)
    {
      if (headerGood == FALSE) break;
      if (inputBuffer[i] == 0)headerGood = TRUE;
      else headerGood = FALSE;
    } 
    i = 24;
    while(headerGood == TRUE && i < 34)
    {
       switch (i)
       {
         case 24:
          if (inputBuffer[i] == 1)headerGood = TRUE;
          else headerGood = FALSE;         
          break;         
         case 25:
          if (inputBuffer[i] == 0)headerGood = TRUE;
          else headerGood = FALSE;
          break;
         case 26:
          if (inputBuffer[i] == 1)headerGood = TRUE;
          else headerGood = FALSE;         
          break;
         case 27:
          if (inputBuffer[i] == 0)headerGood = TRUE;
          else headerGood = FALSE;         
          break;
         case 28:
          if (inputBuffer[i] == 1)headerGood = TRUE;
          else headerGood = FALSE;         
          break;
         case 29:
          if (inputBuffer[i] == 0)headerGood = TRUE;
          else headerGood = FALSE;         
          break;  
         case 30:
          if (inputBuffer[i] == 1)headerGood = TRUE;
          else headerGood = FALSE;         
          break;
         case 31:
          if (inputBuffer[i] == 0) headerGood = TRUE;
          else headerGood = FALSE;         
          break;
         case 32:
          if (inputBuffer[i] == 1)headerGood = TRUE;
          else headerGood = FALSE;       
          break;
         case 33:
          if (inputBuffer[i] == 0)headerGood = TRUE;
          else headerGood = FALSE;       
          break;
      }       
      i++;
    }
    if(headerGood == TRUE && i == 34)
    {
       readingOne[12] = inputBuffer[34];
       readingOne[11] = inputBuffer[35];
       readingOne[10] = inputBuffer[36];
       readingOne[9] = inputBuffer[37];
       readingOne[8] = inputBuffer[38];
       readingOne[7] = inputBuffer[39];
       readingOne[6] = inputBuffer[40];
       readingOne[5] = inputBuffer[41];
       readingOne[4] = inputBuffer[42];
       readingOne[3] = inputBuffer[43];
       readingOne[2] = inputBuffer[44];
       readingOne[1] = inputBuffer[45];
       readingOne[0] = inputBuffer[46];
       totalReadings = 1;
    }        
    isrTrigger = 0;
    //for(i=0; i<13; i++)
    //{
    //  Serial.print(readingOne[i]);
    //}
    //Serial.print("\n");    
   //}
 }

 /*******************************************************************************
 * Function Name: _convertReading()
 * 
 * Author: 
 * 
 * Description:The array is being cast as an int to make it easier to pass the 
 * value to the switch case.
 *******************************************************************************/
 void _convertReading(void)
 {
  for (i = 0; i < 16; i++)
        {
          if (readingOne[i] == 1)finalRoute |= (1 << (i));
          else finalRoute &= ~(1 << (i));
        }
        finalRoute &= ~(1 << 13);
        finalRoute &= ~(1 << 14);
        finalRoute &= ~(1 << 15);
        //Serial.print(finalRoute, BIN);
        //Serial.print("\n");
 }

