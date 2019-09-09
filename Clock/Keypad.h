/*
 * Keypad.h
 *
 * Created: 2019-04-17 오전 11:05:04
 *  Author: user
 */ 


#ifndef KEYPAD_H_
#define KEYPAD_H_

#define Keypad_PORT_DDR DDRD
#define Keypad_PORT PORTD

#define Keypad_PIN_DDR DDRD
#define Keypad_PIN_PORT PORTD
#define Keypad_PIN PIND

char Keyscan_sub(void);
void clock_Keypad_init(void);



#endif /* KEYPAD_H_ */
