#include <stddef.h>
#include "Commissioning.h"
#include "timeServer.h"
#include "utilities.h"
#include "hw.h"
#include "version.h"
#include "Ra_02.h"

#ifndef ACTIVE_REGION

  #warning "No active region defined, LORAMAC_REGION_EU868 will be used as default."

  #define ACTIVE_REGION	LORAMAC_REGION_EU868

#endif




/*!
 * Timer to handle the application LED
 */
static TimerEvent_t TxLedTimer;
static void OnTimerLedEvent( void *context )
{
  BSP_Led_Toggle();
}

/*!
 * Initialises the Lora Parameters
 */
//static LoRaParam


int main(void) {

  /* STM32 HAL Library initialization */
  HAL_Init();
  /* Configure the system clock */
  SystemClock_Config();

  /* TODO Configure the debug mode*/
  // DBG_Init();

  /* Configure the hardware */
  HW_Init();

  PRINTF("VERSION: %X\n\r", VERSION);

  TimerInit( &TxLedTimer, OnTimerLedEvent );
  TimerSetValue( &TxLedTimer, 300 );
  BSP_Led_On();
  TimerStart( &TxLedTimer );








  uint8_t ID[8] = {0};
  HW_GetUniqueId(ID);

	LOG( "Console is enabled\n\r");
	LOG( "ChipId    : 0x%x%x%x%x%x%x%x%x\n\r", ID[0], ID[1], ID[2], ID[3], ID[4], ID[5], ID[6], ID[7] );
	LOG( "CPU SysClk: %d Hz\n\r", HAL_RCC_GetSysClockFreq());

	LOG( "CPU Temp: %d.%d C\n\r",    HW_GetTemperatureLevel()/10, HW_GetTemperatureLevel()%10 );
	LOG( "CPU Voltage: %d.%d V\n\r", HW_GetBatteryLevel()/1000,   HW_GetBatteryLevel()%1000   );


	SX1276Reset();




	LOG( "Ra-2 Version: 0x%x \n\r", SX1276Read( REG_VERSION ) );

	LOG( "Ra-2 RegPllHop: 0x%x \n\r", SX1276Read( REG_TEMP ) );

	LOG( "RSSI value: %d \n\r", Radio.Rssi(0));

	//BSP_Led_On();
	//BSP_Led_Off();





	while (1) {
	    TimerStart( &TxLedTimer );
	}

}


