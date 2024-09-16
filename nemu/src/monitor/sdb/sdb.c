/***************************************************************************************
* Copyright (c) 2014-2022 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <isa.h>
#include <cpu/cpu.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "sdb.h"
#include <memory/paddr.h>

static int is_batch_mode = false;

void init_regex();
void init_wp_pool();

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}


static int cmd_q(char *args) {
  nemu_state.state = NEMU_QUIT;
  return -1;
}


static int cmd_s(char *args) {
  int n;
  // default n=1 when args is NULL
  if (args == NULL) {
    n = 1;
    cpu_exec(n);
  } else {
    n = atoi(args);
    cpu_exec(n);
  }
  return 0;
}


static int cmd_info(char *args) {
  // print regs
  if (strcmp(args, "r") == 0) {
    isa_reg_display();
    return 0;
  }

  // print watch
  if (strcmp(args, "w") == 0) {
    wp_display();
    return 0;
  }  

  return 0;
}


static int cmd_x(char *args) {
  // next n * four bytes
  char *n = strtok(args, " ");
  // starting address
  paddr_t addr = strtol(n + strlen(n) + 1, NULL, 16);
  
  for (int i = 0; i < atoi(n); i++) {
    word_t tmp_addr = paddr_read(addr, 4);
    printf(FMT_WORD ":" FMT_WORD "\n", addr, tmp_addr);
    addr += 4;
  }
  return 0;
}



static int cmd_p(char *args) {
  bool a = true;
  printf(FMT_WORD"\n", expr(args, &a));
  return 0;
}


static int cmd_w(char *args) {
  bool a = true;
  WP* wp = new_wp();
  wp->last_val = expr(args, &a);
  strcpy(wp->expression, args);
  return 0;
}


static int cmd_d(char *args) {
  panic("1111111111111");
  free_wp(atoi(args));
  return 0;
}



static int cmd_help(char *args);

static struct {
  const char *name;
  const char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display information about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },

  /* TODO: Add more commands */
  { "s", "Let the program execute N instructions in a single step and then pause.When N is not given, the default is 1", cmd_s },
  { "info", "Print register status or print monitoring point information", cmd_info },
  { "x", "Scan memory based on four bytes", cmd_x },
  { "p", "Expression evaluation", cmd_p },
  { "w", "Watch point", cmd_w }, 
  { "d", "Delete the given NO of watchpoinit", cmd_d },
};

#define NR_CMD ARRLEN(cmd_table)

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void sdb_set_batch_mode() {
  is_batch_mode = true;
}

void sdb_mainloop() {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef CONFIG_DEVICE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}

void init_sdb() {
  /* Compile the regular expressions. */
  init_regex();

  /* Initialize the watchpoint pool. */
  init_wp_pool();
}
