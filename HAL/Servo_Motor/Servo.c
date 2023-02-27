/*
 * Servo.c
 *
 *  Created on: 10 Oct 2022
 *      Author: El-Wattaneya
 */
#include "Servo.h"


void Servo_angle(uint8 angle){
	Timer1_Init(TMR1_PWM_FAST_MODE_ICR1,OCRA1_NON_INVERTING,OCRB1_DISCONNECTED);
	PWM_FreQ_Hz(50);
	if(angle==90)
	{
		OCR1A=1500;
	}
	else if(angle==180)
	{
		OCR1A=2000;
	}

}
