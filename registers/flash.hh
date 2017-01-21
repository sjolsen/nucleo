#ifndef REGISTERS_FLASH_HH
#define REGISTERS_FLASH_HH

#include "registers/register.hh"
#include <stdint.h>

struct FLASH_ACR
{
  using register_type = uint32_t;

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
  Register<struct FLASH_ACR> ACR;
  uint32_t KEYR;
  uint32_t OPTKEYR;
  uint32_t SR;

  /* 0x10 */
  uint32_t CR;
  uint32_t OPTCR;
};

extern "C" volatile struct FLASH FLASH;

#endif
