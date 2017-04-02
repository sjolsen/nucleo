#ifndef IRQ_H
#define IRQ_H

#define REG32(addr) (*(volatile uint32_t*)(addr))

#define NVIC_ISER(x) REG32(0XE000E100 + 4 * (x))
#define NVIC_ICER(x) REG32(0XE000E180 + 4 * (x))
#define NVIC_ISPR(x) REG32(0XE000E200 + 4 * (x))
#define NVIC_ICPR(x) REG32(0XE000E280 + 4 * (x))
#define NVIC_IABR(x) REG32(0XE000E300 + 4 * (x))
#define NVIC_IPR(x)  REG32(0XE000E400 + 4 * (x))

static inline
void irq_enable(uint8_t irq)
{ NVIC_ISER(irq / 32) = 1 << (irq % 32); }

static inline
void irq_disable(uint8_t irq)
{ NVIC_ICER(irq / 32) = 1 << (irq % 32); }

static inline
void irq_set_pending(uint8_t irq)
{ NVIC_ISPR(irq / 32) = 1 << (irq % 32); }

static inline
void irq_clear_pending(uint8_t irq)
{ NVIC_ICPR(irq / 32) = 1 << (irq % 32); }

static inline
void irq_set_priority(uint8_t irq, uint8_t priority)
{
  uint32_t reg = NVIC_IPR(irq / 4);
  reg &= ~(0xFF << ((irq % 4) * 8));
  reg |= (priority << ((irq % 4) * 8));
  NVIC_IPR(irq / 4) = reg;
}

#endif
