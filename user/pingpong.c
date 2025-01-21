#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char* argv[])
{
  int pp2c[2], pc2p[2];
  pipe(pp2c);
  pipe(pc2p);

  if (fork() != 0) {
    write(pp2c[1], "ping\0", 4);
    close(pp2c[1]);

    char buf[5] = {0};
    read(pc2p[0], buf, 4);
    printf("%d: received %s\n", getpid(), buf);
    wait(0);
  } else {
    char buf[5] = {0};
    read(pp2c[0], buf, 4);
    printf("%d: received %s\n", getpid(), buf);

    write(pc2p[1], "pong\0", 4);
    close(pc2p[1]);
  }

  close(pp2c[0]);
  close(pc2p[0]);

  exit(0);
}