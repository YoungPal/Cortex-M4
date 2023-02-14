/******************************************************************************
  ��Ʈ ����:
    1)  ARM Cortex-M4 ����� ��ƮA�� 9~10������(PA9~PA10) 2�� ���̺��
        UART����� RXD, TXD �ɿ� �����Ѵ�.
******************************************************************************/
// stm32f4xx�� �� �������͵��� ������ �������
#include "stm32f4xx.h"
#include "uart2.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CMD_SIZE 50
#define ARR_CNT 5  

extern volatile unsigned char rx2Flag;
extern char rx2Data[50];


int main()
{
  GPIO_InitTypeDef   GPIO_InitStructure_LED;
  
  GPIO_InitStructure_LED.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure_LED.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure_LED.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure_LED.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure_LED.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  GPIO_Init(GPIOC, &GPIO_InitStructure_LED);  
  
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  
  UART2_init();

  Serial2_Send_String("Start Main()\n\r");
  while(1)
  {
    if(rx2Flag)  // '\r' ���� �ԷµǸ�
      Serial2_Event();
  }
}

void Serial2_Event()
{
  int i=0;
  int num = 0;
  char * pToken;
  char * pArray[ARR_CNT]={0};
  char recvBuf[CMD_SIZE]={0};       
  strcpy(recvBuf,rx2Data);

  rx2Flag = 0; // �ٽ� Rflag �� 0���� ���´�.    
  Serial2_Send_String(recvBuf);
  Serial2_Send_String("\n\r");
  printf("rx : %s\r\n",recvBuf);
     
  pToken = strtok(recvBuf,"[@]");

  while(pToken != NULL)
  {
    pArray[i] =  pToken;
    if(++i >= ARR_CNT)
      break;
    pToken = strtok(NULL,"[@]");
  }
       
  if(!strcmp(pArray[1],"LED"))
  {
    if(pArray != NULL)              
      num = atoi(pArray[3]);
    else
      num = 0;
    if(!strcmp(pArray[2],"ON"))
    {
     GPIO_SetBits(GPIOC, GPIO_Pin_0 << num); 
    }
    else if(!strcmp(pArray[2],"OFF"))
    {
      GPIO_ResetBits(GPIOC, GPIO_Pin_0 << num); 
    }
  }
}