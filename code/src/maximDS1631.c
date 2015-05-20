/**
 * @file maximDS1631.c
 * @brief Termómetro DS1631
 * @author Lénio Passos - 22164
 * @date 19 Mai 2015
 *
 * @e Driver para o Termómetro Maxim DS1631
 *
 */
#include <stdint.h>
#include "maximDS1631.h"
#include "i2c.h"
#include "systick.h"

/* Comandos */
#define START_CONV		0x51
#define STOP_CONV		0x22
#define READ_TEMP		0xAA
#define ACCESS_CONFIG	0xAC
#define ACCESS_TH		0xA1
#define ACCESS_TL		0xA2


#define DIM_DATA	3

#define IDDLE			0
#define INIT			1
#define TEST_INIT		2
#define INIT_MEASURE	3
#define MEASURE_READY	4
#define	COMPLETE		5

static uint8_t hih_state = IDDLE;
static uint8_t address = 0x48; //Endereço base
static uint8_t resolution;
static uint8_t data[DIM_DATA];


static uint32_t start_timer;
/* O tempo de conversão depende da resolução*/
/*      1                2           3                4*/
/*RESOLUTION_12  RESOLUTION_11   RESOLUTION_10    RESOLUTION_9 */
/* O timer_tick está configurado a 10 [ms] */
static const uint8_t wait_time_ms[] = {80, 40, 20, 10}; //x10ms
static uint8_t wait_time; //tempo de conversão seleccionado

void setAddress_maximDS1631(uint8_t add){
	address |= add;
}

void config_maximDS1631(uint8_t res, uint8_t st){
	resolution = res;
	wait_time = RESOLUTION_12 - res;

	data[0] = ACCESS_CONFIG;
	data[1] = 0;
	data[1] |= (resolution-RESOLUTION_9)<<2; //R1 R0 configuração da resolução
	data[1] |= st&0x01;

	i2c_set(address, TRANSMIT_MODE);
	i2c_comm(data, 2, 0x00, 0);
	while( !i2c_status() );
}


void initMeasure_maximDS1631(){
	hih_state = INIT;
}

uint32_t status_maximDS1631(){
	if( hih_state==COMPLETE )
		return 1;
	return 0;
}

uint32_t temp_maximDS1631(){
	uint32_t tmp, value;
	uint8_t flag;
	if( hih_state==COMPLETE ){
		flag = (data[1]>>7)&0x01;
		if(flag){
			data[1] = ~data[1]+1;
		}
		tmp = data[1]<<8;
		tmp |= data[2];
		tmp >>= ((RESOLUTION_12-resolution) + 4);
		switch(resolution){
			case RESOLUTION_9:
				value = tmp * 5;
				tmp = value / 10;
				tmp = (tmp<<16) | value%10;
				break;
			case RESOLUTION_10:
				value = tmp * 25;
				tmp = value / 100;
				tmp = (tmp<<16) | value%100;
				break;
			case RESOLUTION_11:
				value = tmp * 125;
				tmp = value / 1000;
				tmp = (tmp<<16) | value%1000;
				break;
			case RESOLUTION_12:
				value = tmp * 625;
				tmp = value / 10000;
				tmp = (tmp<<16) | value%10000;
				break;
		}
	}
	return tmp|(flag<<31); //int virgula
}






/* Maquina de Estados */
void maximDS1631(){
	switch(hih_state){
		case IDDLE:
				break;
		case INIT: // Início de conversão
				i2c_set(address, TRANSMIT_MODE); //end do sensor
				data[0] = START_CONV;
				i2c_comm(data, 1, 0x00, 0);
				hih_state = TEST_INIT;
				break;
		case TEST_INIT: //Verificar comunicação
				if( i2c_error() ) //Erro - repetir processo
					hih_state = INIT;
				else
					if( i2c_status() ){ //OK iniciar tempo de espera (conversão)
						start_timer = chrono_start();
						hih_state = INIT_MEASURE;
					}
				break;
		case INIT_MEASURE: //Ler
				if( chrono_timeout(start_timer, wait_time_ms[wait_time]) ){
					i2c_set(address, TRANSCEIVER_MODE);
					data[0] = READ_TEMP;
					i2c_comm(data, 1, &data[1], 2);
					hih_state = MEASURE_READY;
				}
				break;
		case MEASURE_READY:// Verificar comunicação
				if( i2c_error() ) //Erro - repetir processo
					hih_state = INIT;
				else
					if( i2c_status() ) //processo completo
						hih_state = COMPLETE;
				break;
		case COMPLETE: //Dados disponiveis
				break;
		default:
				break;
	}
}
