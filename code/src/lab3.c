/**
 * @file lab3.c
 * @brief SE2 - lab4
 *
 * Utilização  do protocolo I2C
 *
 * @author Lénio Passos - 22164
 * @date 16 Mai 2015
 *
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include "systick.h"
#include "i2c.h"
#include "maximDS1631_gui.h"
#include "maximDS1631.h"
#include "nokia6610_lcdgui.h"
#include "nokia6610_lcd.h"
#include "fonts.h"
#include <stdint.h>

#include <cr_section_macros.h>


int main(void) {
	uint32_t tmp, fontInUse;

	initSysTick_Config(); // Iniciar timer
	i2c_init(); // Iniciar I2C
	init_nokia6610(); //Iniciar lcd

	config_maximDS1631(RESOLUTION_9, ONE_SHOT_ON); //definir resolução de 9 bits e modo de conversão singular
	setAddress_maximDS1631(0x00); //definir endereço I2C do termómetro

	/* Eliminar Power-up state */
	/* O valor de leitura inicial on Power-up é de -60º */
	initMeasure_maximDS1631(); //Iniciar medição
	while(1){
		maximDS1631();
		if( status_maximDS1631() )
			break;
	}
	delay(500);

	initMeasure_maximDS1631();//Iniciar pedido de leitura

	backlightControl_nokia6610(1);
	lcd_fill_rectangle(0, 0, 132, 132, WHITE);
	setFont(FONT8by8);
	/* Escreve a string: T[º] */
	LCDPutStr( (uint8_t *)"T[\x80]: " , 20, 131-8, BLACK, WHITE);
	fontInUse = getFontSize();

	while(1){
		maximDS1631();//controlo do termómetro
		if( status_maximDS1631() ){//testar leitura completa
			tmp = temp_maximDS1631();//adquirir temp
			mDS1631_nokia6610(tmp, 20, 75, BLACK, WHITE, RESOLUTION_9, fontInUse);//imprimir valor no LCD
			delay(6000);//delay de 1 min
			backlightControl_nokia6610(0);
			initMeasure_maximDS1631();
		}
	}

    return 0 ;
}
