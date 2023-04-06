#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc,char *argv[]){
  int pipe_N[2];
  // create a new pipe and records the read and write file descriptors in array pipe_N[2].
  pipe(pipe_N);
  int pid_R = fork();

  if(pid_R == 0){
    write(pipe_N[0],"",1);
    printf("%d: received ping\n",getpid());
  }else{
    wait(&pid_R);
    read(pipe_N[1],"",1);
    printf("%d: received pong\n",getpid());
  }
  exit(0);
}