/*
 * ProjetoAlarme.c
 *
 * Created: 20/10/2022 10:56:24
 * Author : Emers
 */ 

#define F_CPU 16000000UL
#define OFF 0
#define ON 1

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "i2c.h"
#include "lcd4bits.h"
#include "keypad3x4.h"
#include "usart.h"

uint8_t alarme = OFF;
uint8_t tempo = 0;
uint8_t config = OFF;

unsigned char senha[]={};
unsigned char msg1[]={"Senha:          "};
unsigned char msg2[]={"Alarme OFF      "};

char *concatena(){
	
}

void atualizaLCD(unsigned char mensagem1[], unsigned char mensagem2[]){
	lcd_clear();
	lcd_gotoxy(0,0);
	lcd_puts(mensagem1);
	lcd_gotoxy(0,1);
	lcd_puts(mensagem2);
}

void setup(){
	lcd4bits_inic();
	
	DDRC |= (1 << DDC0)|(1 << DDC1);
	DDRC &= ~(1 << DDC2);
	atualizaLCD(msg1, msg2);
}

void verificaConfig(){
	if(!(PORTC & (1 << DDC2)){
		config = ON;
	}
}

char *teclado(){
	uint8_t key;
	key = read_keypad();
	if(key != '\0'){;
		strcat(&msg1, key);
		if(strlen(senha)==4){
			alarme = ON;
		}
	}	
	if(alarme == OFF){
		strcpy(msg2, "Alarme OFF");
	}
	else if(alarme == ON){
		strcpy(msg2, "Alarme ON");
	}
	
	atualizaLCD(msg1, msg2);
}

int main(void){
	setup();
	while (1) {
		func();
		_delay_ms(10);
	}
}

