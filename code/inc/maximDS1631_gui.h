/**
 * @file maximDS1631_gui.h
 * @brief Mostrar Temperatura no LCD
 * @author Lénio Passos - 22164
 * @date 19 Mai 2015
 *
 */

#ifndef MAXIMDS1631_GUI_H_
#define MAXIMDS1631_GUI_H_

/** @brief Conversão binário para BCD
 *  @param[in] v valor a 16 bits
 *  @return valor em BCD: Milhares Centenas Dezenas Unidade
 */
uint16_t bin_ToBCD(uint16_t v);
/** @brief Imprime o valor da temperatura no LCD nokia6610
 *
 *  Realiza a conversão para código BCD e imprime no LCD
 *
 *  @param[in] n número
 *  @param[in] x posição x
 *  @param[in] y posição y
 *  @param[in] fColor cor do símbolo
 *  @param[in] bColor cor do @e background do símbolo
 *  @param[in] res Resolução (número de casas decimais)
 *  @param[in] fSize tamanho do símbolo em bytes (tamanho da fonte)
 *
 *
 *
 */
void mDS1631_nokia6610(uint32_t n, uint16_t x, uint16_t y, uint32_t fColor, uint32_t bColor, uint8_t res, uint8_t fSize);


#endif /* MAXIMDS1631_GUI_H_ */
