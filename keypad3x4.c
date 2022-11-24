#include "keypad3x4.h"
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
// Vetor de caracteres ascii na matriz de teclas 3x4 e caracter null
char ascii[13]={'1','2','3','4','5','6','7','8','9','*','0','#',0};
void en_pullups(){
	KP_PORT = COLS;
	KP_PORT = ROWS;
}
void row_output_column_input(){
	KP_DDR |= ROWS; // Seta linhas como saída
	KP_DDR &= ~COLS; // Seta colunas como entrada
	KP_PORT = COLS; // Pullups nas entradas

}
void row_input_column_output(){
	KP_DDR &= ~ROWS; // Seta linhas como entradas
	KP_DDR |= COLS; // Seta colunas como saídas
	KP_PORT = ROWS; // Pullups nas entradas
}
unsigned char scan_keys(){
	uint8_t port,key_index=12; // Inicializa index para NULL (0)
	// Identifica a coluna
	row_output_column_input();
	port=KP_PIN;
	if (!(port & (1<<COL0))) key_index = 0; // Se PD4=0 => COL0
	else if (!(port & (1<<COL1))) key_index = 1; // Se PD5=0 => COL1
	else if (!(port & (1<<COL2))) key_index = 2; // Se PD6=0 => COL2
	else if (!(port & (1<<COL3))) key_index = 3; // Se PD7=0 => COL3
	// Identifica a linha
	row_input_column_output();
	en_pullups();
	port=KP_PIN;
	if (!(port & (1<<ROW0))) key_index += 0; // Se 4x4 => += 0
	else if (!(port & (1<<ROW1))) key_index += 3; // Se 4x4 => += 4
	else if (!(port & (1<<ROW2))) key_index += 6; // Se 4x4 => += 8
	else if (!(port & (1<<ROW3))) key_index += 9; // Se 4x4 => += 12
	row_output_column_input(); // Reconfigura linhas/colunas
	while ((KP_PIN & COLS) != COLS);
	return key_index;
}
unsigned char read_keypad(){
	uint8_t key=0,index;
	en_pullups();
	row_output_column_input();
	KP_PORT &= ~ROWS; // Coloca todas as linhas em NL=0
	if ((KP_PIN & COLS) != COLS){ // Verifica se NL=0 numa coluna
		_delay_ms(10); // Debounce
		// Se tecla ainda pressionada após 10 ms, ação válida
		if ((KP_PIN & COLS) != COLS){
			index=scan_keys();
			key=ascii[index];
		while ((KP_PIN & COLS) != COLS);}} // Espera teclas liberadas
		return key;
	}
