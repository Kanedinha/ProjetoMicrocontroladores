#ifndef _ALCD_H
#define _ALCD_H
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
// Parâmetros do LCD para interface em 8 bits
#define CONFIG 0x38
#define LCD_ON 0x0E
#define ENTRY 0x06
#define CLR_LCD 0x01
#define HOME 0x02
#define LIN1 0xC0
#define DISP_ON 0x0C
#define DISP_OFF 0x08
// Pinos conectados ao LCD
#define LCD_DATA PORTD
#define LCD_CNTR PORTB
#define LCD_DATA_DIR DDRD
#define LCD_CNTR_DIR DDRB
#define EN_LCD PORTB0
#define RS_LCD PORTB1
#define EN_HI() LCD_CNTR |= (1<<EN_LCD);
#define EN_LOW() LCD_CNTR &=~(1<<EN_LCD);
#define RS_CMD() LCD_CNTR &=~(1<<RS_LCD);
#define RS_DAT() LCD_CNTR |= (1<<RS_LCD);
// Macro para gerar pulso de Enable (tudo na mesma linha)
#define EN() _delay_us(1);EN_HI();_delay_us(1);EN_LOW();_delay_us(45);
void lcd_inic(void);
void lcd_cmd(unsigned char dado);
void lcd_putchar(unsigned char dado);
void lcd_puts(uint8_t *msg);
void lcd_putsf(const uint8_t *msg);
void lcd_clear(void);
#endif