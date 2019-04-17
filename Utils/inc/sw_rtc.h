#ifndef INC_SW_RTC_H_
#define INC_SW_RTC_H_

/* Includes */
#include <stdint.h>
#include <stdbool.h>

/* Exported types */

/* Тип времени */
typedef struct
{

  uint32_t SubSecond;
  uint32_t Seconds;
  uint32_t Minutes;
  uint32_t Hours;

} SW_RTC_TimeTypeDef_t;

/* Тип будильника */
typedef struct
{

  SW_RTC_TimeTypeDef_t AlarmTime;
  uint32_t	       AlarmTick_Time;
  bool  	       AlarmIsActive;

} SW_RTC_AlarmTypeDef_t;

/* Type for dummy backup regs */
typedef struct
{

  uint32_t BKU_Seconds;
  uint32_t BKU_SubSeconds;

} SW_BKU_Reg_t;

/* Exported macros */
#define RTC_START_TIME_SECOND	0UL
#define RTC_START_TIME_MINUTE	0UL
#define RTC_START_TIME_HOUR	8UL

#define TICKS_IN_SECOND		1000UL
#define TICKS_IN_MINUTE		TICKS_IN_SECOND*60
#define TICKS_IN_HOUR		TICKS_IN_MINUTE*60

#define MIN_ALARM_DELAY  	3 /* in ticks */

/* Initialization of SW RTC */
void SW_RTC_Init( void );

/*
 * RTC Irq handler;
 */
void SW_RTC_IrqHandler( void );

/*
 * Alarm Irq handler;
 */
void SW_Alarm_IrqHandler( void );


uint32_t SW_RTC_GetTick( void );

/*
 * Get time context;
 */
uint32_t SW_RTC_GetTimeContext( void );

/*
 * Get Calendar value in ticks;
 */
uint32_t SW_RTC_SetTimeContext( void );


uint32_t SW_RTC_GetElapsedTime( void );

uint32_t SW_RTC_GetMinimumTimeout( void );

uint32_t SW_RTC_GetCalendareTime( uint16_t *mSeconds );

void SW_RTC_BKUWrite( uint32_t second, uint32_t subSecond );
void SW_RTC_BKURead ( uint32_t *second, uint32_t *subSecond );

/*
 * Alarm Irq handler;
 */
void SW_RTC_ALarm_IrqHandler( void );

void SW_RTC_SetAlarm( uint32_t timeout );

void SW_RTC_StopAlarm( void );

#endif
