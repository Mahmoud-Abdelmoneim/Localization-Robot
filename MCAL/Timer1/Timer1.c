/*
 * Timer1.c
 *
 *  Created on: 27 Sept 2022
 *      Author: El-Wattaneya
 */
#include "Timer1.h"



static void (*(TMR1_InterriptOveFlow)) (void);
static void (*(TMR1_InterriptComA)) (void);
static void (*(TMR1_InterriptComB)) (void);
static void (*(TMR1_InterriptCAPT))(void);

void Timer1_Init(uint8 mode ,OCRA_Phase_fast_pwm outputa,OCRB_Phase_fast_pwm outputb){
	TIMER1_SET_MODE(mode);
	TIMER1_SET_PRESCALLER(TIMER1_PRESCALLER);
	TIMER1_OCR1A_CHAN_MODE(outputa);
	TIMER1_OCR1A_CHAN_MODE(outputb);
	if(outputa !=OCRA1_DISCONNECTED)
	{
		SET_BIT(DDRD,5);
	}
	if(outputa !=OCRB1_DISCONNECTED)
	{
		SET_BIT(DDRD,4);
	}
}


void Timer1_Interrupt_Enable()
{
	SET_BIT(TIMSK,TOIE1);
}
void Timer1_SetCallBack(void (*tmr1_ptf)(void))
{
	TMR1_InterriptOveFlow = tmr1_ptf;
}

void TIMER1_SetOCR1A(uint16 _value)
{
	OCR1A=_value;
}
void TIMER1_SetOCR1B(uint16 _value)
{
	OCR1B=_value;
}
void TIMER1_SetICR1(uint16 _value)
{
	ICR1=_value;
}
void TIMER1_SetTCNT1(uint16 _value)
{
	TCNT1=_value;
}
void PWM_FreQ_KHz(uint16 _ferq)
{
	uint16 ttotal = (uint32)1000/_ferq;
	if(ttotal >= 1)
	{
		ICR1 = ttotal-1;
	}
	else{ICR1 = 0 ;}
}
void PWM_FreQ_Hz(uint16 _ferq)
{
	uint16 ttotal =(uint32) 1000000/_ferq;
	if(ttotal >= 1)
	{
		ICR1 = ttotal-1;
	}
	else{ICR1 = 0 ;}
}
void PWM_SetDuty_Cycle_OCR1A(uint8 _duty)
{
	uint16 ton =((uint32)_duty*(ICR1+1))/100;
	if(ton >= 1)
	{
		OCR1A = ton - 1;
	}
	else {OCR1A = 0 ;}
}

void PWM_SetDuty_Cycle_OCR1B(uint8 _duty)
{
	uint16 ton =((uint32)_duty*(ICR1+1))/100;
	if(ton >= 1)
	{
		OCR1B = ton - 1;
	}
	else {OCR1B = 0 ;}
}


void TMR1_Interrupt_COMPA_enable(){

}
void Timer1_SetCallBack_COMPA(void (*tmr1_ptf)(void))
{

}
void TMR1_Interrupt_COMPA_Disable(){

}
void TMR1_Interrupt_CAPT_enable()
{

}
void Timer1_SetCallBack_COMPB(void (*tmr1_ptf)(void))
{

}
void TMR1_Interrupt_CAPT_Disable()
{

}

ISR(TIMER1_OVF_vect)
{
	if(TMR1_InterriptOveFlow)
		TMR1_InterriptOveFlow();
}

/***********************ISR For TMR1 For COMPA***************************/


ISR(TIMER1_COMPA_vect)
{
	if(TMR1_InterriptComA)
		TMR1_InterriptComA();
}
/**********************ISR For TMR1 For COMPB**************************/

ISR(TIMER1_COMPB_vect)
{
	if(TMR1_InterriptComB)
		TMR1_InterriptComB();
}
/**********************ISR For TMR1 For ICU****************************/

ISR(TIMER1_CAPT_vect)
{
	if(TMR1_InterriptCAPT)
		TMR1_InterriptCAPT();
}


