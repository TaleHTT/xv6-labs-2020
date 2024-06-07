#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if(argc < 2){
    fprintf(2, "Usage: sleep files...\n");
    exit(1);
  }
  
  int sleepTime = atoi(argv[1]);
  sleep(sleepTime);

  exit(0);
}