#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char **argv)
{
  int status;
  if (argc < 2) {
    fprintf(2, "usage: time1 command [args...]\n");
    exit(1);
  }

  uint start = uptime();
  int pid = fork();
  if (pid < 0) {
    fprintf(2, "time1: fork failed\n");
    exit(1);
  }
  if (pid == 0) {
    // argv + 1 retains the command, its arguments, and the terminating null.
    exec(argv[1], argv + 1);
    fprintf(2, "time1: exec %s failed\n", argv[1]);
    exit(1);
  }

  if (wait(&status) < 0) {
    fprintf(2, "time1: wait failed\n");
    exit(1);
  }
  uint elapsed = (uint)uptime() - start;
  printf("elapsed time: %d ticks\n", elapsed);
  exit(status);
}
