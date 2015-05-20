/**
 * @file maximDS1631_gui.c
 * @brief Mostrar Temperatura no LCD
 * @author Lénio Passos - 22164
 * @date 19 Mai 2015
 *
 */

#include <stdint.h>
#include "maximDS1631_gui.h"
#include "maximDS1631.h"
#include "nokia6610_lcdgui.h"

uint16_t bin_ToBCD(uint16_t v){
	uint8_t t, h, d, u; //valor em BCD: Milhares Centenas Dezenas Unidade
	int8_t i;

	uint16_t res = 0;

	t = h = d = u = 0;
	for(i=14; i>=0; --i){
		if( (t&0x0F)>=5 )
			t += 3;
		if( (h&0x0F)>=5 )
			h += 3;
		if( (d&0x0F)>=5 )
			d += 3;
		if( (u&0x0F)>=5 )
			u += 3;
		t <<= 1;
		t = t | ( (h>>3)&0x01 );
		h <<= 1;
		h = h | ( (d>>3)&0x01 );
		d <<= 1;
		d = d | ( (u>>3)&0x01 );
		u <<= 1;
		u = u | ( (v>>i)&0x01 );
	}
	res = t;
	res = (res<<4) | (h&0x0F);
	res = (res<<4) | (d&0x0F);
	res = (res<<4) | (u&0x0F);
	return res;
}

void mDS1631_nokia6610(uint32_t n, uint16_t x, uint16_t y, uint32_t fColor, uint32_t bColor, uint8_t res, uint8_t fSize){
	uint8_t i, resolution, flag, tmp;
	uint32_t num;

	switch(res){
		case 0:
			resolution = 0;
			break;
		case RESOLUTION_9:
			resolution = 1;
			break;
		case RESOLUTION_10:
			resolution = 2;
			break;
		case RESOLUTION_11:
			resolution = 3;
			break;
		default:
			resolution = 4;
	}

	LCDPutStr( (uint8_t *)"        ", x, y, fColor, bColor);//apagar

	if( (n>>31)&0x01 ){ //negativo
		LCDPutChar('-', x, y, fColor, bColor);
		y = y - fSize;
	}

	num = bin_ToBCD( (n>>16)&0x7FFF );

	flag = 0;
	for(i=0; i<4; ++i){
		tmp = (num>>(12-i*4))&0x0F;
		if( (tmp>0) || flag || (i==3) ){
			LCDPutChar( tmp+0x30, x, y, fColor, bColor);
			y = y - fSize;
			flag = 1;
		}
	}

	if(resolution){
		num = bin_ToBCD(n&0xFFFF);
		LCDPutChar( '.', x, y, fColor, bColor);
		y = y - fSize;
		for(i=resolution; i>0; --i){
			tmp = (num>>(4*i-4))&0x0F;
			LCDPutChar( tmp+0x30, x, y, fColor, bColor);
			y = y - fSize;
		}
	}
}
