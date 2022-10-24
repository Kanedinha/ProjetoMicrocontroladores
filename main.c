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
#include <string.h>
#include "i2c.h"
#include "lcd4bits.h"
#include "keypad3x4.h"
#include "usart.h"

uint8_t alarme = OFF;
uint8_t tempo = 0;

unsigned char senha[]={};
unsigned char key;
unsigned char msg1[]={"Senha:"};
unsigned char msg2[]={"Alarme OFF"};


void atualizaLCD(unsigned char mensagem1[], unsigned char mensagem2[]){
	lcd_clear();
	lcd_gotoxy(0,0);
	lcd_puts(mensagem1);
	lcd_gotoxy(0,1);
	lcd_puts(mensagem2);
	lcd_puts(itoa(strlen(senha)));
}


void setup(){
	lcd4bits_inic();
	USART_Init(UBRRCTE);
	atualizaLCD(msg1, msg2);
}

void func(){
		for(int i=0; i<4; i++){
			key = read_keypad();
			if(key != '\0'){
				strcat(&senha, &key);
				strcat(&msg1, &senha);
				if(strlen(senha)==4){
					alarme = ON;
					senha = '\0';
					
				}
			}
		}
	USART_SendSRAM(key);
	USART_Transmit(13);
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
	}
}

