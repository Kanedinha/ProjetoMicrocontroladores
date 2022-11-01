#define F_CPU 16000000UL
#define AddrPCF8574 0x4E
#define OFF 0
#define ON 1
#define FALSE 0
#define TRUE 1
#define TEMPO 1000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "i2c.h"
#include "lcd4bits.h"
#include "keypad3x4.h"

uint8_t alarme = OFF; // flag do alarme
uint8_t config = OFF; // flag da chave setup
uint8_t sirene = OFF;
uint8_t timeout = FALSE;
uint8_t valida = TRUE;

uint16_t conta = 1000;
uint8_t tentativa = 0;
uint8_t length = 0;
uint8_t testeLength = 0;
uint16_t tempo = TEMPO;

unsigned char senha[4];
unsigned char teste[4];

ISR(TIMER0_OVF_vect)
{
	TCNT0 = 100;
	conta--;
	tempo--;	
	if(conta == 0) {
		conta = 1000;
		timeout = TRUE;
	}
	if(tempo == 0){
		tempo = TEMPO;
		alarme = ON;
		PORTC |= (1 << DDC0);
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
	I2C_Start();
	I2C_WrAddr(AddrPCF8574+I2C_RD);
	dado=I2C_GetNACK();
	I2C_Stop();
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
		EEPROM_write(i,senha[i]);
	lcd_gotoxy(0,1);
	lcd_puts("Senha na EEPROM ");
}

void ligaAlarme(){
	tempo = TEMPO;
	TCNT0 = 100;
	TIMSK0 = 0x01;
	tentativa = 0;
	lcd_gotoxy(0,0);
	lcd_puts("Senha:      ");
}

void desligaAlarme(){
	alarme = OFF;
	TIMSK0 = 0x00;
	tempo = TEMPO;
	tentativa = 0;
	sirene = OFF;
	PORTC &= ~(1 << DDC0);
	lcd_gotoxy(0,0);
	lcd_puts("Senha:      ");
}

void testaSenha(){
		char aux;
		lcd_gotoxy(0,0);
		lcd_puts("Senha:      ");
		while(testeLength < 4){
			// impressão no LCD
			lcd_gotoxy(0,1);
			if(alarme == ON && tentativa == 0){
				lcd_puts("Alarme ON       ");
			}
			else if(alarme == OFF && tentativa == 0){
				if(TIMSK0 == 0x00)
					lcd_puts("Alarme OFF      ");
				else{
					lcd_puts("Ativo em:");
					
					aux = tempo/1000 + 48;
					lcd_putchar(aux);
					aux = ((tempo/100)%10) + 48;
					lcd_putchar(aux);

					lcd_puts("     ");
				}
			}
			else if(sirene == ON){
				lcd_puts("invasão de PWD");
				PORTC |= (1 << DDC1);
			}
			else if(tentativa < 3 && tentativa > 0){
				aux = 48 + tentativa;
				lcd_gotoxy(0,1);
				lcd_puts("PWD NOK   ");
				lcd_putchar(aux);
				lcd_puts("/3");
			}
			
			lcd_gotoxy(6 + testeLength,0);
			// teste de senha
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
			if(tentativa < 3)
				tentativa++;
			if(tentativa == 3)
				sirene = ON;
			valida = TRUE;
		}
		else{
			if(alarme == OFF && sirene == OFF)
				ligaAlarme();
			else
				desligaAlarme();
			PORTC &= ~(1 << DDC1);
		}
		
		testeLength = 0;

}

void leSensor(){
	
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
			testaSenha();
		}
	}
}

