#ifndef __UART2_H__
#define __UART2_H__

void Serial2_Event();
void Serial2_Send(unsigned char t);
void Serial2_Send_String(char* s);
void UART2_init();
int putchar(int ch);
void print_2d1(double number);

#endif 