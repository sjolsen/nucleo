#ifndef REGISTERS_RCC_HH
#define REGISTERS_RCC_HH

#include "registers/register.hh"
#include <stdint.h>

struct RCC_CR
{
  using register_type = uint32_t;

  uint32_t HSION     : 1;
  uint32_t HSIRDY    : 1;
  uint32_t           : 1;
  uint32_t HSITRIM   : 5;
  uint32_t HSICAL    : 8;
  uint32_t HSEON     : 1;
  uint32_t HSERDY    : 1;
  uint32_t HSEBYP    : 1;
  uint32_t CSSON     : 1;
  uint32_t           : 4;
  uint32_t PLLON     : 1;
  uint32_t PLLRDY    : 1;
  uint32_t PLLI2SON  : 1;
  uint32_t PLLI2SRDY : 1;
  uint32_t PLLSAION  : 1;
  uint32_t PLLSAIRDY : 1;
  uint32_t           : 2;
};

struct RCC_PLLCFGR
{
  using register_type = uint32_t;

  uint32_t PLLM   : 6;
  uint32_t PLLN   : 9;
  uint32_t        : 1;
  uint32_t PLLP   : 2;
  uint32_t        : 4;
  uint32_t PLLSRC : 1;
  uint32_t        : 1;
  uint32_t PLLQ   : 4;
  uint32_t PLLR   : 3;
  uint32_t        : 1;
};

enum
{
  RCC_PLLCFGR_PLLP_DIV2 = 0,
  RCC_PLLCFGR_PLLP_DIV4 = 1,
  RCC_PLLCFGR_PLLP_DIV6 = 2,
  RCC_PLLCFGR_PLLP_DIV8 = 3,
};

enum
{
  RCC_PLLCFGR_PLLSRC_HSI = 0,
  RCC_PLLCFGR_PLLSRC_HSE = 1,
};

struct RCC_CFGR
{
  using register_type = uint32_t;

  uint32_t SW      : 2;
  uint32_t SWS     : 2;
  uint32_t HPRE    : 4;
  uint32_t         : 2;
  uint32_t PPRE1   : 3;
  uint32_t PPRE2   : 3;
  uint32_t RTCPRE  : 5;
  uint32_t MCO1    : 2;
  uint32_t         : 1;
  uint32_t MCO1PRE : 3;
  uint32_t MCO2PRE : 3;
  uint32_t MCO2    : 2;
};

enum
{
  RCC_CFGR_SW_HSI   = 0,
  RCC_CFGR_SW_HSE   = 1,
  RCC_CFGR_SW_PLL_P = 2,
  RCC_CFGR_SW_PLL_R = 3,
};

enum
{
  RCC_CFGR_HPRE_DIV1   = 0,
  RCC_CFGR_HPRE_DIV2   = 8,
  RCC_CFGR_HPRE_DIV4   = 9,
  RCC_CFGR_HPRE_DIV8   = 10,
  RCC_CFGR_HPRE_DIV16  = 11,
  RCC_CFGR_HPRE_DIV64  = 12,
  RCC_CFGR_HPRE_DIV128 = 13,
  RCC_CFGR_HPRE_DIV256 = 14,
  RCC_CFGR_HPRE_DIV512 = 15,
};

enum
{
  RCC_CFGR_PPRE_DIV1   = 0,
  RCC_CFGR_PPRE_DIV2   = 4,
  RCC_CFGR_PPRE_DIV4   = 5,
  RCC_CFGR_PPRE_DIV8   = 6,
  RCC_CFGR_PPRE_DIV16  = 7,
};

struct RCC
{
  /* 0x00 */
  Register<struct RCC_CR> CR;
  Register<struct RCC_PLLCFGR> PLLCFGR;
  Register<struct RCC_CFGR> CFGR;
  uint32_t CIR;

  /* 0x10 */
  uint32_t AHB1RSTR;
  uint32_t AHB2RSTR;
  uint32_t AHB3RSTR;
  uint32_t reserved_0x1C;

  /* 0x20 */
  uint32_t APB1RSTR;
  uint32_t APB2RSTR;
  uint32_t reserved_0x28;
  uint32_t reserved_0x2C;

  /* 0x30 */
  uint32_t AHB1ENR;
  uint32_t AHB2ENR;
  uint32_t AHB3ENR;
  uint32_t reserved_0x3C;

  /* 0x40 */
  uint32_t APB1ENR;
  uint32_t APB2ENR;
  uint32_t reserved_0x48;
  uint32_t reserved_0x4C;
};

extern "C" volatile struct RCC RCC;

#endif
