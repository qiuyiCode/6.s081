#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

int main(int argc, char *argv[])
{
  if(argc == 1)
  {
    fprintf(2,"argc must more than 2!\n");
    exit(1);
  }

  // 分配字符串参数数组空间
  char **argument = (char**)malloc(sizeof(char*) * MAXARG);

  int i;
  // 将xargs后面的参数放入argument中
  for(i = 0;i < argc-1;i++){
    int len = strlen(argv[i+1]) + 1;
    argument[i] = malloc(sizeof(char) * len);
    strcpy(argument[i],argv[i+1]);
  }

  char buffer[100];
  char *p = buffer;
  
  while(read(0,p,1) > 0){
    if(*p == '\n'){
      *p = '\0';
      int len = strlen(buffer) + 1;
      argument[i] = (char*)malloc(sizeof(char) * len);
      strcpy(argument[i],buffer);
      memset(buffer,0,100);
      p = buffer;
      i++;
      continue;
    }
    p++;
  }
  
  if(fork() == 0){
    if(exec(argv[1],argument) < 0){
      fprintf(2,"exec Error!\n");
      exit(1);
    }
  }
  int status;
  wait(&status);
  exit(0);
}
