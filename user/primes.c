#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
process(int* pleft) {
  int temp;
  read(pleft[0], &temp, sizeof(temp));
  if (temp == -1) {
    exit(0);
  }
  printf("prime %d\n", temp);

  int pright[2];
  pipe(pright);

  if (fork() == 0) {    // 子进程
    close(pleft[0]);
    close(pright[1]);
    process(pright);
  } else {  // 父进程
    close(pright[0]);
    // 从左邻居接受数据
    int buf;
    while (read(pleft[0], &buf, sizeof(buf)) != 0 && buf != -1) {
      if (buf % temp != 0) {
        write(pright[1], &buf, sizeof(buf));
      }
    }
    write(pright[1], &buf, sizeof(buf));
    wait(0);
    exit(0);
  }
}

int
main(int argc, char* argv[])
{
  int input_pipe[2];
  pipe(input_pipe);

  if (fork() == 0) {    // 子进程
    close(input_pipe[1]);   // 关闭写端
    process(input_pipe);
    exit(0);
  } else {  // 父进程
    close(input_pipe[0]);    // 关闭读端
    int i;
    for (i = 2; i <= 35; ++i) {
        write(input_pipe[1], &i, sizeof(i));
    }
    i = -1;
    write(input_pipe[1], &i, sizeof(i));
    close(input_pipe[1]);
  }

  wait(0);
  exit(0);
}