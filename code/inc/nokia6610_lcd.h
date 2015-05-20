/**
 * @file nokia6610_lcd.h
 * @author Lénio Passos - 22164
 * @date 14 Mai 2015
 * @brief NOKIA6610 LCD (Controlador EPSON)
 * @see https://adeetc.thothapp.com/classes/SE2/1415v/LI61D-LT61D-MI2D/resources/5376
 *
 */
#ifndef NOKIA6610_LCD_H_
#define NOKIA6610_LCD_H_

#define ON 	1
#define OFF	0

#define COMMAND		0
#define DATA		1

#define DISCTL		0xCA
#define COMSCN		0xBB
#define OSCON		0xD1
#define SLPOUT		0x94
#define VOLCTR		0x81
#define TMPGRD		0x82
#define PWRCTR		0x20
#define DISNOR		0xA6
#define DISINV		0xA7
#define DATCTL		0xBC
#define NOP			0x25
#define DISON		0xAF
#define PASET		0x75
#define CASET		0x15
#define RAMWR		0x5C


/** @brief Inicializar
 *
 *Proceder à inicialização  do LCD
 */
void init_nokia6610();

/** @brief Reset
 */
void reset_nokia6610();

/** @brief Controlo do Backlight
 *
 *@param[in] control define o estado do Backlight (1 - Ligado) e (0 - Desligado)
 */
void backlightControl_nokia6610(uint8_t control);

/** @brief Sinal de Enable
 *
 *Controlo do estado do sinal CS
 *
 *@param[in] control define o estado do CS (1 - Activado) e (0 - Desactivado)
 */
void enableControl_nokia6610(uint8_t control);

/** @brief Escrita
 *
 * Escreve dados ou comandos
 *
 *@param[in] command_data define o modo de interpretação da escrita (0 - Comandos) e (1 - Dados)
 *@param[in] value valor a escrever
 */
void write_nokia6610(uint8_t command_data, uint8_t value);

#endif
