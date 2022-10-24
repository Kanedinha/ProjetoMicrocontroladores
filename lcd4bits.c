#include "lcd4bits.h"
// Inicializa o LCD em 4-bits
void lcd4bits_inic(void)
{
	#if(!NIB_LSB_MSB)
	DATA_DD |= 0x0F; // Px0..3 pinos de saida
	#else
	DATA_DD |= 0xF0; // Px4..7 pinos de saida
	#endif
	CNTR_DD |= (1<<EN_LCD)|(1<<RS_LCD); // EN e RS pinos saida
	// UCSR0B=0; // Arduino: desabilitar UART se usar PortD
	_delay_ms(15);
	RS_CMD();
	EN_LOW();
	_delay_ms(20); // Espera um tempo para Vcc atingir 4,5V
	#if(!NIB_LSB_MSB)
	DATA_LCD = (DATA_LCD & 0xF0)|INIC;
	#else
	DATA_LCD = (DATA_LCD & 0x0F)|INIC<<4;
	#endif
	EN();
	_delay_ms(5);
	EN();
	_delay_us(200);
	EN();
	#if(!NIB_LSB_MSB)
	DATA_LCD = (DATA_LCD & 0xF0)|MODE;
	#else
	DATA_LCD = (DATA_LCD & 0x0F)|MODE<<4;
	#endif
	EN();
	lcd_cmd(0x28); // Interface 4 bits 2 linhas
	lcd_cmd(0x08); // Desliga LCD
	lcd_cmd(0x01); // Limpa LCD
	lcd_cmd(0x0c); // LCD ON, Cursor inativo e nÿo piscante
	lcd_cmd(0x80); // Cursor na linha 0 coluna 0
}
// Limpa o LCD
void lcd_clear()
{
	unsigned char dado;
	dado=CLR_LCD;
	RS_CMD();
	nibble2port(dado);
	EN();
	dado<<=4;
	nibble2port(dado);
	EN();
	_delay_ms(2);
}
// Posiciona o cursor do LCD em coluna,linha
void lcd_gotoxy(unsigned char col,unsigned char lin)
{
	unsigned char base,dado;
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
		dado = base+col;
		RS_CMD();
		nibble2port(dado);
		EN();
		dado<<=4;
		nibble2port(dado);
		EN();
	}
	// Escreve string no LCD
	void lcd_puts(unsigned char *s)
	{
		unsigned char i;
		for(i=0;s[i]!='\0';i++)
		lcd_putchar(s[i]);
	}
	// Escreve caracter no LCD
	void lcd_putchar(unsigned char dado)
	{
		RS_DAT();
		nibble2port(dado);
		EN();
		dado<<=4;
		nibble2port(dado);;
		EN();
	}
	// Escreve comando no LCD
	void lcd_cmd(unsigned char dado)
	{
		RS_CMD();
		nibble2port(dado);
		EN();
		dado<<=4;
		nibble2port(dado);
		EN();
		_delay_ms(2);
	}
	void nibble2port(unsigned char dado)
	{
		#if(!NIB_LSB_MSB)
		DATA_LCD = (DATA_LCD & 0xF0)|(dado>>4);
		#else
		DATA_LCD = (DATA_LCD & 0x0F)|(dado);
		#endif
	}