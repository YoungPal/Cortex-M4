/******************************************************************************
  ��Ʈ ����:
    1)  ARM Cortex-M4 ����� ��ƮA�� 9~10������(PA9~PA10) 2�� ���̺��
        UART����� RXD, TXD �ɿ� �����Ѵ�.
******************************************************************************/
// stm32f4xx�� �� �������͵��� ������ �������
#include "stm32f4xx.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CMD_SIZE 50
#define ARR_CNT 5  


volatile unsigned char rx1Flag = 0;
char rx2Data[50];
void UART1_Init();
void Serial2_Event();
void Serial2_Send(unsigned char t);
void Serial2_Send_String(char* s);
int putchar(int ch);
void print_2d1(double number);

int main()
{
  GPIO_InitTypeDef   GPIO_InitStructure_LED;
  GPIO_InitTypeDef   GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure;
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  
  GPIO_InitStructure_LED.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure_LED.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure_LED.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure_LED.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure_LED.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  GPIO_Init(GPIOC, &GPIO_InitStructure_LED);  
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                                             
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3);     //USART3_TX
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);    //USART3_RX

  //���ͷ�Ʈ enable �� Priority ����.
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART3, &USART_InitStructure);

  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); // USART3 Interrupt enable
  USART_Cmd(USART3, ENABLE);

  Serial2_Send_String("Start Main()\n\r");
  while(1)
  {
    if(rx1Flag)  // '\r' ���� �ԷµǸ�
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

  rx1Flag = 0; // �ٽ� Rflag �� 0���� ���´�.    
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

void Serial2_Send(unsigned char t)
{
  USART_SendData(USART3, t);
  while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
}

void Serial2_Send_String(char* s)
{
  while( *s != '\0')
  {
   Serial2_Send((unsigned char)(*s));
   s++;   //s = s + 1;
  }
}

int putchar(int ch)
{
	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
	USART_SendData(USART3,ch);
	return ch;
}

void USART3_IRQHandler(void)
{
  if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
  {
	static int i=0;
	rx2Data[i] = USART_ReceiveData(USART3);
	if(rx2Data[i] == '\r')  
	{
		rx2Data[i] = '\0';
		rx1Flag = 1;
		i = 0;
	}
	else
	{
		i++;
	}
  }
}

void print_2d1(double number)		        /* floating-point number xx.x */
{ 
        unsigned int i, j;

	j = (int)(number*10. + 0.5);
	i = j / 100;					// 10^1
	if(i == 0) Serial2_Send(' ');
	else       Serial2_Send(i + '0');

	j = j % 100;					// 10^0
	i = j / 10;
	Serial2_Send(i + '0');
	Serial2_Send('.');

	i = j % 10;					// 10^-1
	Serial2_Send(i + '0');
}