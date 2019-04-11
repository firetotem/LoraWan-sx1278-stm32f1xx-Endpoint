#ifndef INC_HW_MSP_H_
#define INC_HW_MSP_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/*!
 * \brief GPIOs Macro
 */

#define RCC_GPIO_CLK_ENABLE( __GPIO_PORT__ )              	\
do {                                                    	\
    switch( __GPIO_PORT__)                                	\
    {                                                     	\
      case GPIOA_BASE: __HAL_RCC_GPIOA_CLK_ENABLE(); break;    	\
      case GPIOB_BASE: __HAL_RCC_GPIOB_CLK_ENABLE(); break;    	\
      case GPIOC_BASE: __HAL_RCC_GPIOC_CLK_ENABLE(); break;    	\
      case GPIOD_BASE: __HAL_RCC_GPIOD_CLK_ENABLE(); break;    	\
      default:         __HAL_RCC_GPIOD_CLK_ENABLE(); 		\
    }                                                    	\
  } while(0)

#define RCC_GPIO_CLK_DISABLE( __GPIO_PORT__ )              	\
do {                                                    	\
    switch( __GPIO_PORT__)                                	\
    {                                                     	\
      case GPIOA_BASE: __HAL_RCC_GPIOA_CLK_DISABLE(); break;    \
      case GPIOB_BASE: __HAL_RCC_GPIOB_CLK_DISABLE(); break;    \
      case GPIOC_BASE: __HAL_RCC_GPIOC_CLK_DISABLE(); break;    \
      case GPIOD_BASE: __HAL_RCC_GPIOD_CLK_DISABLE(); break;    \
      default:         __HAL_RCC_GPIOH_CLK_ENABLE(); 		\
    }                                                    	\
  } while(0)

/* Exported functions ------------------------------------------------------- */

/*!
 * \brief Initializes the boards peripherals.
 */
void HW_Init( void );

  /*!
 * \brief De-initializes the target board peripherals to decrease power
 *        consumption.
 */

void HW_DeInit( void );
/*!
 * Returns a pseudo random seed generated using the MCU Unique ID
 *
 * \retval seed Generated pseudo random seed
 */
uint32_t HW_GetRandomSeed( void );

/*!
 * \brief Gets the board 64 bits unique ID
 *
 * \param [IN] id Pointer to an array that will contain the Unique ID
 */
void HW_GetUniqueId( uint8_t *id );

  /*!
 * \brief Initializes the HW and enters stope mode
 */
void HW_EnterStopMode( void);

/*!
 * \brief Exits stop mode and Initializes the HW
 */
void HW_ExitStopMode( void);

/**
  * @brief Enters Low Power Sleep Mode
  * @note ARM exists the function when waking up
  * @param none
  * @retval none
  */
void HW_EnterSleepMode( void);

typedef enum
  {
    e_LOW_POWER_RTC = (1<<0),
    e_LOW_POWER_GPS = (1<<1),
    e_LOW_POWER_UART = (1<<2), /* can be used to forbid stop mode in case of uart Xfer*/
  } e_LOW_POWER_State_Id_t;



/*!
 * \brief Configures the sytem Clock at start-up
 *
 * \param none
 * \retval none
 */
void SystemClock_Config( void );

/**
  * @brief  Configure all GPIO's to Analog input to reduce the power consumption
  * @param  None
  * @retval None
  */
void HW_GpioInit(void);


#ifdef __cplusplus
}
#endif

#endif /* INC_HW_MSP_H_ */
