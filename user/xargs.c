#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

#define RD 0 //pipe的read端
#define WR 1 //pipe的write端
#define INT_LEN sizeof(int)


int main(int argc, char *argv[])
{
  char std_in[512];
  int size = read(0, std_in, sizeof std_in);

  int line = 0;
  for(int i = 0; i < size; i++) {
    if(std_in[i] == '\n') {
      line++;
    }
  }

  char out_put[line][64];
  int i = 0, j = 0;
  for(int k = 0; k < size; k++) {
    out_put[i][j++] = std_in[k];
    if (std_in[k] == '\n') {
        out_put[i][j - 1] = 0; // 用0覆盖掉换行符。C语言没有字符串类型，char类型的数组中，'0'表示字符串的结束
        ++i; // 继续保存下一行数据
        j = 0;
    }
  }

  char *arguments[MAXARG];
  for(j = 0; j < argc-1; ++j) {
    arguments[j] = argv[j + 1];
  }
  i = 0;
  while(i < line) {
    arguments[j] = out_put[i++];
    if(fork() == 0) {
      exec(argv[1], arguments);
      exit(0);
    }
    wait(0);
  }
  
  exit(0);
}
