#ifndef REGISTERS_USART_H
#define REGISTERS_USART_H

#include <stdint.h>

struct USART_SR
{
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

struct USART_DR
{
  uint32_t DR : 9;
  uint32_t    : 23;
};

struct USART_BRR
{
  uint32_t FRACTION : 4;
  uint32_t MANTISSA : 12;
  uint32_t          : 16;
};

struct USART_CR1
{
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

struct USART_CR2
{
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

struct USART_CR3
{
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

struct USART_GTPR
{
  uint32_t PSC : 8;
  uint32_t GT  : 8;
  uint32_t     : 16;
};

struct USART
{
  /* 0x00 */
  struct USART_SR SR;
  struct USART_DR DR;
  struct USART_BRR BRR;
  struct USART_CR1 CR1;

  /* 0x10 */
  struct USART_CR2 CR2;
  struct USART_CR3 CR3;
  struct USART_GTPR GTPR;
};

extern volatile struct USART USART2;

#endif
