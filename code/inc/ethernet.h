/**
 * @file ethernet.h
 * @author Lénio Passos - 22164
 * @date 29 Mai 2015
 * @brief Controlador de ethernet (PHY -> SMSC LAN8720).
 *
 */

#ifndef ETHERNET_H_
#define ETHERNET_H_

/** @brief Primeiro Octeto do endereço MAC
 */
#define MAC_ADDRESS_1	1
/** @brief Segundo Octeto do endereço MAC
 */
#define MAC_ADDRESS_2	2
/** @brief Terceiro Octeto do endereço MAC
 */
#define MAC_ADDRESS_3	3
/** @brief Quarto Octeto do endereço MAC
 */
#define MAC_ADDRESS_4	4
/** @brief Quinto Octeto do endereço MAC
 */
#define MAC_ADDRESS_5	5
/** @brief Sexto Octeto do endereço MAC
 */
#define MAC_ADDRESS_6	6

/** @brief Número de tramas (tamanho do @e buffer de recepção)
 */
#define NUM_RX_FRAG         4           /* Num.of RX Fragments 4*1536= 6.0kB */
/** @brief Número de tramas (tamanho do @e buffer de emissão)
 */
#define NUM_TX_FRAG         4           /* Num.of TX Fragments 3*1536= 4.6kB */
/** @brief Tamanho da trama Ethernet
 */
#define ETH_FRAG_SIZE       1536        /* Packet Fragment size 1536 Bytes   */
/** @brief Tamanho máximo da trama Ethernet
 */
#define ETH_MAX_FLEN        1536        /* Max. Ethernet Frame Size          */

/** @brief Inicialização
 *
 */
void ethernet_init();
/** @brief Recepção das tramas
 *  @return  Valores de retorno:
 *
 *           -1 - Erro
 *
 *            0 - Não existem dados recebidos
 *
 *            Dimensão da trama recebida
 */
int16_t ethernet_receive();
/** @brief Leitura dos dados recebidos
 *  @param[in] data apontador para onde a trama ethernet recebida deve ser copiada
 *  @param[in] dlen número de bytes a copiar
 *  @return  dimensão (bytes) dos dados copiados
 */
int16_t ethernet_read(uint8_t *data, uint16_t dsize);
/** @brief Escrita da trama para envio
 *  @param[in] data apontador para trama a enviar
 *  @param[in] dlen dimensão (bytes)
 *  @return  dimensão (bytes) dos dados copiados
 */
int16_t ethernet_write(const uint8_t *data, int16_t dlen);
/** @brief Envio da trama
 *  @return  Valores de retorno:
 *
 *           -1 - Não existem dados
 *
 *            0 - Não existe ligação
 *
 *            Dimensão da trama recebida
 */
int16_t ethernet_send();


#endif /* ETHERNET_H_ */
