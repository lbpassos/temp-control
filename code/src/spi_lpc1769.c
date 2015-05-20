/**
 * @file spi_lpc1769.c
 * @author Lénio Passos - 22164
 * @date 14 Mai 2015
 * @brief Configuração SPI (Serial Peripheral Interface)
 *
 */
#include <stdint.h>
#include "LPC17xx.h"
#include "spi_lpc1769.h"
#include "pinsel_lpc1769.h"

#define PCSPI	8

#define PINSEL_0	0
#define PINSEL_1	1

#define P_15	15
#define P_16	16
#define P_17	17
#define P_18	18

#define PCLK_SPI	16

void spiSCK_config(){
	uint32_t id_pinsel = PINSEL_0; //PINSEL0
	id_pinsel = (id_pinsel<<8) | P_15; //P0.15
	id_pinsel = (id_pinsel<<8) | 0x3; //SCK
	set_pinsel(id_pinsel);
}

void spiSSEL_config(){
	uint32_t id_pinsel = PINSEL_1; //PINSEL1
	id_pinsel = (id_pinsel<<8) | P_16; //P1.16
	id_pinsel = (id_pinsel<<8) | 0x3; //SSEL
	set_pinsel(id_pinsel);
}

void spiMISO_config(){
	uint32_t id_pinsel = PINSEL_1; //PINSEL1
	id_pinsel = (id_pinsel<<8) | P_17; //P1.17
	id_pinsel = (id_pinsel<<8) | 0x3; //MISO
	set_pinsel(id_pinsel);
}

void spiMOSI_config(){
	uint32_t id_pinsel = PINSEL_1; //PINSEL1
	id_pinsel = (id_pinsel<<8) | P_18; //P1.18
	id_pinsel = (id_pinsel<<8) | 0x3; //MOSI
	set_pinsel(id_pinsel);
}

void spiConfigure(uint32_t data){
	LPC_SPI->SPCR = data;
}

void spiPCLK_config(volatile uint8_t data){
	data = data&0x3;
	LPC_SC->PCLKSEL0 &= ~(3<<PCLK_SPI);
	LPC_SC->PCLKSEL0 |= (data<<PCLK_SPI);
}

//PCLK_SPI / SPCCR0
//data tem que ser par e maior ou igual a 8
void spiSet_clock(volatile uint8_t data){
	LPC_SPI->SPCCR = data&0xFF;
}

void spiWritedata(volatile uint32_t data){
	LPC_SPI->SPDR = data&0x0000FFFF;
	while( ( (LPC_SPI->SPSR)&0x80 )==0 );
}

uint32_t spiReadata(){
	while( ( (LPC_SPI->SPSR)&0x80 )==0 );
	return (LPC_SPI->SPDR) & 0x0000FFFF;
}
