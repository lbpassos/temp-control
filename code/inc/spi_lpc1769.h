/**
 * @file spi_lpc1769.h
 * @author Lénio Passos - 22164
 * @date 14 Mai 2015
 * @brief Configuração SPI (Serial Peripheral Interface)
 *
 */
#ifndef SPI_LPC1769_H_
#define SPI_LPC1769_H_

/** @brief Configuração SCK
 *
 * Define o porto para o sinal SCK
 *
 */
void spiSCK_config();

/** @brief Configuração SSEL
 *
 * Define o porto para o sinal SSEL
 *
 */
void spiSSEL_config();

/** @brief Configuração MISO
 *
 * Define o porto para o sinal MISO
 *
 */
void spiMISO_config();

/** @brief Configuração MOSI
 *
 * Define o porto para o sinal MOSI
 *
 */
void spiMOSI_config();

/** @brief Configuração do modo de operação
 *
 * @param[in] data variável de 32 bits em que cada bit define:
 *
 *                      0-1   -
 *
 *                        2   - BitEnable
 *
 *                        3   - CPHA
 *
 *                        4   - CPOL
 *
 *                        5   - MSTR
 *
 *                        6   - LSBF
 *
 *                        7   - SPIE
 *
 *                      8-11  - Número de bits por transferência
 *
 *                      12-31 -
 *
 */
void spiConfigure(volatile uint32_t data);

/** @brief Configuração do PCLK_SPI
 *
 * O valor definido após reset é CCLK/4
 *
 * @param[in] data variável que define o valor de PCLK. Pode tomar os seguintes valores:
 *
 *                      00 - CCLK/4
 *
 *                      01 - CCLK
 *
 *                      10 - CCLK/2
 *
 *                      11 - CCLK/8
 *
 */
void spiPCLK_config(volatile uint8_t data);

/** @brief Configuração da frequência de SCK
 *
 * SCK = PCLK_SPI/data
 *
 * @param[in] data variável para calcular o valor de SCK. No modo MASTER, data tem que ser par e maior que 8.
 *
 */
void spiSet_clock(volatile uint8_t data);

/** @brief Envio de dados
 *
 * @param[in] data Dados a enviar
 *
 */
void spiWritedata(volatile uint32_t data);

/** @brief Recepção de dados
 *
 */
uint32_t spiReadata();

#endif

