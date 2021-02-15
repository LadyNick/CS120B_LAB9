/*	Author: Nicole Navarro
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab#9  Exercise #3
 *	Video: https://youtu.be/q1v0QSBv8yg
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

enum ThreeLEDsSM{three_sequence}ThreeLED;
enum BlinkingSM{blink_sequence}Blink;
enum CombineSM{combine}Combine_state; 
enum PWM_states{toggle, off}PWM_state;

unsigned char A2;
unsigned char threecount = 1;
unsigned char blinkcount = 1;
unsigned char togglecount = 1;
unsigned char threeLEDS;
unsigned char blinkLED;
unsigned char buttontoggle;

void Three_Tick(){
	switch(ThreeLED){
		case three_sequence:
			if(threecount == 1){
				++threecount;
				threeLEDS = 0x01;
			}
			else if(threecount == 2){
				++threecount;
				threeLEDS = 0x02;
			}
			else if(threecount == 3){
				threecount = 1;
				threeLEDS = 0x04;
			}
			ThreeLED = three_sequence;
			break;
		default:
			ThreeLED = three_sequence;
			break;
	}
}

void Blink_Tick(){
	switch(Blink){
		case blink_sequence:
			if(blinkcount == 1){
				blinkLED = 0x08;
				blinkcount = 0;
			}
			else if(blinkcount == 0){
				blinkLED = 0x00;
				blinkcount = 1;
			}
			Blink = blink_sequence;
			break;
		default:
			Blink = blink_sequence;
			break;
	}

}

void PWM_Tick(){
	switch(PWM_state){
		case toggle:
			if(!A2){
				PWM_state = off;
			}
			else{
			if(togglecount == 1){
				buttontoggle = 0x10;
				togglecount = 0;
			}
			else if(togglecount == 0){
				buttontoggle = 0x00;
				togglecount = 1;	
			}
			PWM_state = toggle;
			}
			break;
		case off: 
			buttontoggle = 0x00;
			if(A2){
				PWM_state = toggle;
			}
			else{
				PWM_state = off;
			}
			break;
		default:
			PWM_state = off;
			break;
	}
}

void Combine_Tick(){
	switch(Combine_state){
		case combine:
			PORTB = blinkLED + threeLEDS + buttontoggle;
			Combine_state = combine;
			break;
		default:
			Combine_state = combine;
	}
}

int main(void) {
	unsigned long Three_elapsedT = 300;
	unsigned long Blink_elapsedT = 1000;
	unsigned long Combine_elapsedT = 1;
	unsigned long PWM_elapsedT = 2;

    /* Insert DDR and PORT initializations */
	DDRB = 0XFF; PORTB = 0X00;
	DDRA = 0X00; PINA = 0XFF;
	TimerSet(1);
	TimerOn();
	ThreeLED = three_sequence;
	Blink = blink_sequence;
	Combine_state = combine;
	PWM_state = off;

    while (1) {
	A2 = ~PINA & 0x04;

	if(Three_elapsedT >= 300){
		Three_Tick();
		Three_elapsedT = 0;
	}
	if(Blink_elapsedT >= 1000){
		Blink_Tick();
		Blink_elapsedT = 0;
	}
	if(Combine_elapsedT >= 1){
		Combine_Tick();
		Combine_elapsedT = 0;
	}
	if(PWM_elapsedT >= 2){
		PWM_Tick();
		PWM_elapsedT = 0;
	}
	while(!TimerFlag){}
	TimerFlag = 0;
	Three_elapsedT += 1;
	Blink_elapsedT += 1;
	Combine_elapsedT += 1;
	PWM_elapsedT += 1;
    }
    return 1;
}
