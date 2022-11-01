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
uint8_t valida = TRUE;

uint16_t conta = 1000;
uint8_t tentativa = 0;
uint8_t fps = 5;
uint8_t length = 0;
uint8_t testeLength = 0;
uint8_t tempo = 0;

unsigned char senha[4];
unsigned char teste[4];

ISR(TIMER0_OVF_vect)
{
	TCNT0 = 100;
	conta--;
	
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

void leEEPROM(){
	for(int i = 0; i < 4; i++)
		senha[i] = EEPROM_read(i);
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
				senha[length] = key;
				length++;
				lcd_putchar(key);
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
	for(int i = 0; i < 4; i++)
		EEPROM_write	(i,senha[i]);
	lcd_gotoxy(0,1);
	lcd_puts("Senha na EEPROM ");
}

void ligaAlarme(){
	alarme = ON;
	tentativa = 0;
	PORTC |= (1 << DDC0);
	lcd_gotoxy(0,0);
	lcd_puts("Senha:      ");
	lcd_gotoxy(0,1);
	lcd_puts("Alarme ON       ");
}

void desligaAlarme(){
	alarme = OFF;
	tentativa = 0;
	PORTC &= ~(1 << DDC0);
	lcd_gotoxy(0,0);
	lcd_puts("Senha:      ");
	lcd_gotoxy(0,1);
	lcd_puts("Alarme OFF      ");
}

void testaSenha(){
	
		lcd_gotoxy(0,1);
		if(alarme == OFF && tentativa == 0){
			lcd_puts("Alarme OFF      ");
		}
		else if(alarme == ON && tentativa == 0){
			lcd_puts("Alarme ON       ");
		}
		else if(tentativa == 3){
			lcd_puts("invasão de PWD");
			PORTC |= (1 << DDC1);
		}
		
			
		lcd_gotoxy(0,0);
		lcd_puts("Senha:      ");
		lcd_gotoxy(6,0);
		while(testeLength < 4){
		char key = read_keypad();
			if(key != '\0' && key != '*' && key != '#'){
				teste[testeLength] = key;
				lcd_putchar(key);
				testeLength++;
				key = '\0';
			}
		}
		
		for(int i = 0; i < 4 && valida == TRUE; i++){
			valida = senha[i] == teste[i];
		}
		if(valida != TRUE){
			tentativa++;
			valida = TRUE;
			char aux = 48 + tentativa;
			lcd_gotoxy(0,1);
			lcd_puts("PWD NOK   ");
			lcd_putchar(aux);
			lcd_puts("/3");
		}
		else{
			if(alarme == OFF)
				ligaAlarme();
			else
				desligaAlarme();
			tentativa = 0;
			PORTC &= ~(1 << DDC1);
		}
		
		testeLength = 0;

}

int main(void){
	setup();
	leEEPROM();
	verificaConfig();
	if(config == ON){
		TIMSK0 = 0x01;
		configSenha();
	}
	while(1){
		if(config == OFF){
			TIMSK0 = 0x00;
			testaSenha();
		}
	}
}

