#include <am.h>
#include <klib-macros.h>
#include <klib.h>
#include <riscv64.h>

extern char _heap_start;
int main(const char *args);
void __am_init_uartlite(void);
void __am_uartlite_putchar(char ch);

extern char _pmem_start;
#define PMEM_SIZE (128 * 1024 * 1024)
#define PMEM_END  ((uintptr_t)&_pmem_start + PMEM_SIZE)

Area heap = RANGE(&_heap_start, PMEM_END);
#ifndef MAINARGS
#define MAINARGS ""
#endif
static const char mainargs[] = MAINARGS;

void putch(char ch) {
  // asm volatile("mv a0, %0; .word 0x00000077 " : :"r"(ch));
  __am_uartlite_putchar(ch);
}

void halt(int code) {
 asm volatile("mv a0, %0; .word 0x0000006b" : :"r"(code));
 
 while (1);
}

void _trm_init() {
  int ret = main(mainargs);
  halt(ret);
}


// uartlite 
#define UARTLITE_MMIO 0x40600000
#define UARTLITE_RX_FIFO  0x0
#define UARTLITE_TX_FIFO  0x4
#define UARTLITE_STAT_REG 0x8
#define UARTLITE_CTRL_REG 0xc

#define UARTLITE_RST_FIFO 0x03
#define UARTLITE_TX_FULL  0x08
#define UARTLITE_RX_VALID 0x01

extern void __am_init_uartlite(void);
extern void __am_uartlite_putchar(char ch);

void __am_init_uartlite(void) {
  outb(UARTLITE_MMIO + UARTLITE_CTRL_REG, UARTLITE_RST_FIFO);
}

void __am_uartlite_putchar(char ch) {
  // if (ch == '\n') __am_uartlite_putchar('\r');

  // while (inb(UARTLITE_MMIO + UARTLITE_STAT_REG) & UARTLITE_TX_FULL);
  outb(UARTLITE_MMIO + UARTLITE_TX_FIFO, ch);
}

int __am_uartlite_getchar() {
  if (inb(UARTLITE_MMIO + UARTLITE_STAT_REG) & UARTLITE_RX_VALID)
    return inb(UARTLITE_MMIO + UARTLITE_RX_FIFO);
  return 0;
}