/*
 * Timer.c
 *
 * Created: 2019-04-10 오전 11:04:02
 *  Author: Kim Hee Ram
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h> // interrupt 사용하기 위함
#include "FND4digit.h"
#include "Timer.h"

#define CLOCK_MODE 0
#define COOK_TIMER_MODE 1
#define STOP_WATCH_MODE 2
#define MODIFY_MODE 3
#define ALARM_MODE 4

//FND4digit
extern char FND4digit_digit[4];
extern char FND[4];
extern char mode , cursor, long_key_flag;
extern int husuabi_bell_tune[], husuabi_bell_beat[], san_rabit_bell_tune[],san_rabit_bell_beat[];

//전역 변수는 초기화 안하면 0으로 초기화된다
volatile char start_flag=1, w_flag, clear_flag, time_print_flag, countdown_flag, speakout_flag, stop_start_flag, next_beat_flag = 1, long_key_keep_flag, alarm_enable_flag;		
volatile int msec = 0, stop_msec = 0, music_i, long_key_msec = 0;				
volatile char i = 0, sec = 0, min = 0, hour = 0, cook_sec = 0, cook_min = 0, stop_sec = 0, stop_min = 0, alarm_min, alarm_hour;			
															//volatile : 워킹레지스터에 선언하지말고 RAM에다가 할당하라는 의미(휘발성)
															//전역변수 선언시에 많이 사용
volatile int beat, interval;																	

//isr은 가급적 빨리 실행해야 되기 때문에 딜레이 x, 함수호출x ...
ISR(TIMER0_COMPA_vect)						//interrupt 걸리면 실행하는 내용 //Interrupt Service Routine
{	
	if(start_flag) msec++;
	if(stop_start_flag) stop_msec++;
	
	//mode 변경 : key를 1초 누르면 mode 변경 모드로 
	if(!long_key_flag)
	{
		long_key_msec++;
		if(long_key_msec >= 1000)
		{
			long_key_msec = 0;
			long_key_keep_flag = 1;
		}
	}
	else long_key_msec = 0;
	
	
	//speaker
	if(speakout_flag) {
		if(!interval) {
			if(next_beat_flag) {
				next_beat_flag = 0;
				
				if(san_rabit_bell_tune[music_i]) OCR1A = F_CPU / 2 / 8 / san_rabit_bell_tune[music_i];
				else OCR1A = 0;
				beat = san_rabit_bell_beat[music_i];
				music_i++;
				if(san_rabit_bell_tune[music_i] == '/0')
				{
					speakout_flag = 0;
					OCR1A = 0;
				}
			}
		
			beat--;
			if(beat == 0){
				
				next_beat_flag = 1;
				interval = 10;		
			}
		}
		else {
			OCR1A = 0;
			interval--;			
		}
		
	}
	
	
	
	//COOK_TIMER
	if(countdown_flag) {
		if(msec >= 1000)
		{
			if(cook_sec > 0)
			{
				cook_sec--;
				time_print_flag = 1;
			}
			else if(cook_sec == 0) {
				if(cook_min == 0) 
				{
					countdown_flag = 0;
					speakout_flag = 1;
				}
				else {
					cook_sec = 60;
					cook_min--;
				} 
			}
		}
	}
	
	//CLOCK
	if(msec >= 1000)
	{
		msec = 0;
		sec++; //fnd 출력이 바뀌는 시점
		if(sec >= 60)
		{
			sec = 0;
			min++;
			if(min >= 60) {
				min = 0;
				hour++;
				if(hour >= 12) hour = 0;
			}
			
			if((hour == alarm_hour) && (min == alarm_min)) 
			{
				speakout_flag = 1; 
			}
			time_print_flag = 1;
		}
		//if(w_flag)
		
		//if(mode == 0) FND_clock(sec, min);
	}
	

	//STOP_WATCH
	
	if(stop_msec >= 1000)
	{
		stop_msec = 0;
		stop_sec++;
		if(stop_sec >= 60)
		{
			stop_sec = 0;
			stop_min++;
			if(stop_min >= 60) stop_min = 0;
		}
			
	}
	
	if(!(stop_msec % 10) && mode == STOP_WATCH_MODE) //ISR에 오래 머무르면 안되기때문에 time_flag를 사용해서 main문에서 시간 출력
	{
		time_print_flag = 1;
	}
	
	
	
	if(clear_flag)
	{
		clear_flag = 0;
		time_print_flag = 1;
		w_flag = 1;
		stop_msec = 0;
		stop_sec = 0;
		stop_min = 0;
	}


	
	//FND를 실제로 출력시키는 
	i++;
 	
 	if(i >=4) i = 0;
	

	//끄고 데이터 주고 끄고
	FND_COM_PORT &= 0b00001111; //상위 4비트 끄고
	FND_shift_out(FND[i]);
	
	if(mode == COOK_TIMER_MODE){
		FND[2] &= (~(1 << FND_p));
		FND_COM_PORT |= FND4digit_digit[i];
	}
	if(mode == MODIFY_MODE) {
		FND[2] &= (~(1 << FND_p));
		if(msec <= 500){
			
			FND_COM_PORT |= FND4digit_digit[i];
		}
		else {
			
			if(cursor){
				if(i < 2) {
					 FND_COM_PORT |= FND4digit_digit[i];
				}
			}
			else{
				if(i >= 2) {
					FND_COM_PORT |= FND4digit_digit[i];
				}
			}
			
		}
	}
	
	else if(mode == CLOCK_MODE) {
		
		if(msec <= 500){
			FND[2] &= (~(1 << FND_p));
			
		}
		else {
			FND[2] |= 1 << FND_p;
			
		}
		FND_COM_PORT |= FND4digit_digit[i];
	}
	
	else if(mode == ALARM_MODE) {
		if(alarm_enable_flag) FND[2] &= (~(1 << FND_p));
		else FND[2] |= 1 << FND_p;
		
		FND_COM_PORT |= FND4digit_digit[i];
	}
	else FND_COM_PORT |= FND4digit_digit[i];//< FND_shift_out보다 먼저하면 이전 데이터가 출력돼서 정확한 출력이 나오지 않음
	//FND_DATA_PORT = FND[i];
	
	
} 

void Timer0_init(void)
{
	//타이머 초기화
	TCCR0A |= 1 << WGM01;					//0b00000010;  //CTC Mode
	TCCR0B |= (1 << CS00 | 1 << CS01);		//0b00000011;  //64분주 사용(하위 3bit : 011)
	OCR0A = 249;							//비교할 값 => (16,000,000 / 64) / 1000 = 250 (0 ~ "249")			//1ms 주기
	TIMSK0 |= 1 << OCIE0A;                  //0b00000010; //어떤 interrupt 걸리게 할건지 정하는	// OC0A 비교 매치 인터럽트 활성화
	
	return;
}


void Timer1_init_CTC_outA(void) //A 출력을 쓰기위한 초기화 함수
{
	DDRB |= 1 << PORTB1;
	TCCR1A |= 1 << COM1A0; //TOGGLE
	TCCR1B |= 1 << WGM12 | 1 <<  CS11;  //CTC 모드//64분주
	OCR1A = 0; // 소리 끄기
	
	//OCR0A = 30; //4000Hz
	//OCR0A = 63; // 2000Hz
	//OCR0A = 124;							//(250 / 2) - 1 //한 주기를 1msec
	//OCR0A = 249;							//한 주기 2msec //500Hz
	//250Hz를 만들고싶다면? => OCR0A를 늘리면 오버플로우(8bit) => 분주를 늘린다
	//CS0x (분주)랑 OCR값을 이용해서 주기, 주파수 조정
	//TIMSK0 |= 1 << OCIE0A;                  //0b00000010; //어떤 interrupt 걸리게 할건지 정하는	// OC0A 비교 매치 인터럽트 활성화
	
	return;
}
