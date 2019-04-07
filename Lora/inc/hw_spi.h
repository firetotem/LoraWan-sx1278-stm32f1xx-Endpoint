#ifndef INC_HW_SPI_H_
#define INC_HW_SPI_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/*!
 * @brief Initializes the SPI object and MCU peripheral
 *
 * @param [IN] none
 */
void HW_SPI_Init( void );

/*!
 * @brief De-initializes the SPI object and MCU peripheral
 *
 * @param [IN] none
 */
void HW_SPI_DeInit( void );

/*!
 * @brief Initializes the SPI IOs
 *
 * @param [IN] none
 */
void HW_SPI_IoInit( void );

/*!
 * @brief De-initializes the SPI IOs
 *
 * @param [IN] none
 */
void HW_SPI_IoDeInit( void );

/*!
 * @brief Sends outData and receives inData
 *
 * @param [IN] outData Byte to be sent
 * @retval inData      Received byte.
 */
uint16_t HW_SPI_InOut( uint16_t outData );



#ifdef __cplusplus
}
#endif

#endif /* INC_HW_SPI_H_ */
