#include <stdlib.h>
#include "stm32f4xx.h"
#include "tim10_motor.h"

volatile int pwm = 50;

long map(long x, long in_min, long in_max, long out_min, long out_max);

void TIM10_Motor_Init(){
  
    GPIO_InitTypeDef   GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
  
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;     // TIM10_CH1
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
    GPIO_Init(GPIOB, &GPIO_InitStructure); 
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;     // motor
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Init(GPIOB, &GPIO_InitStructure); 

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_TIM10);    

    //SYSCLK(168MHz) / AHB Pre(1) == 168Mhz / APB2  Pre(2) == 84Mhz * 2  == 168MHz (Timer Clock) 
    TIM_TimeBaseStructure.TIM_Prescaler = 168-1;         //168Mhz/168 = 1MHz(1us)
    TIM_TimeBaseStructure.TIM_Period = 17700-1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM10, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  //  TIM_OCInitStructure.TIM_Pulse = 7080-1;         //40% duty
    TIM_OCInitStructure.TIM_Pulse = 8850-1;         //50% duty
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM10, &TIM_OCInitStructure);

    //타이머10을 동작시킨다.
    TIM_ClearITPendingBit(TIM10, TIM_IT_CC1|TIM_IT_Update);
    TIM_ITConfig(TIM10, TIM_IT_CC1|TIM_IT_Update, ENABLE);

    TIM_OC1PreloadConfig(TIM10, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM10, ENABLE);

    TIM_Cmd(TIM10, ENABLE);

    GPIO_WriteBit(GPIOB,GPIO_Pin_9,Bit_RESET);  //motor stop
    GPIO_WriteBit(GPIOB,GPIO_Pin_10,Bit_RESET);//motor stop      
    
}


void Motor_ADC(int adc1Flag, uint16_t adc_data, int adc_pwm, int old_pwm)
{
    if(adc1Flag)
    {
      adc_pwm = map(adc_data,0,4095,0,100);
      if(abs(adc_pwm - old_pwm)>=5)
          pwm = adc_pwm;
      adc1Flag = 0;
    }
  
}

void Motor_PWM(int old_pwm, int ccr1, int* pwmCount)
{
    if(pwm != old_pwm  )
    {
        if(pwm == 0)          
             ccr1 = 1;
        else if(pwm == 100)
             ccr1 = 177 * 100 - 1;
        else
             ccr1 = 177 * pwm;
        
        TIM10->CCR1 = ccr1;
        old_pwm = pwm;
        *pwmCount = pwm * 100;
    }    
  
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
