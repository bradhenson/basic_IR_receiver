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
uint8_t finalRoute[]
uint8_t totalReadings = 0;		// keeps track of how many readings have been received
bool headerGood = false;
bool readingMatch = true;
void compareInputbuffer(uint8_t value);

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
			uint8_t i = 0;
			do(readingMatch == true && i < 11) 
			{
				if (readingMatch == true && i == 10)
				{
					///////More to do right here. We have to convert the array to an int.
					break;				
				}
				if (readingOne[i] == readingTwo[i] && readingTwo[i] == readingThree[i])
				{
					readingMatch = true;
				}
				else
				{
					readingMatch = false;
					readingOne[] = {0,0,0,0,0,0,0,0,0,0};
					readingTwo[] = {0,0,0,0,0,0,0,0,0,0};
					readingThree[] = {0,0,0,0,0,0,0,0,0,0};
					totalReadings = 0;
					break;
				}
			}
			++i;
			}

		}
    }

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
			if (inputBuffer[i] == 1)
			{
				headerGood = true;
			}
			else
			{
				headerGood = false;
				break;
			}
		}
		
		for (i = 15; i < 24; i++)
		{
			if (headerGood == false)
			{
				break;
			}
			if (inputBuffer[i] == 0)
			{
				headerGood = true;
			}
			else
			{
				headerGood = false;
				break;
			}
		}		
		
		i = 24;
		do (headerGood == true && int < 36)
		{
			 switch (i)
			 {
				 case 0:
					compareInputbuffer(1);				 
					break;				 
				 case 1:
					compareInputbuffer(0);
					break;
				 case 2:
					compareInputbuffer(1);				 
					break;
				 case 3:
					compareInputbuffer(0);				 
					break;
				 case 4:
					compareInputbuffer(1);				 
					break;
				 case 5:
					compareInputbuffer(0);				 
					break;	
				 case 6:
					compareInputbuffer(1);				 
					break;
				 case 7:
					compareInputbuffer(0);				 
					break;
				 case 8:
					compareInputbuffer(1);				 
					break;
				 case 9:
					compareInputbuffer(0);				 
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

/////////////////////////////////////////////////////////////////////////////
 
void compareInputbuffer(uint8_t value)
	{
		if (inputBuffer[i] == value)
		{
			headerGood = true;
		}
		else
		{
			headerGood = false;
			break;
		}
	}

}
