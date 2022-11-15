#define F_CPU 16000000UL
#include "keypad3x4.h"
#include <avr/io.h>
#include <util/delay.h>
// Vetor de caracteres ascii na matriz de teclas 3x4 e caracter null
char ascii[13] = {'1','2','3','4','5','6','7','8','9','*','0','#','\0'};
// Configura linhas como pinos de saída e colunas como pinos de entrada
void row_output_column_input()
{
	// Seta linhas como saída
	KP_DDR |= (1<<ROW0)|(1<<ROW1)|(1<<ROW2)|(1<<ROW3);
	// Seta colunas como entrada
	KP_DDR &= ~((1<<COL0)|(1<<COL1)|(1<<COL2)|(1<<COL3));
	// Pullups nas entradas
	KP_PORT = (1<<COL0)|(1<<COL1)|(1<<COL2)|(1<<COL3);
}
// Configura linhas como pinos de entrada e colunas como pinos de saída
void row_input_column_output()
{
	// Seta linhas como entradas
	KP_DDR &= ~((1<<ROW0)|(1<<ROW1)|(1<<ROW2)|(1<<ROW3));
	// Seta colunas como saídas
	KP_DDR |= (1<<COL0)|(1<<COL1)|(1<<COL2)|(1<<COL3);
	// Pullups nas entradas
	KP_PORT = (1<<ROW0)|(1<<ROW1)|(1<<ROW2)|(1<<ROW3);
}
// Retorna index da tecla
unsigned char scan_keys()
{
	uint8_t key_index=0;
	// Verifica as colunas
	if(!(KP_PIN & (1<<COL0))) key_index = 0; // Se PD4=0 => COL0
	if(!(KP_PIN & (1<<COL1))) key_index = 1; // Se PD5=0 => COL1
	if(!(KP_PIN & (1<<COL2))) key_index = 2; // Se PD6=0 => COL2
	if(!(KP_PIN & (1<<COL3))) key_index = 3; // Se PD7=0 => COL3
	// Verica as linhas
	row_input_column_output();
	if(!(KP_PIN & (1<<ROW0))) key_index += 0; // Se PD0=0 => ROW0
	if(!(KP_PIN & (1<<ROW1))) key_index += 3; // Se PD1=0 => ROW1
	if(!(KP_PIN & (1<<ROW2))) key_index += 6; // Se PD2=0 => ROW2
	if(!(KP_PIN & (1<<ROW3))) key_index += 9; // Se PD3=0 => ROW3
	// Reconfigura linhas/colunas
	row_output_column_input();
	return key_index;
}
unsigned char read_keypad()
{
	char key='\0',
	uint8_t index;
	row_output_column_input();
	// Verifica se NL=0 em alguma coluna
	if((KP_PIN & COLS) != COLS) {
		// Debounce
		_delay_ms(10);
		// Se tecla ainda pressionada após 10 ms, ação válida
		if((KP_PIN & COLS) != COLS) {
			index=scan_keys();
			key=ascii[index];
			// Espera todas teclas serem
		while ((KP_PIN & COLS) != COLS);}}
		return key;
	}
