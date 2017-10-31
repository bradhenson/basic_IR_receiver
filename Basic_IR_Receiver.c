/////////////////////////////////////////////////////////////////////////////
//
// Basic_IR_Receiver.c
// Created: 10/30/2017
// Author: Bradford Henson
// License: Use at your own risk
//
/////////////////////////////////////////////////////////////////////////////

#define F_CPU 16000000UL		// define it now as 16 MHz unsigned long

#include <avr/io.h>				// included in all avr projects
#include <util/delay.h>			// add this to use the delay function
#include <avr/interrupt.h>		// add this to use the interrupt function
uint8_t inputBuffer[44];		// creates a buffer array for storing input values
uint8_t readingOne[10];
uint8_t readingTwo[10];
uint8_t readingThree[10];
uint8_t totalReadings = 0;		// keeps track of how many readings have been received
uint8_t x;
uint8_t y;
uint8_t finalRoute = 0;
int convertedRoute = 0;
bool headerGood = false;
bool readingMatch = true;
bool compareStatus = false;
bool readyStatus = false;
void compareinputbuffer(uint8_t value);
void cleareverything();

int main(void)
{  

	/* PIN Registers */
	DDRD &= ~(1 << 2);			// clear DDRD bit 2, sets PD2 (pin 4) for input
	PORTD |= (1 << 2);			// set PD2/INT0 (pin 4) internal pull-up resistor
	
	/* Interrupt Registers */
	EIMSK = 0b00000001;			// enable External Interrupt Request for INIT0
	EICRA = 0b00000011;			// enables interrupt on rising edge of INIT0 
	sei();						// enables all interrupts
	
	while (1) 
    {
		if (totalReadings == 3)
		{
			EIMSK =& ~(1 << 0);
			uint8_t i = 0;
			do(i < 10)
			{				
				if (readingOne[i] == readingTwo[i] && readingTwo[i] == readingThree[i])
				{
					readingMatch = true;
				}
				else
				{
					cleareverything();
					break;
				}
				i++;
			}
			if(readingMatch == true && i == 10) 
			{
				for (uint8_t k = 0; k < 16; k++);
				{
					if (readingOne[k] == 0)
					{
						convertedRoute =& ~(1 = k);
					}
					else
					{
						convertedRoute =| (1 = k);
					}
				}
				switch (convertedRoute)
				{
					case 0b0000000001010101:
					finalRoute = 0b00000000;	// route 1 - 000
					compareStatus = true;
					break;
				}
				case 0b0000000010101001:
				{
					finalRoute = 0b00000001;	// route 2 - 001
					compareStatus = true;
					break;
				}
				case 0b0000000010100101:
				{
					finalRoute = 0b00000010;	// route 3 - 010
					compareStatus = true;
					break;
				}
				case 0b0000000101001001:
				{
					finalRoute = 0b00000011;	// route 4 - 011
					compareStatus = true;
					break;
				}
				case 0b0000000010010101:
				{
					finalRoute = 0b00000100;	// route 5 - 100
					compareStatus = true;
					break;
				}
					case 0b0000000100101001:
				{
					finalRoute = 0b00000101;	// route 6 - 101
					compareStatus = true;
					break;
				}
				case 0b0000000100100101:
				{
					finalRoute = 0b00000110;	// route 7 - 110
					compareStatus = true;
					break;
				}
				case 0b0000001001001001:
				{
					finalRoute = 0b00000111;	// route 8 - 111
					compareStatus = true;
					break;
				}
						
				if (compareStatus == true)
				{
					readyStatus = true;
					// need to add code here to display the final route
					// on a display
				} 
				else
				{
					readyStatus = false;
					cleareverything();
				}
			break;				
			}
		}
	/////////////////////////////////////////////////////////////////////////
	//
	//				Finally, the vehicle is ready to start
	//
	/////////////////////////////////////////////////////////////////////////
	
	// add code here to start moving the vehicle
	
	}	// end of while(1) loop

/////////////////////////////////////////////////////////////////////////////

	ISR(INT0_vect)
	{
		
		_delay_us(230);
		inputBuffer[0] = PIND2;
				
		for (uint8_t i = 1; i < 44, i++)
		{
			_delay_us(560);
			inputBuffer[i] = PIND2;
		}
		for (i = 0; i < 16; i++)
		{
			compareinputbuffer(i,1);
		}
		
		for (i = 15; i < 24; i++)
		{
			if (headerGood == false)
			{
				break;
			}
			compareinputbuffer(i,0);
		}		
		
		i = 24;
		do (headerGood == true && int < 36)
		{
			 switch (i)
			 {
				 case 0:
					compareinputbuffer(i,1);				 
					break;				 
				 case 1:
					compareinputbuffer(i,0);
					break;
				 case 2:
					compareinputbuffer(i,1);				 
					break;
				 case 3:
					compareinputbuffer(i,0);				 
					break;
				 case 4:
					compareinputbuffer(i,1);				 
					break;
				 case 5:
					compareinputbuffer(i,0);				 
					break;	
				 case 6:
					compareinputbuffer(i,1);				 
					break;
				 case 7:
					compareinputbuffer(i,0);				 
					break;
				 case 8:
					compareinputbuffer(i,1);				 
					break;
				 case 9:
					compareinputbuffer(i,0);				 
					break;
				 case 10:
					for (i = 35; i < 45; i++)
					{
						switch(totalReadings)
						{
						case 0:
							readingOne[i-35] = inputBuffer[i];
							if(i == 45)
							{
								totalReadings = 1;
							}
							break;
						case 1:
							readingTwo[i-35] = inputBuffer[i];
							if(i == 45)
							{
								totalReadings = 2;
							}
							break;
						case 2:
							readingThree[i-35] = inputBuffer[i];
							if(i == 45)
							{
								totalReadings = 3;
							}
							break;	
						}
					}
					break;
			}				
			i++;
		
		}
	}
/////////////////////////////////////////////////////////////////////////////
 
	void compareinputbuffer(uint8_t x, uint8_t y)
	{
		if (inputBuffer[x] == y)
		{
			headerGood = true;
		}
		else
		{
			headerGood = false;
			break;
		}
	return;
	}
	
	void cleareverything()
	{
		readingMatch = false;
		readingOne[] = {0,0,0,0,0,0,0,0,0,0};
		readingTwo[] = {0,0,0,0,0,0,0,0,0,0};
		readingThree[] = {0,0,0,0,0,0,0,0,0,0};
		totalReadings = 0;
	}

}
