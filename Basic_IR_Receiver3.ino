/********************************************************************************
Author: 
Date: 03/17/2018
Version: 0.0.0.4
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

#include <avr/io.h>               // included in all avr projects
#include <util/delay.h>           // add this to use the delay function
#include <avr/interrupt.h>        // add this to use the interrupt function
#include <LiquidCrystal_I2C.h>    // new LCD Arduino library for use with the backpack
#include <Wire.h>                 // add this to use I2C functions in Arduino

uint8_t i = 0;                    // temp variable used in most arrays and count tracking
uint8_t isrTrigger = 0;           // flag for the interrupt event
uint8_t masterSwitch = 1;         // creates a variable to control operating mode (position vs operational)

 /****************************************************************
 * There are a total of 57 samples (1 or 0) that have to be measured 
 * and stored for an entire signal. The First 24 of those values are
 * stored as bit values in the header variable. This means that each 
 * bit of the header variable coorispond with a sample reading from the
 * signal. The Last 32 values are stored in the inputBuffer variable.
 * Again, each bit of the variable coorispond to a sample reading.
 ****************************************************************/
unsigned long header = 0;         // 32 bit variable to hold the values for the header
unsigned long inputBuffer = 0;    // 32 bit variable to hold the values for the message

/*Function Prototypes:*/
uint8_t _captureData();

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
  DDRD &= ~(1 << PD2);            // clear DDRD bit 2, sets PD2 for input (IR Reciever)
  PORTD |= (1 << PD2);            // set PD2/INT0 internal pull-up resistor (IR Reciever)
  DDRC &= ~(1 << PC2);            // clear DDRC bit 2, sets PC2 for input (masterSwitch)
  PORTD |= (1 << PC2);            // set PC2 internal pull-up resistor (masterSwitch)
   
  /* Interrupt Registers */
  EIMSK |= (1 << INT0);           // enable External Interrupt Request for INIT0
  EICRA &= ~(1 << ISC00);         // enables interrupt on falling edge of INIT0 
  EICRA |= (1 << ISC01);
  sei();                          // enables all interrupts globally

  /* Initiate LCD */
  lcd.begin(16,2);                // Set up the LCD's number of columns and rows
  lcd.backlight();                // Turn on Backlight
  
  /****************************************************************************
  * Power-On Test Statement 
  * It may seem like this is just for show, but the truth is at this stage the 
  * control board has powered on, initiated all of the variable and pins, and started
  * the LCD. All of that makes up a quasi self test. The below bit of code gives the user
  * a warm feeling that everything is okay. A part of software development is user interface. 
  * This is a user interface feature. In a more advanced setup, you could actually 
  * measure voltages around the system as a self test mode.
  ****************************************************************************/
  lcd.setCursor(0, 0);
  lcd.print(F(" Power On Test  "));
  lcd.setCursor(0, 1);
  lcd.print(F("                "));
  for (i = 0; i < 6; i++)
      {
        lcd.setCursor((i+5), 1);
        lcd.print(F("*"));
        _delay_ms(600);
      } 
  lcd.setCursor(0, 0);
  lcd.print(F(" Test Complete  "));
  lcd.setCursor(0, 1);
  lcd.print(F(" Status: Good   "));  
  _delay_ms(3000);

}

/********************************************************************************

                                EVENT LOOP

********************************************************************************/ 
  void loop() {
       
      do{
      /* Set Display Message for Positioning */
      lcd.setCursor(0, 0);
      lcd.print(F("Standing By For "));
      lcd.setCursor(0, 1);
      lcd.print(F("Positioning Data"));
      
      if (isrTrigger == 1) 
      {
        i = _captureData();
        switch (i)
        {
        case 9:
          // All "1"s were received
            lcd.setCursor(0, 0);
            lcd.print(F("RECEIVED READING"));
            lcd.setCursor(0, 1);
            lcd.print(F(" IR IS ALIGNED  "));
           break;    
        default:
         /****************************************************************
         *A code was received, but it was not all "1"s
         ****************************************************************/
         isrTrigger = 0;        
         break;
        }
       } 
       /********************************************************
       * The masterSwitch is used to determine if the robot
       * should be in Positioning Mode or in Operational Mode.
       * Positioning Mode is used at the start of the match to
       * determine if the alignment is correct with the IR emitter.
       * Operational Mode is used when waiting to recieve map
       * coordinates. This is written to read Analog Pin 2 / PC2
       ********************************************************/       
      if((PINC & (1 << 2)) == 0)masterSwitch = 0;
      else masterSwitch = 1;            
      }while(masterSwitch == 1);
      
      /* Set Display Message for Coordinates */
      lcd.setCursor(0, 0);
      lcd.print(F("Standing By For "));
      lcd.setCursor(0, 1);
      lcd.print(F("Coordinates Data"));

      if (isrTrigger == 1) 
      {
        i = _captureData();
        switch (i)
        {
        case 1:
          // route 1 - 000
            lcd.setCursor(0, 0);
            lcd.print(F("RECEIVED READING"));
            lcd.setCursor(0, 1);
            lcd.print(F(" ROUTE 1 - 000  "));
            while(1){

               /////Code for route 1 goes here/////
            }
          break;
        case 2:
          // route 2 - 001
            lcd.setCursor(0, 0);
            lcd.print(F("RECEIVED READING"));
            lcd.setCursor(0, 1);
            lcd.print(F(" ROUTE 2 - 001  "));
            while(1){

               /////Code for route 2 goes here/////
            }
          break;
        case 3:
          // route 3 - 010
            lcd.setCursor(0, 0);
            lcd.print(F("RECEIVED READING"));
            lcd.setCursor(0, 1);
            lcd.print(F(" ROUTE 3 - 010  "));
            while(1){

               /////Code for route 3 goes here/////
            }
          break;
        case 4:
          // route 4 - 011
            lcd.setCursor(0, 0);
            lcd.print(F("RECEIVED READING"));
            lcd.setCursor(0, 1);
            lcd.print(F(" ROUTE 4 - 011  "));
            while(1){

               /////Code for route 4 goes here/////
            }
          break;
        case 5:
          // route 5 - 100
            lcd.setCursor(0, 0);
            lcd.print(F("RECEIVED READING"));
            lcd.setCursor(0, 1);
            lcd.print(F(" ROUTE 5 - 100  "));
            while(1){

               /////Code for route 5 goes here/////
            }
          break;
        case 6:
          // route 6 - 101
            lcd.setCursor(0, 0);
            lcd.print(F("RECEIVED READING"));
            lcd.setCursor(0, 1);
            lcd.print(F(" ROUTE 6 - 101  "));
            while(1){

               /////Code for route 6 goes here/////
            }
          break;
        case 7:
          // route 7 - 110
            lcd.setCursor(0, 0);
            lcd.print(F("RECEIVED READING"));
            lcd.setCursor(0, 1);
            lcd.print(F(" ROUTE 7 - 110  "));
            while(1){

               /////Code for route 7 goes here/////
            }
          break;
        case 8: 
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
          //clear reading and isr flag to start over
          inputBuffer = 0;
          header = 0;
          isrTrigger = 0; 
       }        
        
 } // end of event loop
 
/********************************************************************************

                            FUNCTIONS SECTION

********************************************************************************/ 

 /*******************************************************************************
 * Function Name: ISR(INT0_vect
 * 
 * Author: 
 * 
 * Description: When the microcontroller sees an interrupt on Digital Pin 2 of the
 * Arduino (PD2 on the chip itself), the following interrupt service routine is 
 * executed. The goal of the ISR is to be short and quick to execute, so it merely
 * sets a flag to let the main loop know that a falling edge was detected. By setting
 * isrTrigger to 1 or TRUE, the micro wil execute the data capture and processing
 * functions.
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
 * Description: after the ISR sets the captureData variable to 1, a total of 52 
 * measurements are taken at 560us. This is off set by 230us on the first reading.
 * Taking measurement at this interval will put the measurement in the
 * middle of a signal (HIGH or LOW). 
 * 
 * The function continues by processing the recieved data looking for specific
 * patterns, in the event that a specific pattern is not found, the function returns
 * a value of 0 to indicate that there is an error or bad data captured.
 *******************************************************************************/
 uint8_t _captureData()
 { 
      // Reset isrTrigger flag, in case data is not usable
      isrTrigger = 0; 
      // The first sample is offset by 230us, in order to read all subsequent
      // samples around the middle of the high or low signal.
      _delay_us(230);
      if((PIND & (1 << 2)) == 0b00000100) header &= ~(1 << 0);  
      else header |= (1 << 0);

      // 24 total samples make up the first 9 ms HIGH and 4.5 ms LOW signal
      // this is stored in the header variable.
      for (i = 1; i < 24; i++)
      {
       _delay_us(560);
       if((PIND & (1 << 2)) == 0b00000100) header &= ~(1 << i);  
       else header |= (1 << i);
      }

       /****************************************************************
       * 32 total samples make up the rest of the message and are stored
       * in the inputBuffer variable. It should be noted that in the case
       * of all logical "1", there would be a 33rd sample that makes up the
       * the stop bit, however in this case it is left off to accommodate a
       * 32 bit size variable. We will know through processing the data
       * that all of the values are logical ones and don't need the stop bit 
       ****************************************************************/
      for (i = 0; i < 32; i++)
      {
       _delay_us(560);
       if((PIND & (1 << 2)) == 0b00000100) inputBuffer &= ~(1 << i);  
       else inputBuffer |= (1 << i);
      }

       /****************************************************************
       * Now that the data capture portion is complete, we need to zero 
       * out the last portion of the header variable since we didn't use 
       * all of the bits. This will ensure that we don't get unwanted 
       * values as part of our recorded data set.
       ****************************************************************/
      for(i = 24; i < 32; i++)
      { 
       header &= ~(1 << i);
      }

       /*****************************************************************
       *Now we have the header and the message saved as a decimal 
       *value. We can simply use the expected decimal value of the header  
       *to evaluate against for correctness and set the headerGood Flag as True. 
       *65535 is decimal equivalent of the binary 0b00000000000000001111111111111111
       *or 9ms HIGH and 4.5ms LOW and the rest of the variable is zero. Keep
       *in mind to read the variable binary from right to left. If the header is 
       *true or "good" then work through the switch case of all the other comparisons.
       *
       *The return "0" in the event the header is false lets the program know that
       *there was an error and the reading should not be used.
       *****************************************************************/
      if(header == 65535)
      {
        switch (inputBuffer)
        {
        case 87381:
         /****************************************************************
         *87381 is equal to 0b00000000000000010101010101010101 Route - 000
         *Remember the decimal bit value is read from right to left when 
         *comparing it to the received waveform.
         ****************************************************************/
         return 1;
         break;
        case 283989:
         /****************************************************************
         *283989 is equal to 0b00000000000001000101010101010101 Route - 001
         *Remember the decimal bit value is read from right to left when 
         *comparing it to the received waveform.
         ****************************************************************/
         return 2;
         break;
        case 333141:
         /****************************************************************
         *333141 is equal to 0b00000000000001010001010101010101 Route - 010
         *Remember the decimal bit value is read from right to left when 
         *comparing it to the received waveform.
         ****************************************************************/
         return 3;
         break;
        case 1119573:
         /****************************************************************
         *1119573 is equal to 0b00000000000100010001010101010101 Route - 011
         *Remember the decimal bit value is read from right to left when 
         *comparing it to the received waveform.
         ****************************************************************/
         return 4;
         break;
        case 345429:
         /****************************************************************
         *345429 is equal to 0b00000000000001010100010101010101 Route - 100
         *Remember the decimal bit value is read from right to left when 
         *comparing it to the received waveform.
         ****************************************************************/
         return 5;
         break;
        case 1131861:
         /****************************************************************
         *1131861 is equal to 0b00000000000100010100010101010101 Route - 101
         *Remember the decimal bit value is read from right to left when 
         *comparing it to the received waveform.
         ****************************************************************/
         return 6;
         break;
        case 1328469:
         /****************************************************************
         *1328469 is equal to 0b00000000000101000100010101010101 Route - 110
         *Remember the decimal bit value is read from right to left when 
         *comparing it to the received waveform.
         ****************************************************************/
         return 7;
         break;
        case 4474197: 
         /****************************************************************
         *4474197 is equal to 0b00000000010001000100010101010101 Route - 111
         *Remember the decimal bit value is read from right to left when 
         *comparing it to the received waveform.
         ****************************************************************/
         return 8;
         break;
        case 286331153: 
         /****************************************************************
         *286331153 is equal to 0b00010001000100010001000100010001 All "1"
         *Remember the decimal bit value is read from right to left when 
         *comparing it to the received waveform.
         ****************************************************************/
         return 9;
         break;
        default:
         /****************************************************************
         *If the signal doesn't match anything we are expecting, then simply
         *return 0 to indicate the capture has an error or bad data.
         ****************************************************************/        
         return 0;
         break;
       }
      } 
      else
      {
       return 0;
      }      
 }


