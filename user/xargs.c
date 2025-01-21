#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

void
run(char* program, char** args) {
  if (fork() == 0) {
    exec(program, args);
    exit(0);
  }
  return;
}

int
main(int argc, char* argv[])
{
  char buf[2048];
  char* p = buf, *last_p = buf;
  char* args_buf[MAXARG];
  char** args = args_buf;

  for (int i = 1; i < argc; i++) {
    *args++ = argv[i];
  }
  char** cur_args = args;

  while (read(0, p, 1) != 0) {
    if (*p == '\n' || *p == ' ') {
      *p = '\0';

      *cur_args++ = last_p;
      last_p = p + 1;

      if (*p == '\n') {
        *cur_args = 0;
        run(argv[1], args_buf);
        cur_args = args;
      }
    }
    ++p;
  }

  if (cur_args != args) {
    *p = '\0';
    *cur_args++ = last_p;
    *cur_args = 0;

    run(argv[1], args_buf);
  }

  while (wait(0) != -1) {};
  exit(0);
}