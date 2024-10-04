#include <am.h>
#include <nemu.h>
#include </home/garlic/ics2023/abstract-machine/am/src/riscv/riscv.h>

static uint64_t sys_init_time;

void __am_timer_init() {
  ioe_read(6, &sys_init_time);  
}


void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  ioe_read(6, uptime);
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
