#include "../../HW_BSP/inc/hw.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define SPI_CS_LOW()	HW_GPIO_Write( RADIO_NSS_PORT, RADIO_NSS_PIN, 0 )
#define SPI_CS_HIGH()	HW_GPIO_Write( RADIO_NSS_PORT, RADIO_NSS_PIN, 1 )

/* Private variables ---------------------------------------------------------*/
static SPI_HandleTypeDef hspi;

/* Private function prototypes -----------------------------------------------*/

/* Exported functions ---------------------------------------------------------*/
/*!
 * @brief Initializes the SPI object and MCU peripheral
 *
 * @param [IN] none
 */
void HW_SPI_Init( void )
{

  /*##-1- Configure the SPI peripheral */
  /* Set the SPI parameters */

  hspi.Instance = SPIx;

  hspi.Init.BaudRatePrescaler 		= SPI_BAUDRATEPRESCALER_32;
  hspi.Init.Direction      		= SPI_DIRECTION_2LINES;
  hspi.Init.Mode           		= SPI_MODE_MASTER;
  hspi.Init.CLKPolarity    		= SPI_POLARITY_LOW;
  hspi.Init.CLKPhase       		= SPI_PHASE_1EDGE;
  hspi.Init.DataSize       		= SPI_DATASIZE_8BIT;
  hspi.Init.CRCCalculation 		= SPI_CRCCALCULATION_DISABLE;
  hspi.Init.FirstBit       		= SPI_FIRSTBIT_MSB;
  hspi.Init.NSS            		= SPI_NSS_SOFT;
  hspi.Init.TIMode         		= SPI_TIMODE_DISABLE;

  SPI_CLK_ENABLE();

  if(HAL_SPI_Init( &hspi) != HAL_OK)
  {
    /* Initialization Error */
     Error_Handler();
  }
}

/*!
 * @brief De-initializes the SPI object and MCU peripheral
 *
 * @param [IN] none
 */
void HW_SPI_DeInit( void )
{

  HAL_SPI_DeInit( &hspi);

  /*##-1- Reset peripherals ####*/
  __HAL_RCC_SPI1_FORCE_RESET();
  __HAL_RCC_SPI1_RELEASE_RESET();
}

/**
  * @brief  Initialize the SPI MSP.
  * @param  hspi: pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
  GPIO_InitTypeDef initStruct = { 0 };

  if ( hspi->Instance == SPIx )
  {
      initStruct.Mode 	= GPIO_MODE_AF_PP;
      initStruct.Pull 	= GPIO_NOPULL;
      initStruct.Speed 	= GPIO_SPEED_FREQ_HIGH;
      HW_GPIO_Init( RADIO_SCLK_PORT, RADIO_SCLK_PIN, &initStruct);
      HW_GPIO_Init( RADIO_MISO_PORT, RADIO_MISO_PIN, &initStruct);
      HW_GPIO_Init( RADIO_MOSI_PORT, RADIO_MOSI_PIN, &initStruct);

      initStruct.Mode = GPIO_MODE_OUTPUT_PP;
      initStruct.Pull = GPIO_NOPULL;
      HW_GPIO_Init( RADIO_NSS_PORT, RADIO_NSS_PIN, &initStruct );
      HW_GPIO_Write( RADIO_NSS_PORT, RADIO_NSS_PIN, 1 );
  }
}

void HAL_SPI_MspDeInit( SPI_HandleTypeDef *hspi )
{
  if ( hspi->Instance == SPIx )
  {
      HAL_GPIO_DeInit( RADIO_MOSI_PORT, RADIO_MOSI_PIN );
      HAL_GPIO_DeInit( RADIO_MOSI_PORT, RADIO_MISO_PIN );
      HAL_GPIO_DeInit( RADIO_MOSI_PORT, RADIO_SCLK_PIN );
      HAL_GPIO_DeInit( RADIO_MOSI_PORT, RADIO_NSS_PIN );
  }
}

/*!
 * @brief Sends outData and receives inData
 *
 * @param [IN] outData Byte to be sent
 * @retval inData      Received byte.
 */
uint16_t HW_SPI_InOut( uint16_t txData )
{
  uint16_t rxData ;

  SPI_CS_LOW();
  BSP_Led_On();

  HAL_SPI_TransmitReceive( &hspi, ( uint8_t * ) &txData, ( uint8_t* ) &rxData, 1, HAL_MAX_DELAY);

  BSP_Led_Off();
  SPI_CS_HIGH();

  return rxData;
}

/* Private functions ---------------------------------------------------------*/

