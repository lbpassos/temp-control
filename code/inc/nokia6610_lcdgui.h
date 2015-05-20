/**
 * @file nokia6610_lcdgui.h
 * @author Lénio Passos - 22164
 * @date 14 Mai 2015
 * @brief NOKIA6610 LCD GUI (Graphical User Interface)
 *
 * Implementação de uma GUI básica
 *
 */
#ifndef NOKIA6610_LCDGUI_H_
#define NOKIA6610_LCDGUI_H_

/** @brief Cores definidas
 *
 */
enum Color { WHITE = 0xFFF, BLACK = 0x000, RED = 0xF00,	GREEN = 0x0F0,
			 BLUE = 0x00F,  CYAN = 0x0FF,  MAGENTA = 0xF0F, YELLOW = 0xFF0,
			 BROWN = 0xB22, ORANGE = 0xFA0, PINK = 0xF6A };

/** @brief Escrever um símbolo
 *
 *Escrever um símbolo definido no módulo fonts.
 *
 *@param[in] c símbolo
 *@param[in] x coordenada vertical
 *@param[in] y coordenada horizontal
 *@param[in] fColor cor do símbolo
 *@param[in] bColor cor do símbolo (background)
 *
 *@see fonts.c
 *@warning Símbolo tem que estar definido no módulo fonts
 */
void LCDPutChar(uint8_t c, uint32_t x, uint32_t y, uint32_t fColor, uint32_t bColor);

/** @brief Escrever um conjunto de símbolos (strings)
 *
 *Escrever símbolos (strings) definidos no módulo fonts.
 *
 *@param[in] c String
 *@param[in] x coordenada vertical
 *@param[in] y coordenada horizontal
 *@param[in] fColor cor do símbolo
 *@param[in] bColor cor do símbolo (background)
 *
 *@see fonts.c
 *@warning Cada símbolo tem que estar definido no módulo fonts
 */
void LCDPutStr(uint8_t *pString, uint32_t x, uint32_t y, uint32_t fColor, uint32_t bColor);

/** @brief Define a fonte dos símbolos
 *
 *@param[in] fontIndex indica a fonte a utilizar
 *
 *@see fonts.h
 */
void setFont(uint8_t fontIndex);

/** @brief Retorna o tamanho da fonte (em bytes) utilizada
 *
 *  @return tamanho da fonte (em bytes)
 */
uint8_t getFontSize();

/** @brief Desenha um rectângulo
 *
 *@param[in] x coordenada vertical (início)
 *@param[in] y coordenada horizontal (início)
 *@param[in] dx distância vertical
 *@param[in] dy distância horizontal
 *@param[in] color cor de preenchimento
 */
void lcd_fill_rectangle(uint32_t x, uint32_t y, uint32_t dx, uint32_t dy, uint32_t color);

#endif
