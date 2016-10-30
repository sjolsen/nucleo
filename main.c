#include <stdbool.h>
#include <stdint.h>

#define CONFIG_CLOCKSOURCE_PLL

/* static inline __attribute__((always_inline)) */
/* void __wfi(void) */
/* { */
/*   __asm__("wfi"); */
/* } */

union __attribute__((packed)) RCC_CR
{
  struct __attribute__((packed)) {
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
  uint32_t raw;
};

union __attribute__((packed)) RCC_PLLCFGR
{
  struct __attribute__((packed)) {
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
  uint32_t raw;
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

union __attribute__((packed)) RCC_CFGR
{
  struct __attribute__((packed)) {
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
  uint32_t raw;
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

struct __attribute__((packed)) RCC
{
  /* 0x00 */
  uint32_t CR;
  uint32_t PLLCFGR;
  uint32_t CFGR;
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

extern volatile struct RCC RCC;

enum
{
  GPIO_MODER_INPUT              = 0,
  GPIO_MODER_OUTPUT             = 1,
  GPIO_MODER_ALTERNATE_FUNCTION = 2,
  GPIO_MODER_ANALOG             = 3,
};

enum
{
  GPIO_OTYPER_PUSH_PULL  = 0,
  GPIO_OTYPER_OPEN_DRAIN = 1,
};

struct __attribute__((packed)) GPIO
{
  /* 0x00 */
  uint32_t MODER;
  uint32_t OTYPER;
  uint32_t OSPEEDR;
  uint32_t PUPDR;

  /* 0x10 */
  uint32_t IDR;
  uint32_t ODR;
  uint32_t BSRR;
  uint32_t LCKR;

  /* 0x20 */
  uint32_t AFRL;
  uint32_t AFRH;
};

extern volatile struct GPIO GPIOA;
extern volatile struct GPIO GPIOC;

union __attribute__((packed)) USART_SR
{
  struct __attribute__((packed)) {
    uint32_t PE   : 1;
    uint32_t FE   : 1;
    uint32_t NF   : 1;
    uint32_t ORE  : 1;
    uint32_t IDLE : 1;
    uint32_t RXNE : 1;
    uint32_t TC   : 1;
    uint32_t TXE  : 1;
    uint32_t LBD  : 1;
    uint32_t CTS  : 1;
    uint32_t      : 22;
  };
  uint32_t raw;
};

union __attribute__((packed)) USART_DR
{
  struct __attribute__((packed)) {
    uint32_t DR : 9;
    uint32_t    : 23;
  };
  uint32_t raw;
};

union __attribute__((packed)) USART_BRR
{
  struct __attribute__((packed)) {
    uint32_t FRACTION : 4;
    uint32_t MANTISSA : 12;
    uint32_t          : 16;
  };
  uint32_t raw;
};

union __attribute__((packed)) USART_CR1
{
  struct __attribute__((packed)) {
    uint32_t SBK    : 1;
    uint32_t RWU    : 1;
    uint32_t RE     : 1;
    uint32_t TE     : 1;
    uint32_t IDLEIE : 1;
    uint32_t RXNEIE : 1;
    uint32_t TCIE   : 1;
    uint32_t TXEIE  : 1;
    uint32_t PEIE   : 1;
    uint32_t PS     : 1;
    uint32_t PCE    : 1;
    uint32_t WAKE   : 1;
    uint32_t M      : 1;
    uint32_t UE     : 1;
    uint32_t        : 1;
    uint32_t OVER8  : 1;
    uint32_t        : 16;
  };
  uint32_t raw;
};

union __attribute__((packed)) USART_CR2
{
  struct __attribute__((packed)) {
    uint32_t ADD   : 4;
    uint32_t       : 1;
    uint32_t LBDL  : 1;
    uint32_t LBDIE : 1;
    uint32_t       : 1;
    uint32_t LBCL  : 1;
    uint32_t CPHA  : 1;
    uint32_t CPOL  : 1;
    uint32_t CLKEN : 1;
    uint32_t STOP  : 2;
    uint32_t LINEN : 1;
    uint32_t       : 17;
  };
  uint32_t raw;
};

union __attribute__((packed)) USART_CR3
{
  struct __attribute__((packed)) {
    uint32_t EIE    : 1;
    uint32_t IREN   : 1;
    uint32_t IRLP   : 1;
    uint32_t HDSEL  : 1;
    uint32_t NACK   : 1;
    uint32_t SCEN   : 1;
    uint32_t DMAR   : 1;
    uint32_t DMAT   : 1;
    uint32_t RTSE   : 1;
    uint32_t CTSE   : 1;
    uint32_t CTSIE  : 1;
    uint32_t ONEBIT : 1;
    uint32_t        : 20;
  };
  uint32_t raw;
};

union __attribute__((packed)) USART_GTPR
{
  struct __attribute__((packed)) {
    uint32_t PSC : 8;
    uint32_t GT  : 8;
    uint32_t     : 16;
  };
  uint32_t raw;
};

struct __attribute__((packed)) USART
{
  /* 0x00 */
  uint32_t SR;
  uint32_t DR;
  uint32_t BRR;
  uint32_t CR1;

  /* 0x10 */
  uint32_t CR2;
  uint32_t CR3;
  uint32_t GTPR;
};

extern volatile struct USART USART2;

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
