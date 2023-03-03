/******************************************************************************
  ��Ʈ ����:
    1)  ARM Cortex-M4 ����� ��ƮC (PC0 ~ PC7)�� 8�� ���̺� �̿��ؼ�
        Array FND����� SA_A ~ SA_H�� �����Ѵ�. (SA_A�� PC0�� ����Ǿ� �Ѵ�.)
    2)  ARM Cortex-M4 ����� ��ƮC (PC8 ~ PC11)�� 4�� ���̺� �̿��ؼ�
        Array FND����� C0 ~ C3�� �����Ѵ�. (C0�� PC8�� ����Ǿ� �Ѵ�.)
    3)  ARM Cortex-M4 ����� ��ƮA (PA0)�� 2�� ���̺� �̿��ؼ�
        Switch����� BT0�� �����Ѵ�. (BT0�� PA0�� ����Ǿ� �Ѵ�.)
******************************************************************************/
// stm32f4xx�� �� �������͵��� ������ �������
#include "stm32f4xx.h"
#include "uart2.h"
#include <stdio.h>


// delay �Լ�
static void Delay(const uint32_t Count)
{
  __IO uint32_t index = 0; 
  for(index = (16800 * Count); index != 0; index--);
}

// 7-���׸�Ʈ ��Ʈ�� �迭�� �����Ѵ�.
unsigned char Font[18] = {0x3F, 0X06, 0X5B, 0X4F, 
                                         0X66, 0X6D, 0X7C, 0X07,
                                         0X7F, 0X67, 0X77, 0X7C, 
                                         0X39, 0X5E, 0X79, 0X71, 
                                         0X08, 0X80};

volatile int count = 0; // ���ͷ�Ʈ�� �� �� ����Ǿ��°��� ��Ÿ���ִ� �����̴�.

// Dec Segment �Լ� ���� ==> �� �Լ��� �ԷµǴ� ���� 10�������� 
//  7-���׸�Ʈ�� ǥ���� �ִ� �Լ��̴�  
void DecSegment ( unsigned char N )
{
  int  i ;
  unsigned char  N10, N1 ;


  N10 = N /10;  // ���׸�Ʈ���� ����ϴ� ù��°�� �ڸ��� ����
  N1 = N % 10 ;


  for( i = 0 ; i < 30; i++ )
  {
    GPIO_Write(GPIOC, Font[N10]|0x0b00); // ���� �� ��° ���׸�Ʈ�� ON�ϰ�, ù ��°  ���ڸ� ����� �ش� 
    Delay(1);        

    GPIO_Write(GPIOC, Font[N1]|0x0700); // ���� �� ��° ���׸�Ʈ�� ON�ϰ�, �� ��°  ���ڸ� ����� �ش� 
    Delay(1);     
  }
} 

void EXTI9_5_IRQHandler(void)
{
  count++;
  if(EXTI_GetITStatus(EXTI_Line8) != RESET)
  {
    printf("EXIT : 8\tcount : %d\r\n",count);
    EXTI_ClearITPendingBit(EXTI_Line8);
  }
  else if(EXTI_GetITStatus(EXTI_Line9) != RESET)
  {
    printf("EXIT : 9\tcount : %d\r\n",count);
    EXTI_ClearITPendingBit(EXTI_Line9);
  }
}

void EXTI15_10_IRQHandler(void)
{
  count++;
  if(EXTI_GetITStatus(EXTI_Line10) != RESET)
  {
    printf("EXIT : 10\tcount : %d\r\n",count);
    EXTI_ClearITPendingBit(EXTI_Line10);
  }
  else if(EXTI_GetITStatus(EXTI_Line11) != RESET)
  {
    printf("EXIT : 11\tcount : %d\r\n",count);
    EXTI_ClearITPendingBit(EXTI_Line11);
  }
  else if(EXTI_GetITStatus(EXTI_Line12) != RESET)
  {
    printf("EXIT : 12\tcount : %d\r\n",count);
    EXTI_ClearITPendingBit(EXTI_Line12);
  }
}


int main()
{
  GPIO_InitTypeDef   GPIO_InitStructure;
  EXTI_InitTypeDef   EXTI_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure; 

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);  

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|
                                GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|
                                GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  UART2_init();
   printf("Strat main()\r\n");

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //��ƮA�� 0������ �ܺ� ���ͷ�Ʈ 0 �� �����Ѵ�.
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource8);
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource9);
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource10);
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource11);
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource12);
  //�ܺ� ���ͷ�Ʈ0 ���ͷ�Ʈ�� ����Ѵ�.(�켱������ ������� ����)
  //�ܺ� ���ͷ�Ʈ0�� ��¿������� �߻��ǰ� �����Ѵ�.
  EXTI_InitStructure.EXTI_Line = EXTI_Line8|EXTI_Line9|EXTI_Line10|EXTI_Line11|EXTI_Line12;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  //���ͷ�Ʈ enable �� Priority ����.
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  NVIC_Init(&NVIC_InitStructure);

  count = 0;

  while(1)
  {
    DecSegment(count);
  }
}
