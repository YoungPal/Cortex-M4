#include "stm32f4xx.h"
#include "tim1_counter.h"

volatile int tr_cnt = 0,mr_cnt=0;
volatile int tl_cnt =0 ,ml_cnt = 0;

void TIM1_Counter_Init()
{
  GPIO_InitTypeDef   GPIO_InitStructure;
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;             //TIM1_CH1
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1);
  TIM_TIxExternalClockConfig(TIM1,TIM_TIxExternalCLK1Source_TI1,TIM_ICPolarity_Rising,0x03);
  TIM_Cmd(TIM1,ENABLE); 
}

void TIM1_UP_TIM10_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM10, TIM_IT_CC1) != RESET)
  {
    TIM_ClearITPendingBit(TIM10, TIM_IT_CC1);
    tl_cnt++;
    if(tl_cnt >= 28)   //500ms / 17.7ms = 28.2
    {
      tl_cnt = 0;
      ml_cnt ++ ;       //500ms count
      if(ml_cnt > 99) ml_cnt = 0;
    }  
  }
  if(TIM_GetITStatus(TIM10, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM10, TIM_IT_Update);
    tr_cnt++ ;
    if(tr_cnt >= 56)   //1000ms / 17.7ms = 56.4
    {
      tr_cnt = 0;
      mr_cnt ++ ;       //1000ms count
      if(mr_cnt > 99) mr_cnt = 0;
    } 
  }
}