/*
 * main.c
 *
 *  Created on: 23.07.2024
 *      Author: Zwiado
 *      	  : v.1.0
 */

#include <avr/io.h>
#define F_CPU 1000000UL
#include <avr/interrupt.h>
#include "led_7seg.h"

volatile uint16_t number = 5000; //domyślny czas naświetlania
volatile uint8_t timer_flag = 0;	//przerwanie

void initPorts() {
    DDRA &= ~(1 << PA0); // wejście 
    PORTA |= (1 << PA0); // pullup
	DDRA &= ~(1 << PA1);
	PORTA |= (1 << PA1);
}

void initTimer1() {
    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS12) | (1 << CS10);
    OCR1A = 9775;
    TIMSK |= (1 << OCIE1A);
}

ISR(TIMER1_COMPA_vect) {
    timer_flag = 1;
}

int main(void)
{
    initPorts();
    d_led_init();
    sei();
    initTimer1();
    while(1)
    {
        

        cy0 = number % 10;
        cy1 = (number / 10) % 10;
        cy2 = (number / 100) % 10;
        cy3 = (number / 1000) % 10;
        number--; // odejmij sekundę 
        timer_flag = 0; 
        while (!timer_flag) {
            if (!(PINA & (1 << PA0))) { // Czy przycisk wciśnięty
            if (!(PINA & (1 << PA0))) { // Nadal wciśnięty?
                number += 10;
                while (!(PINA & (1 << PA0))); // Czekanie na przycisk
            }
        }
        }
    }
}
