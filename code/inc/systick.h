/**
 * @file systick.h
 * @author Lénio Passos - 22164
 * @date 16 Mai 2015
 * @brief Temporização
 *
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

/** @brief delay de múltiplos de 10 [ms]
 *
 * Exemplo para um delay de 20 [ms]:
 * @code
 * delay(2);
 * @endcode
 * @param[in] delayTime indica o delay a realizar.
 *
 */
inline void delay(uint32_t delayTime);

/** @brief Configuração
 *
 * Systick configurado para cclk=100 [MHz]
 */
void initSysTick_Config();

/** @brief @e timeout
 *
 *  @param[in] initial valor inicial
 *  @param[in] delta tempo de contagem
 *  @see chrono_start()
 *  @return 1 se tempo de contagem esgotado. Caso contrário retorna 0.
 *
 */
uint8_t chrono_timeout(uint32_t initial, uint32_t delta);
/** @brief Retorna valor corrente do systick
 */
uint32_t chrono_start();

#endif /* SYSTICK_H_ */
