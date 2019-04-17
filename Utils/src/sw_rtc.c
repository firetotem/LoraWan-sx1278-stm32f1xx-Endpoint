#include "sw_rtc.h"

typedef struct
{

  uint32_t             TickTime;
  SW_RTC_TimeTypeDef_t RTC_Calndr_Time;

} SW_RTC_TimeContextTypeDef_t;


/* Register of sw ticks */
static volatile uint32_t 		SW_TICK_REG = 0;

/* BKU Regs */
static volatile SW_BKU_Reg_t 		SW_BKU_REGS = { 0 };

/* Time */
static SW_RTC_TimeContextTypeDef_t 	SwTimeContext = { 0 };

/* Alarm */
static SW_RTC_AlarmTypeDef_t 		SwAlarm = { 0 };



/*
 * Установка времени. Устанавливается время и потом пересчитывается
 * регистор тиков.
 */
static void     SW_RTC_SetTime(SW_RTC_TimeTypeDef_t *sTime);
static uint32_t SW_RTC_GetTime(SW_RTC_TimeTypeDef_t *sTime);
static uint32_t SW_RTC_GetCalndrValue( SW_RTC_TimeTypeDef_t *sTime );

uint32_t SW_RTC_GetMinimumTimeout( void )
{
    return ( MIN_ALARM_DELAY );
}


uint32_t SW_RTC_GetTick( void )
{
  return SW_TICK_REG;
}

/* Rtc irq handler */
void SW_RTC_IrqHandler( void )
{
  SW_TICK_REG++;

  if (SwAlarm.AlarmIsActive)
  {
      if (SwAlarm.AlarmTick_Time <= SW_TICK_REG)
      {
	  SW_RTC_ALarm_IrqHandler();
      }
  }

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


static uint32_t SW_RTC_GetTime( SW_RTC_TimeTypeDef_t *sTime )
{
    uint32_t calendar_time 	= SW_TICK_REG;
    uint32_t hour 		= 0;
    uint32_t minute 		= 0;
    uint32_t second		= 0;
    uint32_t subsecond		= 0;

    hour  	= calendar_time / TICKS_IN_HOUR;
    minute	= calendar_time % TICKS_IN_HOUR;
    second      = (calendar_time % TICKS_IN_HOUR) / TICKS_IN_SECOND;
    subsecond   = calendar_time % TICKS_IN_SECOND;

    sTime->Hours 	= hour;
    sTime->Minutes 	= minute;
    sTime->Seconds      = second;
    sTime->SubSecond    = subsecond;

    return calendar_time;
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
  SwTimeContext.TickTime = SW_RTC_GetCalndrValue( &SwTimeContext.RTC_Calndr_Time );
  return SwTimeContext.TickTime;
}


/* Get Calendar value in ticks */
static uint32_t SW_RTC_GetCalndrValue( SW_RTC_TimeTypeDef_t *sTime )
{
    uint32_t calendar_time 	= SW_TICK_REG;
    uint32_t hour 		= 0;
    uint32_t minute 		= 0;
    uint32_t second		= 0;
    uint32_t subsecond		= 0;
    uint32_t total_ticks	= calendar_time;


    while ( calendar_time / TICKS_IN_HOUR )
    {
	hour++;
	calendar_time -= TICKS_IN_HOUR;
    }

    while ( calendar_time / TICKS_IN_MINUTE )
    {
	minute++;
	calendar_time -= TICKS_IN_MINUTE;
    }

    while ( calendar_time / TICKS_IN_SECOND )
    {
	second++;
	calendar_time -= TICKS_IN_SECOND;
    }

    subsecond = calendar_time;

    sTime->Hours 	= hour;
    sTime->Minutes 	= minute;
    sTime->Seconds      = second;
    sTime->SubSecond    = subsecond;

    return total_ticks;
}

uint32_t SW_RTC_GetCalendareTime( uint16_t *mSeconds )
{
    SW_RTC_TimeTypeDef_t RTC_TimeStruct;
    uint32_t 		 ticks;
    uint32_t		 seconds;

    ticks 	= SW_RTC_GetCalndrValue( &RTC_TimeStruct );
    seconds	= ticks / TICKS_IN_SECOND;
    *mSeconds   = (uint16_t)(ticks % TICKS_IN_SECOND);

    return seconds;
}

void SW_RTC_BKUWrite( uint32_t second, uint32_t subSecond )
{

  SW_BKU_REGS.BKU_Seconds 	= second;
  SW_BKU_REGS.BKU_SubSeconds 	= subSecond;

}


void SW_RTC_BKURead ( uint32_t *second, uint32_t *subSecond )
{

  *second 	= SW_BKU_REGS.BKU_Seconds;
  *subSecond	= SW_BKU_REGS.BKU_SubSeconds;

}


void SW_RTC_SetAlarm( uint32_t timeout )
{

    SwAlarm.AlarmTick_Time = SW_TICK_REG + timeout;
    SwAlarm.AlarmIsActive = true;

}

uint32_t SW_RTC_GetElapsedTime( void )
{
   return (SW_TICK_REG - SwTimeContext.TickTime);
}

void SW_RTC_StopAlarm( void )
{

  SwAlarm.AlarmIsActive 	= false;
  SwAlarm.AlarmTick_Time 	= 0;

}

