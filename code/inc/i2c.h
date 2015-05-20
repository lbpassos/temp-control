/**
 * @file i2c.h
 * @brief I2C
 * @author Lénio Passos - 22164
 * @date 19 Mai 2015
 *
 * O protocolo I2C implementado só realiza as funções de @e Master @e Transmit e de @e Master @e Receive
 *
 */

#ifndef I2C_H_
#define I2C_H_

/** @brief @e MASTER escreve (e não espera resposta) no @e SLAVE
 */
#define TRANSMIT_MODE		0x00
/** @brief MASTER recebe dados do @e SLAVE
 */
#define RECEIVER_MODE		0x01
/** @brief MASTER pede dados (escreve e espera resposta) ao @e SLAVE
 */
#define TRANSCEIVER_MODE	0x02

/** @brief Inicialização
 *
 * Configura a interface I2C1 para operar com os seguintes parâmetros:
 *
 *                      1 - PCLCK = CCLK/4
 *
 *                      2 - I2C_freq = 100 [kHz]
 *
 *                      3 - Activação de interrupts
 */
void i2c_init();
/** @brief Define os parâmetros de comunicação
 *
 * @param[in] slave_add endereço do @e SLAVE
 * @param[in] mode define o comportamento do @e MASTER:
 *
 *                   1 - TRANSMIT_MODE
 *
 *                   2 - RECEIVER_MODE
 *
 *                   3 - TRANSCEIVER_MODE
 *
 */
void i2c_set(uint8_t slave_add, uint8_t mode);
/** @brief Especificação dos dados
 *
 * Inicia o processo de comunicação
 *
 * @param[in] ptr_datatx dados a transmitir
 * @param[in] dimtx dimensão (em bytes) dos dados a transmitir
 * @param[in] ptr_datarx apontador para o endereço do depósito dos dados a receber
 * @param[in] dimrx dimensão (em bytes) do depósito dos dados
 *
 */
void i2c_comm(uint8_t *ptr_datatx, uint32_t dimtx, uint8_t *ptr_datarx, uint32_t dimrx);
/** @brief Verificação do estado da comunicação
 *
 * Permite determinar se a comunicação está terminada
 *
 *@return retorna dois possíveis valores:
 *
 *                        1 - Comunicação terminada e dados disponíveis
 *
 *                        0 - Comunicação não terminada
 *
 */
uint8_t i2c_status();
/** @brief Verificação de erros
 *
 * Detecta a existência de erros durante a comunicação.
 *
 *@return retorna dois possíveis valores:
 *
 *                        1 - Erro (Comunicação Inválida)
 *
 *                        0 - Sucesso
 *
 */
uint8_t i2c_error();


#endif /* I2C_H_ */
