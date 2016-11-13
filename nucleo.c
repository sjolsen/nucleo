#include "nucleo.h"
#include <stdbool.h>
#include <stddef.h>

static __attribute__((naked)) __attribute__((noreturn))
void __reset(void)
{
  volatile int __attribute__((unused)) rv = main();

  while (true) {
    __dsb();
    __wfi();
  }
}

DECLARE_ISR(__default_handler, default_handler);

#define DEFAULT_ISR(NAME) \
  extern __attribute__((naked)) __attribute__((weak, alias("__default_handler"))) \
  void NAME(void); \

DEFAULT_ISR(__nmi);
DEFAULT_ISR(__hard_fault);
DEFAULT_ISR(__mem_manage);
DEFAULT_ISR(__bus_fault);
DEFAULT_ISR(__usage_fault);
DEFAULT_ISR(__svcall);
DEFAULT_ISR(__debug_monitor);
DEFAULT_ISR(__pendsv);
DEFAULT_ISR(__systick);

static
void irq_entry(__attribute__((unused)) struct armv7m_exception_frame* frame,
               __attribute__((unused)) uint32_t* exc_return)
{
  uint32_t ipsr = __read_ipsr();
  uint32_t irq = ipsr & 0x1F;
  handle_irq(irq);
}
DECLARE_ISR(__irq_entry, irq_entry);

#define N_IRQS 97

struct __attribute__((packed)) vector_table
{
  void* sp_main;
  void (*reset)(void);

  void (*nmi)(void);
  void (*hard_fault)(void);
  void (*mem_manage)(void);
  void (*bus_fault)(void);
  void (*usage_fault)(void);

  void (*reserved_7)(void);
  void (*reserved_8)(void);
  void (*reserved_9)(void);
  void (*reserved_10)(void);

  void (*svcall)(void);
  void (*debug_monitor)(void);

  void (*reserved_13)(void);

  void (*pendsv)(void);
  void (*systick)(void);

  void (*irqs[N_IRQS])(void);
};

extern __attribute__((section(".vector_table")))
const struct vector_table __vector_table;
const struct vector_table __vector_table = {
  .sp_main       = &__stack_top,
  .reset         = &__reset,
  .nmi           = &__nmi,
  .hard_fault    = &__hard_fault,
  .mem_manage    = &__mem_manage,
  .bus_fault     = &__bus_fault,
  .usage_fault   = &__usage_fault,
  .svcall        = &__svcall,
  .debug_monitor = &__debug_monitor,
  .pendsv        = &__pendsv,
  .systick       = &__systick,

  .irqs = {
    [0 ... N_IRQS - 1] = &__irq_entry,
  },
};
