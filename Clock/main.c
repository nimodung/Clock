/*
 * Clock.c
 *
 * Created: 2019-05-01 오후 1:40:18
 * Author : Kim Hee Ram
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

#include "FND4digit.h"
#include "Keypad.h"
#include "Timer.h"
#include "Speaker.h"

#define CLOCK_MODE 0
#define COOK_TIMER_MODE 1
#define STOP_WATCH_MODE 2
#define MODIFY_MODE 3
#define ALARM_MODE 4

#define CURSOR_MIN 0
#define CURSOR_HOUR 1

//btn define
#define BTN_MODE 1
#define BTN_SEC_INC 2
#define BTN_MIN_INC 3
#define BTN_COUNTDOWN 4

#define BTN_CLEAR 2
#define BTN_LAP 3
#define BTN_START_STOP 4

#define BTN_DEC 2
#define BTN_INC 3
#define BTN_CURSOR 4

#define BTN_ALARM_MIN_INC 2
#define BTN_ALARM_HOUR_INC 3
#define BTN_ALARM_ENABLE 4

void cook_timer_process(char key_value);
void clock_process(char key_value);
void stop_watch_process(char key_value);
void modify_process(char key_value);
void alarm_process(char key_value);

extern volatile char start_flag, w_flag, clear_flag, time_print_flag, countdown_flag, speakout_flag, stop_start_flag, long_key_keep_flag, alarm_enable_flag;
extern volatile char i, sec, min, hour, cook_sec, cook_min, stop_sec, stop_msec, alarm_min, alarm_hour;
extern int san_rabit_bell_tune[], san_rabit_bell_beat[], music_i;
extern char FND[4];

char mode = CLOCK_MODE, cursor; 
char long_key_flag;

int main(void)
{
	
    Timer0_init();
	Timer1_init_CTC_outA(); 
	clock_Keypad_init();
	FND4digit_init_shiftR();
	sei();
	
    while (1) 
    {
		/*if(speakout_flag) 
		{
			speakout_flag = 0;
			//Music_player(san_rabit_bell_tune, san_rabit_bell_beat);
			if(mode == COOK_TIMER_MODE) beep(3);
			else if(mode == STOP_WATCH_MODE) beep(1);
		}*/
		
		if(time_print_flag) //ISR에서 stop_msec가 0인 경우에도 time_print_flag가 1이 되기때문에 mode를 사용해서 출력 제어 
		{
			if(mode == CLOCK_MODE) FND_clock(min, hour);
			else if(mode == MODIFY_MODE) 
			{
				FND_clock(min, hour);
				/*if(sec%2){ //홀수 sec일때
					if(cursor) // == CURSOR_HOUR
					{
						FND[2] = 0b11111111;
						FND[3] = 0xff;
					}
					else
					{
						FND[0] = 0xff;
						FND[1] = 0xff;
					}
				}
				*/
				
			}
			else if(mode == STOP_WATCH_MODE) {
				if(w_flag) {
					FND_update_time(stop_msec, stop_sec);	
				}
			}
			else if(mode == COOK_TIMER_MODE) FND_clock(cook_sec, cook_min);
			else if(mode == ALARM_MODE) FND_clock(alarm_min, alarm_hour);
			
			time_print_flag = 0;
		}
		
		
		
		if(long_key_flag) 
		{
			if(Keyscan_sub()) 
			{
				_delay_ms(1);
				if(Keyscan_sub())
				{
					if(speakout_flag) {
						speakout_flag = 0; music_i = 0; OCR1A = 0;
					}
				
					if(mode == CLOCK_MODE) clock_process(Keyscan_sub());
					else if(mode == MODIFY_MODE) modify_process(Keyscan_sub());
					else if(mode == COOK_TIMER_MODE) cook_timer_process(Keyscan_sub());
					else if(mode == STOP_WATCH_MODE) stop_watch_process(Keyscan_sub());
					else if(mode == ALARM_MODE) alarm_process(Keyscan_sub());
					
					long_key_flag = 0;
				}
				
			}
		}
		else 
		{
			if(Keyscan_sub() == 0) 
			{
				long_key_flag = 1;
			}
		
		}
		
		//MODE KEY longkey로 입력받기
		if(long_key_keep_flag) {
			long_key_keep_flag = 0;
			if(Keyscan_sub() == BTN_MODE) {
				switch(mode)
				{
					case CLOCK_MODE :
					mode = MODIFY_MODE;
					time_print_flag = 1;
					break;
					case MODIFY_MODE :
					cursor = 0;
					mode = COOK_TIMER_MODE;
					time_print_flag = 1;
					break;
					case COOK_TIMER_MODE :
					countdown_flag = 0;
					mode = STOP_WATCH_MODE;
					time_print_flag = 1;
					break;
					case STOP_WATCH_MODE :
					stop_start_flag = 0;
					clear_flag = 1;
					mode = ALARM_MODE;
					time_print_flag = 1;
					break;
					case ALARM_MODE :
					mode = CLOCK_MODE;
					time_print_flag = 1;
					break;
					default: break;
				}
			}
			
		}
		
	}
}

void cook_timer_process(char key_value)
{
	switch(key_value)
	{
		/*case BTN_MODE :
			countdown_flag = 0;
			mode = STOP_WATCH_MODE;
			time_print_flag = 1;
			//FND_update_time(stop_msec, stop_sec);
			break;*/
		case BTN_SEC_INC :
			cook_sec++;
			if(cook_sec >= 60) cook_sec = 0;
			time_print_flag = 1;
			//FND_clock(cook_sec, cook_min);
			break;
		case BTN_MIN_INC :
			cook_min++;
			if(cook_min >= 60) cook_min = 0;
			time_print_flag = 1;
			//FND_clock(cook_sec, cook_min);
			break;
		case BTN_COUNTDOWN : 
			countdown_flag = 1;
			break;
		default: break;
	}
	return;
}

void clock_process(char key_value)
{
	
	/*if(key_value == BTN_MODE)
	{
		mode = MODIFY_MODE;
		time_print_flag = 1;
		
		//FND_clock(cook_sec, cook_min);
	}*/
	return;
}

void stop_watch_process(char key_value) {
	switch(key_value)
	{
		
		/*case BTN_MODE :
			stop_start_flag = 0;
			clear_flag = 1;
			mode = CLOCK_MODE;
			time_print_flag = 1;
			//FND_clock(sec, min);
			break;*/
		case BTN_CLEAR : //clear //clear 버튼이 따로 있기 때문에 굳이 flag 쓰지 않아도 된다
			clear_flag = 1;
			stop_start_flag = 0;
			w_flag = 0;
			break;
		case BTN_LAP : //lap
			stop_start_flag = 1;
			if(w_flag == 0)w_flag = 1;
			else w_flag = 0;
			break;
		case BTN_START_STOP :
			if(stop_start_flag == 0)stop_start_flag = 1;
			else stop_start_flag = 0;
			w_flag = 1;
			time_print_flag = 1;
			beep(1);
			break;
		default: break;
	}
	return;
}


void modify_process(char key_value) {
	
	switch(key_value)
	{
		/*case BTN_MODE : //모드 변경
			cursor = 0;
			mode = COOK_TIMER_MODE;
			FND_clock(cook_sec, cook_min);
			break;
		*/
		case BTN_DEC : //감소
			if(cursor) { // == CURSOR_HOUR
				hour--;
				if(!hour) hour = 12; //hour가 0이되면 12로 변경
			}
			else { // == CURSOR_MIN
				if(!min) min = 59;
				else min--;
			}
			time_print_flag = 1;
			break;
		case BTN_INC : //증가 
			if(cursor) { // == CURSOR_HOUR
				hour++;
				if(hour >= 13) hour = 1; //hour가 13이되면 1시
			}
			else { // == CURSOR_MIN
				min++;
				if(min >= 60) min = 0;
			}
			time_print_flag = 1;
			break;
		case BTN_CURSOR : //커서 이동
			if(cursor) cursor = 0; //CURSOR_MIN
			else cursor = 1; //CURSOR_HOUR
			break;
		default: break;
	}
	return;
}

void alarm_process(char key_value)
{
	switch(key_value) 
	{
		case BTN_ALARM_MIN_INC :
			alarm_min++;
			if(alarm_min >= 60) {
				alarm_min = 0;
			}
			time_print_flag = 1;
			break;
		case BTN_ALARM_HOUR_INC :
			alarm_hour++;
			if(alarm_hour >= 13) {
				alarm_hour = 0;
			}
			time_print_flag = 1;
			break;
		case BTN_ALARM_ENABLE :
			if(alarm_enable_flag) alarm_enable_flag = 0;
			else alarm_enable_flag = 1;
			break;
		default: break;	
	}
	
	return;
}