#include "alcd.h"
// Inicializa o LCD para 8 bits
void lcd_inic(void){
	UCSR0A = UCSR0B = UCSR0C = 0; // Para placas Arduino
	LCD_CNTR_DIR |= (1<<EN_LCD)|(1<<RS_LCD);
	LCD_CNTR = 0x00;
	LCD_DATA_DIR = 0xFF;
	LCD_DATA = 0x00;
	_delay_ms(16);
	EN_LOW();
	RS_CMD();
	lcd_cmd(CONFIG);
	EN();
	_delay_ms(5);
	EN();
	_delay_us(200);
	lcd_cmd(LCD_ON);
	lcd_cmd(ENTRY);
	lcd_cmd(CLR_LCD);
}
// Limpa o LCD
void lcd_clear(void){
	EN_HI();
	RS_CMD();
	LCD_DATA = CLR_LCD;
	EN_LOW();
	_delay_ms(5);
}
// Escreve caracter no LCD
void lcd_putchar(unsigned char dado){
	EN_HI();
	RS_DAT();
	LCD_DATA = dado;
	EN_LOW();
	_delay_ms(5);
}

// Escreve string de caracteres no LCD
void lcd_puts(uint8_t *msg){
	unsigned char i,s;
	for(i=0;(s=msg[i])!='\0';i++)
	lcd_putchar(s);
}
// Posiciona cursor do LCD
void lcd_gotoxy(unsigned char col,unsigned char lin){
	unsigned char base;
	if(lin==0 || lin==1){
		switch(lin){
			case 0: base=0x80;
			break;
			case 1: base=0xC0;
			break;
		default: base=0;}}
		else
		return;
		if(col>15) return;
		EN_HI();
		RS_CMD();
		LCD_DATA = base+col;
		EN_LOW();
		_delay_ms(5);
	}
	// Escreve string no LCD residente na flash
	void lcd_putsf(const uint8_t *msg){
		unsigned char i,s;
		for(i=0;(s=pgm_read_byte(&msg[i]))!=0;i++)
		lcd_putchar(s);
	}
	// Envia comando ao LCD
	void lcd_cmd(unsigned char dado){
		EN_HI();
		RS_CMD();
		LCD_DATA = dado;
		EN_LOW();
		_delay_ms(5);
	}