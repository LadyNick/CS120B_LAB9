/*	Author: Nicole Navarro
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab#9  Exercise #1
 *	Video: https://youtu.be/J-3rqYjdqw0
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

unsigned char threecount = 1;
unsigned char blinkcount = 1;
unsigned char threeLEDS;
unsigned char blinkLED;

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

void Combine_Tick(){
	switch(Combine_state){
		case combine:
			PORTB = blinkLED + threeLEDS;
			Combine_state = combine;
			break;
		default:
			Combine_state = combine;
	}
}

int main(void) {
	unsigned long Three_elapsedT = 1000;
	unsigned long Blink_elapsedT = 1000;
	unsigned long Combine_elapsedT = 1000;

    /* Insert DDR and PORT initializations */
	DDRB = 0XFF; PORTB = 0X00;
	TimerSet(100);
	TimerOn();
	ThreeLED = three_sequence;
	Blink = blink_sequence;
	Combine_state = combine;

    while (1) {
	if(Three_elapsedT >= 1000){
		Three_Tick();
		Three_elapsedT = 0;
	}
	if(Blink_elapsedT >= 1000){
		Blink_Tick();
		Blink_elapsedT = 0;
	}
	if(Combine_elapsedT >= 1000){
		Combine_Tick();
		Combine_elapsedT = 0;
	}
	while(!TimerFlag){}
	TimerFlag = 0;
	Three_elapsedT += 100;
	Blink_elapsedT += 100;
	Combine_elapsedT += 100;
    }
    return 1;
}
