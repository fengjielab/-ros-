/*
 * robot.c
 *
 *  Created on: 2026年8月28日
 *      Author: Lenovo
 */


#include "robot.h"
#include "motor.h"

void Robot_Forward(float speed)
{
    Motor_SetTarget(1, speed);
    Motor_SetTarget(2, speed);
    Motor_SetTarget(3, speed);
    Motor_SetTarget(4, speed);
}

void Robot_Backward(float speed)
{
    Motor_SetTarget(1, -speed);
    Motor_SetTarget(2, -speed);
    Motor_SetTarget(3, -speed);
    Motor_SetTarget(4, -speed);
}

void Robot_Left(float speed)
{
    Motor_SetTarget(1, -speed);
    Motor_SetTarget(2,  speed);
    Motor_SetTarget(3,  speed);
    Motor_SetTarget(4, -speed);
}

void Robot_Right(float speed)
{
    Motor_SetTarget(1,  speed);
    Motor_SetTarget(2, -speed);
    Motor_SetTarget(3, -speed);
    Motor_SetTarget(4,  speed);
}

void Robot_TurnLeft(float speed)
{
    Motor_SetTarget(1, -speed);
    Motor_SetTarget(2,  speed);
    Motor_SetTarget(3, -speed);
    Motor_SetTarget(4,  speed);
}

void Robot_TurnRight(float speed)
{
    Motor_SetTarget(1,  speed);
    Motor_SetTarget(2, -speed);
    Motor_SetTarget(3,  speed);
    Motor_SetTarget(4, -speed);
}

void Robot_Stop(void)
{
    Motor_StopAll();
}
