#ifndef REGISTERS_GPIO_H
#define REGISTERS_GPIO_H

#include <stdint.h>

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

struct GPIO
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

#endif
