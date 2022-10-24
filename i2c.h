#ifndef _I2C_H
#define _I2C_H
#include <inttypes.h>
// Macros p/ Registros de Controle de Status
#define TWI_START (1<<TWINT)|(1<<TWSTA)|(1<<TWEN)
#define TWI_STOP (1<<TWINT)|(1<<TWSTO)|(1<<TWEN)
#define TWI_NACK (1<<TWINT)|(1<<TWEN)
#define TWI_ACK (1<<TWINT)|(1<<TWEN)|(1<<TWEA)
#define TWI_SEND (1<<TWINT)|(1<<TWEN)
#define TWI_RDY (TWCR & (1<<TWINT))
#define TWI_STATUS (TWSR & 0xF8)
// Códigos de retorno do Reg. de Status Modo Master Transmitter
#define START_ANSW 0x08 // start condition transmitted
#define TW_REP_START 0x10 // repeated start cond. transmitted
#define MT_SLA_ACK 0x18 // SLA+W transmitted, ACK received
#define MT_SLA_NACK 0x20 // SLA+W transmitted, NACK received
#define MT_DATA_ACK 0x28 // data transmitted, ACK received
#define MT_DATA_NACK 0x30 // data transmitted, NACK received
#define MT_ARB_LOST 0x38 // arbitration lost in SLA+W or data
// Códigos de retorno do Reg. de Status Modo Master Receiver
#define MR_ARB_LOST 0x38 // arbitration lost in SLA+R or NACK
#define MR_SLA_ACK 0x40 // SLA+R transmitted, ACK received
#define MR_SLA_NACK 0x48 // SLA+R transmitted, NACK received
#define MR_DATA_ACK 0x50 // data received, ACK returned
// Códigos de retorno do Reg. de Status Modo Slave Transmitter
#define ST_SLA_ACK 0xA8 // SLA+R received, ACK returned
// arbitration lost in SLA+RW, SLA+R received, ACK returned
#define ST_ARB_LOST_SLA_ACK 0xB0
#define ST_DATA_ACK 0xB8 // data transmitted, ACK received
#define ST_DATA_NACK 0xC0 // data transmitted, NACK received
// last data byte transmitted, and ACK received
#define ST_LAST_DATA 0xC8

// Códigos de retorno do Reg. de Status Modo Slave Receiver
#define SR_SLA_ACK 0x60 // SLA+W received, ACK returned
// arbitration lost in SLA+RW, SLA+W received, ACK returned
#define SR_ARB_LOST_SLA_ACK 0x68
#define SR_GCALL_ACK 0x70 // general call received, ACK returned
// arbitration lost in SLA+RW, general call received, ACK returned
#define SR_ARB_LOST_GCALL_ACK 0x78
#define SR_DATA_ACK 0x80 // data received, ACK returned
#define SR_DATA_NACK 0x88 // data received, NACK returned
// general call data received, and ACK returned
#define SR_GCALL_DATA_ACK 0x90
// general call data received, and NACK returned
#define SR_GCALL_DATA_NACK 0x98
// stop or repeated start condition received while selected
#define SR_STOP 0xA0
// Constantes Simbólicas
#define I2C_RD 1
#define I2C_WR 0
#define F_SCL 100000L // Clock da TWI 100kHz
// Incializa clock da interface I2C
void I2C_Inic();
// Envia condição de START
unsigned char I2C_Start();
// Envia Endereço do Slave
unsigned char I2C_WrAddr(uint8_t addr);
// Envia Dado p/ o Slave
unsigned char I2C_SendData(uint8_t dado);
// Envia condição de STOP
void I2C_Stop();
// Leitura do dado com retorno de NACK (última leitura do pacote)
unsigned char I2C_GetNACK(void);
// Escreve dado em registro de dispositivo I2C
void I2C_WrReg(uint8_t DevAddr,uint8_t DevReg,uint8_t data);
// Lê dado de registro de dispositivo I2C
unsigned char I2C_RdReg(uint8_t DevAddr,uint8_t DevReg);
#endif