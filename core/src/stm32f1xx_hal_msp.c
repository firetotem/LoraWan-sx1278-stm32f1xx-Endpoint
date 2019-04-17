#include "hw.h"
#include "timeServer.h"
/* when fast wake up is enabled, the mcu wakes up in ~20us  * and
 * does not wait for the VREFINT to be settled. THis is ok for
 * most of the case except when adc must be used in this case before
 *starting the adc, you must make sure VREFINT is settled*/
#define ENABLE_FAST_WAKEUP

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief This function configures the source of the time base.
  * @brief  don't enable systick
  * @param TickPriority: Tick interrupt priority.
  * @retval HAL status
  */

//HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
//{
//   /* Return function status */
//  return HAL_OK;
//}

/**
  * @brief This function provides delay (in ms)
  * @param Delay: specifies the delay time length, in milliseconds.
  * @retval None
  */
//void HAL_Delay(__IO uint32_t Delay)
//{
//  HW_RTC_DelayMs( Delay ); /* based on RTC */
//}

/**
  * @brief  Initializes the MSP.
  * @retval None
  */
void HAL_MspInit(void)
{
  __HAL_RCC_PWR_CLK_ENABLE();

  /* Disable the Power Voltage Detector */
  HAL_PWR_DisablePVD( );

  /* Set MCU in ULP (Ultra Low Power) */
  //HAL_PWREx_EnableUltraLowPower( ); TODO Ultra Power Mode
#ifdef ENABLE_FAST_WAKEUP
  /*Disable fast wakeUp*/
  //HAL_PWREx_EnableFastWakeUp( );    TODO Fast Wake Up
#else
  HAL_PWREx_DisableFastWakeUp( );
#endif

  /* Configure all IOs in analog input              */
  /* Except PA143 and PA14 (SWCLK and SWD) for debug*/
  /* PA13 and PA14 are configured in debug_init     */
  HW_GpioInit( );
}

/**
  * @brief RTC MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  * @param hrtc: RTC handle pointer
  * @note  Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select
  *        the RTC clock source; in this case the Backup domain will be reset in
  *        order to modify the RTC Clock source, as consequence RTC registers (including
  *        the backup registers) and RCC_CSR register are set to their reset values.
  * @retval None
  */
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
  RCC_OscInitTypeDef 		RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef  	PeriphClkInitStruct;

  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWR_EnableBkUpAccess();

  /*##-1- Configue the RTC clock soucre ######################################*/
  /* -a- Enable LSE Oscillator */
  RCC_OscInitStruct.OscillatorType 	= RCC_OSCILLATORTYPE_LSE | RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.PLL.PLLState 	= RCC_PLL_NONE;
  RCC_OscInitStruct.LSEState 		= RCC_LSE_ON;
  RCC_OscInitStruct.LSIState		= RCC_LSI_OFF;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* -b- Select LSI as RTC clock source */
  PeriphClkInitStruct.PeriphClockSelection 	= RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection 	= RCC_RTCCLKSOURCE_LSE;
  if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /*##-2- Enable the RTC peripheral Clock ####################################*/
  /* Enable RTC Clock */
  __HAL_RCC_RTC_ENABLE();

  /*##-3- Configure the NVIC for RTC Alarm ###################################*/
  HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 0x0, 0);
  HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
}

/**
  * @brief RTC MSP De-Initialization
  *        This function freeze the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  * @param hrtc: RTC handle pointer
  * @retval None
  */
void HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc)
{
  /* Reset peripherals */
  __HAL_RCC_RTC_DISABLE();
}

/**
  * @brief  Alarm A callback.
  * @param  hrtc: RTC handle
  * @retval None
  */
void SW_RTC_ALarm_IrqHandler( void )
{
  TimerIrqHandler( );
}

/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin: Specifies the pins connected to the EXTI line.
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  HW_GPIO_IrqHandler( GPIO_Pin );
}

/**
  * @brief  Gets IRQ number as a function of the GPIO_Pin.
  * @param  GPIO_Pin: Specifies the pins connected to the EXTI line.
  * @retval IRQ number
  */
IRQn_Type MSP_GetIRQn( uint16_t GPIO_Pin)
{
  switch( GPIO_Pin )
  {
    case GPIO_PIN_0:  return EXTI0_IRQn;
    case GPIO_PIN_1:  return EXTI1_IRQn;
    case GPIO_PIN_2:  return EXTI2_IRQn;
    case GPIO_PIN_3:  return EXTI3_IRQn;
    case GPIO_PIN_4:  return EXTI4_IRQn;
    case GPIO_PIN_5:
    case GPIO_PIN_6:
    case GPIO_PIN_7:
    case GPIO_PIN_8:
    case GPIO_PIN_9:  return EXTI9_5_IRQn;
    case GPIO_PIN_10:
    case GPIO_PIN_11:
    case GPIO_PIN_12:
    case GPIO_PIN_13:
    case GPIO_PIN_14:
    case GPIO_PIN_15:
	default: return EXTI15_10_IRQn;
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
