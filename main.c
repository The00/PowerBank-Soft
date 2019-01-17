/*
 * PwrBank.c
 *
 * Created: 12/01/2019 11:20:37
 * Author : Theo
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "main.h"
void init()
{
	DDRB = (1<<LED_OUT); //set LED_OUT as output
	PORTB = 0x0;
	SMCR =  (1<<SM1);// sleep mode = power down
	
}

void setup_timer()
{	
	OCR0A = 30000; //compare match value
	TCCR0A = 0;
	TIMSK0 = (1<< OCIE0A) ; //enable interrupt on compare match A
	TCCR0B = (1<<WGM02) | (1<< CS01);  // set prescaler 8 & start counting & reset cnt at compare match
}

int main(void)
{
   init();
   setup_timer();
   sei();
   
    while (1) 
    {
    }
}


ISR(TIM0_COMPA_vect)
{
	//blink LED
	//PORTB ^= (1<< LED_OUT);
	
	if((PINB & (1<<VSENSE)) == 1) // voltage alarm
	{
		//blink LED
		PORTB ^= (1<< LED_OUT);
	}
	else PORTB |= (1<<LED_OUT);
	
}
