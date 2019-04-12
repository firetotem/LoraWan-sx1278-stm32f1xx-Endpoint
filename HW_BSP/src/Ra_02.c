#include "hw.h"
#include "radio.h"
#include "sx1276.h"
#include "Ra_02.h"


#define IRQ_HIGH_PRIORITY	0

static bool RadioIsActive = false;

static void SX1276AntSwInit( void );
static void SX1276AntSwDeInit( void );
void SX1276SetXO( uint8_t state );
uint32_t SX1276GetWakeTime( void );
void SX1276IoIrqInit( DioIrqHandler **irqHandlers );
uint8_t SX1276GetPaSelect( uint32_t channel );
void SX1276SetRfTxPower( int8_t power );
void SX1276SetAntSwLowPower( bool status );
void SX1276SetAntSw( uint8_t opMode );

static LoRaBoardCallback_t	BoardCallBacks = { SX1276SetXO,
						   SX1276GetWakeTime,
						   SX1276IoIrqInit,
						   SX1276SetRfTxPower,
						   SX1276SetAntSwLowPower,
						   SX1276SetAntSw };


/* Public radio interface */
const struct Radio_s Radio =
{
    SX1276IoInit,
    SX1276IoDeInit,
    SX1276Init,
    SX1276GetStatus,
    SX1276SetModem,
    SX1276SetChannel,
    SX1276IsChannelFree,
    SX1276Random,
    SX1276SetRxConfig,
    SX1276SetTxConfig,
    SX1276CheckRfFrequency,
    SX1276GetTimeOnAir,
    SX1276Send,
    SX1276SetSleep,
    SX1276SetStby,
    SX1276SetRx,
    SX1276StartCad,
    SX1276SetTxContinuousWave,
    SX1276ReadRssi,
    SX1276Write,
    SX1276Read,
    SX1276WriteBuffer,
    SX1276ReadBuffer,
    SX1276SetMaxPayloadLength,
    SX1276SetPublicNetwork,
    SX1276GetWakeupTime
};


uint32_t SX1276GetWakeTime( void )
{
  return BOARD_WAKEUP_TIME;
}


void SX1276SetXO( uint8_t state )
{
}


void SX1276IoIrqInit( DioIrqHandler **irqHandlers )
{
  HW_GPIO_SetIrq( RADIO_DIO_0_PORT, RADIO_DIO_0_PIN, IRQ_HIGH_PRIORITY, irqHandlers[0] );
  HW_GPIO_SetIrq( RADIO_DIO_1_PORT, RADIO_DIO_1_PIN, IRQ_HIGH_PRIORITY, irqHandlers[1] );
  HW_GPIO_SetIrq( RADIO_DIO_2_PORT, RADIO_DIO_2_PIN, IRQ_HIGH_PRIORITY, irqHandlers[2] );
  HW_GPIO_SetIrq( RADIO_DIO_3_PORT, RADIO_DIO_3_PIN, IRQ_HIGH_PRIORITY, irqHandlers[3] );
}


void SX1276IoInit( void )
{
  GPIO_InitTypeDef initStruct = { 0 };

  SX1276BoardInit( &BoardCallBacks );

  initStruct.Mode	= GPIO_MODE_IT_RISING;
  initStruct.Pull	= GPIO_PULLDOWN;
  initStruct.Speed	= GPIO_SPEED_FREQ_HIGH;

  HW_GPIO_Init( RADIO_DIO_0_PORT, RADIO_DIO_0_PIN, &initStruct );
  HW_GPIO_Init( RADIO_DIO_1_PORT, RADIO_DIO_1_PIN, &initStruct );
  HW_GPIO_Init( RADIO_DIO_2_PORT, RADIO_DIO_2_PIN, &initStruct );
  HW_GPIO_Init( RADIO_DIO_3_PORT, RADIO_DIO_3_PIN, &initStruct );
}

void SX1276IoDeInit( void )
{
  GPIO_InitTypeDef initStruct={0};

  initStruct.Mode = GPIO_MODE_IT_RISING ;
  initStruct.Pull = GPIO_PULLDOWN;

  HW_GPIO_Init( RADIO_DIO_0_PORT, RADIO_DIO_0_PIN, &initStruct );
  HW_GPIO_Init( RADIO_DIO_1_PORT, RADIO_DIO_1_PIN, &initStruct );
  HW_GPIO_Init( RADIO_DIO_2_PORT, RADIO_DIO_2_PIN, &initStruct );
  HW_GPIO_Init( RADIO_DIO_3_PORT, RADIO_DIO_3_PIN, &initStruct );
}


void SX1276SetRfTxPower( int8_t power )
{
  uint8_t paConfig 	= 0;
  uint8_t paDac 	= 0;

  paConfig 	= SX1276Read( REG_PACONFIG );
  paDac 	= SX1276Read( REG_PADAC );

  paConfig = ( paConfig & RF_PACONFIG_PASELECT_MASK )  | SX1276GetPaSelect( SX1276.Settings.Channel );
  paConfig = ( paConfig & RF_PACONFIG_MAX_POWER_MASK ) | 0x70;

  if( ( paConfig & RF_PACONFIG_PASELECT_PABOOST ) == RF_PACONFIG_PASELECT_PABOOST )
      {
          if( power > 17 )
          {
              paDac = ( paDac & RF_PADAC_20DBM_MASK ) | RF_PADAC_20DBM_ON;
          }
          else
          {
              paDac = ( paDac & RF_PADAC_20DBM_MASK ) | RF_PADAC_20DBM_OFF;
          }
          if( ( paDac & RF_PADAC_20DBM_ON ) == RF_PADAC_20DBM_ON )
          {
              if( power < 5 )
              {
                  power = 5;
              }
              if( power > 20 )
              {
                  power = 20;
              }
              paConfig = ( paConfig & RF_PACONFIG_OUTPUTPOWER_MASK ) | ( uint8_t )( ( uint16_t )( power - 5 ) & 0x0F );
          }
          else
          {
              if( power < 2 )
              {
                  power = 2;
              }
              if( power > 17 )
              {
                  power = 17;
              }
              paConfig = ( paConfig & RF_PACONFIG_OUTPUTPOWER_MASK ) | ( uint8_t )( ( uint16_t )( power - 2 ) & 0x0F );
          }
      }
      else
      {
          if( power < -1 )
          {
              power = -1;
          }
          if( power > 14 )
          {
              power = 14;
          }
          paConfig = ( paConfig & RF_PACONFIG_OUTPUTPOWER_MASK ) | ( uint8_t )( ( uint16_t )( power + 1 ) & 0x0F );
      }
      SX1276Write( REG_PACONFIG, paConfig );
      SX1276Write( REG_PADAC, paDac );
}


uint8_t SX1276GetPaSelect( uint32_t channel )
{
    if( channel < RF_MID_BAND_THRESH )
    {
        return RF_PACONFIG_PASELECT_RFO;
    }
    else
    {
        return RF_PACONFIG_PASELECT_PABOOST;
    }
}


void SX1276SetAntSwLowPower( bool status )
{
    if( RadioIsActive != status )
    {
        RadioIsActive = status;

        if( status == false )
        {
            SX1276AntSwInit( );
        }
        else
        {
            SX1276AntSwDeInit( );
        }
    }
}


static void SX1276AntSwInit( void )
{
//  GPIO_InitTypeDef initStruct={0};
//
//  initStruct.Mode =GPIO_MODE_OUTPUT_PP;
//  initStruct.Pull = GPIO_NOPULL;
//  initStruct.Speed = GPIO_SPEED_HIGH;
//
//  HW_GPIO_Init( RADIO_ANT_SWITCH_PORT, RADIO_ANT_SWITCH_PIN, &initStruct  );
//  HW_GPIO_Write( RADIO_ANT_SWITCH_PORT, RADIO_ANT_SWITCH_PIN, RADIO_ANT_SWITCH_SET_RX);
}


static void SX1276AntSwDeInit( void )
{
//  GPIO_InitTypeDef initStruct={0};
//
//  initStruct.Mode = GPIO_MODE_OUTPUT_PP ;
//
//  initStruct.Pull = GPIO_NOPULL;
//  initStruct.Speed = GPIO_SPEED_HIGH;
//
//  HW_GPIO_Init(  RADIO_ANT_SWITCH_PORT, RADIO_ANT_SWITCH_PIN, &initStruct );
//  HW_GPIO_Write( RADIO_ANT_SWITCH_PORT, RADIO_ANT_SWITCH_PIN, 0);
}


void SX1276SetAntSw( uint8_t opMode )
{
//    switch( opMode )
//    {
//    case RFLR_OPMODE_TRANSMITTER:
//        HW_GPIO_Write( RADIO_ANT_SWITCH_PORT, RADIO_ANT_SWITCH_PIN, RADIO_ANT_SWITCH_SET_TX);
//        break;
//    case RFLR_OPMODE_RECEIVER:
//    case RFLR_OPMODE_RECEIVER_SINGLE:
//    case RFLR_OPMODE_CAD:
//    default:
//        HW_GPIO_Write( RADIO_ANT_SWITCH_PORT, RADIO_ANT_SWITCH_PIN, RADIO_ANT_SWITCH_SET_RX);
//        break;
//    }
}


bool SX1276CheckRfFrequency( uint32_t frequency )
{
    // Implement check. Currently all frequencies are supported
    return true;
}
