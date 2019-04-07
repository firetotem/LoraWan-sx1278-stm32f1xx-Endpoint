/* Includes ------------------------------------------------------------------*/
#include "hw.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void DBG_Init(void)
{
#ifdef DEBUG
  GPIO_InitTypeDef  gpioinitstruct = {0};

  /* Enable the GPIO_B Clock */
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* Configure the GPIO pin */
  gpioinitstruct.Mode   = GPIO_MODE_OUTPUT_PP;
  gpioinitstruct.Pull   = GPIO_PULLUP;
  gpioinitstruct.Speed  = GPIO_SPEED_HIGH;

  gpioinitstruct.Pin    = (GPIO_PIN_12 | GPIO_PIN_13| GPIO_PIN_14 | GPIO_PIN_15);
  HAL_GPIO_Init(GPIOB, &gpioinitstruct);

  /* Reset debug Pins */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);

  __HAL_RCC_DBGMCU_CLK_ENABLE( );

  HAL_DBGMCU_EnableDBGSleepMode( );
  HAL_DBGMCU_EnableDBGStopMode( );
  HAL_DBGMCU_EnableDBGStandbyMode( );

#else /* DEBUG */
  /* sw interface off*/
  GPIO_InitTypeDef GPIO_InitStructure ={0};

  GPIO_InitStructure.Mode   = GPIO_MODE_ANALOG;
  GPIO_InitStructure.Pull   = GPIO_NOPULL;
  GPIO_InitStructure.Pin    = (GPIO_PIN_13 | GPIO_PIN_14);
  __GPIOA_CLK_ENABLE() ;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
  __GPIOA_CLK_DISABLE() ;

  __HAL_RCC_DBGMCU_CLK_ENABLE( );
  HAL_DBGMCU_DisableDBGSleepMode( );
  HAL_DBGMCU_DisableDBGStopMode( );
  HAL_DBGMCU_DisableDBGStandbyMode( );
  __HAL_RCC_DBGMCU_CLK_DISABLE( );
#endif
}

void Error_Handler(void)
{
  // TODO: Uncomment this line
  //PRINTF("Error_Handler\n\r");
  while (1)
  {
    ;
  }
}
