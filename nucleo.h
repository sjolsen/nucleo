#ifndef NUCLEO_H
#define NUCLEO_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* --- C function wrappers for ARM instructions --- */

static inline __attribute__((always_inline))
void __dsb(void)
{
  __asm__("dsb");
}

static inline __attribute__((always_inline))
void __wfi(void)
{
  __asm__("wfi");
}

static inline __attribute__((always_inline))
uint32_t __read_ipsr(void)
{
  uint32_t result;
  __asm__ volatile("mrs %0, ipsr" : "=r"(result));
  return result;
}

__attribute__((noreturn))
void halt(void);

/* --- Exception handling --- */

struct __attribute__((packed)) armv7m_exception_frame
{
  uint32_t R0;
  uint32_t R1;
  uint32_t R2;
  uint32_t R3;
  uint32_t R12;
  uint32_t LR;
  uint32_t PC;
  uint32_t xPSR;
};

#define DECLARE_ISR(NAME, TARGET) \
  __attribute__((used)) \
  void TARGET(struct armv7m_exception_frame* frame, uint32_t* exc_return); \
  \
  extern __attribute__((naked)) \
  void NAME(void) \
  { \
    __asm__( \
      "mov r0, sp       \n" \
      "sub r1, sp, #8   \n" \
      "bic r1, #7       \n" \
      "mov sp, r1       \n" \
      "str lr, [sp]     \n" \
      "str r0, [sp, #4] \n" \
      "bl " #TARGET "   \n" \
      "ldr lr, [sp]     \n" \
      "ldr sp, [sp, #4] \n" \
      "bx lr            \n" \
    ); \
  }

/* --- Global symbols defined by the linker --- */

extern unsigned char __stack_bottom[];
extern unsigned char __stack_top[];

/* --- Application entry points --- */

extern
int main(void);

extern
void default_handler(struct armv7m_exception_frame* frame, uint32_t* exc_return);

extern
void handle_irq(uint32_t irq);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
