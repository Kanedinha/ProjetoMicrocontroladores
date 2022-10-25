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

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "i2c.h"
#include "lcd4bits.h"
#include "keypad3x4.h"

uint8_t alarme = OFF; // flag do alarme
uint8_t config = OFF; // flag da chave setup

uint8_t conta = 1000;
uint8_t length = 0; 
uint8_t tempo = 0;

unsigned char senha[] = {};
unsigned char msg1[] = {"Senha:          "};
unsigned char msg2[] = {"Alarme OFF      "};

void atualizaLCD(unsigned char mensagem1[], unsigned char mensagem2[]){
	lcd_clear();
	lcd_gotoxy(0,0);
	lcd_puts(mensagem1);
	lcd_gotoxy(0,1);
	lcd_puts(mensagem2);
}

ISR(TIMER0_OVF_vect)
{
	TCNT0=100; 
	conta--; 	
	if(conta==0) { 
		conta=1000;
		length = 0;
		for(int i = 0; i < 4; i++){
			senha[i] = '\0';
		} 
		*msg2 = "Setup timeout";
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
	sei();
	
	atualizaLCD(msg1, msg2);

}

void verificaConfig(){
	if(!(PORTC & (0 << DDC2))){
		config = 1;
	}
	else {
		config = 0;
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

void recebeSenha(){
	char key = read_keypad();
	if(length == 4){
		//armazena na eeprom
		*msg1 = NULL;
		*msg2 = NULL;
		*msg1 = "Senha:";
		*msg1 = strcat((char*)msg1, (char*)senha);
		*msg2 = "Senha na EEPROM";
		atualizaLCD(msg1,msg2);
	}
	if(key != '\0'){
		senha[length] = key;
		key = '\0';
		length++;
		*msg1 = strcat((char*)msg1, (char*)senha);
		*msg2 = "insira a senha";
		atualizaLCD(msg1,msg2);
	}
	
}

int main(void){
	setup();
	while(1){
		verificaConfig();
		while(config == ON){
			TIMSK0 = 0x01;
			recebeSenha();
			verificaConfig();
		}
		while(config == OFF){
			
			verificaConfig();
		}
	}
}

