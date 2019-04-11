#ifndef __VERSION_H__
#define __VERSION_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "lora_mac_version.h"
/* Exported constants --------------------------------------------------------*/
#define TEST_VERSION 	(uint32_t) 0x00000000  /*1 lsb is always 0 in releases   */
#define LRWAN_VERSION  	(uint32_t) 0x00001210  /*3 next hex is i_cube release*/
#define VERSION   	(uint32_t) ( LORA_MAC_VERSION | LRWAN_VERSION | TEST_VERSION )

/* Exported types ------------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /*__VERSION_H__*/
