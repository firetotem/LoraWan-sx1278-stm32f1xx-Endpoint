#include "hw.h"


static ADC_HandleTypeDef hadc;

static bool AdcInitialized = false;

static uint16_t HW_Adc_ReadChannel( uint32_t channel );

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
  RCC_PeriphCLKInitTypeDef 	PeriphClkInit;

  ADCCLK_ENABLE();

  PeriphClkInit.PeriphClockSelection 	= RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection	= RCC_ADCPCLK2_DIV8;


  HAL_RCCEx_PeriphCLKConfig( &PeriphClkInit );
}


void HW_Adc_Init( void )
{
  if ( AdcInitialized == false )
  {
      hadc.Instance 			= ADCx;
      hadc.Init.DataAlign		= ADC_DATAALIGN_RIGHT;
      hadc.Init.ContinuousConvMode	= ENABLE;
      hadc.Init.DiscontinuousConvMode   = DISABLE;
      hadc.Init.ScanConvMode		= ADC_SCAN_DISABLE;
      hadc.Init.NbrOfConversion		= 1;
      hadc.Init.NbrOfDiscConversion	= 1;
      hadc.Init.ExternalTrigConv	= ADC_SOFTWARE_START;

      if ( HAL_ADC_Init(&hadc) != HAL_OK )
      {
	 Error_Handler();
      }

      AdcInitialized = true;
  }
}


void HW_Adc_DeInit( void )
{
  HAL_ADC_DeInit(&hadc);
  AdcInitialized = false;
}


static uint16_t HW_Adc_ReadChannel( uint32_t channel )
{
  ADC_ChannelConfTypeDef sConfig;
  uint16_t adcSample;

  sConfig.Channel	= channel;
  sConfig.Rank		= ADC_REGULAR_RANK_1;
  sConfig.SamplingTime	= ADC_SAMPLETIME_239CYCLES_5;

  if ( HAL_ADC_ConfigChannel( &hadc, &sConfig ) != HAL_OK )
  {
      Error_Handler();
  }

  HAL_ADC_Start( &hadc );
  HAL_ADC_PollForConversion( &hadc, HAL_MAX_DELAY );
  adcSample = HAL_ADC_GetValue( &hadc );
  HAL_ADC_Stop( &hadc );

  return adcSample;
}


uint16_t HW_GetTemperatureLevel( void )
{
  uint16_t adcSample = HW_Adc_ReadChannel( ADC_CHANNEL_TEMPSENSOR );

  adcSample = (uint32_t) ((adcSample*3300/4096 - 1430)*10/43 + 250);



  //adcSample = (int32_t)( ( (143 - adcSample*100) / (43) ) + 250 );


  //Temperature (in °C) = {(V25 - VSENSE) / Avg_Slope} + 25.

  return adcSample;
}

uint16_t HW_GetBatteryLevel( void )
{
  uint16_t vbatLevel = HW_Adc_ReadChannel( ADC_CHANNEL_VREFINT );

  vbatLevel *= 33000;
  vbatLevel /= 4096;

  return vbatLevel;
}
