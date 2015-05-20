/**
 * @file nokia6610_lcdgui.c
 * @author Lénio Passos - 22164
 * @date 14 Mai 2015
 * @brief  NOKIA6610 LCD GUI (Graphical User Interface)
 *
 */
#include <stdint.h>
#include "nokia6610_lcdgui.h"
#include "nokia6610_lcd.h"
#include "fonts.h"


static uint8_t *pFont;
static uint8_t nCols;
static uint8_t nRows;
static uint8_t nBytes;

void setFont(uint8_t fontIndex){
	pFont = fontHandler( fontIndex );
	nCols = *pFont; //Num colunas
	nRows = *(pFont + 1); //Num linhas
	nBytes = *(pFont + 2); //Tamanho do símbolo
}

uint8_t getFontSize(){
	return nBytes;
}
void LCDPutChar(uint8_t c, uint32_t x, uint32_t y, uint32_t fColor, uint32_t bColor) {
	int32_t i,j;
	uint8_t PixelRow;
	uint8_t Mask;
	uint32_t Word0;
	uint32_t Word1;
	uint8_t *pChar;

	pChar = pFont + (nBytes * (c - 0x1F));//ptr para o primeiro byte do símbolo

	enableControl_nokia6610( ON );

	write_nokia6610(COMMAND, PASET);
	write_nokia6610(DATA, x);
	write_nokia6610(DATA, x + nRows - 1);

	write_nokia6610(COMMAND, CASET);
	write_nokia6610(DATA, y);
	write_nokia6610(DATA, y + nCols - 1);

	write_nokia6610(COMMAND, RAMWR);
	for (i = nRows - 1; i >= 0; i--) {
		PixelRow = *pChar++;
		Mask = 0x80;
		for (j = 0; j < nCols; j += 2) {
			if ((PixelRow & Mask) == 0)
				Word0 = bColor;
				else
					Word0 = fColor;
			Mask = Mask >> 1;
			if ((PixelRow & Mask) == 0)
				Word1 = bColor;
				else
					Word1 = fColor;
			Mask = Mask >> 1;
			write_nokia6610(DATA, (Word0 >> 4) & 0xFF);
			write_nokia6610(DATA, ((Word0 & 0xF) << 4) | ((Word1 >> 8) & 0xF));
			write_nokia6610(DATA, Word1 & 0xFF);
		}
	}
	write_nokia6610(COMMAND, NOP);
	enableControl_nokia6610( OFF );
}

void LCDPutStr(uint8_t *pString, uint32_t x, uint32_t y, uint32_t fColor, uint32_t bColor) {
	while (*pString != 0x00) {
		LCDPutChar(*pString++, x, y, fColor, bColor);
		y = y - nBytes;
		if(y < 0) break;
	}
}

void lcd_fill_rectangle(uint32_t x, uint32_t y, uint32_t dx, uint32_t dy, uint32_t color) {
	int32_t i;

	enableControl_nokia6610( ON );

	write_nokia6610(COMMAND, CASET);
	write_nokia6610(DATA, x);
	write_nokia6610(DATA, x + dx -1);
	write_nokia6610(COMMAND, PASET);
	write_nokia6610(DATA, y);
	write_nokia6610(DATA, y + dy -1);
	write_nokia6610(COMMAND, RAMWR);
	for (i = (dx * dy + 1) / 2 + 1; i > 0; --i) {
		write_nokia6610(DATA, color >> 4);
		write_nokia6610(DATA, ((color << 4) & 0xf0) | ((color >> 8) & 0xf));
		write_nokia6610(DATA, color);
	}
	write_nokia6610(COMMAND, NOP);
	enableControl_nokia6610( OFF );
}

