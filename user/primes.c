#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define RD 0 //pipe的read端
#define WR 1 //pipe的write端
#define INT_LEN sizeof(int)

/// @brief 读取左邻居的数据，将第一个数据打印出来
/// @param lpipe 左邻居的管道符
/// @param dst 右邻居的管道符
/// @return 第一个数据有数据返回1， 没有数据范围0
int lpipe_first_data(int lpipe[2], int *dst) 
{
  if(read(lpipe[RD], dst, INT_LEN) == INT_LEN) {
    printf("prime %d\n", *dst);
    return 1;
  }
  return 0;
}

/**
 * @brief 读取左邻居的数据，将不能被first整除的写入右邻居
 * @param lpipe 左邻居的管道符
 * @param rpipe 右邻居的管道符
 * @param first 左邻居的第一个数据
 */
void transmit_data(int lpipe[2], int rpipe[2], int first) 
{
  int num;
  while(read(lpipe[RD], &num, INT_LEN) == INT_LEN) {
    if(num % first) {
      write(rpipe[WR], &num, INT_LEN);
    }
  }
  close(lpipe[RD]);
  close(rpipe[WR]);
}

/**
 * @brief 寻找素数
 * @param lpipe 左邻居管道
 */
void primes(int lpipe[2]) {
  close(lpipe[WR]);

  int first;
  if(lpipe_first_data(lpipe, &first) == 1) {
    int pipefd[2];
    pipe(pipefd);
    transmit_data(lpipe, pipefd, first);

    if(fork() == 0) {
      primes(pipefd);
    } else {
      close(pipefd[RD]);
      // close(pipefd[WR]);
      wait(0);
    }
  }
  exit(0);
}

int main(int argc, char *argv[])
{
  int pipefd[2];
  pipe(pipefd);

  for(int i = 2; i <= 35; i++) {
    write(pipefd[WR], &i, INT_LEN);
  }

  if(fork() == 0) {
    primes(pipefd);
  } else {
    close(pipefd[WR]);
    close(pipefd[RD]);
    wait(0);
  }

  exit(0);
}


