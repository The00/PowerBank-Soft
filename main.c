/*
 * PwrBank.c
 *
 * Created: 12/01/2019 11:20:37
 * Author : Theo
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "main.h"

volatile uint8_t status =0;

void init()
{
	// gpio 
	DDRB = (1<<LED_OUT); //set LED_OUT as output
	PORTB = 0x0;
	PUEB = (1<<BOOST_EN);
	
	
	// pin change isr 	
	PCMSK = ((1<< BOOST_EN) | (1<<OTG_EN));
	PCICR = 1; // enable gpio interrupt
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
		
		if(status ==0)
		{
			// go power down
			SMCR =  SLEEP_MODE_PWR_DOWN;
			sleep_enable();
			TCNT0 =0; // clear timer
			PORTB |= (1<<LED_OUT); // turn off LED
			sleep_cpu();
		}
		else
		{
			// go idle
			SMCR =  SLEEP_MODE_IDLE;
			sleep_enable();
			sleep_cpu();
		}
		
    }
}

ISR(PCINT0_vect)
{
	status = PINB & ((1<<BOOST_EN) | (1<<OTG_EN));	
}

ISR(TIM0_COMPA_vect)
{	
	if((PINB & (1<<VSENSE)) == 0) // voltage alarm triggered
	{
		//blink LED
		PORTB ^= (1<< LED_OUT);
	}
	else PORTB &= ~(1<<LED_OUT); // turn ON LED
	
}
