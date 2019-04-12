#ifndef INC_UTILITIES_H_
#define INC_UTILITIES_H_

#include "console.h"
#include "hw_conf.h"
#include "systime.h"

/* BACKUP_PRIMASK MUST be implemented at the begining of the funtion
   that implement a critical section
   PRIMASK is saved on STACK and recovered at the end of the funtion
   That way RESTORE_PRIMASK ensures critical sections are maintained even in nested calls...*/
#define BACKUP_PRIMASK()  	uint32_t primask_bit=__get_PRIMASK()
#define DISABLE_IRQ() 		__disable_irq()
#define ENABLE_IRQ() 		__enable_irq()
#define RESTORE_PRIMASK() 	__set_PRIMASK(primask_bit)


#define CRITICAL_SECTION_BEGIN( )     	uint32_t primask_bit=__get_PRIMASK();\
					__disable_irq()

#define CRITICAL_SECTION_END( )   	__set_PRIMASK(primask_bit)

#define LOG(...)     do{ TraceSend(__VA_ARGS__); } while(0);

/* prepocessor directive to align buffer*/
#define ALIGN(n)             __attribute__((aligned(n)))

/* delay definition */
 #define DelayMs(n)             HAL_Delay(n)

#define PPRINTF(...)     do{ } while( 0!= TraceSend(__VA_ARGS__) ) //Polling Mode
#define PRINTF(...)     do{  TraceSend(__VA_ARGS__); } while(0)
#define PRINTNOW()      do{                                                           \
                          SysTime_t stime  =SysTimeGetMcuTime();                      \
                          TraceSend("%3ds%03d: ",stime.Seconds, stime.SubSeconds); \
                         }while(0)

#define TVL1(X)    do{ if(VERBOSE_LEVEL>=VERBOSE_LEVEL_1) { X } } while(0);
#define TVL2(X)    do{ if(VERBOSE_LEVEL>=VERBOSE_LEVEL_2) { X } } while(0);


typedef uint32_t TimerTime_t;


/*!
 * \brief Returns the minimum value between a and b
 *
 * \param [IN] a 1st value
 * \param [IN] b 2nd value
 * \retval minValue Minimum value
 */
#define MIN( a, b ) ( ( ( a ) < ( b ) ) ? ( a ) : ( b ) )

/*!
 * \brief Returns the maximum value between a and b
 *
 * \param [IN] a 1st value
 * \param [IN] b 2nd value
 * \retval maxValue Maximum value
 */
#define MAX( a, b ) ( ( ( a ) > ( b ) ) ? ( a ) : ( b ) )

/*!
 * \brief Returns 2 raised to the power of n
 *
 * \param [IN] n power value
 * \retval result of raising 2 to the power n
 */
#define POW2( n ) ( 1 << n )

/*!
 * Version
 */
typedef union Version_u
{
    struct Version_s
    {
        uint8_t Rfu;
        uint8_t Revision;
        uint8_t Minor;
        uint8_t Major;
    }Fields;
    uint32_t Value;
}Version_t;

/*!
 * \brief Initializes the pseudo random generator initial value
 *
 * \param [IN] seed Pseudo random generator initial value
 */
void srand1( uint32_t seed );

/*!
 * \brief Computes a random number between min and max
 *
 * \param [IN] min range minimum value
 * \param [IN] max range maximum value
 * \retval random random value in range min..max
 */
int32_t randr( int32_t min, int32_t max );

/*!
 * \brief Computes a random number between
 *
 * \retval random random value in range min..max
 */
int32_t rand1( void );

/*!
 * \brief Copies size elements of src array to dst array
 *
 * \remark STM32 Standard memcpy function only works on pointers that are aligned
 *
 * \param [OUT] dst  Destination array
 * \param [IN]  src  Source array
 * \param [IN]  size Number of bytes to be copied
 */
void memcpy1( uint8_t *dst, const uint8_t *src, uint16_t size );

/*!
 * \brief Copies size elements of src array to dst array reversing the byte order
 *
 * \param [OUT] dst  Destination array
 * \param [IN]  src  Source array
 * \param [IN]  size Number of bytes to be copied
 */
void memcpyr( uint8_t *dst, const uint8_t *src, uint16_t size );

/*!
 * \brief Set size elements of dst array with value
 *
 * \remark STM32 Standard memset function only works on pointers that are aligned
 *
 * \param [OUT] dst   Destination array
 * \param [IN]  value Default value
 * \param [IN]  size  Number of bytes to be copied
 */
void memset1( uint8_t *dst, uint8_t value, uint16_t size );

/*!
 * \brief Converts a nibble to an hexadecimal character
 *
 * \param [IN] a   Nibble to be converted
 * \retval hexChar Converted hexadecimal character
 */
int8_t Nibble2HexChar( uint8_t a );


#endif /* INC_UTILITIES_H_ */
