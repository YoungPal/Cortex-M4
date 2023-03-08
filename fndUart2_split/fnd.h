#ifndef __FND_H__ //중복 방지를 위해 반드시 선언 
#define __FND_H__ //define은 대문자 언더바

void FND_init();
void display_fnd( int N );
void display_digit(int positon,int number);
static void Delay(const uint32_t Count);


#endif 