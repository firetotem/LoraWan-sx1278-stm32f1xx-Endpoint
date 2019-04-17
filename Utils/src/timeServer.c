/* Includes ------------------------------------------------------------------*/
#include <time.h>
#include "hw.h"
#include "timeServer.h"
//#include "low_power.h"


/*!
 * safely execute call back
 */
#define exec_cb( _callback_, context )    	\
  do {                          		\
      if( _callback_ == NULL )    		\
      {                           		\
        while(1);                 		\
      }                           		\
      else                        		\
      {                           		\
        _callback_( context );               	\
      }                           		\
  } while(0);



/*!
 * Timers list head pointer
 */
static TimerEvent_t *TimerListHead = NULL;

/*!
 * \brief Adds or replace the head timer of the list.
 *
 * \remark The list is automatically sorted. The list head always contains the
 *         next timer to expire.
 *
 * \param [IN]  obj Timer object to be become the new head
 * \param [IN]  remainingTime Remaining time of the previous head to be replaced
 */
static void TimerInsertNewHeadTimer( TimerEvent_t *obj );

/*!
 * \brief Adds a timer to the list.
 *
 * \remark The list is automatically sorted. The list head always contains the
 *         next timer to expire.
 *
 * \param [IN]  obj Timer object to be added to the list
 * \param [IN]  remainingTime Remaining time of the running head after which the object may be added
 */
static void TimerInsertTimer( TimerEvent_t *obj );

/*!
 * \brief Sets a timeout with the duration "timestamp"
 *
 * \param [IN] timestamp Delay duration
 */
static void TimerSetTimeout( TimerEvent_t *obj );

/*!
 * \brief Check if the Object to be added is not already in the list
 *
 * \param [IN] timestamp Delay duration
 * \retval true (the object is already in the list) or false
 */
static bool TimerExists( TimerEvent_t *obj );

/* +++ */
void TimerInit( TimerEvent_t *obj, void ( *callback )( void *context ) )
{
  obj->Timestamp 	= 0;
  obj->ReloadValue 	= 0;
  obj->IsStarted 	= false;
  obj->IsNext2Expire 	= false;
  obj->Callback 	= callback;
  obj->Context 		= NULL;
  obj->Next 		= NULL;
}

/* +++ */
void TimerSetContext( TimerEvent_t *obj, void* context )
{
  obj->Context = context;
}

/* +++ */
void TimerStart( TimerEvent_t *obj )
{
  uint32_t elapsedTime = 0;

  BACKUP_PRIMASK();

  DISABLE_IRQ( );


  if( ( obj == NULL ) || ( TimerExists( obj ) == true ) )
  {
    RESTORE_PRIMASK( );
    return;
  }
  obj->Timestamp 	= obj->ReloadValue;
  obj->IsStarted 	= true;
  obj->IsNext2Expire 	= false;

  if( TimerListHead == NULL )
  {
    SW_RTC_SetTimeContext();
    TimerInsertNewHeadTimer( obj ); // insert a timeout at now+obj->Timestamp
  }
  else
  {
    elapsedTime = SW_RTC_GetElapsedTime();
    obj->Timestamp += elapsedTime;

    if( obj->Timestamp < TimerListHead->Timestamp )
    {
      TimerInsertNewHeadTimer( obj);
    }
    else
    {
      TimerInsertTimer( obj);
    }
  }
  RESTORE_PRIMASK( );
}


/* +++ */
bool TimerIsStarted( TimerEvent_t *obj )
{
  return obj->IsStarted;
}

void TimerIrqHandler( void )
{
  TimerEvent_t* cur;
  TimerEvent_t* next;

  uint32_t old =  SW_RTC_GetTimeContext();
  uint32_t now =  SW_RTC_GetTimeContext();
  uint32_t DeltaContext = now - old; //intentionnal wrap around

  /* Update timeStamp based upon new Time Reference*/
  /* because delta context should never exceed 2^32*/
  if ( TimerListHead != NULL )
  {
    for (cur=TimerListHead; cur->Next != NULL; cur= cur->Next)
    {
      next =cur->Next;
      if (next->Timestamp > DeltaContext)
      {
        next->Timestamp -= DeltaContext;
      }
      else
      {
        next->Timestamp = 0 ;
      }
    }
  }

  /* execute imediately the alarm callback */
  if ( TimerListHead != NULL )
  {
    cur 		= TimerListHead;
    TimerListHead 	= TimerListHead->Next;
    cur->IsStarted 	= false;
    exec_cb( cur->Callback, cur->Context );
  }


  // remove all the expired object from the list
  while( ( TimerListHead != NULL ) && ( TimerListHead->Timestamp < SW_RTC_GetElapsedTime() ))
  {
   cur 			= TimerListHead;
   TimerListHead 	= TimerListHead->Next;
   cur->IsStarted 	= false;
   exec_cb( cur->Callback, cur->Context );
  }

  /* start the next TimerListHead if it exists AND NOT running */
  if( ( TimerListHead != NULL ) && ( TimerListHead->IsNext2Expire == false ) )
  {
    TimerSetTimeout( TimerListHead );
  }
}

/* +++ */
void TimerStop( TimerEvent_t *obj )
{
  BACKUP_PRIMASK();

  DISABLE_IRQ( );

  TimerEvent_t* prev 	= TimerListHead;
  TimerEvent_t* cur 	= TimerListHead;

  // List is empty or the Obj to stop does not exist
  if( ( TimerListHead == NULL ) || ( obj == NULL ) )
  {
    RESTORE_PRIMASK( );
    return;
  }

  obj->IsStarted = false;

  if( TimerListHead == obj ) // Stop the Head
  {
    if( TimerListHead->IsNext2Expire == true ) // The head is already running
    {

      TimerListHead->IsNext2Expire = false;
      if( TimerListHead->Next != NULL )
      {
        TimerListHead = TimerListHead->Next;
        TimerSetTimeout( TimerListHead );
      }
      else
      {
        SW_RTC_StopAlarm();
        TimerListHead = NULL;
      }
    }
    else // Stop the head before it is started
    {
      if( TimerListHead->Next != NULL )
      {
        TimerListHead = TimerListHead->Next;
      }
      else
      {
        TimerListHead = NULL;
      }
    }
  }
  else // Stop an object within the list
  {
    while( cur != NULL )
    {
      if( cur == obj )
      {
        if( cur->Next != NULL )
        {
          cur = cur->Next;
          prev->Next = cur;
        }
        else
        {
          cur = NULL;
          prev->Next = cur;
        }
        break;
      }
      else
      {
        prev = cur;
        cur = cur->Next;
      }
    }
  }

  RESTORE_PRIMASK( );
}


/* +++ */
void TimerReset( TimerEvent_t *obj )
{
  TimerStop( obj );
  TimerStart( obj );
}

/* +++ */
void TimerSetValue( TimerEvent_t *obj, uint32_t value )
{
  uint32_t minValue 	= 0;
  uint32_t ticks 	= value;

  TimerStop( obj );

  minValue = SW_RTC_GetMinimumTimeout();

  if( ticks < minValue )
  {
    ticks = minValue;
  }

  obj->Timestamp 	= ticks;
  obj->ReloadValue 	= ticks;
}

/* +++ */
TimerTime_t TimerGetCurrentTime( void )
{
//  uint32_t now = HW_RTC_GetTimerValue( );
//  return  HW_RTC_Tick2ms(now);
  return SW_RTC_GetTick();
}

/* +++ */
TimerTime_t TimerGetElapsedTime( TimerTime_t past )
{
//  uint32_t nowInTicks = HW_RTC_GetTimerValue( );
//  uint32_t pastInTicks = HW_RTC_ms2Tick( past );
//  /* intentional wrap around. Works Ok if tick duation below 1ms */
//  return HW_RTC_Tick2ms( nowInTicks- pastInTicks );

  uint32_t nowInTicks = SW_RTC_GetTick();
  return (nowInTicks - past);
}

/* +++ */
static bool TimerExists( TimerEvent_t *obj )
{
  TimerEvent_t* cur = TimerListHead;

  while( cur != NULL )
  {
    if( cur == obj )
    {
      return true;
    }
    cur = cur->Next;
  }
  return false;
}
/* +++ */
static void TimerSetTimeout( TimerEvent_t *obj )
{
  int32_t minTicks = SW_RTC_GetMinimumTimeout();

  obj->IsNext2Expire = true;

  // In case deadline too soon
  if(obj->Timestamp  < (SW_RTC_GetElapsedTime() + minTicks) )
  {
    obj->Timestamp = SW_RTC_GetElapsedTime() + minTicks;
  }
  SW_RTC_SetAlarm( obj->Timestamp );
}

//TimerTime_t TimerTempCompensation( TimerTime_t period, float temperature )
//{
//    return RtcTempCompensation( period, temperature );
//}

/* +++ */
static void TimerInsertTimer( TimerEvent_t *obj)
{
  TimerEvent_t* cur = TimerListHead;
  TimerEvent_t* next = TimerListHead->Next;

  while (cur->Next != NULL )
  {
    if( obj->Timestamp  > next->Timestamp )
    {
        cur = next;
        next = next->Next;
    }
    else
    {
        cur->Next = obj;
        obj->Next = next;
        return;

    }
  }
  cur->Next = obj;
  obj->Next = NULL;
}

/* +++ */
static void TimerInsertNewHeadTimer( TimerEvent_t *obj )
{
  TimerEvent_t* cur = TimerListHead;

  if( cur != NULL )
  {
    cur->IsNext2Expire = false;
  }

  obj->Next 	= cur;
  TimerListHead = obj;
  TimerSetTimeout( TimerListHead );
}
