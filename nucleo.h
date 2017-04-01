#ifndef NUCLEO_H
#define NUCLEO_H

#include <stdint.h>

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
void __svc0(uint32_t arg)
{
  register uint32_t r0 __asm__("r0") = arg;
  __asm__("svc #0" :: "r"(r0));
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

/* --- Global symbols defined by the linker --- */

extern unsigned char __stack_bottom[];
extern unsigned char __stack_top[];

/* --- Application entry points --- */

extern
int main(void);

extern
void handle_exception(uint32_t exc, struct armv7m_exception_frame* frame,
                      uint32_t* exc_return);

#endif
