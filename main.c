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
#include <util/delay.h>
#include "led_7seg.h"

volatile uint16_t number = 5000; //domyślny czas naświetlania
volatile uint8_t timer_flag = 0;	//przerwanie
volatile uint8_t mode = 1; //tryb 1-Ścieżki pcb 2-soldermaska 3- kasowanie eeprom 4 - bez okresu np. bielenie obudowy

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
	cy0 = mode; //wyświetlenie wybranego trybu
    while(1)
    {	
		_delay_ms(1000);
		 if (!(PINA & (1 << PA0))) { // Czy przycisk wciśnięty +
			 if (!(PINA & (1 << PA0))) { // Nadal wciśnięty?
				 mode += 1; //Kolejny tryb
				 if(mode > 4) {
					 mode = 1; // Powrót do trybu 1 po przekroczeniu trybu 4 (ostatniego)
				 }
				 cy0 = mode; //wyświetlenie wybranegi trybu
				 while (!(PINA & (1 << PA0))); // Czekanie na przycisk
			 }
		 }
				 if (!(PINA & (1 << PA1))) { // start naświetlania
			     _delay_ms(1000);
				 if(mode == 1){
					 number = 50;
				 }
				 if(mode == 2){
					 number = 100;
				 }
				 if(mode == 3){
					 number = 150;
				 }
				 if(mode == 4){
					 number = 9999;
				 }
				 
		while(1)
		{
        cy0 = number % 10;
        cy1 = (number / 10) % 10;
        cy2 = (number / 100) % 10;
        cy3 = (number / 1000) % 10;
        number--; // odejmij sekundę 
        timer_flag = 0; 
        while (!timer_flag) {
            if (!(PINA & (1 << PA0))) { // Czy przycisk wciśnięty +
            if (!(PINA & (1 << PA0))) { // Nadal wciśnięty?
                number += 10;
				if(number > 9999) {
					number = 0; //  za mało liczb na wyświetlaczu mój panie
				}
                while (!(PINA & (1 << PA0))); // Czekanie na przycisk
            }
			
        }          
			if (!(PINA & (1 << PA1))) { // Czy przycisk wciśnięty -
			if (!(PINA & (1 << PA1))) { // Nadal wciśnięty?
	        number -= 10;
			if(number < 0) {
				 number = 0; //  a kysz dla ujemnego czasu
			 }
	        while (!(PINA & (1 << PA1))); // Czekanie na przycisk
			}
        }
        }
    }
		}
		 
				 }
	}
