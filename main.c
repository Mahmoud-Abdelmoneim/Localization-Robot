/*
 * main.c
 *  Created on: 9 Sept 2022
 *      Author: Mahmoud Abdelmoniem
 */


#include "main.h"

Motor_t motor1={
		.in_1=PORTB_0,
		.in_2=PORTB_1
};
Motor_t motor2={
		.in_1=PORTB_2,
		.in_2=PORTB_3
};

uint8 FlagStop=0; //flag to indicate that the line is end

/*default start coordinates x and y of robot*/
sint8 Robot_XstartCoordinate=0;
sint8 Robot_YstartCoordinate=0;

int main()
{

	Dio_init();
	/* configure timer1 in PWM mode with pin OCRA1 and OCRB1 */
	Timer1_Init(TMR1_PWM_FAST_MODE_ICR1,OCRA1_NON_INVERTING,OCRB1_NON_INVERTING);
	/* set frequency of PWM is 50kHz for DC motor*/

	PWM_FreQ_Hz(50000);

	UART_Init();

	Robot_Localization();
}


void Robot_Move_Forward()
{
	Motor_Move_Right(&motor1);
	Motor_Move_Right(&motor2);
}

void Robot_Move_Backward()
{
	Motor_Move_Left(&motor1);
	Motor_Move_Left(&motor2);
}

void Robot_Move_Right()
{
	Motor_Move_Right(&motor1);
	Motor_Stop(&motor2);
}

void Robot_Move_Left()
{
	Motor_Stop(&motor1);
	Motor_Move_Right(&motor2);
}

void Robot_turn_Right90()
{
	Robot_Speed(HIGH_SPEED);
	Robot_Move_Right();
	_delay_ms(1500);
	Robot_Stop();

}

void Robot_turn_Left90()
{
	Robot_Speed(HIGH_SPEED);
	Robot_Move_Left();
	_delay_ms(1500);
	Robot_Stop();

}

void Robot_Reverse()
{
	Robot_Speed(HIGH_SPEED);
	Robot_Move_Right();
	_delay_ms(3000);
}

void Robot_Move_Step()
{
	Robot_Speed(HIGH_SPEED);
	Robot_Move_Forward();
	_delay_ms(10);

}

void Robot_Stop()
{
	Motor_Stop(&motor1);
	Motor_Stop(&motor2);
	_delay_ms(600);
}

void Robot_Speed(Motor_Sped_t speed)
{
	PWM_SetDuty_Cycle_OCR1A(speed);
	PWM_SetDuty_Cycle_OCR1B(speed);
}

void Robot_LineFollower()
{

	IR_Sensor_s IR_statusRight = NON_BODY;
	IR_Sensor_s IR_statusLeft = NON_BODY;

    Robot_Speed(HIGH_SPEED);
	while(1)
	{
		IR_statusRight = IR_Sensor_Status(IR_SENSOR_RIGHT);
		IR_statusLeft  = IR_Sensor_Status(IR_SENSOR_LEFT);


		if(FlagStop == 1)
		{
			Robot_Move_Step();
			Robot_Stop();
			break;
		}
		//if the two sensor saw the black line at the same time flagStop = 1
		if(
				(IR_statusLeft == NON_BODY)
				&& (IR_statusRight == NON_BODY)
				&& (FlagStop == 0)
				)
		{
			Robot_Stop();
			FlagStop=1;
		}
		else if(
				(IR_statusLeft == BODY)
				&& (IR_statusRight == NON_BODY)
				&& (FlagStop == 0)
				)
		{
			Robot_Move_Right();
		}
		else if(
				(IR_statusLeft == NON_BODY)
				&& (IR_statusRight == BODY)
				&& (FlagStop == 0)
				)
		{
			Robot_Move_Left();
		}
		else if(
				(IR_statusLeft == BODY)
				&& (IR_statusRight == BODY)
				&& (FlagStop == 0)
				)
		{
			Robot_Move_Forward();
		}

	}
}

void Robot_Localization()
{

	sint8 l_XendCoordinate=0;
	sint8 l_YendCoordinate=0;
	sint8 l_Xstep=0;
	sint8 l_Ystep=0;
	while(1)
	{
		//Get the X coordinate
		UART_SendString("\nenter the X coordinate = ");
		l_XendCoordinate = UART_ReceiveByte();
		UART_SendString("\nyour enter X coordinate = ");
		UART_SendByte(l_XendCoordinate);
		//Get the X coordinate
		UART_SendString("\nenter the Y coordinate = ");
		l_YendCoordinate = UART_ReceiveByte();
		UART_SendString("\nyour enter Y coordinate = ");
		UART_SendByte(l_YendCoordinate);
		//convert the coordinates from character to integer
		l_XendCoordinate = l_XendCoordinate - 48;
		l_YendCoordinate = l_YendCoordinate - 48;
		//calculate the number of steps should the robot do it to reach the destination
		l_Xstep = l_XendCoordinate - Robot_XstartCoordinate;
		l_Ystep = l_YendCoordinate - Robot_YstartCoordinate;
		/*
		 * Default Robot orientation in Y coordinate
		 * Execute Y coordinate
		 * Orientation
		 * Execute X coordinate
		 */

		if(l_Xstep == 0
		   && l_Ystep>0)
		{
			for(sint8 l_counter=0;l_counter<l_Ystep;l_counter++)
			{
				FlagStop=0;
				while(FlagStop==0)
				{
					Robot_LineFollower();
				}
			}
		}
		else if(l_Xstep == 0
				&& l_Ystep<0)
		{
			Robot_Reverse();

			for(sint8 l_counter=l_Ystep;l_counter<0;l_counter++)
			{
				FlagStop=0;
				while(FlagStop==0)
				{
					Robot_LineFollower();
				}
			}
			Robot_Reverse();

		}
		else if(l_Xstep > 0
				&& l_Ystep == 0)
		{
			Robot_turn_Right90();
			for(sint8 l_counter=0;l_counter<l_Xstep;l_counter++)
			{
				FlagStop=0;
				while(FlagStop==0)
				{
					Robot_LineFollower();
				}
			}
			Robot_turn_Left90();
		}
		else if(l_Xstep < 0
				&& l_Ystep == 0)
		{
			Robot_turn_Left90();

			for(sint8 l_counter=l_Xstep;l_counter<0;l_counter++)
			{
				FlagStop=0;
				while(FlagStop==0)
				{
					Robot_LineFollower();
				}
			}
			Robot_turn_Right90();
		}
		else if(l_Xstep < 0
				&& l_Ystep < 0)
		{
			Robot_Reverse();

			for(sint8 l_counter=l_Ystep;l_counter<0;l_counter++)
			{
				FlagStop=0;
				while(FlagStop==0)
				{
					Robot_LineFollower();
				}
			}
			//Robot_Reverse();

			//Robot_turn_Left90();
			Robot_turn_Right90();

			for(sint8 l_counter=l_Xstep;l_counter<0;l_counter++)
			{
				FlagStop=0;
				while(FlagStop==0)
				{
					Robot_LineFollower();
				}
			}
			Robot_turn_Right90();


		}
		else if(l_Xstep > 0
				&& l_Ystep > 0)
		{
			for(sint8 l_counter=0;l_counter<l_Ystep;l_counter++)
			{
				FlagStop=0;
				while(FlagStop==0)
				{
					Robot_LineFollower();
				}
			}
			Robot_turn_Right90();
			for(sint8 l_counter=0;l_counter<l_Xstep;l_counter++)
			{
				FlagStop=0;
				while(FlagStop==0)
				{
					Robot_LineFollower();
				}
			}
			Robot_turn_Left90();

		}
		else if(l_Xstep > 0
				&& l_Ystep < 0)
		{
			Robot_Reverse();

			for(sint8 l_counter=l_Ystep;l_counter<0;l_counter++)
			{
				FlagStop=0;
				while(FlagStop==0)
				{
					Robot_LineFollower();
				}
			}
			//Robot_Reverse();
			//Robot_turn_Right90();
			Robot_turn_Left90();
			for(sint8 l_counter=0;l_counter<l_Xstep;l_counter++)
			{
				FlagStop=0;
				while(FlagStop==0)
				{
					Robot_LineFollower();
				}
			}
			Robot_turn_Left90();

		}
		else if(l_Xstep < 0
				&& l_Ystep > 0)
		{
			for(sint8 l_counter=0;l_counter<l_Ystep;l_counter++)
			{
				FlagStop=0;
				while(FlagStop==0)
				{
					Robot_LineFollower();
				}
			}
			Robot_turn_Left90();

			for(sint8 l_counter=l_Xstep;l_counter<0;l_counter++)
			{
				FlagStop=0;
				while(FlagStop==0)
				{
					Robot_LineFollower();
				}
			}
			Robot_turn_Right90();

		}
	    Robot_XstartCoordinate = l_XendCoordinate;
		Robot_YstartCoordinate = l_YendCoordinate;
	}
}

