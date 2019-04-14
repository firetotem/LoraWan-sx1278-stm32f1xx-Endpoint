#include "sw_rtc.h"

typedef struct
{
	uint32_t TickTime;
	SW_RTC_TimeTypeDef_t RTC_Calndr_Time;

} SW_RTC_TimeContextTypeDef_t;

static SW_RTC_AlarmTypeDef_t SwAlarm = { 0 };

/* Переменная для хранения времени */
static SW_RTC_TimeContextTypeDef_t SwTimeContext = { 0 };

/*
 * Установка времени. Устанавливается время и потом пересчитывается
 * регистор тиков.
 */
static void SW_RTC_SetTime(SW_RTC_TimeTypeDef_t *sTime);


/* Инициализация контекста времени; */
void SW_RTC_Init( void )
{
	SW_RTC_TimeTypeDef_t timeStruct;

	timeStruct.Seconds = RTC_START_TIME_SECOND;
	timeStruct.Minutes = RTC_START_TIME_MINUTE;
	timeStruct.Hours   = RTC_START_TIME_HOUR;

	SW_RTC_SetTime( &timeStruct );
}


/*
 * Установка регистров контекста времени;
 */
static void SW_RTC_SetTime(SW_RTC_TimeTypeDef_t *sTime)
{
	uint32_t tickNum = 0;

	SwTimeContext.RTC_Calndr_Time = *sTime;

	tickNum += sTime->SubSecond;
	tickNum += sTime->Seconds 	* 1000;
	tickNum += sTime->Minutes 	* 1000 * 60;
	tickNum += sTime->Hours 	* 1000 * 60 * 60;

	SwTimeContext.TickTime = tickNum;
}


void SW_RTC_SetAlarm( uint32_t timeout )
{
	uint32_t rtcAlarmSubseconds = 0;
	uint32_t rtcAlarmSeconds    = 0;
	uint32_t rtcAlarmMinutes    = 0;
	uint32_t rtcAlarmHour		= 0;


	rtcAlarmHour = SwTimeContext.RTC_Calndr_Time.Hours;
	while ( timeout >= TICKS_IN_HOUR )
	{
		timeout -= TICKS_IN_HOUR;
		rtcAlarmHour++;
	}






}



/* Обработчик прерывания */
void SW_RTC_IrqHandler( void )
{
	SwTimeContext.TickTime++;
}

