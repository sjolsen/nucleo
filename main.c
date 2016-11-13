#include "nucleo.h"
#include "registers/gpio.h"
#include "registers/usart.h"
#include "registers/rcc.h"
#include <stdbool.h>

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

  // All clocks are 16 MHz at reset. We want 115200 baud.
  // Tx/Rx baud = fCK / (8 × (2 - OVER8) × USARTDIV)
  // 115200 = 16000000 / (16 * USARTDIV)
  // USARTDIV = 8.68
  {
    union USART_BRR BRR = {.raw = USART2.BRR};
    BRR.MANTISSA = 8;
    BRR.FRACTION = 11;
    USART2.BRR = BRR.raw;
  }
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

static __attribute__((noreturn))
void halt(void)
{
  while (true) {
    __dsb();
    __wfi();
  }
}

int main(void)
{
  gpio_init();
  uart_init();

  uart_puts("Hello, STM\n");

  return 0;
}

void default_handler(__attribute__((unused)) struct armv7m_exception_frame* frame,
                     __attribute__((unused)) uint32_t* exc_return)
{
  halt();
}

void handle_irq(__attribute__((unused)) uint32_t irq)
{
  halt();
}
