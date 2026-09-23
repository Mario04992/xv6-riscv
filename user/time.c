#include "kernel/types.h"
#include "kernel/pstat.h"
#include "user/user.h"

int
main(int argc, char **argv)
{
  int status;
  if (argc < 2) {
    fprintf(2, "usage: time command [args...]\n");
    exit(1);
  }

  uint start = uptime();
  int pid = fork();
  if (pid < 0) {
    fprintf(2, "time: fork failed\n");
    exit(1);
  }
  if (pid == 0) {
    // argv + 1 retains the command, its arguments, and the terminating null.
    exec(argv[1], argv + 1);
    fprintf(2, "time: exec %s failed\n", argv[1]);
    exit(1);
  }

  struct rusage usage;
  if (wait2(&status, &usage) < 0) {
    fprintf(2, "time: wait failed\n");
    exit(1);
  }
  uint elapsed = (uint)uptime() - start;
  printf("elapsed time: %d ticks, cpu time: %d ticks, ",
         elapsed, usage.cputime);
  if (elapsed == 0)
    printf("N/A CPU (less than one tick)\n");
  else
    printf("%d%% CPU\n", (uint)(((uint64)usage.cputime * 100) / elapsed));
  exit(status);
}
