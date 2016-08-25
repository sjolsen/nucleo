#include <stdint.h>

static inline __attribute__((always_inline))
void __wfi(void)
{
  __asm__("wfi");
}

struct RCC_CR
{
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

extern volatile struct
{
  struct RCC_CR      CR;
  struct RCC_PLLCFGR PLLCFGR;
} RCC;

int main(void)
{
  /*
   * Enable the HSE clock input.
   *
   * The STM32 Nucleo board has an 8 MHz HSE clock input provided by
   * the ST-Link IC. The bypass bit is set to use this clock input
   * directly.
   */
  {
    struct RCC_CR CR = RCC.CR;
    CR.HSEBYP = 1;
    CR.HSEON  = 1;
    RCC.CR = CR;
  }

  /*
   * Configure the PLL.
   *
   * The VCO clock is set to 432 MHz. To this end, we have:
   *
   *   f(VCO clock) = f(PLL clock input) * (PLLN / PLLM)
   *   432 MHz = 8 MHz * (PLLN / PLLM)
   *   (PLLN / PLLM) = 54
   *   PLLM = 2
   *   PLLN = 108
   *
   * The system clock output is set to 108 MHz. To this end, we have:
   *
   *   f(PLL general clock output) = f(VCO clock) / PLLP
   *   108 MHz = 432 MHz / PLLP
   *   PLLP = 4
   *
   * The USB FS/SDIO output is set to 48 MHz. To this end, we have:
   *
   *   f(USB OTG FS, SDIO) = f(VCO clock) / PLLQ
   *   48 MHz = 432 MHz / PLLQ
   *   PLLQ = 9
   *
   * The I2S/SPDIF-Rx output is left in its reset configuration.
   */
  {
    struct RCC_PLLCFGR PLLCFGR = RCC.PLLCFGR;
    PLLCFGR.PLLM   = 2;
    PLLCFGR.PLLN   = 108;
    PLLCFGR.PLLP   = RCC_PLLCFGR_PLLP_DIV4;
    PLLCFGR.PLLSRC = RCC_PLLCFGR_PLLSRC_HSE;
    PLLCFGR.PLLQ   = 9;
    RCC.PLLCFGR = PLLCFGR;
  }

  /*
   * Turn the PLL on and wait for it to lock.
   */
  RCC.CR.PLLON = 1;
  while (!RCC.CR.PLLRDY)
    __wfi();

  return 0;
}
