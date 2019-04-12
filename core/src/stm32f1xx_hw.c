#include "utilities.h"
#include "hw.h"
#include "radio.h"
#include "vcom.h"

/*!
 *  \brief Unique Devices IDs register set ( STM32F1xxx )
 */
#define	ID1                                 ( 0x1FFFF7E8 )
#define ID2                                 ( 0x1FFFF7EC )
#define ID3                                 ( 0x1FFFF7F0 )

/*!
 * \brief ADC Vbat measurement constants
 */

#define LORAWAN_MAX_BAT   254

/*!
 * Flag to indicate if the MCU is Initialized
 */
static bool McuInitialized = false;

/**
  * @brief This function initializes the hardware
  * @param None
  * @retval None
  */
void HW_Init( void )
{
  if( McuInitialized == false )
  {
    HW_Adc_Init();

    Radio.IoInit( );

    HW_SPI_Init( );

    HW_RTC_Init( );

    ConsoleInit();

    BSP_Led_Init();

    BSP_Radio_Reset_Init();

    McuInitialized = true;
  }
}

/**
  * @brief This function Deinitializes the hardware
  * @param None
  * @retval None
  */
void HW_DeInit( void )
{
  HW_Adc_DeInit();

  HW_SPI_DeInit( );

  Radio.IoDeInit( );

  vcom_DeInit( );

  McuInitialized = false;
}

/**
  * @brief This function Initializes the hardware Ios
  * @param None
  * @retval None
  */
static void HW_IoInit( void )
{
  Radio.IoInit( );
  vcom_IoInit( );
}

/**
  * @brief This function Deinitializes the hardware Ios
  * @param None
  * @retval None
  */
static void HW_IoDeInit( void )
{
  Radio.IoDeInit( );
  vcom_IoDeInit( );
}


void HW_GpioInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct={0};

  /* Configure all GPIO as analog to reduce current consumption on non used IOs */
  /* Enable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();

  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  /* All GPIOs except debug pins (SWCLK and SWD) */
  GPIO_InitStruct.Pin = GPIO_PIN_All & (~( GPIO_PIN_13 | GPIO_PIN_14) );
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* All GPIOs */
  GPIO_InitStruct.Pin = GPIO_PIN_All;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /* Disable GPIOs clock */
  //__HAL_RCC_GPIOA_CLK_DISABLE();
  //__HAL_RCC_GPIOB_CLK_DISABLE();
  //__HAL_RCC_GPIOC_CLK_DISABLE();
  //__HAL_RCC_GPIOD_CLK_DISABLE();
  //__HAL_RCC_GPIOE_CLK_DISABLE();
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 32000000
  *            HCLK(Hz)                       = 32000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 8000000
  *            PLLMUL                         = 8
  *            Flash Latency(WS)              = 1
  * @retval None
  */
void SystemClock_Config( void )
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* Enable HSE Oscillator and Activate PLL with HSE as source -------------*/
  /* PLL configuration: PLLCLK = (HSI / 2) * PLLMUL = ( 8 / 2) * 8 = 32 MHz */
  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSEState		= RCC_HSE_OFF;
  RCC_OscInitStruct.LSEState		= RCC_LSE_OFF;
  RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL          = RCC_PLL_MUL8;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  __HAL_RCC_PWR_CLK_ENABLE();

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
  clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}
/**
  * @brief This function return a random seed
  * @note based on the device unique ID
  * @param None
  * @retval see
  */
uint32_t HW_GetRandomSeed( void )
{
  return ( ( *( uint32_t* )ID1 ) ^ ( *( uint32_t* )ID2 ) ^ ( *( uint32_t* )ID3 ) );
}

/**
  * @brief This function return a unique ID
  * @param unique ID
  * @retval none
  */
void HW_GetUniqueId( uint8_t *id )
{
    id[7] = ( ( *( uint32_t* )ID1 )+ ( *( uint32_t* )ID3 ) ) >> 24;
    id[6] = ( ( *( uint32_t* )ID1 )+ ( *( uint32_t* )ID3 ) ) >> 16;
    id[5] = ( ( *( uint32_t* )ID1 )+ ( *( uint32_t* )ID3 ) ) >> 8;
    id[4] = ( ( *( uint32_t* )ID1 )+ ( *( uint32_t* )ID3 ) );
    id[3] = ( ( *( uint32_t* )ID2 ) ) >> 24;
    id[2] = ( ( *( uint32_t* )ID2 ) ) >> 16;
    id[1] = ( ( *( uint32_t* )ID2 ) ) >> 8;
    id[0] = ( ( *( uint32_t* )ID2 ) );
}


/**
  * @brief Enters Low Power Stop Mode
  * @note ARM exists the function when waking up
  * @param none
  * @retval none
  */
void LPM_EnterStopMode( void)
{
  BACKUP_PRIMASK();

  DISABLE_IRQ( );

  HW_IoDeInit( );

  /*clear wake up flag*/
  SET_BIT(PWR->CR, PWR_CR_CWUF);

  RESTORE_PRIMASK( );

  /* Enter Stop Mode */
  HAL_PWR_EnterSTOPMode ( PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI );
}
/**
  * @brief Exists Low Power Stop Mode
  * @note Enable the pll at 32MHz
  * @param none
  * @retval none
  */
void LPM_ExitStopMode( void)
{
  /* Disable IRQ while the MCU is not running on HSI */

  BACKUP_PRIMASK();

  DISABLE_IRQ( );

  /* After wake-up from STOP reconfigure the system clock */
  /* Enable HSI */
  __HAL_RCC_HSI_ENABLE();

  /* Wait till HSI is ready */
  while( __HAL_RCC_GET_FLAG(RCC_FLAG_HSIRDY) == RESET ) {}

  /* Enable PLL */
  __HAL_RCC_PLL_ENABLE();
  /* Wait till PLL is ready */
  while( __HAL_RCC_GET_FLAG( RCC_FLAG_PLLRDY ) == RESET ) {}

  /* Select PLL as system clock source */
  __HAL_RCC_SYSCLK_CONFIG ( RCC_SYSCLKSOURCE_PLLCLK );

  /* Wait till PLL is used as system clock source */
  while( __HAL_RCC_GET_SYSCLK_SOURCE( ) != RCC_SYSCLKSOURCE_STATUS_PLLCLK ) {}

  /*initilizes the peripherals*/
  HW_IoInit( );

  RESTORE_PRIMASK( );
}

/**
  * @brief Enters Low Power Sleep Mode
  * @note ARM exits the function when waking up
  * @param none
  * @retval none
  */
void LPM_EnterSleepMode( void)
{
    HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
}







