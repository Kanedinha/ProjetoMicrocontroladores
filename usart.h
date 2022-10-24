#ifndef _USART_H
#define _USART_H
#define BAUD 9600
#define UBRRCTE F_CPU/16/BAUD-1 // Taxa dobrada desabilit.
#define UBRRCTE F_CPU/8/BAUD-1 // Taxa dobrada habilitada
// Rotina de inicialização da UART
void USART_Init(unsigned int ubrr);
// Rotina de Recepção pela UART
unsigned char USART_Receive();
// Rotina de Transmissão pela UART
void USART_Transmit(unsigned char data);
// Rotina de envio de string na SRAM via UART
void USART_SendSRAM(char *str);
// Rotina de envio de string na Flash via UART
void send_USART_Flash(const char *str);
// Rotina para liberar o buffer de recepção
void USART_Flush();
#endif