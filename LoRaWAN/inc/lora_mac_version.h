#ifndef __LORA_MAC_VERSION_H__
#define __LORA_MAC_VERSION_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/*the 4 MSBs define the version based on Github version         */
/*https://github.com/Lora-net/LoRaMac-node/wiki/LoRaMAC-node-Wiki*/
/* version 4.4.2-rc.5 from develop branch */
#define LORA_MAC_VERSION   (uint32_t) 0x44250000

/* Exported types ------------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /*__LORA_MAC_VERSION_H__*/
