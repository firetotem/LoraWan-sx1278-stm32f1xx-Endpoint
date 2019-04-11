#include <stddef.h>
#include "utilities.h"
#include "../../HW_BSP/inc/hw.h"

int main(void) {

	HAL_Init();
	SystemClock_Config();
	HW_Init();

	uint8_t ID[8] = {0};
	HW_GetUniqueId(ID);

	LOG( "Console is enabled\n\r");
	LOG( "ChipId    : 0x%x%x%x%x%x%x%x%x\n\r", ID[0], ID[1], ID[2], ID[3], ID[4], ID[5], ID[6], ID[7] );
	LOG( "CPU SysClk: %d Hz\n\r", HAL_RCC_GetSysClockFreq());

	LOG( "CPU Temp: %d.%d C\n\r",    HW_GetTemperatureLevel()/10, HW_GetTemperatureLevel()%10 );
	LOG( "CPU Voltage: %d.%d V\n\r", HW_GetBatteryLevel()/1000,   HW_GetBatteryLevel()%1000   );


	BSP_Radio_Reset_Off();

	HW_SPI_InOut(0x42);
	LOG( "Ra-2 Version: 0x%x \n\r", HW_SPI_InOut(0x00) );

	HW_SPI_InOut(0x44);
	LOG( "Ra-2 RegPllHop: 0x%x \n\r", HW_SPI_InOut(0x00) );

	BSP_Led_On();
	BSP_Led_Off();





	while (1) {
	    HAL_Delay(1);
	    BSP_Led_Toggle();
	    LOG( "CPU Temp: %d.%d C\n\r", HW_GetTemperatureLevel()/10, HW_GetTemperatureLevel()%10);
	}

}


