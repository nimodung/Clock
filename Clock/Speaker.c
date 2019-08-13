/*
 * Speaker.c
 *
 * Created: 2019-04-29 오후 3:18:18
 *  Author: Kim Hee Ram
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include "Timer.h"
#include "Speaker.h"

int doremi[] = {DO_05, RE_05, MI_05, PA_05, SOL_05, LA_05, SI_05, DO_06};

int school_bell_tune[] = {
	SOL_05, SOL_05, LA_05, LA_05, SOL_05, SOL_05, MI_05,
	SOL_05, SOL_05, MI_05, MI_05, RE_05,
	SOL_05, SOL_05, LA_05, LA_05, SOL_05, SOL_05, MI_05,
	SOL_05, MI_05, RE_05, MI_05, DO_05, '/0'
};


int school_bell_beat[] = {
	BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_2,
	BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1,
	BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_2,
	BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1,
};

int san_rabit_bell_tune[] = {
	SOL_05, MI_05, MI_05, SOL_05, MI_05, DO_05, RE_05, MI_05, RE_05, DO_05, MI_05, SOL_05,
	DO_06, SOL_05, DO_06, SOL_05, DO_06, SOL_05, MI_05, SOL_05, RE_05, PA_05, MI_05, RE_05, DO_05, '/0'
};

int san_rabit_bell_beat[] = {
	BEAT_1_2, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_2, 
	BEAT_1_2, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_2,
	BEAT_3_8, BEAT_1_8, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_2, 
	BEAT_1_2, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_2
};

int san_rabit_bell_beat_16[] = {
	BEAT_1_2, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_2,
	BEAT_1_2, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_2,
	BEAT_3_8, BEAT_1_8, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_2,
	BEAT_1_2, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_2
};

int husuabi_bell_tune[] = {
	DO_05, RE_05, MI_05, PA_05, SOL_05, SOL_05, LA_05, LA_05, SOL_05,
	LA_05, DO_06, SI_05, LA_05, SOL_05, PA_05, MI_05, RE_05, DO_05, 0,
	DO_06, DO_06, DO_06, DO_06, SOL_05, 0,  DO_06, DO_06, DO_06, DO_06, SOL_05,0,
	DO_06, SI_05, LA_05, SOL_05, PA_05, MI_05, SOL_05, RE_05, 0,
	DO_05, RE_05, MI_05, PA_05, SOL_05, SOL_05, LA_05, LA_05, SOL_05,
	LA_05, DO_06, SI_05, LA_05, SOL_05, PA_05, MI_05, RE_05, DO_05, 0, '/0'
};

int husuabi_bell_beat[] = {
	BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_2,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4,
	 BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_2,
	 BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4
};

int Speakr_main()
{
	Timer1_init_CTC_outA();	
	//Timer0_init_CTC_outA();
while(1)
	{
		
		//Music_player(school_bell_tune, school_bell_beat);
		//Music_player(san_rabit_bell_tune, san_rabit_bell_beat);
		Music_player(san_rabit_bell_tune, san_rabit_bell_beat_16);
		//Music_player(husuabi_bell_tune, husuabi_bell_beat);
		//beep(3);
		//siren(3);
		//_delay_ms(1000);
		
		/*
		for(int i = 0 ; i < 24; i++)
		{
			OCR0A = F_CPU / 2 / 64 /school_bell_tune[i]; //F_CPU / 2 / 분주 / Hz
			delay_ms(school_bell_beat[i]);
			OCR0A = 0; //소리 끄기
			_delay_ms(20);
		}
		*/
	}
	
	return 0 ;
}

void Music_player(int *tune, int *beat)
{
	while(*tune != '/0') {
		if(*tune)OCR1A = F_CPU / 2 / 8 / *tune; //F_CPU / 2 / 분주 / Hz
		else OCR1A = 0;
		delay_ms(*beat);
		tune++;
		beat++;
		OCR1A = 0; //소리 끄기
		_delay_ms(20);
	}
	return;
}

void delay_ms(int ms) //
{
	while(ms--) _delay_ms(1);	
}

void beep(char repeat) //매개변수 : 반복횟수
{
	for(int i = 0; i < repeat; i++)
	{
		OCR1A = 500;
		_delay_ms(200);
		OCR1A = 0;
		_delay_ms(200);
	}
	return;
}

void siren(char repeat)
{
	
	OCR1A = 900; //900~1900
	for(int j = 0; j < repeat; j++) 
	{
		for(int i = 0; i < 100; i++)
		{
			OCR1A += 10;
			_delay_ms(20);
		}
		for(int i = 0; i < 100; i++)
		{
			OCR1A -= 10;
			_delay_ms(20);
		}
	}
	return;
}