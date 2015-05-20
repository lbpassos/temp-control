/*
 * i2c.c
 *
 *  Created on: May 16, 2015
 *      Author: lpassos
 */
#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <stdint.h>
#include "i2c.h"
#include "pinsel_lpc1769.h"


#define SDA1_PIN	0x00000003 //x pinsel porto funcao - P0.0
#define SCL1_PIN	0x00000103 //x pinsel porto funcao - P0.1
#define SDA1_SCL1_PINMODE	0x0A
#define SDA1_SCL1_PINMODEOD	0x03

#define RUN		0x00
#define END		0X01
#define ERROR	0x02


static uint8_t slave_address; // end do slave
static uint8_t i2c_condition;//modo de comunicacao: TX, RX e TRX

static uint8_t *data_tx; // dados a transmitir
static uint32_t dim_data_tx; // dim dos dados a transmitir
static uint8_t *data_rx; // dados a receber
static uint32_t dim_data_rx;// dim dos dados a receber

static uint8_t i2c_state; // estado da comunicacao


/* Inicializar I2C */
void i2c_init(){
	/* Seleccionar pinos para SDA e SCL */
	set_pinsel( SDA1_PIN ); //SDA1
	set_pinsel( SCL1_PIN ); //SCL1

	/* Colocar pinos em open drain */
	LPC_PINCON->PINMODE0 |= SDA1_SCL1_PINMODE;
	LPC_PINCON->PINMODE_OD0 |= SDA1_SCL1_PINMODEOD;

	/* Configurar frequência I2C*/
	/* I2C_freq = PCLKI2C/(I2CSCLH + I2CSCLL) */
	/* PCLKI2C = CCLK/4 = 25 [MHz] */
	/* Para I2C_freq=100 [kHz]: I2CSCLH + I2CSCLL = 250 */
	LPC_I2C1->I2SCLH = 100;
	LPC_I2C1->I2SCLL = 150;

	/* Enable */
	LPC_I2C1->I2CONCLR = 0x6C; //clear flags
	LPC_I2C1->I2CONSET = 0x40; //Enable I2C
	NVIC_EnableIRQ(I2C1_IRQn);
}

/* i2c_set
 * Define o end. do slave e o modo(TX, RX e TRX)
 */
void i2c_set(uint8_t slave_add, uint8_t mode){
	slave_address = slave_add<<1;
	i2c_condition = mode;
}

/* i2c_comm
 * Define os parametros da comunicacao:
 *               datos a transmitir
 *               dimensao dos dados a transmitir
 *               destino dos dados recebidos
 *               dimensao dos dados a receber
 */
void i2c_comm(uint8_t *ptr_datatx, uint32_t dimtx, uint8_t *ptr_datarx, uint32_t dimrx){

	data_tx = ptr_datatx;
	dim_data_tx = dimtx;
	data_rx = ptr_datarx;
	dim_data_rx = dimrx;
	LPC_I2C1->I2CONSET |= 1<<5; //STA=1
	i2c_state = RUN;
}

/* i2c_status
 * Verifica o estado de um pedido
 * Retorna: 1 - Pedido completo
 *          0 - Em processamento
 */
uint8_t i2c_status(){
	if( i2c_state == END )
		return 1; //escrita/leitura completa
	else
		return 0;
}

uint8_t i2c_error(){
	if( i2c_state == ERROR )
		return 1; //escrita/leitura completa
	else
		return 0;
}

/** @brief @e Handler da função de Interrupção
 */
void I2C1_IRQHandler(){
	uint32_t handle = LPC_I2C1->I2STAT;
	switch( (handle)&0xFF ){
		case 0x08: // Envio do end. do slave e a accao (leiura ou escrita) depois de um START
				if( i2c_condition==TRANSMIT_MODE || i2c_condition==TRANSCEIVER_MODE )
					LPC_I2C1->I2DAT = slave_address; // address + R/W=0
				else
					LPC_I2C1->I2DAT = slave_address | 0x01; // address + R/W=1
				i2c_state = RUN;
				LPC_I2C1->I2CONCLR = 0x28; // clear STA e SI
				break;
		case 0x10: // Envio do end. do slave para leitura depois de um START repetido
				LPC_I2C1->I2DAT = slave_address | 0x01; // address + R/W=1
				LPC_I2C1->I2CONCLR = 0x28;
				break;
		case 0x18: // Envio do primeiro byte.
				if( !dim_data_tx ){
					LPC_I2C1->I2CONSET = 0x50; // STOP flag
					i2c_state = END;
				}
				else{
					LPC_I2C1->I2DAT = *(data_tx++);
					--dim_data_tx;
					i2c_state = RUN;
				}
				LPC_I2C1->I2CONCLR = 0x08; // clear SI
				break;
		case 0x20:
				LPC_I2C1->I2CONSET = 0x50; // STOP flag
				LPC_I2C1->I2CONCLR = 0x08; // clear SI
				i2c_state = ERROR;
				break;
		case 0x28: // Envio dos restantes bytes
				if( dim_data_tx ){
					LPC_I2C1->I2DAT = *(data_tx++);
					--dim_data_tx;
					i2c_state = RUN;
				}
				else{
					if( i2c_condition==TRANSMIT_MODE ){
						LPC_I2C1->I2CONSET = 0x50; // STOP flag
						i2c_state = END;
					}
					else
						if( i2c_condition==TRANSCEIVER_MODE ){
							LPC_I2C1->I2CONSET = 0x60; //Repetir o START
							i2c_state = RUN;
						}
				}
				LPC_I2C1->I2CONCLR = 0x08; // clear SI
				break;
		case 0x30:
				LPC_I2C1->I2CONSET = 0x50; // STOP flag
				LPC_I2C1->I2CONCLR = 0x08; // clear SI
				i2c_state = ERROR;
				break;
		case 0x38:
				LPC_I2C1->I2CONSET = 0x60; //STA=1
				LPC_I2C1->I2CONCLR = 0x08; // clear SI
				i2c_state = RUN;
				break;
		case 0x40: // Recepcao do primeiro byte
				if( dim_data_rx==1 )
					LPC_I2C1->I2CONSET = 0x40; // so existe uma palavra para receber
				else
					LPC_I2C1->I2CONSET = 0x44; //colocar AA=1
				i2c_state = RUN;
				LPC_I2C1->I2CONCLR = 0x08; // clear SI
				break;
		case 0x48:
				LPC_I2C1->I2CONSET = 0x50; // STOP flag
				LPC_I2C1->I2CONCLR = 0x08; // clear SI
				i2c_state = ERROR;
				break;
		case 0x50: // Recepcao dos seguintes bytes
				*(data_rx++) = LPC_I2C1->I2DAT;
				--dim_data_rx;
				if( dim_data_rx == 1 ){
					LPC_I2C1->I2CONCLR = 0x04; //Clear AA
					LPC_I2C1->I2CONSET = 0x40;
				}
				else
					LPC_I2C1->I2CONSET = 0x44;
				i2c_state = RUN;
				LPC_I2C1->I2CONCLR = 0x08; // clear SI
				break;
		case 0x58: // Terminar recepcao
				*data_rx = LPC_I2C1->I2DAT;
				LPC_I2C1->I2CONSET = 0x50; // STOP
				LPC_I2C1->I2CONCLR = 0x08; // clear SI
				i2c_state = END;
				break;

	}

}

