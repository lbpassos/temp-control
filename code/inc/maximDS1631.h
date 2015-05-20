/**
 * @file maximDS1631.h
 * @brief Termómetro DS1631
 * @author Lénio Passos - 22164
 * @date 19 Mai 2015
 *
 * @e Driver para o Termómetro Maxim DS1631
 *
 * O modo de utilização (medida da temperatura) consiste no seguinte:
 *
 *               1 - Definir a resolução e o modo de conversão
 *
 *               2 - Definir o endereço (3 bits)
 *
 *               3 - Iniciar o pedido de leitura
 *
 *               4 - Verificar se a leitura está realizada
 *
 *               5 - Adquirir o valor da temperatura
 *
 */

#ifndef MAXIMDS1631_H_
#define MAXIMDS1631_H_

/** @brief Resolução de 9 bits
 */
#define RESOLUTION_9	9
/** @brief Resolução de 10 bits
 */
#define RESOLUTION_10	10
/** @brief Resolução de 11 bits
 */
#define RESOLUTION_11	11
/** @brief Resolução de 12 bits
 */
#define RESOLUTION_12	12

/** @brief Modo de conversão singular
 *
 * Após conversão, o dispositivo entra em @e stand-by.
 */
#define ONE_SHOT_ON		1
/** @brief Modo de conversão contínua
 */
#define ONE_SHOT_OFF	0


/** @brief Configuração
 *
 * Configura o termómetro com o valor da resolução e do modo de conversão
 *
 * @param[in] res Resolução. Valores possíveis: RESOLUTION_9, RESOLUTION_10, RESOLUTION_11 e RESOLUTION_12
 * @param[in] st Modo de conversão. Valores possíveis: ONE_SHOT_ON e ONE_SHOT_OFF
 * @warning A função não retorna se a comunicação I2C não for realizada com êxito
 */
void config_maximDS1631(uint8_t res, uint8_t st);
/** @brief Define endereço
 *
 * @param[in] add Endereço (3 bits)
 */
void setAddress_maximDS1631(uint8_t add);
/** @brief Inicia o processo de conversão
 *
 */
void initMeasure_maximDS1631();
/** @brief Testa se a conversão está terminada
 *  @return retorna:
 *
 *              1 - Conversão terminada e dados disponíveis
 *
 *              0 - Conversão em curso
 *
 */
uint32_t status_maximDS1631();
/** @brief Cálculo da temperatura
 *
 *  @return Valor da Temperatura. O valor retornado tem o seguinte formato:
 *
 *               Bits:    31         30...16               x15...x0
 *
 *                       Sinal     Valor Inteiro        Valor Decimal
 *
 *                                    Exemplos:
 *
 *                -55:     1            55                     0
 *
 *               55.5:     0            55                     5
 */
uint32_t temp_maximDS1631();
/** @brief Controlador
 *
 *  Realiza a máquina de estados (controlo)
 */
void maximDS1631();

#endif /* MAXIMDS1631_H_ */
