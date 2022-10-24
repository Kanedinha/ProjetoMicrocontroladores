#ifndef _KEYPAD3X4_H
#define _KEYPAD3X4_H
#define KP_PORT PORTD
#define KP_PIN PIND
#define KP_DDR DDRD
#define COL0 4 // Pino de KP_PORT p/ coluna 0
#define COL1 5 // Pino de KP_PORT p/ coluna 1
#define COL2 6 // Pino de KP_PORT p/ coluna 2
#define COL3 7 // Pino de KP_PORT p/ coluna 3
#define ROW0 0 // Pino de KP_PORT p/ linha 0
#define ROW1 1 // Pino de KP_PORT p/ linha 1
#define ROW2 2 // Pino de KP_PORT p/ linha 2
#define ROW3 3 // Pino de KP_PORT p/ linha 3
#define COLS ((1<<COL0)|(1<<COL1)|(1<<COL2)|(1<<COL3))
void row_output_column_input();
void row_input_column_output();
unsigned char scan_keys();
unsigned char read_keypad();
#endif