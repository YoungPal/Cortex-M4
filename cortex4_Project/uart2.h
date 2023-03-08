#ifndef __UART2_H__ //중복 방지를 위해 반드시 선언 
#define __UART2_H__ //define은 대문자 언더바

void UART2_init();
void Serial2_Event();
void Serial2_Send(unsigned char t);
void Serial2_Send_String(char* s);
int putchar(int ch);            //printf() 사용시 필요
void print_2d1(double number);

#endif //define부터 여기까지 정의