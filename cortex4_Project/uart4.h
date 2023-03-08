#ifndef __UART4_H__ //중복 방지를 위해 반드시 선언 
#define __UART4_H__ //define은 대문자 언더바

void UART4_init();
void Serial4_Event();
void Serial4_Send(unsigned char t);
void Serial4_Send_String(char* s);


#endif //define부터 여기까지 정의