/**
 * @file ethernet.c
 * @author Lénio Passos - 22164
 * @date 29 Mai 2015
 * @brief Controlador de ethernet (PHY -> SMSC LAN8720).
 *
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif
#include <stdint.h>
#include <string.h>
#include <cr_section_macros.h>
#include "pinsel_lpc1769.h"
#include "systick.h"
#include "ethernet.h"


//x Pinsel porto cais
/* Definição dos sinais RMII->PHY*/
#define ENET_TXD0		0x00020001 //x 2 0 1
#define ENET_TXD1		0x00020101
#define ENET_TX_EN		0x00020401
#define ENET_CRS		0x00020801
#define ENET_RXD0		0x00020901
#define ENET_RXD1		0x00020A01
#define ENET_RX_ER		0x00020E01
#define ENET_REF_CLK	0x00020F01
#define ENET_MDC		0x00031001
#define ENET_MDIO		0x00031101

/* MAC Configuration Register 1 */
#define MAC1_RECEIVE_ENABLE         	0x01
#define MAC1_PASS_ALL_RECEIVE_FRAMES    (1<<1)
#define MAC1_RX_FLOW_CONTROL       		(1<<2)
#define MAC1_TX_FLOW_CONTROL       		(1<<3)
#define MAC1_LOOPBACK					(1<<4)
#define MAC1_RESET_TX					(1<<8)
#define MAC1_RESET_MCS_TX     			(1<<9)
#define MAC1_RESET_RX         			(1<<10)
#define MAC1_RESET_MCS_RX     			(1<<11)
#define MAC1_SIMULATION_RESET        	(1<<14)
#define MAC1_SOFT_RESET					(1<<15)

/* MAC Configuration Register 2 */
#define MAC2_FULL_DUPLEX   		    	0x01
#define MAC2_FRAME_LENGTH_CHECKING  	(1<<1)
#define MAC2_HUGE_FRAME_ENABLE			(1<<2)
#define MAC2_DELAYED_CRC        		(1<<3)
#define MAC2_CRC_ENABLE         		(1<<4)
#define MAC2_PAD_CRC_ENABLE         	(1<<5)
#define MAC2_VLAN_PAD_ENABLE 		   	(1<<6)
#define MAC2_AUTO_DETECT_PAD_ENABLE    	(1<<7)
#define MAC2_PURE_PREAMBLE_ENFORCEMENT  (1<<8)
#define MAC2_LONG PREAMBLE_ENFORCEMENT  (1<<9)
#define MAC2_NO_BACKOFF     			(1<<12)
#define MAC2_BACK PRESSURE_NO_BACKOFF  	(1<<13)
#define MAC2_EXCESS DEFER				(1<<14)

/* Command Register */
#define COMMAND_RxEnable    	0x01
#define COMMAND_TxEnable    	(1<<1)
#define COMMAND_RegReset        (1<<3)
#define COMMAND_TxReset         (1<<4)
#define COMMAND_RxReset         (1<<5)
#define COMMAND_PassRuntFrame   (1<<6)
#define COMMAND_PassRxFilter    (1<<7)
#define COMMAND_TxFlowControl   (1<<8)
#define COMMAND_RMII            (1<<9)
#define COMMAND_FullDuplex      (1<<10)

/* Collision Window/Retry Register */
#define RETRANSMISSION_MAXIMUM  0x0F  //Default value
#define COLLISION_WINDOW		(0x37<<8) //Default value

/* Non Back-to-Back Inter-Packet-Gap Register */
#define IPGR_1	0x0C //Valores recomendados
#define IPGR_2	0x12 //Valores recomendados

/* MII Mgmt Configuration Register */
#define MII_Mgmt_Configuration_SCAN_INCREMENT		1
#define MII_Mgmt_Configuration_SUPPRESS_PREAMBLE	(1<<1)
#define MII_Mgmt_Configuration_CLOCK_SELECT			(0x0B<<2) //Maximum clock 120MHz
#define MII_Mgmt_Configuration_RESET_MII_MGMT		(1<<15)

/* The LAN8720 SMI address may be configured using hardware configuration to either the value 0 or 1 */
#define PHY_DEF_ADR    0x0100

/* SMSC LAN8720 Registos */
#define LAN8720_Control_Register								0
#define LAN8720_Status_Register									1
#define LAN8720_PHY_Identifier_1								2
#define LAN8720_PHY_Identifier_2								3
#define LAN8720_Auto_Negotiation_Advertisement_Register			4
#define LAN8720_Auto_Negotiation_Link_Partner_Ability_Register	5
#define LAN8720_Auto_Negotiation_Expansion_Register				6
#define LAN8720_Silicon_Revision_Register						16
#define LAN8720_Mode_Control_Status_Register					17
#define LAN8720_Special_Modes									18
#define LAN8720_Symbol_Error_Counter_Register					26
#define LAN8720_Control_Status_Indication_Register				27
#define LAN8720_Special_internal_testability_controls			28
#define LAN8720_Interrupt_Source_Register						29
#define LAN8720_Interrupt_Mask_Register							30
#define LAN8720_PHY_Special_Control_Status_Register				31

/* PHY Support Register (SUPP)*/
#define SUPP_SPEED	(1<<8)

/* RX Descriptor Control Word */
#define RX_DescriptorControl_SizeMask	0x07FF
#define RX_DescriptorControl_Interrupt	(1<<31)

/* Receive Filter Control Register */
#define ReceiveFilterControlRegister_AcceptUnicastEn        1
#define ReceiveFilterControlRegister_AcceptBroadcastEn		(1<<1)
#define ReceiveFilterControlRegister_AcceptMulticastEn		(1<<2)
#define ReceiveFilterControlRegister_AcceptUnicastHashEn	(1<<3)
#define ReceiveFilterControlRegister_AcceptMulticastHashEn	(1<<4)
#define ReceiveFilterControlRegister_AcceptPerfectEn		(1<<5)
#define ReceiveFilterControlRegister_MagicPacketEnWoL		(1<<12)
#define ReceiveFilterControlRegister_RxFilterEnWoL			(1<<13)

/* Receive Status Information Word */
#define ReceiveStatusInformationWord_RxSize   		0x07FF
#define ReceiveStatusInformationWord_ControlFrame   (1<<18)
#define ReceiveStatusInformationWord_VLAN   		(1<<19)
#define ReceiveStatusInformationWord_FailFilter   	(1<<20)
#define ReceiveStatusInformationWord_Multicast 		(1<<21)
#define ReceiveStatusInformationWord_Broadcast 		(1<<22)
#define ReceiveStatusInformationWord_CRCError 		(1<<23)
#define ReceiveStatusInformationWord_SymbolError	(1<<24)
#define ReceiveStatusInformationWord_LengthError	(1<<25)
#define ReceiveStatusInformationWord_RangeError		(1<<26)
#define ReceiveStatusInformationWord_AlignmentError	(1<<27)
#define ReceiveStatusInformationWord_Overrun		(1<<28)
#define ReceiveStatusInformationWord_NoDescriptor	(1<<29)
#define ReceiveStatusInformationWord_LastFlag		(1<<30)
#define ReceiveStatusInformationWord_Error			(1<<31)

#define ReceiveStatusInformationWord_MASK	(ReceiveStatusInformationWord_FailFilter | \
											ReceiveStatusInformationWord_SymbolError | \
											ReceiveStatusInformationWord_LengthError | \
											ReceiveStatusInformationWord_AlignmentError | \
											ReceiveStatusInformationWord_Overrun)


/** @brief RX Descritor
 */
typedef struct{
   uint32_t Packet; /*!< Endereço Base do @e buffer de RX*/
   uint32_t Control;/*!< Informação de controlo */
}RX_DESC_TypeDef;
/** @brief RX Status
 */
typedef struct{
   uint32_t StatusInfo;/*!< @e Flags */
   uint32_t StatusHashCRC;/*!< MAC_Dest hash CRC e MAC_Origem hash CRC. */
}RX_STAT_TypeDef;

/** @brief TX Descritor
 */
typedef struct{
   uint32_t Packet;/*!< Endereço Base do @e buffer de TX*/
   uint32_t Control;/*!< Informação de controlo */
}TX_DESC_TypeDef;
/** @brief TX Status
 */
typedef struct{
   uint32_t StatusInfo;/*!< @e Flags */
}TX_STAT_TypeDef;


#ifndef AHBSRAM0
#define AHBSRAM0  __DATA(RAM2)
AHBSRAM0 volatile RX_DESC_TypeDef rx_desc[NUM_RX_FRAG];
AHBSRAM0 volatile RX_STAT_TypeDef rx_stat[NUM_RX_FRAG];
AHBSRAM0 volatile TX_DESC_TypeDef tx_desc[NUM_TX_FRAG];
AHBSRAM0 volatile TX_STAT_TypeDef tx_stat[NUM_TX_FRAG];
AHBSRAM0 volatile uint8_t rx_buffer[NUM_RX_FRAG][ETH_FRAG_SIZE];
AHBSRAM0 volatile uint8_t tx_buffer[NUM_TX_FRAG][ETH_FRAG_SIZE];
#endif


static int16_t rx_consume_offset = -1; //num de bytes RX (controlo)
static int16_t tx_produce_offset = -1; //num de bytes TX (controlo)

/** @brief Escreve no PHY -> SMSC LAN8720
 *  @param[in] reg Endereço do registo
 *  @param[in] writeval Conteúdo
 *
 *  Exemplo de utilização: Realizar um @e reset.
 *  @code
 *  WriteToPHY (0, 1<<15); //Activa o bit 15 no registo de endereço 0 (Basic Control)
 *  @endcode
 *
 */
void WriteToPHY (int32_t reg, int32_t writeval)
{
  uint32_t chrono;
  // Set up address to access in MII Mgmt Address Register
  LPC_EMAC->MADR = PHY_DEF_ADR | reg;
  // Write value into MII Mgmt Write Data Register
  LPC_EMAC->MWTD = writeval;
  // Loop whilst write to PHY completes

  chrono = chrono_start();
  while( chrono_timeout(chrono, 100)==0 ){ //1 s
	  if ((LPC_EMAC->MIND & 0x01) == 0) { break; }
  }
}

// CodeRed - function added to read from external ethernet PHY chip
/** @brief Lê do PHY -> SMSC LAN8720
 *  @param[in] reg Endereço do registo
 *  @return Conteúdo do registo
 *
 *  Exemplo de utilização: Ler conteúdo do registo (Basic Control).
 *  @code
 *  uint16_t r = ReadFromPHY(0); //Endereço do registo Basic Control = 0
 *  @endcode
 *
 */
uint16_t ReadFromPHY(uint8_t reg)
{
  uint32_t chrono;
  // Set up address to access in MII Mgmt Address Register
  LPC_EMAC->MADR = PHY_DEF_ADR | reg;
  // Trigger a PHY read via MII Mgmt Command Register
  LPC_EMAC->MCMD = 1; //Ler

  // Loop whilst read from PHY completes
  chrono = chrono_start();
  while( chrono_timeout(chrono, 100)==0 ){ //1 s
	  if ((LPC_EMAC->MIND & 0x01) == 0){ break; }
  }

  LPC_EMAC->MCMD = 0; // Cancel read
  // Returned value is in MII Mgmt Read Data Register
  return (LPC_EMAC->MRDD);
}


void ethernet_init() {
	uint32_t chrono;

	LPC_SC->PCONP |= (1<<30); // Enable Ethernet block power/clock control bit

	/* Configurar Portos para Ethernet */
	set_pinsel( ENET_TXD0 );
	set_pinsel( ENET_TXD1 );
	set_pinsel( ENET_TX_EN );
	set_pinsel( ENET_CRS );
	set_pinsel( ENET_RXD0 );
	set_pinsel( ENET_RXD1 );
	set_pinsel( ENET_RX_ER );
	set_pinsel( ENET_REF_CLK );
	set_pinsel( ENET_MDC );
	set_pinsel( ENET_MDIO );

	/* Reset */
	LPC_EMAC->MAC1 = MAC1_RESET_TX | MAC1_RESET_MCS_TX | MAC1_RESET_RX | MAC1_RESET_MCS_RX |
					MAC1_SIMULATION_RESET | MAC1_SOFT_RESET;
	LPC_EMAC->Command = COMMAND_RegReset | COMMAND_TxReset | COMMAND_RxReset;

	delay(10); //100ms delay

	LPC_EMAC->MAC1 = MAC1_PASS_ALL_RECEIVE_FRAMES; //também  SOFT_RESET=0
	LPC_EMAC->MAC2 = MAC2_CRC_ENABLE | MAC2_PAD_CRC_ENABLE; //CRC e PAD enable
	LPC_EMAC->MAXF = ETH_MAX_FLEN; //tamanho máximo da trama recebida
	LPC_EMAC->CLRT = RETRANSMISSION_MAXIMUM | COLLISION_WINDOW;
	LPC_EMAC->IPGR = IPGR_2 | (IPGR_1<<8); //Non Back-to-Back Inter-Packet-Gap Register;
	LPC_EMAC->Command = COMMAND_RMII | COMMAND_PassRuntFrame; //enable RMII
	LPC_EMAC->MCFG = MII_Mgmt_Configuration_CLOCK_SELECT | MII_Mgmt_Configuration_RESET_MII_MGMT;

	delay(10); //100ms delay

	LPC_EMAC->MCFG &= ~MII_Mgmt_Configuration_RESET_MII_MGMT; //clear reset

	WriteToPHY( LAN8720_Control_Register, (1<<15) ); //Activar Reset no PHY
	delay(10);//100ms
	chrono = chrono_start();
	while( chrono_timeout(chrono, 500)==0 ){ //5 segundos timeout
		if( (ReadFromPHY(LAN8720_Control_Register)&0x8000)==0 ) //verificar se o bit de reset = 0 (reset concluido)
			break;
	}

	WriteToPHY( LAN8720_Control_Register, (1<<12) ); //Autonegociação activada
	delay(10);
	chrono = chrono_start();
	while( chrono_timeout(chrono, 500)==0 ){ //5 segundos timeout
		if( ((ReadFromPHY(LAN8720_Status_Register))&(1<<5)) ) //testar autonegociação activada
				break;
	}

	//Full ou Half duplex
	chrono = ReadFromPHY(LAN8720_PHY_Special_Control_Status_Register); //ler status
	if( (chrono>>4)&0x01 ){
		//Full Duplex
		LPC_EMAC->MAC2 |= MAC2_FULL_DUPLEX;
		LPC_EMAC->Command |= COMMAND_FullDuplex;
		LPC_EMAC->IPGT = 0x15; //Valor recomendado
	}
	else{
		//Half duplex
		LPC_EMAC->Command &= ~COMMAND_FullDuplex;
		LPC_EMAC->IPGT = 0x12; //Valor recomendado
	}

	//100Mbit ou 10Mbit mode
	if( (chrono>>3)&0x01 ){
		LPC_EMAC->SUPP = SUPP_SPEED; //100Mbits
	}
	else{
		LPC_EMAC->SUPP = 0; //10Mbits
	}

	/* Definição do endereço MAC */
	LPC_EMAC->SA0 = (MAC_ADDRESS_1<<8) | MAC_ADDRESS_2;
	LPC_EMAC->SA1 = (MAC_ADDRESS_3<<8) | MAC_ADDRESS_4;
	LPC_EMAC->SA2 = (MAC_ADDRESS_5<<8) | MAC_ADDRESS_6;

	/* RX Descriptor */
	for(chrono = 0; chrono < NUM_RX_FRAG; chrono++){
		rx_desc[chrono].Packet  = (uint32_t)&rx_buffer[chrono];
		rx_desc[chrono].Control    = RX_DescriptorControl_Interrupt | ((ETH_FRAG_SIZE-1)&RX_DescriptorControl_SizeMask);
		rx_stat[chrono].StatusInfo    = 0;
		rx_stat[chrono].StatusHashCRC = 0;
	}
    LPC_EMAC->RxDescriptor = (uint32_t)(&rx_desc); //endereço base
    LPC_EMAC->RxStatus = (uint32_t)(&rx_stat); //endereço base
    LPC_EMAC->RxDescriptorNumber = NUM_RX_FRAG-1; //Número de descritores
	LPC_EMAC->RxConsumeIndex  = 0;//descritor a ser usado

	/* TX Descriptor */
	for(chrono = 0; chrono < NUM_TX_FRAG; chrono++) {
		tx_desc[chrono].Packet = (uint32_t)&tx_buffer[chrono];
		tx_desc[chrono].Control = 0;
		tx_stat[chrono].StatusInfo = 0;
	}
	LPC_EMAC->TxDescriptor = (uint32_t)(&tx_desc);
	LPC_EMAC->TxStatus = (uint32_t)(&tx_stat);
	LPC_EMAC->TxDescriptorNumber = NUM_TX_FRAG-1;
	LPC_EMAC->TxProduceIndex  = 0;

	/* Receive Broadcast, Perfect Match Packets */
	LPC_EMAC->RxFilterCtrl = ReceiveFilterControlRegister_AcceptUnicastEn |
							ReceiveFilterControlRegister_AcceptMulticastEn |
							ReceiveFilterControlRegister_AcceptBroadcastEn |
							ReceiveFilterControlRegister_AcceptPerfectEn;

	/* Interrupt Enable Register */
	LPC_EMAC->IntEnable = (1<<3) | (1<<7); // RxDoneIntEn | TxDoneIntEn
	LPC_EMAC->IntClear  = 0xFFFF; // Reset a todos interrupts
	/* Enable */
	LPC_EMAC->Command  |= (COMMAND_RxEnable | COMMAND_TxEnable);
	LPC_EMAC->MAC1     |= MAC1_RECEIVE_ENABLE;
}

/** @brief Controlador do buffer circular
 *  @param[in] idx index no @e buffer
 *  @param[in] mod dimensão do @e buffer
 *  @return idx+1
 *
 *  Define a noção de @e buffer circular. Exemplo de utilização: buffer de dimensão 256
 *  @code
 *  uint16_t idx = 1; //posição corrente: idx=1
 *  idx = circBuffer_controlador(idx, 256); //idx após incremento fica com o valor 2;
 *  @endcode
 *  @code
 *  uint16_t idx = 255; //posição corrente: idx=255
 *  idx = circBuffer_controlador(idx, 256); //idx após incremento fica com o valor 0;
 *  @endcode
 *
 */
uint16_t circBuffer_controlador(uint16_t idx, uint16_t mod){
  ++idx;
  idx %= mod;
  return idx;
}

//Receber a trama Ethernet
int16_t ethernet_receive(){
	uint32_t info;

	//Se já existem tramas lidas é necessário incrementar o index do descritor
	if(rx_consume_offset >= 0) {
		LPC_EMAC->RxConsumeIndex = circBuffer_controlador(LPC_EMAC->RxConsumeIndex, NUM_RX_FRAG);
	}

	// Testar se o buffer está vazio
	if(LPC_EMAC->RxConsumeIndex == LPC_EMAC->RxProduceIndex) {
		rx_consume_offset = -1;
		return 0;
	}

	info = rx_stat[LPC_EMAC->RxConsumeIndex].StatusInfo;
	rx_consume_offset = 0;

	//Testar:
	//        1 - se a trama está completa (todos os fragmentos recebidos)
	//        2 - se algum erro existe na trama
	if(!(info&ReceiveStatusInformationWord_LastFlag) || (info&ReceiveStatusInformationWord_MASK)) {
		return -1;
	}

	//Retorna o tamanho da trama recebida - 4 bytes do campo checksum
	return (info&ReceiveStatusInformationWord_RxSize) + 1 - 4;
}

//Ler os dados
int16_t ethernet_read(uint8_t *data, uint16_t dsize) {
	int16_t size; //Dimensão da trama
	int16_t remaining; // Dimensão da trama não lida
	int16_t ncopy;
	uint32_t info;
	uint8_t *psrc; // ptr para o descritor (dados recebidos)

	if(rx_consume_offset < 0) { //Não existem dados
		return 0;
	}

	//Adquirir valores actuais
	info = rx_stat[LPC_EMAC->RxConsumeIndex].StatusInfo;
	size = (info&ReceiveStatusInformationWord_RxSize) + 1 - 4; // -checksum(4 bytes)

	/* Bloco concebido para o caso em que dsize é menor que a dimensão da trama */
	remaining = size - rx_consume_offset;//Calcular o que falta ler
	if( remaining<=dsize ){
		ncopy = remaining;
	}
	else{
		ncopy = dsize;
	}

	psrc = (uint8_t *)(rx_desc[LPC_EMAC->RxConsumeIndex].Packet + rx_consume_offset);

	if(data != NULL && ncopy > 0) {
		memcpy(data, psrc, ncopy);
	}

	rx_consume_offset += ncopy;

	return ncopy;
}

int16_t ethernet_send() {
	int16_t size, index, next;

	if(tx_produce_offset < 0) { // Não existe trama
		return -1;
	}

	// verificar linkstatus
	if( !(ReadFromPHY(LAN8720_Status_Register)&(1<<2)) ) {
		return -2;
	}

	size = tx_produce_offset; //Num de bytes a enviar
	index = LPC_EMAC->TxProduceIndex;
	tx_desc[index].Control = (tx_produce_offset-1) | ((1<<31) | (1<<30));//interrupt e trama completa

	next = circBuffer_controlador(index, NUM_TX_FRAG); //
	while( next==LPC_EMAC->TxConsumeIndex );//garantir que o próximo descritor está livre

	LPC_EMAC->TxProduceIndex = next;//actualizar TxProduceIndex e enviar
	tx_produce_offset = -1;//nada para enviar
	return size;
}

//Enviar dados
int16_t ethernet_write(const uint8_t *data, int16_t dlen){
	int16_t index, remaining, ncopy;
	uint8_t *pdst;

	if(tx_produce_offset < 0) { // Nada foi enviado
		tx_produce_offset = 0;
	}

	index = LPC_EMAC->TxProduceIndex;

	/* garantir espaço para checksum (que é realizado pelo hardware) */
	remaining = ETH_MAX_FLEN - tx_produce_offset - 4;
	if( remaining<=dlen ){
		ncopy = remaining;
	}
	else{
		ncopy = dlen;
	}

	pdst = (uint8_t *)(tx_desc[index].Packet + tx_produce_offset);

	if(ncopy > 0 ){
		if(data != NULL) {
			memcpy(pdst, data, ncopy);
		}
		else{
			memset(pdst, 0, ncopy);
		}
	}

	tx_produce_offset += ncopy;

	return ncopy;
}























