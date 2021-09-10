#include <am.h>
#include <riscv64.h>

# define RTC_ADDR     0x3800bff8

void __am_timer_init() {
}

static inline uint32_t read_time(void) {
  return ind(RTC_ADDR) / 1000;  // unit: ms
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  // uint64_t  cycle;
  // asm volatile(
  //   "csrr %[dest], 0xc00"
  //   :[dest]"=r"(cycle)
  //   :
  //   );
  uptime->us = read_time();
  // uptime->us = uptime->us + 1;
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
