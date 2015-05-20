/**
 * @file fonts.h
 * @author Lénio Passos - 22164
 * @date 16 Mai 2015
 * @brief  Fontes dos símbolos
 *
 */
#ifndef FONTS_H_
#define FONTS_H_

/** @brief Define símbolos de tamanho 8x8
 *
 * Os símbolos definidos são os 96 caracteres gráficos da tabela ASCII mais o símbolo de graus Celsius (128)
 */
#define FONT8by8		0

/** @brief Selecciona a fonte a utilizar
 *
 * @param[in] fontselect tipo de fonte
 * @return Retorna um apontador para a lista de símbolos seleccionada
 * @warning Se a lista seleccionada não for suportada a função @b não retorna
 *
 */
uint8_t *fontHandler(uint8_t fontselect);

#endif /* FONTS_H_ */
