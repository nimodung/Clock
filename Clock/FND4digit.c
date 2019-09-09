/*
 * FND4digit.c
 *
 * Created: 2019-04-04 오후 4:01:50
 *  Author: Kim Hee Ram
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

#include "Timer.h"
#include "FND4digit.h"

char FND4digit_font[10] = {
	~(1 << FND_a | 1 << FND_b | 1 << FND_c | 1 << FND_d | 1 << FND_e | 1 << FND_f), //0
	~(1 << FND_c | 1 << FND_b), //1
	~(1 << FND_a | 1 << FND_b | 1 << FND_g | 1 << FND_e | 1 << FND_d), //2
	~(1 << FND_a | 1 << FND_b | 1 << FND_g | 1 << FND_c | 1 << FND_d), //3
	~(1 << FND_c | 1 << FND_b | 1 << FND_g | 1 << FND_f), //4
	~(1 << FND_a | 1 << FND_f | 1 << FND_g | 1 << FND_d | 1 << FND_d | 1 << FND_c), //5
	~(1 << FND_a | 1 << FND_f | 1 << FND_e | 1 << FND_d | 1 << FND_d | 1 << FND_g | 1 << FND_c), //6
	~(1 << FND_a | 1 << FND_b | 1 << FND_c), //7
	~(1 << FND_a | 1 << FND_b | 1 << FND_c | 1 << FND_d | 1 << FND_e | 1 << FND_f | 1 << FND_g), //8
	~(1 << FND_a | 1 << FND_b | 1 << FND_c | 1 << FND_d | 1 << FND_f | 1 << FND_g), //9
};

char FND4digit_digit[4] = {
	1 << digit1, 1 << digit2, 1 << digit3, 1 << digit4
};

char FND[4];

void FND4digit_init_shiftR(void)
{
	FND_COM_DDR |= 0b11110000;  //출력 설정
	FND_COM_PORT &= 0b00001111; //fnd 끄기
	FND_DATA_DDR |= 1 << RClk | 1 << SRClk | 1 << SER; //595 3bit 연결 // 3, 4,5번 연결
	FND_DATA_PORT |= 1 << RClk; // 0b00010000; //RClk : 1 //RClk을 상승 edge로 둬서 다른 데이터가 들어가지 않게
	
	FND_update_value(0);
	return;
}

void FND_shift_out(char data)
{
	FND_DATA_PORT &= ~(1 << RClk); 
	for(int i = 0; i < 8; i++) //data 한개씩 넣는걸 8번
	{
		FND_DATA_PORT &= ~(1 << SRClk);
		FND_DATA_PORT &= ~(1 << SER); //일단 0으로 떨어트려
		FND_DATA_PORT |= ((data >> i) & 1) << SER; //받아온 data 값을 0 아니면 1
		FND_DATA_PORT |= 1 << SRClk;//0b00100000; //상승 edge
	}
	FND_DATA_PORT |= 1 << RClk; //RClk 상승 edge
	return;  
}

void FND_update_time(int msec, char sec)
{
	FND[0] = FND4digit_font[msec /10 % 10];
	FND[1] = FND4digit_font[msec /100 % 10];
	FND[2] = FND4digit_font[sec % 10] & (~(1<< FND_p));
	FND[3] = FND4digit_font[sec /10 % 10];
	return;
}

void FND_clock(char sec, char min)
{
	FND[0] = FND4digit_font[sec % 10];
	FND[1] = FND4digit_font[sec /10 % 10];
	FND[2] = FND4digit_font[min % 10];
	FND[3] = FND4digit_font[min /10 % 10];
	return;
}
