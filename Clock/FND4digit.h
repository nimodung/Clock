/*
 * FND4digit.h
 *
 * Created: 2019-04-05 오후 2:52:22
 *  Author: Kim Hee Ram
 */ 


#ifndef FND4DIGIT_H_
#define FND4DIGIT_H_

#define FND_COM_DDR DDRD
#define FND_COM_PORT PORTD
#define FND_DATA_DDR DDRB
#define FND_DATA_PORT PORTB

#define FND_e 3
#define FND_d 2
#define FND_p 1
#define FND_c 0
#define FND_g 7
#define FND_a 6
#define FND_f 5
#define FND_b 4

#define digit4 7
#define digit3 6
#define digit2 5
#define digit1 4

#define RClk PORTB4
#define SRClk PORTB5
#define SER PORTB3

void FND4digit_main();
void FND_update_time(int msec, char sec);
void FND_clock(char sec, char min);
void FND4digit_init_shiftR(void);
void FND_shift_out(char data);


#endif /* FND4DIGIT_H_ */
