#ifndef _LCD4BITS_H
#define _LCD4BITS_H
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
// Portas e pinos conectados ao LCD
#define NIB_LSB_MSB 0 // Px0..3 = 0, Px4..7 = 1
#define DATA_LCD PORTB // Porta associada aos dados do LCD
#define DATA_DD DDRB // Direção de dados para dados do LCD
#define CNTR_LCD PORTB // Porta associada a pinos EN e RS
#define CNTR_DD DDRB // Direção de dados pinos EN e RS
#define RS_LCD PORTB4 // Pino associado ao sinal de RS do LCD
#define EN_LCD PORTB5 // Pino associado ao sinal de EN do LCD
// Parâmetros de configuração do LCD
#define INIC 0x03 // Inicializa o controlador LCD
#define MODE 0x02 // Interface em 4-Bits
#define CONFIG 0x28 // 2 linhas, matriz caracter 7x5 ptos
#define CURS_ON 0x0E // LCD ON, Cursor do LCD ON
#define CURS_OFF 0x0C // LCD ON, Cursor do LCD OFF
#define CURS_PIS 0x0D // LCD ON, Cursor piscante
#define LCD_OFF 0x08 // Desliga LCD
#define CLR_LCD 0x01 // Limpa o LCD todo retorno com do cursor
#define ENTRY 0x06 // Cursor autoincrementado à direita
#define HOME 0x02 // Posiciona no cursor na posição 0,0
#define LIN0 0x80 // Posiciona cursor na linha 0
#define LIN1 0xC0 // Posiciona cursor na linha 1
// Macros para setar e resetar sinais EN e RS
#define EN_HI() (CNTR_LCD |= (1<<EN_LCD));
#define EN_LOW() (CNTR_LCD &= ~(1<<EN_LCD));
#define RS_DAT() (CNTR_LCD |= (1<<RS_LCD));
#define RS_CMD() (CNTR_LCD &= ~(1<<RS_LCD));
// Macro para gerar pulso de Enable (tudo na mesma linha)
#define EN() _delay_us(1);EN_HI();_delay_us(1);EN_LOW();_delay_us(45);
// Protótipos das Funções
void lcd4bits_inic(void);
void lcd_cmd(unsigned char dado);
void lcd_putchar(unsigned char dado);
void lcd_puts(unsigned char *s);
void lcd_clear();
void nibble2port(unsigned char dado);
#endif