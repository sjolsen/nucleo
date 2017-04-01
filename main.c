#include "nucleo.h"
#include "registers/flash.h"
#include "registers/gpio.h"
#include "registers/usart.h"
#include "registers/rcc.h"
#include <stdbool.h>
#include <stddef.h>

#define CONFIG_CLOCKSOURCE_PLL

static
void clock_init(void)
{
  /* -8<--- TODO: Document ------ */
  {
    // Enable 21.6 MHz MCO2 clock out
    struct RCC_CFGR CFGR = RCC.CFGR;
    CFGR.MCO2    = 0; // SYSCLK
    CFGR.MCO2PRE = 7; // Divided by 5
    RCC.CFGR = CFGR;
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
    struct RCC_CR CR = RCC.CR;
    CR.HSEBYP = 1;
    CR.HSEON  = 1;
    RCC.CR = CR;

    do CR = RCC.CR;
    while (!CR.HSERDY);
  }

  /*
   * Configure the PLL.
   *
   * The VCO clock is set to 432 MHz, using the 8 MHz HSE input
   * divided down to 2 MHz per the datasheet's recommendation. To this
   * end, we have:
   *
   *   f(VCO clock) = f(PLL clock input) * (PLLN / PLLM)
   *   432 MHz = 8 MHz * (PLLN / PLLM)
   *   (PLLN / PLLM) = 54
   *   PLLM = 4
   *   PLLN = 216
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
    PLLCFGR.PLLM   = 4;
    PLLCFGR.PLLN   = 216;
    PLLCFGR.PLLP   = RCC_PLLCFGR_PLLP_DIV4;
    PLLCFGR.PLLSRC = RCC_PLLCFGR_PLLSRC_HSE;
    PLLCFGR.PLLQ   = 9;
    RCC.PLLCFGR = PLLCFGR;
  }

  /*
   * Turn the PLL on and wait for it to lock.
   */
  {
    struct RCC_CR CR = RCC.CR;
    CR.PLLON = 1;
    RCC.CR = CR;

    do CR = RCC.CR;
    while (!CR.PLLRDY);
  }

  #ifdef CONFIG_CLOCKSOURCE_PLL
  /*
   * Configure the flash memory read interface.
   *
   * The number of flash memory read wait states must be programmed
   * according to the table in the reference manual before
   * reconfiguring the AHB clock to ensure correct operation.
   *
   * At 3.3 V (Vdd on the Nucleo board), the flash memory supports AHB
   * speeds in the range:
   *
   *   N × 30 MHz < f(HCLK) ≤ (N + 1) × 30 MHz,
   *
   * where N is the number of wait states programmed in the LATENCY
   * field of the FLASH_ACR register. We program the AHB below to run
   * at 108 MHz, so we configure the flash memory interface for three
   * wait states.
   */
  {
    struct FLASH_ACR ACR = FLASH.ACR;
    ACR.LATENCY = 3;
    FLASH.ACR = ACR;

    do ACR = FLASH.ACR;
    while (ACR.LATENCY != 3);
  }

  /*
   * Configure the system clock.
   *
   * The following frequencies are configured:
   *
   *   - System clock at 108 MHz
   *   - AHB at 108 MHz
   *   - APB1 at 27 MHz
   *   - APB2 at 54 MHz
   *   - RTC at 1 MHz
   *
   * The system and clock buses are run at the maximum speeds allowed
   * by the PLL configuration. The RTC is run at the 1 MHz required by
   * the RCC documentation.
   */
  {
    struct RCC_CFGR CFGR = RCC.CFGR;
    CFGR.SW     = RCC_CFGR_SW_PLL_P;
    CFGR.HPRE   = RCC_CFGR_HPRE_DIV1;
    CFGR.PPRE1  = RCC_CFGR_PPRE_DIV4;
    CFGR.PPRE2  = RCC_CFGR_PPRE_DIV2;
    CFGR.RTCPRE = 8;
    RCC.CFGR = CFGR;
  }
  #endif
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
    struct USART_CR1 CR1 = USART2.CR1;
    CR1.UE = 0;
    USART2.CR1 = CR1;
  }

  #ifdef CONFIG_CLOCKSOURCE_PLL
  // APB1 clock is configured at 27 MHz. We want 115200 baud.
  // Tx/Rx baud = fCK / (8 × (2 - OVER8) × USARTDIV)
  // 115200 = 27000000 / (16 * USARTDIV)
  // USARTDIV = 14.65
  {
    struct USART_BRR BRR = USART2.BRR;
    BRR.MANTISSA = 14;
    BRR.FRACTION = 10;
    USART2.BRR = BRR;
  }
  #else
  // All clocks are 16 MHz at reset. We want 115200 baud.
  // Tx/Rx baud = fCK / (8 × (2 - OVER8) × USARTDIV)
  // 115200 = 16000000 / (16 * USARTDIV)
  // USARTDIV = 8.68
  {
    struct USART_BRR BRR = USART2.BRR;
    BRR.MANTISSA = 8;
    BRR.FRACTION = 11;
    USART2.BRR = BRR;
  }
  #endif
  {
    struct USART_CR1 CR1 = USART2.CR1;
    CR1.M   = 0;
    CR1.PCE = 0;
    CR1.TE  = 1;
    CR1.RE  = 1;
    USART2.CR1 = CR1;
  }
  {
    struct USART_CR2 CR2 = USART2.CR2;
    CR2.STOP = 0;
    USART2.CR2 = CR2;
  }

  {
    struct USART_CR1 CR1 = USART2.CR1;
    CR1.UE = 1;
    USART2.CR1 = CR1;
  }
}

static
void uart_write(uint8_t byte)
{
  struct USART_SR SR;

  do SR = USART2.SR;
  while (!SR.TXE);

  USART2.DR.DR = byte;
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

static
void uart_write_dec(uint32_t val)
{
  char buf[10];
  size_t i = 0;

  do {
    buf[i++] = '0' + val % 10;
    val /= 10;
  } while (val != 0);

  for (size_t j = i; j > 0; --j)
    uart_write(buf[j - 1]);
}

int main(void)
{
  clock_init();
  gpio_init();
  uart_init();

  uart_puts("RCC.CFGR.SWS = ");
  uart_write_dec(RCC.CFGR.SWS);
  uart_puts("\n");

  __svc(0, 88);

  return 0;
}

void handle_exception(uint32_t exc, struct armv7m_exception_frame* frame,
                      __attribute__((unused)) uint32_t* exc_return)
{
  /* SVCall */
  if (exc == 11) {
    const uint8_t* ret = (const uint8_t*)frame->PC;
    uint8_t imm = ret[-2];
    uint32_t arg = frame->R0;

    uart_puts("SVCall #");
    uart_write_dec(imm);
    uart_puts(" (");
    uart_write_dec(arg);
    uart_puts(")\n");
  }

  halt();
}
