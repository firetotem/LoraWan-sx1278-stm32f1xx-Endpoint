#ifndef INC_CONSOLE_H_
#define INC_CONSOLE_H_

#ifdef __cplusplus
extern "C"
{
#endif

  /* Exported types ------------------------------------------------------------*/
  /* External variables --------------------------------------------------------*/
  /* Exported macros -----------------------------------------------------------*/
  /* Exported functions ------------------------------------------------------- */

  /**
   * @brief TraceInit Initializes Logging feature.
   * @brief Initializes the queue and the hardware
   * @param:  None
   * @retval: None
   */
  void ConsoleInit( void );

  /**
   * @brief TraceSend decode the strFormat and post it to the circular queue for printing
   *
   * @param:  None
   * @retval: 0 when ok, -1 when circular queue is full
   */
  int32_t ConsoleSend( const char *strFormat, ...);

  /**
   * @brief  TraceGetFileName: Return filename string extracted from full path information
   * @param  *fullPath Fullpath string (path + filename)
   * @retval char* Pointer on filename string
   */
  const char *ConsoleGetFileName( const char *fullpath );

#ifdef __cplusplus
}
#endif

#endif /* INC_CONSOLE_H_ */
