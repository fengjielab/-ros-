/*
 * motor.c
 *
 *  Created on: 2026年8月24日
 *      Author: Lenovo
 */


#include "motor.h"

#include "main.h"
#include "tim.h"
#include "gpio.h"

/* =========================================================
 * 电机控制参数
 * ========================================================= */

#define CONTROL_DT     0.1f

#define BASE_PWM       300

#define PWM_MAX        700

#define M1_ENCODER_SIGN   -1
#define M2_ENCODER_SIGN    -1
#define M3_ENCODER_SIGN   1
#define M4_ENCODER_SIGN    1
/*
 * 这里填写你现在已经调通的目标值
 *
 * 我暂时写290作为示例。
 */
static float motor_target[4] =
{
    0, 0, 0, 0
};

typedef struct
{
    float kp;
    float ki;
    float kd;

    float integral;
    float last_error;

} PID_t;

static PID_t motor_pid[4] =
{
    {6.0f, 1.0f, 0.01f, 0.0f, 0.0f},
    {6.0f, 1.0f, 0.01f, 0.0f, 0.0f},
    {6.0f, 1.0f, 0.01f, 0.0f, 0.0f},
    {6.0f, 1.0f, 0.01f, 0.0f, 0.0f}
};

static int16_t motor_count[4] =
{
    0, 0, 0, 0
};


static uint16_t motor_pwm[4] =
{
    BASE_PWM,
    BASE_PWM,
    BASE_PWM,
    BASE_PWM
};

static void Motor_SetPWM(uint8_t motor,
                         uint16_t pwm)
{
    if (pwm > PWM_MAX)
    {
        pwm = PWM_MAX;
    }


    switch (motor)
    {
        case 1:

            __HAL_TIM_SET_COMPARE(
                &htim8,
                TIM_CHANNEL_1,
                pwm);

            break;


        case 2:

            __HAL_TIM_SET_COMPARE(
                &htim8,
                TIM_CHANNEL_2,
                pwm);

            break;


        case 3:

            __HAL_TIM_SET_COMPARE(
                &htim8,
                TIM_CHANNEL_3,
                pwm);

            break;


        case 4:

            __HAL_TIM_SET_COMPARE(
                &htim5,
                TIM_CHANNEL_1,
                pwm);

            break;


        default:
            break;
    }
}

static void Motor_SetDirection(uint8_t motor,
                               int8_t direction)
{
    switch (motor)
    {
        case 1:

            if (direction > 0)
            {
                HAL_GPIO_WritePin(
                    M1_IN1_GPIO_Port,
                    M1_IN1_Pin,
                    GPIO_PIN_SET);

                HAL_GPIO_WritePin(
                    M1_IN2_GPIO_Port,
                    M1_IN2_Pin,
                    GPIO_PIN_RESET);
            }
            else
            {
                HAL_GPIO_WritePin(
                    M1_IN1_GPIO_Port,
                    M1_IN1_Pin,
                    GPIO_PIN_RESET);

                HAL_GPIO_WritePin(
                    M1_IN2_GPIO_Port,
                    M1_IN2_Pin,
                    GPIO_PIN_SET);
            }

            break;


        case 2:

            if (direction > 0)
            {
                HAL_GPIO_WritePin(
                    M2_IN1_GPIO_Port,
                    M2_IN1_Pin,
                    GPIO_PIN_RESET);

                HAL_GPIO_WritePin(
                    M2_IN2_GPIO_Port,
                    M2_IN2_Pin,
                    GPIO_PIN_SET);
            }
            else
            {
                HAL_GPIO_WritePin(
                    M2_IN1_GPIO_Port,
                    M2_IN1_Pin,
                    GPIO_PIN_SET);

                HAL_GPIO_WritePin(
                    M2_IN2_GPIO_Port,
                    M2_IN2_Pin,
                    GPIO_PIN_RESET);
            }

            break;


        case 3:

            if (direction > 0)
            {
                HAL_GPIO_WritePin(
                    M3_IN1_GPIO_Port,
                    M3_IN1_Pin,
                    GPIO_PIN_SET);

                HAL_GPIO_WritePin(
                    M3_IN2_GPIO_Port,
                    M3_IN2_Pin,
                    GPIO_PIN_RESET);
            }
            else
            {
                HAL_GPIO_WritePin(
                    M3_IN1_GPIO_Port,
                    M3_IN1_Pin,
                    GPIO_PIN_RESET);

                HAL_GPIO_WritePin(
                    M3_IN2_GPIO_Port,
                    M3_IN2_Pin,
                    GPIO_PIN_SET);
            }

            break;


        case 4:

            if (direction > 0)
            {
                HAL_GPIO_WritePin(
                    M4_IN1_GPIO_Port,
                    M4_IN1_Pin,
                    GPIO_PIN_RESET);

                HAL_GPIO_WritePin(
                    M4_IN2_GPIO_Port,
                    M4_IN2_Pin,
                    GPIO_PIN_SET);
            }
            else
            {
                HAL_GPIO_WritePin(
                    M4_IN1_GPIO_Port,
                    M4_IN1_Pin,
                    GPIO_PIN_SET);

                HAL_GPIO_WritePin(
                    M4_IN2_GPIO_Port,
                    M4_IN2_Pin,
                    GPIO_PIN_RESET);
            }

            break;


        default:
            break;
    }
}

static float PID_Update(PID_t *pid,
                        float target,
                        float actual)
{
    float error;
    float derivative;
    float correction;
    float output;


    /* 当前误差 */
    error = target - actual;


    /* I */
    pid->integral +=
        error * CONTROL_DT;


    if (pid->integral > 100.0f)
    {
        pid->integral = 100.0f;
    }

    if (pid->integral < -100.0f)
    {
        pid->integral = -100.0f;
    }


    /* D */
    derivative =
        (error - pid->last_error)
        /
        CONTROL_DT;


    /* PID输出修正量 */
    correction =
          pid->kp * error
        + pid->ki * pid->integral
        + pid->kd * derivative;


    pid->last_error = error;


    /* 基础PWM + PID修正 */
    output =
        BASE_PWM + correction;


    if (output > PWM_MAX)
    {
        output = PWM_MAX;
    }

    if (output < 0.0f)
    {
        output = 0.0f;
    }


    return output;
}

void Motor_Init(void)
{
    /* ==========================
     * 启动4个编码器
     * ========================== */

    HAL_TIM_Encoder_Start(
        &htim2,
        TIM_CHANNEL_ALL);

    HAL_TIM_Encoder_Start(
        &htim3,
        TIM_CHANNEL_ALL);

    HAL_TIM_Encoder_Start(
        &htim4,
        TIM_CHANNEL_ALL);

    HAL_TIM_Encoder_Start(
        &htim1,
        TIM_CHANNEL_ALL);


    /* 清零 */

    __HAL_TIM_SET_COUNTER(&htim2, 0);
    __HAL_TIM_SET_COUNTER(&htim3, 0);
    __HAL_TIM_SET_COUNTER(&htim4, 0);
    __HAL_TIM_SET_COUNTER(&htim1, 0);


    /* ==========================
     * 启动PWM
     * ========================== */

    HAL_TIM_PWM_Start(
        &htim8,
        TIM_CHANNEL_1);

    HAL_TIM_PWM_Start(
        &htim8,
        TIM_CHANNEL_2);

    HAL_TIM_PWM_Start(
        &htim8,
        TIM_CHANNEL_3);

    HAL_TIM_PWM_Start(
        &htim5,
        TIM_CHANNEL_1);


    /* 设置方向 */

    Motor_SetDirection(1, 1);
    Motor_SetDirection(2, 1);
    Motor_SetDirection(3, 1);
    Motor_SetDirection(4, 1);


    /* 初始PWM */

    motor_pwm[0] = 0;
    motor_pwm[1] = 0;
    motor_pwm[2] = 0;
    motor_pwm[3] = 0;

    Motor_SetPWM(1, 0);
    Motor_SetPWM(2, 0);
    Motor_SetPWM(3, 0);
    Motor_SetPWM(4, 0);
}

void Motor_ControlStep(void)
{
    int16_t c1;
    int16_t c2;
    int16_t c3;
    int16_t c4;


    /* ==========================
     * 读取四个编码器
     * ========================== */

    c1 =
        (int16_t)__HAL_TIM_GET_COUNTER(
            &htim2);

    c2 =
        (int16_t)__HAL_TIM_GET_COUNTER(
            &htim3);

    c3 =
        (int16_t)__HAL_TIM_GET_COUNTER(
            &htim4);

    c4 =
        (int16_t)__HAL_TIM_GET_COUNTER(
            &htim1);


    /*
     * 当前阶段只比较速度大小
     */

    c1 = M1_ENCODER_SIGN * c1;
    c2 = M2_ENCODER_SIGN * c2;
    c3 = M3_ENCODER_SIGN * c3;
    c4 = M4_ENCODER_SIGN * c4;


    motor_count[0] = c1;
    motor_count[1] = c2;
    motor_count[2] = c3;
    motor_count[3] = c4;


    /* ==========================
     * 为下一个100ms重新计数
     * ========================== */

    __HAL_TIM_SET_COUNTER(&htim2, 0);
    __HAL_TIM_SET_COUNTER(&htim3, 0);
    __HAL_TIM_SET_COUNTER(&htim4, 0);
    __HAL_TIM_SET_COUNTER(&htim1, 0);


    /* ==========================
     * 四路PID
     * ========================== */

    for (int i = 0; i < 4; i++)
    {
        float target;
        float target_abs;
        float actual_abs;


        target = motor_target[i];


        /* =================================
         * 情况1：目标为0
         * → 停止电机
         * ================================= */

        if (target == 0.0f)
        {
            Motor_SetPWM(i + 1, 0);

            motor_pwm[i] = 0;

            /* 停止时清除PID历史 */
            motor_pid[i].integral = 0.0f;
            motor_pid[i].last_error = 0.0f;

            continue;
        }


        /* =================================
         * 情况2：根据目标正负决定方向
         * ================================= */

        if (target > 0.0f)
        {
            Motor_SetDirection(i + 1, 1);

            target_abs = target;
        }
        else
        {
            Motor_SetDirection(i + 1, -1);

            target_abs = -target;
        }


        /* =================================
         * PID只比较速度大小
         * ================================= */

        actual_abs = (float)motor_count[i];

        if (actual_abs < 0.0f)
        {
            actual_abs = -actual_abs;
        }


        motor_pwm[i] =
            (uint16_t)PID_Update(
                &motor_pid[i],
                target_abs,
                actual_abs);


        Motor_SetPWM(
            i + 1,
            motor_pwm[i]);
    }
}

void Motor_SetTarget(uint8_t motor, float target)
{
    if (motor < 1 || motor > 4)
    {
        return;
    }

    motor_target[motor - 1] = target;
}

float Motor_GetTarget(uint8_t motor)
{
    if (motor < 1 || motor > 4)
    {
        return 0.0f;
    }

    return motor_target[motor - 1];
}

int16_t Motor_GetCount(uint8_t motor)
{
    if (motor < 1 || motor > 4)
    {
        return 0;
    }

    return motor_count[motor - 1];
}

uint16_t Motor_GetPWM(uint8_t motor)
{
    if (motor < 1 || motor > 4)
    {
        return 0;
    }

    return motor_pwm[motor - 1];
}

void Motor_StopAll(void)
{
    for (int i = 0; i < 4; i++)
    {
        motor_target[i] = 0.0f;

        motor_pwm[i] = 0;

        motor_pid[i].integral = 0.0f;
        motor_pid[i].last_error = 0.0f;

        Motor_SetPWM(i + 1, 0);
    }
}


