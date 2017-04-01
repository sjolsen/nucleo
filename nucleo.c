#include "nucleo.h"
#include <stdbool.h>
#include <stddef.h>

__attribute__((noreturn))
void halt(void)
{
  while (true) {
    __dsb();
    __wfi();
  }
}

static __attribute__((naked)) __attribute__((noreturn))
void __reset(void)
{
  main();
  halt();
}

static __attribute__((naked))
void __exception_entry(void)
{
  __asm__(
    "mov r1, sp          \n"
    "sub r2, sp, #8      \n"
    "bic r2, #7          \n"
    "mov sp, r2          \n"
    "str lr, [sp]        \n"
    "str r1, [sp, #4]    \n"
    "mrs r0, ipsr        \n"
    "and r0, r0, #0x1F   \n"
    "bl handle_exception \n"
    "ldr lr, [sp]        \n"
    "ldr sp, [sp, #4]    \n"
    "bx lr               \n"
  );
}

#define N_IRQS 97

struct __attribute__((packed)) vector_table
{
  void* sp_main;
  void (*reset)(void);
  void (*exceptions[14])(void);
  void (*irqs[N_IRQS])(void);
};

extern __attribute__((section(".vector_table")))
const struct vector_table __vector_table;
const struct vector_table __vector_table = {
  .sp_main       = &__stack_top,
  .reset         = &__reset,

  .exceptions = {
    [0 ... 13] = &__exception_entry,
  },

  .irqs = {
    [0 ... N_IRQS - 1] = &__exception_entry,
  },
};
