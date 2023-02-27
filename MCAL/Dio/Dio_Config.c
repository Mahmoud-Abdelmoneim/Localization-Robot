/*
 * Dio_Config.c
 *
 *  Created on: 14 Sept 2022
 *      Author: Mahmoud Abdelmoniem
 */
#include "Dio_Config.h"


Dio_pin_config pin[PIN_COUNT]={
		//DC_motors_Pins
		{PORT_B,PIN_0,OUTPUT},
		{PORT_B,PIN_1,OUTPUT},
		{PORT_B,PIN_2,OUTPUT},
		{PORT_B,PIN_3,OUTPUT},
		{PORT_B,PIN_7,OUTPUT},


		//IR_Sensor
		{PORT_C,PIN_0,INPUT},
		{PORT_C,PIN_1,INPUT},



};

