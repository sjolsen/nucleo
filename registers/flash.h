#ifndef REGISTERS_FLASH_H
#define REGISTERS_FLASH_H

#include <stdint.h>

struct FLASH_ACR
{
  uint32_t LATENCY : 4;
  uint32_t         : 4;
  uint32_t PRFTEN  : 1;
  uint32_t ICEN    : 1;
  uint32_t DCEN    : 1;
  uint32_t ICRST   : 1;
  uint32_t DCRST   : 1;
  uint32_t         : 19;
};

struct FLASH
{
  /* 0x00 */
  struct FLASH_ACR ACR;
  uint32_t KEYR;
  uint32_t OPTKEYR;
  uint32_t SR;

  /* 0x10 */
  uint32_t CR;
  uint32_t OPTCR;
};

extern volatile struct FLASH FLASH;

#endif
