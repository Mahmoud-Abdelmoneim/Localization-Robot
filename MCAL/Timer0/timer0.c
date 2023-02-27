/*
 * timer0.c
 *
 *  Created on: 24 Sept 2022
 *      Author: El-Wattaneya
 */
#include "timer0.h"
static void (*timer0_ptf)(void);
void Timer0_Init(uint8 mode,uint8 outputpin){
	switch(mode){
		case TIMER0_NORMAL_MODE:
			CLEAR_BIT(TCCR0,WGM00);
			CLEAR_BIT(TCCR0,WGM01);
		break;
		case TIMER0_PHASSCORRECT_MODE:
			SET_BIT(TCCR0,WGM00);
			CLEAR_BIT(TCCR0,WGM01);
		break;
		case TIMER0_CTC_MODE:
			CLEAR_BIT(TCCR0,WGM00);
			SET_BIT(TCCR0,WGM01);
		break;
		case TIMER0_FASTPWM_MODE:
			SET_BIT(TCCR0,WGM00);
			SET_BIT(TCCR0,WGM01);
		break;
	}
	switch(outputpin)
	{
		case OC0_DISCONNECTED:
			CLEAR_BIT(TCCR0,COM00);
			CLEAR_BIT(TCCR0,COM01);
			break;
		case OC0_TOGGLE:
			SET_BIT(TCCR0,COM00);
			CLEAR_BIT(TCCR0,COM01);
			SET_BIT(DDRB,3);
			break;
		case OC0_NON_INVERTING:
			CLEAR_BIT(TCCR0,COM00);
			SET_BIT(TCCR0,COM01);
			SET_BIT(DDRB,3);
			break;
		case OC0_INVERTING:
			SET_BIT(TCCR0,COM00);
			SET_BIT(TCCR0,COM01);
			SET_BIT(DDRB,3);
			break;
	}

	#if TIMER0_PRESCALLER == TIMER0_DISABLE
		CLEAR_BIT(TCCR0,CS00);
		CLEAR_BIT(TCCR0,CS01);
		CLEAR_BIT(TCCR0,CS02);
	#elif TIMER0_PRESCALLER == TIMER0_1_PRESCALER
		SET_BIT(TCCR0,CS00);
		CLEAR_BIT(TCCR0,CS01);
		CLEAR_BIT(TCCR0,CS02);
	#elif TIMER0_PRESCALLER == TIMER0_8_PRESCALER
		CLEAR_BIT(TCCR0,CS00);
		SET_BIT(TCCR0,CS01);
		CLEAR_BIT(TCCR0,CS02);

	#elif TIMER0_PRESCALLER == TIMER0_64_PRESCALER
		SET_BIT(TCCR0,CS00);
		SET_BIT(TCCR0,CS01);
		CLEAR_BIT(TCCR0,CS02);
	#elif TIMER0_PRESCALLER == TIMER0_256_PRESCALER
		CLEAR_BIT(TCCR0,CS00);
		CLEAR_BIT(TCCR0,CS01);
		SET_BIT(TCCR0,CS02);
	#elif TIMER0_PRESCALLER == TIMER0_1024_PRESCALER
		SET_BIT(TCCR0,CS00);
		CLEAR_BIT(TCCR0,CS01);
		SET_BIT(TCCR0,CS02);
	#elif TIMER0_PRESCALLER == TIMER0_EXITRNAL_FALLING
		CLEAR_BIT(TCCR0,CS00);
		SET_BIT(TCCR0,CS01);
		SET_BIT(TCCR0,CS02);
	#elif TIMER0_PRESCALLER == TIMER0_EXITRNAL_RISING
		SET_BIT(TCCR0,CS00);
		SET_BIT(TCCR0,CS01);
		SET_BIT(TCCR0,CS02);
	#endif

}


void Timer0_Set_OCR0(uint8 value){
	OCR0=value;
}
void Timer0_Interrupt_Enable(){
	SET_BIT(TIMSK,TOIE0);
}
void Timer0_SetCallBack(void (*tmr0_ptf)(void)){
	timer0_ptf=tmr0_ptf;
}

ISR(TIMER0_OVF_vect)
{
	if(timer0_ptf)
		timer0_ptf();
}


