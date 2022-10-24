#include "i2c.h"
#include <avr/io.h>
#define F_CPU 16000000UL
// Incializa clock da interface TWI
void I2C_Inic()
{
	TWSR |= 1; // Prescaler de 4
	TWBR=((F_CPU/F_SCL)-16)/2; // Carrega TWBR com cte p/ bit rate
	TWCR |= (1<<TWINT)|(1<<TWEN);// Habilita o TWI e a interrup��o
}
// Envia Start Condition, espera por TWINT e c�d.de status Start
unsigned char I2C_Start()
{
	TWCR=TWI_START; // Envia condi��o de START
	while(!TWI_RDY); // Espera resposta TWINT pelo HW
	if(TWI_STATUS!=START_ANSW) // Verifica c�d. TWSR c/ m�scara F8
	return 1; // Caso erro de START retorna 1
	return 0; // Sen�o Start inviado corretamente
}
// Envia Endere�o do Slave
unsigned char I2C_WrAddr(uint8_t addr)
{
	TWDR=addr; // Carrega SLA_W em TWDR
	TWCR=TWI_SEND; // Limpa TWINT e seta TWEN em TWICR
	while(!TWI_RDY); // Espera resposta TWINT pelo HW
	if(TWI_STATUS!=MT_SLA_ACK) // Verifica c�d. TWSR c/ m�scara F8
	return 1; // Erro escrita endere�o retorna 1
	return 0; // Sen�o ACK recebido corretamente
}
// Envia Dado p/ o Slave, espera por TWINT e c�d.de status Start
unsigned char I2C_SendData(uint8_t dado)
{
	TWDR=dado; // Carrega SLA_W em TWDR
	TWCR=TWI_SEND; // Limpa TWINT e seta TWEN em TWICR
	while(!TWI_RDY); // Espera resposta TWINT pelo HW
	
	if(TWI_STATUS!=MT_DATA_ACK) // Verifica c�d. TWSR c/ m�scara F8
	return 1; // Erro de envio de dado retorna 1
	return 0; // Sen�o dado enviado corretamente
}
// Envia condi��o de STOP
void I2C_Stop()
{
	TWCR=TWI_STOP; // Envia STOP
	while(TWCR & (1<<TWSTO)); // Espera envio de STOP
}
// Leitura dado c/ retorno NACK (�ltima leitura do pacote)
unsigned char I2C_GetNACK(void)
{
	TWCR = TWI_NACK; // Envia NACK indicando �ltimo byte
	while(!TWI_RDY); // ESpera pela condi��o READY
	return TWDR; // Retorna dado no reg de dados
}
// Escreve dado em registro de dispositivo TWI
void I2C_WrReg(uint8_t DevAddr,uint8_t DevReg,uint8_t data)
{
	I2C_Start(); // Envia condi��o de START
	I2C_WrAddr(DevAddr+I2C_WR); // Envia endere�o do dispositivo
	I2C_SendData(DevReg); // Envia endere�o do registro
	I2C_SendData(data); // Envia dado para o registro
	I2C_Stop(); // Envia condi��o de STOP
}
// L� dado de registro de dispositivo TWI
unsigned char I2C_RdReg(uint8_t DevAddr,uint8_t DevReg)
{
	unsigned char data=0;
	I2C_Start(); // Envia condi��o de START
	I2C_WrAddr(DevAddr); // Envia endere�o do Dispositivo
	I2C_SendData(DevReg); // Envia Endere�o do Registro
	I2C_Start(); // Envia condi��o de START
	I2C_WrAddr(DevAddr+I2C_RD); // Envia ender. Dispositivo+READ
	data=I2C_GetNACK(); // Recebe byte do Reg. do Slave
	I2C_Stop(); // Envia condi��o de STOP
	return data; // Retorna dado lido do Slave
}