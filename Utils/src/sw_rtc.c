#include "sw_rtc.h"

typedef struct
{

  uint32_t             TickTime;
  SW_RTC_TimeTypeDef_t RTC_Calndr_Time;

} SW_RTC_TimeContextTypeDef_t;



/* Переменная для хранения времени */
static SW_RTC_TimeContextTypeDef_t SwTimeContext = { 0 };

/* Register of sw ticks */
static volatile uint32_t SW_TICK_REG = 0;

/* Alarm */
static SW_RTC_AlarmTypeDef_t SwAlarm = { 0 };



/*
 * Установка времени. Устанавливается время и потом пересчитывается
 * регистор тиков.
 */
static void SW_RTC_SetTime(SW_RTC_TimeTypeDef_t *sTime);


/* Rtc irq handler */
void SW_RTC_IrqHandler( void )
{
  SW_TICK_REG++;
}


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
  SW_TICK_REG = tickNum;
}


/*
 * Get time context;
 */
uint32_t SW_RTC_GetTimeContext( void )
{
  return SwTimeContext.TickTime;
}


/*
 * Get Calendar value in ticks;
 */
uint32_t SW_RTC_SetTimeContext( void )
{
  SwTimeContext.TickTime = SW_RTC_GetCalndrValue( &SwTimeContext );
  return SwTimeContext.TickTime;
}


static uint32_t SW_RTC_GetTime( SW_RTC_TimeTypeDef_t *sTime )
{

}



void SW_RTC_SetAlarm( uint32_t timeout )
{

  uint32_t rtcAlarmSubseconds = 0;
  uint32_t rtcAlarmSeconds    = 0;
  uint32_t rtcAlarmMinutes    = 0;
  uint32_t rtcAlarmHour	      = 0;


	rtcAlarmHour = SwTimeContext.RTC_Calndr_Time.Hours;
	while ( timeout >= TICKS_IN_HOUR )
	{
		timeout -= TICKS_IN_HOUR;
		rtcAlarmHour++;
	}






}





