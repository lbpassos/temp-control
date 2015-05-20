/**
 * @file gpio_lpc1769.h
 * @author Lénio Passos - 22164
 * @date 14 Mai 2015
 * @brief Configuração GPIO (General Purpose Input Output)
 *
 */
#ifndef GPIO_LPC1769_H_
#define GPIO_LPC1769_H_

/** @brief Configuração
 *
 *Exemplo: P1.18 como entrada -> port= x 1 18 0 = 0x00011200
 *
 *            Número do Porto = 1
 *
 *            Número do Cais  = 18
 *
 *                   Direcção = 0
 *
 * @param[in] port variavel de 32 bits. Cada octeto define:
 *
 *                      3 - x
 *
 *                      2 - Número do Porto
 *
 *                      1 - Número do Cais
 *
 *                      0 - Direcção
 *
 */
void portConfigure(uint32_t port);

/** @brief Porto = 1
 *
 * @param[in] port variavel de 16 bits. Cada octeto define:
 *
 *                      1 - Número do Porto
 *
 *                      0 - Número do Cais
 *
 */
void setPort(uint16_t port);

/** @brief Porto = 0
 *
 * @param[in] port variavel de 16 bits. Cada octeto define:
 *
 *                      1 - Número do Porto
 *
 *                      0 - Número do Cais
 *
 */
void clearPort(uint16_t port);

/** @brief Retorna o valor presente no Porto
 *
 * @param[in] port variavel de 16 bits. Cada octeto define:
 *
 *                      1 - Número do Porto
 *
 *                      0 - Número do Cais
 *
 */
uint8_t readPort(uint16_t port);

#endif

