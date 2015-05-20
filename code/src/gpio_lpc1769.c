/**
 * @file gpio_lpc1769.c
 * @author Lénio Passos - 22164
 * @date 14 Mai 2015
 * @brief Configuração GPIO (General Purpose Input Output)
 *
 */
#include <stdint.h>
#include "LPC17xx.h"
#include "gpio_lpc1769.h"
#include "pinsel_lpc1769.h"

// X PortBank port dir
//Ex: P2.0 -> X 2 0 dir
void portConfigure(uint32_t port){
	uint8_t portBank = (port>>16)&0xFF;
	uint8_t portNum = (port>>8)&0x0FF;
	uint8_t direction = port&0x1;

	uint8_t pinsel;
	switch(portBank){
		case 0:
			if(portNum<16)
				pinsel = 0;
			else
				pinsel = 1;
			break;
		case 1:
			if(portNum<16)
				pinsel = 2;
			else
				pinsel = 3;
			break;
		case 2:
			pinsel = 4;
			break;
		case 3:
			pinsel = 7;
			break;
		case 4:
			pinsel = 9;
			break;
	}

	//id_pinsel= x Numero_do_PINSEL Porto funcao
	uint32_t configPinsel = pinsel<<8;
	configPinsel |= portNum;
	configPinsel <<= 8;
	//GPIO é 00
	set_pinsel(configPinsel); //seleccionar PINSEL como GPIO

	volatile uint32_t *low = &(LPC_GPIO0->FIODIR) + portBank;

	*low &= ~(1<<portNum); //por zero porto
	*low |= direction<<portNum; //seleccionar direcção

}

//PortBank portnum
void setPort(uint16_t port){
	uint8_t portBank = (port>>8)&0xFF;
	uint8_t portNum = port&0xFF;

	volatile uint32_t *low = &(LPC_GPIO0->FIOSET) + portBank;

	*low |= (1<<portNum); //porto = 1
}

void clearPort(uint16_t port){
	uint8_t portBank = (port>>8)&0xFF;
	uint8_t portNum = port&0xFF;

	volatile uint32_t *low = &(LPC_GPIO0->FIOCLR) + portBank;

	*low |= (1<<portNum); //porto = 0
}

uint8_t readPort(uint16_t port){
	uint8_t portBank = (port>>8)&0xFF;
	uint8_t portNum = port&0xFF;

	volatile uint32_t *low = &(LPC_GPIO0->FIOPIN) + portBank;

	return (*low>>portNum)&0x01;
}
