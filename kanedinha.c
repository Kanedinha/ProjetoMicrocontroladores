/*
 * ProjetoAlarme.c
 *
 * Created: 20/10/2022 10:56:24
 * Author : Emers
 */ 

#define F_CPU 16000000UL
#define AddrPCF8574 0x4E
#define OFF 0
#define ON 1
#define FALSE 0
#define TRUE 1

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "i2c.h"
#include "lcd4bits.h"
#include "keypad3x4.h"

uint8_t alarme = OFF; // flag do alarme
uint8_t config = OFF; // flag da chave setup
uint8_t timeout = FALSE;
uint8_t senhaGrav = FALSE;

uint16_t conta = 1000;
uint8_t t_timeout = 80;
uint8_t fps = 5;
uint8_t length = 0; 
uint8_t tempo = 0;

unsigned char senha[4];
	
ISR(TIMER0_OVF_vect)
{
	TCNT0 = 100; 
	conta--;	
	t_timeout--;
	if(conta == 0) { 
		conta = 1000;
		timeout = TRUE;
	}	
}

void setup(){
	// periféricos
	lcd4bits_inic();
	I2C_Inic();
	
	// entrada do setup e saída dos leds
	DDRC |= (1 << DDC0)|(1 << DDC1);
	DDRC &= ~(1 << DDC2);
	PORTC |= (1 << DDC2);
	
	// temporizador para o alarme
	TCCR0A=(0<<COM0A1) | (0<<COM0A0) | (0<<COM0B1) | (0<<COM0B0) | (0<<WGM01) | (0<<WGM00);
	TCCR0B=(0<<WGM02) | (1<<CS02) | (0<<CS01) | (1<<CS00);
	TCNT0=100; // 10ms.
	TIMSK0=0x00; // temporizador inicia desligado 
	
	TCCR2A=(0<<COM2A1) | (0<<COM2A0) | (0<<COM2B1) | (0<<COM2B0) | (0<<WGM21) | (0<<WGM20);
	TCCR2B=(0<<WGM22) | (1<<CS22) | (0<<CS21) | (1<<CS20);
	TCNT2=100; // 10ms.
	TIMSK2=0x00; // temporizador inicia desligado
	
	sei();
}

void verificaConfig(){
	if(!(PINC & (1 << DDC2))){
		config = ON;
	}
	else {
		config = OFF;
	}
}

int leituraSensor(){
	unsigned int dado = 0;
	I2C_Inic();
	I2C_Start(); 
	I2C_WrAddr(AddrPCF8574+I2C_RD); 
	dado=I2C_GetNACK(); 
	I2C_Stop(); 
	_delay_ms(1000); 
	return dado;
}

void configSenha(){
	lcd_gotoxy(0,1);
	lcd_puts("Insira PWD 4-Dig");
	lcd_gotoxy(0,0);
	lcd_puts("Senha:      ");
	lcd_gotoxy(6,0);
	while(length < 4){
		if(!timeout){
		char key = read_keypad();
			if(key != '\0' && key != '*' && key != '#'){
				senha[lenbgth] = key;
				lcd_putschar(key);
				key = '\0';
				conta = 1000;
				TCNT0 = 100;
			}
		}
		else{
			lcd_gotoxy(0,1);
			lcd_puts("Setup Timeout   ");
		}
	}
	for(int i = 0;)
		eeprom_write(i,senha[i]);
	}
}

int main(void){
	setup();
	if(config == ON){
		TIMSK0 = 0x01;
		configSenha();
	}
	while(1){
		if(config == OFF){
			TIMSK0 = 0x00;
			
			
		}
	}
}

