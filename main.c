#include "registers/gpio.h"
#include "registers/usart.h"
#include "registers/rcc.h"
#include <stdbool.h>

#define CONFIG_CLOCKSOURCE_PLL

static
bool clock_pll_ready(void)
{
  union RCC_CR CR = {.raw = RCC.CR};
  return CR.PLLRDY;
}

static
void clock_init(void)
{
  /* -8<--- TODO: Document ------ */
  {
    // Enable 21.6 MHz MCO2 clock out
    union RCC_CFGR CFGR = {.raw = RCC.CFGR};
    CFGR.MCO2    = 3; // PLL
    CFGR.MCO2PRE = 7; // Divided by 5
    RCC.CFGR = CFGR.raw;
  }
  /* ------ TODO: Document --->8- */

  /*
   * Enable the HSE clock input.
   *
   * The STM32 Nucleo board has an 8 MHz HSE clock input provided by
   * the ST-Link IC. The bypass bit is set to use this clock input
   * directly.
   */
  {
    union RCC_CR CR = {.raw = RCC.CR};
    CR.HSEBYP = 1;
    CR.HSEON  = 1;
    RCC.CR = CR.raw;
  }

  /*
   * Configure the PLL.
   *
   * The VCO clock is set to 432 MHz. To this end, we have:
   *
   *   f(VCO clock) = f(PLL clock input) * (PLLN / PLLM)
   *   432 MHz = 8 MHz * (PLLN / PLLM)
   *   (PLLN / PLLM) = 54
   *   PLLM = 8
   *   PLLN = 432
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
    union RCC_PLLCFGR PLLCFGR = {.raw = RCC.PLLCFGR};
    PLLCFGR.PLLM   = 8;
    PLLCFGR.PLLN   = 432;
    PLLCFGR.PLLP   = RCC_PLLCFGR_PLLP_DIV4;
    PLLCFGR.PLLSRC = RCC_PLLCFGR_PLLSRC_HSE;
    PLLCFGR.PLLQ   = 9;
    RCC.PLLCFGR = PLLCFGR.raw;
  }

  /*
   * Turn the PLL on and wait for it to lock.
   */
  {
    union RCC_CR CR = {.raw = RCC.CR};
    CR.PLLON = 1;
    RCC.CR = CR.raw;
  }
  while (!clock_pll_ready()) {
  }

  /*
   * Configure the system clock.
   *
   * The following frequencies are configured:
   *
   *   - System clock at 108 MHz
   *   - AHB at 27 MHz
   *   - APB1 at 27 MHz
   *   - APB2 at 27 MHz
   *   - RTC at 1 MHz
   *
   * The system clock is run at the maximum speed allowed by the PLL
   * configuration. The processor reads garbage from the instruction
   * bus if the AHB prescaler is set to 1 or 2, but 4 works. The APB
   * clocks are set to the maximum values allowed by this choice of
   * AHB clock. The RTC is run at the 1 MHz required by the RCC
   * documentation.
   */
  {
    #ifdef CONFIG_CLOCKSOURCE_PLL
    union RCC_CFGR CFGR = {.raw = RCC.CFGR};
    CFGR.SW     = RCC_CFGR_SW_PLL_P;
    CFGR.HPRE   = RCC_CFGR_HPRE_DIV4;
    CFGR.PPRE1  = RCC_CFGR_PPRE_DIV1;
    CFGR.PPRE2  = RCC_CFGR_PPRE_DIV1;
    CFGR.RTCPRE = 8;
    RCC.CFGR = CFGR.raw;
    #endif
  }
}

static
void gpio_init(void)
{
  /* -8<--- TODO: Document ------ */
  // Enable and reset all the GPIO IP blocks
  RCC.AHB1ENR  |= 0x000000FF;
  RCC.AHB1RSTR |= 0x000000FF;
  RCC.AHB1RSTR &= 0xFFFFFF00;

  // Configure MCO2 (PC9) AF
  // 1. MODER
  // 2. OTYPER, PUPDR, OSPEEDR
  // 3. AFRL, AFRH (AF0)
  {
    uint32_t MODER = GPIOC.MODER;
    MODER &= ~(3 << 18);
    MODER |= GPIO_MODER_ALTERNATE_FUNCTION << 18;
    GPIOC.MODER = MODER;
  }
  {
    uint32_t OTYPER = GPIOC.OTYPER;
    OTYPER &= ~(1 << 9);
    OTYPER |= GPIO_OTYPER_PUSH_PULL << 9;
    GPIOC.OTYPER = OTYPER;
  }
  {
    uint32_t AFRH = GPIOC.AFRH;
    AFRH &= ~(15 << 4);
    AFRH |= 0 << 4;
    GPIOC.AFRH = AFRH;
  }

  // Configure USART2_TX (PA2) and USART2_RX (PA3)
  // 1. MODER
  // 2. OTYPER, PUPDR, OSPEEDR
  // 3. AFRL, AFRH (AF7)
  {
    uint32_t MODER = GPIOA.MODER;
    MODER &= ~(3 << 4);
    MODER &= ~(3 << 6);
    MODER |= GPIO_MODER_ALTERNATE_FUNCTION << 4;
    MODER |= GPIO_MODER_ALTERNATE_FUNCTION << 6;
    GPIOA.MODER = MODER;
  }
  {
    uint32_t AFRL = GPIOA.AFRL;
    AFRL &= ~(15 << 8);
    AFRL &= ~(15 << 12);
    AFRL |= 7 << 8;
    AFRL |= 7 << 12;
    GPIOA.AFRL = AFRL;
  }
  /* ------ TODO: Document --->8- */
}

static
void uart_init(void)
{
  // USART2 runs off the APB1 clock domain
  RCC.APB1ENR  |= 1 << 17;
  RCC.APB1RSTR |= 1 << 17;
  RCC.APB1RSTR &= ~(1 << 17);

  {
    union USART_CR1 CR1 = {.raw = USART2.CR1};
    CR1.UE = 0;
    USART2.CR1 = CR1.raw;
  }

  #ifdef CONFIG_CLOCKSOURCE_PLL
  // APB1 clock is configured at 27 MHz. We want 115200 baud.
  // Tx/Rx baud = fCK / (8 × (2 – OVER8) × USARTDIV)
  // 115200 = 27000000 / (16 * USARTDIV)
  // USARTDIV = 14.65
  {
    union USART_BRR BRR = {.raw = USART2.BRR};
    BRR.MANTISSA = 14;
    BRR.FRACTION = 10;
    USART2.BRR = BRR.raw;
  }
  #else
  // All clocks are 16 MHz at reset. We want 115200 baud.
  // Tx/Rx baud = fCK / (8 × (2 – OVER8) × USARTDIV)
  // 115200 = 16000000 / (16 * USARTDIV)
  // USARTDIV = 8.68
  {
    union USART_BRR BRR = {.raw = USART2.BRR};
    BRR.MANTISSA = 8;
    BRR.FRACTION = 11;
    USART2.BRR = BRR.raw;
  }
  #endif
  {
    union USART_CR1 CR1 = {.raw = USART2.CR1};
    CR1.M   = 0;
    CR1.PCE = 0;
    CR1.TE  = 1;
    CR1.RE  = 1;
    USART2.CR1 = CR1.raw;
  }
  {
    union USART_CR2 CR2 = {.raw = USART2.CR2};
    CR2.STOP = 0;
    USART2.CR2 = CR2.raw;
  }

  {
    union USART_CR1 CR1 = {.raw = USART2.CR1};
    CR1.UE = 1;
    USART2.CR1 = CR1.raw;
  }
}

static
bool uart_tx_empty(void)
{
  union USART_SR SR = {.raw = USART2.SR};
  return SR.TXE;
}

static
void uart_write(uint8_t byte)
{
  while (!uart_tx_empty()) {
  }

  USART2.DR = byte;
}

static
void uart_puts(const char* str)
{
  for (const char* p = str; *p != '\0'; ++p) {
    if (*p == '\n')
      uart_write('\r');
    uart_write(*p);
  }
}

int main(void)
{
  clock_init();
  gpio_init();
  uart_init();

  uart_puts("Hello, STM\n");

  return 0;
}
