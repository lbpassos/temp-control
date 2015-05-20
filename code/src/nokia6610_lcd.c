/**
 * @file nokia6610_lcd.c
 * @author Lénio Passos - 22164
 * @date 14 Mai 2015
 * @brief NOKIA6610 LCD (Controlador EPSON)
 *
 */
#include <stdint.h>
#include "gpio_lpc1769.h"
#include "nokia6610_lcd.h"
#include "spi_lpc1769.h"
#include "systick.h"

//P0.21
#define BL_PORT		0
#define BL_PIER		21
//P0.3
#define RST_PORT	0
#define RST_PIER	3
//P0.2
#define CS_PORT		0
#define CS_PIER		2


uint32_t configureGPIO_pins(volatile uint8_t harbour, volatile uint8_t pier, volatile uint8_t dir){
	uint32_t tmp = harbour;

	tmp <<= 8;
	tmp |= pier;
	tmp <<= 8;
	tmp |= dir;

	return tmp;
}

void reset_nokia6610(){
	uint16_t tmp = RST_PORT;
	tmp <<= 8;
	tmp |= RST_PIER;

	clearPort(tmp); //Reset=0 (ON)
	delay(50);
	setPort(tmp);
	delay(50);

}

void backlightControl_nokia6610(uint8_t control){
	uint16_t tmp = BL_PORT;
	tmp <<= 8;
	tmp |= BL_PIER;

	if( control==1 )
		setPort(tmp);
	else
		clearPort(tmp);
}

void enableControl_nokia6610(uint8_t control){
	uint16_t tmp = CS_PORT;
	tmp <<= 8;
	tmp |= CS_PIER;

	if( control==1 )// enable on
		clearPort(tmp);
	else
		setPort(tmp);
}

void write_nokia6610(uint8_t command_data, uint8_t value){
	uint32_t tmp;
	if(command_data==0)//Is command
		tmp = 0;
	else // Is data
		tmp = 1;
	tmp <<= 8;
	tmp |= value;

	spiWritedata(tmp);
}

void init_nokia6610(){

	/* Configurar os portos GPIO */
	portConfigure( configureGPIO_pins(BL_PORT, BL_PIER, 1) ); // Porto para Backlight
	portConfigure( configureGPIO_pins(RST_PORT, RST_PIER, 1) ); // Porto para RST
	portConfigure( configureGPIO_pins(CS_PORT, CS_PIER, 1) ); // Porto para CS

	backlightControl_nokia6610( OFF );
	enableControl_nokia6610( OFF );

	uint16_t tmp = RST_PORT;
	tmp <<= 8;
	tmp |= RST_PIER;
	setPort(tmp);

	/* Configurar protocolo SPI */
	spiSCK_config();
	spiMOSI_config();
	spiSet_clock( 0x08 ); // 25/8 Mhz
	spiConfigure( 0x924 ); // 100100100100 -
	                       // 1001 - 9 bits
						   //     0 - SPIE disable
	                       //      0 - MSB first
	                       //       1 - Master mode enable
	                       //        0 - SCK active high
	                       //         0 - CPHA no primeiro clock
	                       //          1 - SPI a mais de 8 bits

	/* Configurar LCD */
	reset_nokia6610();

	enableControl_nokia6610( ON );
	write_nokia6610(COMMAND, DISCTL);
	write_nokia6610(DATA, 0x00);
	write_nokia6610(DATA, 0x20);
	write_nokia6610(DATA, 0x00);

	write_nokia6610(COMMAND, COMSCN);
	write_nokia6610(DATA, 0x01);

	write_nokia6610(COMMAND, OSCON);

	write_nokia6610(COMMAND, SLPOUT);

	write_nokia6610(COMMAND, VOLCTR);	//Voltage control (contrast setting)
	write_nokia6610(DATA, 45);	//P1 = 32
	write_nokia6610(DATA, 3);

	write_nokia6610(COMMAND, PWRCTR);
	write_nokia6610(DATA, 0x0F);

	write_nokia6610(COMMAND, DISINV);

	write_nokia6610(COMMAND, DATCTL);
	write_nokia6610(DATA, 0x03); //Display Invertido
	write_nokia6610(DATA, 0x00);
	write_nokia6610(DATA, 0x02);

	delay(50);
	write_nokia6610(COMMAND, DISON);
	enableControl_nokia6610( OFF );

}
