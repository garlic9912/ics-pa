#include <am.h>
#include <nemu.h>
#include </home/garlic/ics2023/abstract-machine/am/src/riscv/riscv.h>

#define KEYDOWN_MASK 0x8000

// static uint32_t keymap[256] = {};
// #define SDL_KEYMAP(k) keymap[AM_KEY_ ## k] = AM_KEY_ ## k;

// static void init_keymap() {
//   AM_KEYS(SDL_KEYMAP);
// }


void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  kbd->keydown = false;
  kbd->keycode = AM_KEY_NONE;

  uint8_t code = inb(KBD_ADDR);
  if (code != 0) {
    kbd->keycode = code;
    kbd->keydown = true;
  }
}
