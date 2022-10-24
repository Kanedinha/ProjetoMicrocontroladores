#include "usart.h"
#define F_CPU 16000000UL
#include <avr/io.h>
// Rotina de inicialização da UART
void USART_Init(unsigned int ubrr)
{
	// Cte do Baud Rate UBRRCTE como param. entrada
	UBRR0H=(unsigned char)(ubrr>>8);
	
	UBRR0L=(unsigned char)ubrr;
	// USART0 Modo: Assincrono
	UCSR0A=0; // Taxa dobrada desabilitada
	// USART: 8 bits, 1 Stop, s/ Parid. Recep:On, Transm:On
	UCSR0B=(0<<RXCIE0)|(0<<TXCIE0)|(0<<UDRIE0)|(1<<RXEN0)|
	(1<<TXEN0)|(0<<UCSZ02)|(0<<RXB80)|(0<<TXB80);
	UCSR0C=(0<<UMSEL01)|(0<<UMSEL00)|(0<<UPM01)|(0<<UPM00)|
	(0<<USBS0)|(1<<UCSZ01)|(1<<UCSZ00)|(0<<UCPOL0);
}
// Rotina de Recepção pela UART
unsigned char USART_Receive()
{
	while(!(UCSR0A & (1<<RXC0))); // Espera pela recepção terminar
	return UDR0; // Lê dada do buffer e retorna
}
// Rotina de Transmissão pela UART
void USART_Transmit(unsigned char data)
{
	while(!(UCSR0A&(1<<UDRE0))); // Espera transmissão terminar
	UDR0 = data; // Coloca dado no buffer e envia
}
// Rotina de envio de string na SRAM via UART
void USART_SendSRAM(char *str)
{
	for(;*str!=0;str++) USART_Transmit(*str);
}
// Rotina de envio de string na Flash via UART
void USART_SendFlash(const char *str)
{
	for(;pgm_read_byte(&(*str))!=0;str++)
	USART_Transmit(pgm_read_byte(&(*str)));
}
// Rotina para liberar o buffer de recepção
void USART_Flush()
{
	unsigned char dummy;
	while(UCSR0A & (1<<RXC0)) dummy = UDR0;
}