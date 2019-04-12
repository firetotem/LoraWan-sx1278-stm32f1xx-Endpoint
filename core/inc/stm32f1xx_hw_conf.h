#ifndef INC_STM32F1XX_HW_CONF_H_
#define INC_STM32F1XX_HW_CONF_H_

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* --------------------------- LORA I/O definition -------------------------- */
#define RADIO_DIO_4
#define RADIO_DIO_5

#define RADIO_RESET_PORT                          GPIOA
#define RADIO_RESET_PIN                           GPIO_PIN_0

#define RADIO_MOSI_PORT                           GPIOA
#define RADIO_MOSI_PIN                            GPIO_PIN_7

#define RADIO_MISO_PORT                           GPIOA
#define RADIO_MISO_PIN                            GPIO_PIN_6

#define RADIO_SCLK_PORT                           GPIOA
#define RADIO_SCLK_PIN                            GPIO_PIN_5

#define RADIO_NSS_PORT                            GPIOA
#define RADIO_NSS_PIN                             GPIO_PIN_4

#define RADIO_DIO_0_PORT                          GPIOA
#define RADIO_DIO_0_PIN                           GPIO_PIN_10

#define RADIO_DIO_1_PORT                          GPIOB
#define RADIO_DIO_1_PIN                           GPIO_PIN_3

#define RADIO_DIO_2_PORT                          GPIOB
#define RADIO_DIO_2_PIN                           GPIO_PIN_4

#define RADIO_DIO_3_PORT                          GPIOB
#define RADIO_DIO_3_PIN                           GPIO_PIN_5

#ifdef RADIO_DIO_4
  #define RADIO_DIO_4_PORT                        GPIOB
  #define RADIO_DIO_4_PIN                         GPIO_PIN_6
#endif

#ifdef RADIO_DIO_5
  #define RADIO_DIO_5_PORT                        GPIOB
  #define RADIO_DIO_5_PIN                         GPIO_PIN_7
#endif

#define RADIO_LEDRX_PORT                          GPIOC
#define RADIO_LEDRX_PIN                           GPIO_PIN_13


/* --------------------------- SPI MACRO redefinition -------------------------- */
#define SPI_CLK_ENABLE()                __HAL_RCC_SPI1_CLK_ENABLE()
#define SPIx				SPI1
#define SPIx_SCLK_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE()
#define SPIx_MISO_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE()
#define SPIx_MOSI_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE()
#define SPIx_NSS_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE()
/* --------------------------- ADC HW definition ------------------------------- */
#define ADC_READ_CHANNEL                ADC_CHANNEL_3
#define ADCx				ADC1
#define ADCCLK_ENABLE()                 __HAL_RCC_ADC1_CLK_ENABLE() ;
#define ADCCLK_DISABLE()                __HAL_RCC_ADC1_CLK_DISABLE() ;


/* --------------------------- RTC HW definition -------------------------------- */

    //TODO#define RTC_OUTPUT       		DBG_RTC_OUTPUT

/* --------------------------- USART HW definition -------------------------------*/
#define USARTx                           USART2
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART2_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define DMAx_CLK_ENABLE()                __HAL_RCC_DMA1_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __HAL_RCC_USART2_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART2_RELEASE_RESET()

#define USARTx_TX_PIN                  	 GPIO_PIN_2
#define USARTx_TX_GPIO_PORT              GPIOA
#define USARTx_TX_AF                     GPIO_AF7_USART2
#define USARTx_RX_PIN                    GPIO_PIN_3
#define USARTx_RX_GPIO_PORT              GPIOA
#define USARTx_RX_AF                     GPIO_AF7_USART2

/* Definition for USARTx's NVIC */
#define USARTx_IRQn                      USART2_IRQn
#define USARTx_IRQHandler                USART2_IRQHandler

/* Definition for USARTx's DMA */
#define USARTx_TX_DMA_CHANNEL            DMA1_Channel7

/* Definition for USARTx's NVIC */
#define USARTx_DMA_TX_IRQn               DMA1_Channel7_IRQn
#define USARTx_DMA_TX_IRQHandler         DMA1_Channel7_IRQHandler

#define USARTx_Priority 0
#define USARTx_DMA_Priority 0

/* --------------------------- DEBUG redefinition -------------------------------*/

#define __HAL_RCC_DBGMCU_CLK_ENABLE()
#define __HAL_RCC_DBGMCU_CLK_DISABLE()

#define LED_Toggle( x )
#define LED_On( x )
#define LED_Off( x )


#ifdef __cplusplus
}
#endif

#endif /* INC_STM32F1XX_HW_CONF_H_ */
