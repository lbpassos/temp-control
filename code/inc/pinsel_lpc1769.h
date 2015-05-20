/**
 * @file pinsel_lpc1769.h
 * @author Lénio Passos - 22164
 * @date 14 Mai 2015
 * @brief Configuração das funções de cada porto
 *
 */
#ifndef PINSEL_LPC1769_H_
#define PINSEL_LPC1769_H_

/** @brief Define as funções de cada porto
 *
 * Manipula o conteúdo dos registos PINSEL (Pin Function Select Register)
 *
 *  @param[in] id_pinsel Variável de 32 bits em que cada octeto define:
 *
 *                           3- x
 *
 *                           2- Número do PINSEL
 *
 *                           1- Número do porto
 *
 *                           0- Função
 *
 */
void set_pinsel(uint32_t id_pinsel);


#endif

