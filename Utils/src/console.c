#include "queue.h"
#include "console.h"
#include "utilities.h"
#include "hw.h"

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

#define TEMPBUFSIZE 256

/* Private variables ---------------------------------------------------------*/
static queue_param_t MsgTraceQueue;
static uint8_t MsgTraceQueueBuff[DBG_TRACE_MSG_QUEUE_SIZE];

__IO ITStatus	TracePeripheralReady = SET;

/* Private function prototypes -----------------------------------------------*/

/**
 * @brief  Trace buffer Transfer completed callback
 * @param  none
 * @note   Indicate the end of the transmission of a  trace buffer. If queue
 *         contains new trace data to transmit, start a new transmission.
 * @retval None
 */
static void Console_TxCpltCallback(void);

/* Functions Definition ------------------------------------------------------*/
void ConsoleInit( void )
{
  OutputInit( Console_TxCpltCallback );

  circular_queue_init( &MsgTraceQueue, MsgTraceQueueBuff, DBG_TRACE_MSG_QUEUE_SIZE );

  return;
}

int32_t TraceSend( const char *strFormat, ...)
{
  char buf[ TEMPBUFSIZE ];
  va_list vaArgs;
  uint8_t* buffer;
  va_start( vaArgs, strFormat);
  uint16_t bufSize = vsnprintf( buf, TEMPBUFSIZE, strFormat, vaArgs );
  va_end(vaArgs);
  int status = 0;

  BACKUP_PRIMASK();

  DISABLE_IRQ(); /**< Disable all interrupts by setting PRIMASK bit on Cortex*/
  //DBG_GPIO_SET(GPIOB, GPIO_PIN_15);
  //DBG_GPIO_RST(GPIOB, GPIO_PIN_15);
  status = circular_queue_add(&MsgTraceQueue,(uint8_t*)buf, bufSize);

  if ( (status == 0 ) && ( TracePeripheralReady == SET ) )
  {
    circular_queue_get( &MsgTraceQueue, &buffer, &bufSize );
    TracePeripheralReady = RESET;
    //DBG_GPIO_RST(GPIOB, GPIO_PIN_12);

    //TODO LPM_SetStopMode( LPM_UART_TX_Id, LPM_Disable );

    RESTORE_PRIMASK();
    OutputTrace( buffer, bufSize );
  }
  else
  {
    RESTORE_PRIMASK();
  }

  return status;
}


const char *TraceGetFileName(const char *fullpath)
{
  const char *ret = fullpath;

  if (strrchr(fullpath, '\\') != NULL)
  {
    ret = strrchr(fullpath, '\\') + 1;
  }
  else if (strrchr(fullpath, '/') != NULL)
  {
    ret = strrchr(fullpath, '/') + 1;
  }

  return ret;
}


/* Private Functions Definition ------------------------------------------------------*/

static void Console_TxCpltCallback(void)
{
  int status;
  uint8_t* buffer;
  uint16_t bufSize;

  BACKUP_PRIMASK();

  DISABLE_IRQ(); /**< Disable all interrupts by setting PRIMASK bit on Cortex*/
  /* Remove element just sent to UART */
  circular_queue_remove(&MsgTraceQueue);
  //DBG_GPIO_SET(GPIOB, GPIO_PIN_13);
  //DBG_GPIO_RST(GPIOB, GPIO_PIN_13);
  /* Sense if new data to be sent */
  status=circular_queue_sense(&MsgTraceQueue);

  if ( status == 0)
  {
    circular_queue_get(&MsgTraceQueue,&buffer,&bufSize);
    RESTORE_PRIMASK();
    //DBG_GPIO_SET(GPIOB, GPIO_PIN_14);
    //DBG_GPIO_RST(GPIOB, GPIO_PIN_14);
    OutputTrace(buffer, bufSize);
  }
  else
  {
    //DBG_GPIO_SET(GPIOB, GPIO_PIN_12);

    //TODO LPM_SetStopMode(LPM_UART_TX_Id , LPM_Enable );
    TracePeripheralReady = SET;
    RESTORE_PRIMASK();
  }
}

