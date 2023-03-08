#ifndef __TIM10_MOTOR_H__
#define __TIM10_MOTOR_H__

void TIM10_Motor_Init();
void Motor_ADC(int adc1Flag, uint16_t adc_data, int adc_pwm, int old_pwm);
void Motor_PWM(int old_pwm, int ccr1, int* pwmCount);

#endif