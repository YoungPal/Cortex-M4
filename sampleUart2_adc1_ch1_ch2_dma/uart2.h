#ifndef __UART2_H__ //�ߺ� ������ ���� �ݵ�� ���� 
#define __UART2_H__ //define�� �빮�� �����

void UART2_init();
void Serial2_Event();
void Serial2_Send(unsigned char t);
void Serial2_Send_String(char* s);
int putchar(int ch);            //printf() ���� �ʿ�
void print_2d1(double number);

#endif //define���� ������� ����