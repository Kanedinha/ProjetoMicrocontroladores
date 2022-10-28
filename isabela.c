
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include "keypad3x4.h"
#include "lcd4bits.h"
#include "eeprom.h"
#include "i2c.h"
#include <util/delay.h>

#define FALSE 0;
#define TRUE 1;
#define TstBit_Reg(Reg,Bit) (Reg&(1<<Bit))

uint8_t key, i=0;
uint16_t conta=1000;
uint8_t timeout= FALSE;

unsigned char senha[4];

ISR(TIMER0_OVF_vect)
{
	TCNT0 = 100;
	conta--;
	if(conta == 0) {
		conta = 1000;
		timeout = TRUE;
	}
	
	
}



void prog_senha(){
	
	lcd4bits_inic();
	lcd_gotoxy(0,1);
	lcd_puts("Entre PWD 4-Dig");
	lcd_gotoxy(0,0);	
	lcd_puts("Senha:");
	TIMSK0 = 0x01;
	while(i<4){
		// Varredura do teclado
		key=read_keypad();
		if(!timeout){
			if(key != '\0' && key != '*' &&  key != '#'){ // Se tecla pression. envia ao LCD
				senha[i]=key;
				lcd_putchar(key);
				key = '\0';
				conta = 1000;
				if(i==3){
					for(int i=0; i<4;i++){
						EEPROM_write(i,senha[i]);
					}
					lcd_gotoxy(0,1);
					lcd_puts("Senha na EEPROM");
				}
				i++;
			}
		}
		
		else{
			lcd_gotoxy(0,1);
			lcd_puts("setup timeout   ");
			_delay_ms(1000);
			//senha padrao
			for(int i=0; i<4;i++){
				EEPROM_write(i,'1');
			}
			lcd_clear();
			lcd_gotoxy(0,1);
			lcd_puts("Senha padrao");
			
			return;
		}
	}
}

void inc_setup(){
	
	lcd4bits_inic();
	I2C_Inic();
	
	DDRC &= ~(1 << DDC2);
	PORTC |= (1 << DDC2);
	
	TCCR0A=(0<<COM0A1) | (0<<COM0A0) | (0<<COM0B1) | (0<<COM0B0) | (0<<WGM01) | (0<<WGM00);
	TCCR0B=(0<<WGM02) | (1<<CS02) | (0<<CS01) | (1<<CS00);
	TCNT0=100; // 10ms.
	TIMSK0=0x01; // temporizador inicia desligado
	sei();
}


	
int main(void){
	
	inc_setup();
	if(!TstBit_Reg(PINC,DDC2)){
		i=0;
		prog_senha();
	}
	while(1){
		
		if(TstBit_Reg(PINC,DDC2)){
			
		}
	}
}
