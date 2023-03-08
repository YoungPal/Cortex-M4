#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f4xx.h"
#include "uart2.h"
#include "uart4.h"
#include "adc1.h"
//#include "tim7_fnd.h"
#include "key.h"
#include "tim10_motor.h"
#include "tim1_counter.h"
#include "lcd.h"
#include "led.h"

/******************************************************************************
* General-purpose timers TIM10

    포트 연결:
    PA0, PA1 : UART4 RX,Tx
    PA2, PA3 : USART2 RX,Tx
    PA5 ~ PA12 : CLCD D0 ~ D7
    PA8 : M_SEN[Motor Sensor

    PB0 ~ PB2 : CLCD RW, RW, E
    PB8  ==> DC Motor M_EN
    PB9  ==> DC Motor M_D2
    PB10 ==> DC Motor M_D1

    PC0~Pc11 ==> FND A~H, C0~C3
    PC12~PC15 ==> Button0~3
     
    FND_OUTPUT
    FND12 ==> PWM
    FND34 ==>Duty rate
    
    

    4) PA0 ~ PA7 button0~7



     PA0 ==> VRES
    Key2 ==> Left
    Key3 == > Right
    Key4 ==> Stop

    Key5 ==> PWM - 10%
    Key6 ==> PWM + 10%

    FND ==> Duty rate

******************************************************************************/
#define CMD_SIZE 50
#define ARR_CNT 5  
//#define PORTC_FND

int vresFlag=0;

extern volatile int pwm;
volatile int counter = 0;

extern volatile unsigned char rx2Flag;
extern char rx2Data[50];
extern volatile unsigned char rx4Flag;
extern char rx4Data[50];

extern volatile int key;

extern uint16_t adc_data;
extern volatile int fndNumber;
extern volatile int adc1Flag;
extern volatile unsigned long systick_sec;            //1sec
extern volatile int systick_secFlag;

void MOTOR_RIGHT();
void MOTOR_LEFT();
void MOTOR_STOP();
void MOTOR_PWM();


int main()
{
  int old_pwm=50;
  int adc_pwm=50;
  int ccr1;
  vresFlag=0;
  
  Init_ADC1();
  //ADC_SoftwareStartConv(ADC1); 
  
  TIM10_Motor_Init();
  GPIOAB_LCD_Init();
  //M1_Counter_Init(); //PORTA8
  Key_Init();

#ifdef PORTC_FND
  IM7_Fnd_Init();  
#else
  PORTC_Led_Init();
#endif
  
  UART2_init();
  UART4_init();
  
  int pwmCount = 0;
  pwmCount = pwm * 100;
  
  lcd(0,0,"LCD test");
  
  Serial2_Send_String("start main()\r\n");
    
  while(1)
  {    
    if(rx2Flag)
    {
      printf("rxData : %s\r\n", rx2Data);
      Serial4_Send_String(rx2Data);
      Serial4_Send_String("\r");
      rx2Flag=0;
    }
    if(rx4Flag)
    {
      printf("%s\r\n", rx4Data);
      Serial4_Event();
      rx4Flag=0;
    }
    
    if(key != 0)
    {
      printf("Key : %d\r\n", key);
      if(key==1) //motor left
      {
        GPIO_ResetBits(GPIOB,GPIO_Pin_9);
        GPIO_SetBits(GPIOB,GPIO_Pin_10);
        
        GPIO_SetBits(GPIOC,GPIO_Pin_0);
      }
      else if(key==2) //motor right
      {
        GPIO_SetBits(GPIOB,GPIO_Pin_9);
        GPIO_ResetBits(GPIOB,GPIO_Pin_10);
      }
      else if(key==3) //motor stop
      {        
        GPIO_WriteBit(GPIOB,GPIO_Pin_9,Bit_RESET); 
        GPIO_WriteBit(GPIOB,GPIO_Pin_10,Bit_RESET);
      }
      else if(key==4)
      {
        if(vresFlag)
          vresFlag=0;
        else
          vresFlag = 1;        
      }
      
      key=0;
    }
    
//    pwm = (int)((adc_data/4096.0) * 100);
    if(systick_secFlag&&vresFlag)
    {
      ADC_SoftwareStartConv(ADC1); 
      systick_secFlag = 0;
    }
    Motor_ADC(adc1Flag, adc_data, adc_pwm, old_pwm);
    Motor_PWM(old_pwm, ccr1, &pwmCount);
   

//    LSegment(pwm);

    
    counter = TIM_GetCounter(TIM1);
    if(counter >= 100)
      counter = counter % 100;
    
//    RSegment(counter);
#ifdef PORTC_FND
    fndNumber = pwmCount + counter;
    
#endif
  }
}

void Serial4_Event()
{
  
//  Serial2_Send_String(rx4Data);
//  Serial2_Send_String("\r\n");

  int i=0;
  int num = 0;
  char * pToken;
  char * pArray[ARR_CNT]={0};
  char recvBuf[CMD_SIZE]={0};
  char sendBuf[CMD_SIZE]={0}; 
  strcpy(recvBuf,rx4Data);
  i=0; 
//  Serial2_Send_String(recvBuf);
//  Serial2_Send_String("\n\r");
  printf("rx4Data : %s\r\n",recvBuf);
     
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
    if(pArray[3] != NULL)              
      num = atoi(pArray[3]);
    else
      num = 0;
    if(!strcmp(pArray[2],"ON"))
    {
     GPIO_SetBits(GPIOC, 0x01 << num); 
    }
    else 
    {
      GPIO_ResetBits(GPIOC, 0x01 << num); 
    }
  }
  else if(!strcmp(pArray[1],"MOTOR"))
  {
    if(pArray[3] != NULL)              
      num = atoi(pArray[3]);
    else
      num = 0;
    if(!strcmp(pArray[2],"RIGHT"))
    {
     MOTOR_RIGHT(); 
    }
    else if(!strcmp(pArray[2],"LEFT"))
    {
      MOTOR_LEFT();
    }
    else if(!strcmp(pArray[2],"STOP"))
    {
      MOTOR_STOP();
    }
    else if(!strcmp(pArray[2],"PWM"))
    {
      MOTOR_PWM();
    }
    
    
  }
  else if(!strncmp(pArray[1]," New conn",sizeof(" New conn")))
  {
      return;
  }
  else if(!strncmp(pArray[1]," Already log",sizeof(" Already log")))
  {
      return;
  }    
  sprintf(sendBuf,"[%s]%s@%s@%d\n",pArray[0],pArray[1],pArray[2],num);
  Serial4_Send_String(sendBuf);
}

void MOTOR_RIGHT()
{
  GPIO_WriteBit(GPIOB,GPIO_Pin_9,Bit_SET); 
  GPIO_WriteBit(GPIOB,GPIO_Pin_10,Bit_RESET);  
}
void MOTOR_LEFT()
{
  GPIO_WriteBit(GPIOB,GPIO_Pin_9,Bit_RESET); 
  GPIO_WriteBit(GPIOB,GPIO_Pin_10,Bit_SET);   
}
void MOTOR_STOP()
{
  GPIO_WriteBit(GPIOB,GPIO_Pin_9,Bit_RESET);  
  GPIO_WriteBit(GPIOB,GPIO_Pin_10,Bit_RESET); 
  
}
void MOTOR_PWM()
{
  if(vresFlag)
    vresFlag=0;
  else
    vresFlag = 1;  
}
