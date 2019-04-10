#include "bsp.h"

void BSP_Led_Init( void )
{
  GPIO_InitTypeDef initStruc = {0};

  initStruc.Mode 	= GPIO_MODE_OUTPUT_PP;
  initStruc.Pin 	= RADIO_LEDRX_PIN;
  initStruc.Pull 	= GPIO_NOPULL;
  initStruc.Speed 	= GPIO_SPEED_FREQ_LOW;

  HAL_GPIO_Init( RADIO_LEDRX_PORT, &initStruc );
  HAL_GPIO_WritePin( RADIO_LEDRX_PORT, RADIO_LEDRX_PIN, SET);
  __HAL_RCC_GPIOC_CLK_ENABLE();
}


void BSP_Led_On( void )
{
  HAL_GPIO_WritePin( RADIO_LEDRX_PORT, RADIO_LEDRX_PIN, RESET);
}


void BSP_Led_Off( void )
{
  HAL_GPIO_WritePin( RADIO_LEDRX_PORT, RADIO_LEDRX_PIN, SET);
}


void BSP_Led_Toggle( void )
{
  HAL_GPIO_TogglePin( RADIO_LEDRX_PORT, RADIO_LEDRX_PIN );
}


void BSP_Radio_Reset_Init( void )
{
  GPIO_InitTypeDef initStruc = {0};

  initStruc.Mode 	= GPIO_MODE_OUTPUT_PP;
  initStruc.Pin 	= RADIO_RESET_PIN;
  initStruc.Pull 	= GPIO_NOPULL;
  initStruc.Speed 	= GPIO_SPEED_FREQ_LOW;

  HAL_GPIO_Init( RADIO_RESET_PORT, &initStruc );
  __HAL_RCC_GPIOA_CLK_ENABLE();
}


void BSP_Radio_Reset_On( void )
{
  HAL_GPIO_WritePin( RADIO_RESET_PORT, RADIO_RESET_PIN, SET );
}


void BSP_Radio_Reset_Off( void )
{
  HAL_GPIO_WritePin( RADIO_RESET_PORT, RADIO_RESET_PIN, RESET );
}
