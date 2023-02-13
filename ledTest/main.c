
#include "stm32f4xx.h"

static void Delay(const uint32_t Count)
{
  uint32_t index = 0;
  for(index = (16800*Count); index!=0; index--);
}
int main()
{
  //int delay=500;
  
  
  GPIO_InitTypeDef GPIO_InitStructure; //구조체
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  //uint16_t LED_PIN=0b01;
  //uint16_t temp=0b01;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  //GPIO_Write(GPIOC, 0x55); 
  
  while(1)
  {
    GPIO_WriteBit(GPIOC, GPIO_Pin_0,Bit_SET);
    Delay(300);
    GPIO_WriteBit(GPIOC, GPIO_Pin_0,Bit_RESET);
    Delay(300);
    /*
    int led_Val = 0x01;
    for(int i=0; i<8; i++){       
      GPIO_ToggleBits(GPIOC, led_Val<<i);
      Delay(500); 
    }

    int ledVal=0x01;
    for(int i=0; i<8;i++){
      GPIO_Write(GPIOC, ledVal<<i);
      Delay(200);
      GPIO_Write(GPIOC, 0);
      Delay(200);
    }
    GPIO_Write(GPIOC, 0);
    Delay(500);
    for(int i=7; i>0;i--){
      GPIO_Write(GPIOC, ledVal<<i);
      Delay(500);
    }

    GPIO_Write(GPIOC, GPIO_Pin_0); // LED 0을 ON 시킨다. 
    Delay(500); // 0.5초 딜레이 

    GPIO_Write(GPIOC, GPIO_Pin_1); // LED 1을 ON 시킨다. 
    Delay(500); // 0.5초 딜레이 

    GPIO_Write(GPIOC, GPIO_Pin_2); // LED 2를 ON 시킨다. 
    Delay(500); // 0.5초 딜레이 

    GPIO_Write(GPIOC, GPIO_Pin_3); // LED 3을 ON 시킨다. 
    Delay(500); // 0.5초 딜레이 

    GPIO_Write(GPIOC, GPIO_Pin_4); // LED 4를 ON 시킨다. 
    Delay(500); // 0.5초 딜레이 

    GPIO_Write(GPIOC, GPIO_Pin_5); // LED 5를 ON 시킨다. 
    Delay(500); // 0.5초 딜레이 

    GPIO_Write(GPIOC, GPIO_Pin_6); // LED 6을 ON 시킨다. 
    Delay(500); // 0.5초 딜레이 

    GPIO_Write(GPIOC, GPIO_Pin_7); // LED 7을 ON 시킨다. 
    Delay(500); // 0.5초 딜레이
    
    for(int i=0 ; i<9; i++){
      GPIO_SetBits(GPIOC, LED_PIN);    
      Delay(delay);
      LED_PIN &= 0b00;
      LED_PIN |= temp<<i;
      GPIO_ResetBits(GPIOC, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);   
    }
    Delay(700);
    for(int i=1; i<9;i++){
      GPIO_SetBits(GPIOC, LED_PIN);    
      Delay(delay);
      LED_PIN = LED_PIN>>1;       
      GPIO_ResetBits(GPIOC, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);    
    }  
*/                                        
  }

}

