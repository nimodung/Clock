/*
 * Keypad.c
 *
 * Created: 2019-04-16 오후 12:06:32
 *  Author: Kim Hee Ram
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <stdio.h>

#include "Keypad.h"

void clock_Keypad_init(void)
{
	Keypad_PIN_DDR &= 0b11110000; //포트B 하위 4bit 입력으로 설정
	Keypad_PIN_PORT |= 0b00001111; //내부 풀업저항 사용하겠다는 설정
	
	return;
}

char Keyscan_sub(void)
{
	if(!(Keypad_PIN & 0b00000001)) return 1; //키가 눌렸을때
	else if(!(Keypad_PIN & 0b00000010)) return 2;
	else if(!(Keypad_PIN & 0b00000100)) return 3;
	else if(!(Keypad_PIN & 0b00001000)) return 4;
	else return 0;
}


