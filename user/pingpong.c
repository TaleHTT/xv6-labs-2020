#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int pipefd[2][2];
  pipe(pipefd[0]);//father to child
  pipe(pipefd[1]);//child to father

  int pid = fork();
  if(pid == 0) {
    // close(0);
    char readchar[64];
    int charnum = read(pipefd[0][0], readchar, 64);
    close(pipefd[0][0]);

    if(charnum > 0) {
      printf("%d: received ping\n", getpid());
    }

    write(pipefd[1][1], "hello\n", 6);
    close(pipefd[1][1]);

  } else {
    char sendchar = 'a';
    write(pipefd[0][1], &sendchar, 1);
    close(pipefd[0][1]);

    char readchar[64];
    int charnum = read(pipefd[1][0], readchar, 64);
    close(pipefd[1][0]);

    if(charnum > 0) {
      printf("%d: received pong\n", getpid());
    }
  }
  exit(0);
}