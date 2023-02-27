/*
 * main.h
 *
 *  Created on: 11 Oct 2022
 *      Author: Mahmoud AbdelMoniem
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "MCAL/Dio/Dio.h"
#include "HAL/DC_motor/DC_motor.h"
#include "HAL/IR_Sensor/IR_Sensor.h"
#include "MCAL/Timer1/Timer1.h"
#include "MCAL/USART/USART.h"

#define   IR_SENSOR_RIGHT	PORTC_0
#define   IR_SENSOR_LEFT	PORTC_1



typedef enum{
	MAX_SPEED=100,
	HIGH_SPEED=90,
	HALF_SPEED =50,
	LOW_SPEED=30,
	MIN_SPEED=15
}Motor_Sped_t;


void Robot_Move_Forward();
void Robot_Move_Backward();
void Robot_Move_Right();
void Robot_Move_Left();
void Robot_turn_Right90();
void Robot_turn_Left90();
void Robot_Reverse();
void Robot_Move_Step();
void Robot_Stop();
void Robot_Speed(Motor_Sped_t speed);
void Robot_Localization();
void Robot_LineFollower();
#endif /* MAIN_H_ */
