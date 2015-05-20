/**
 * @file pinsel_lpc1769.c
 * @author Lénio Passos - 22164
 * @date 14 Mai 2015
 * @brief Configuração das funções de cada porto
 *
 */
#include <stdint.h>
#include "LPC17xx.h"
#include "pinsel_lpc1769.h"



void set_pinsel(uint32_t id_pinsel){
	uint8_t pinselId = (id_pinsel >> 16)&0x0F; //Número_do_PINSEL
	uint8_t portId = (id_pinsel >> 8)&0xFF; //Número_do_porto
	uint8_t func = id_pinsel&0x03; //Função

	if(portId>15)
		portId -= 16;

	volatile uint32_t *low = &(LPC_PINCON->PINSEL0) + pinselId;

	*low &= (~(3 << (portId<<1)));
	*low |= ((func << (portId<<1)));

}





