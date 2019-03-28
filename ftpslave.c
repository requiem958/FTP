#include "csapp.h"
#include "ftpslave_utils.h"

#define NPROC 4
#define FTP_PORT 2121
#define MASTER_PORT 2122

void handler(int sig){
  kill(0,sig);
  exit(0);
}

void handler_child(int sig){
  while(waitpid(-1, NULL, 0) > 0);
}

int main(int argc, char **argv){
  int i,masterfd;
  int listenfd;
  char *host;

  if (argc != 2) {
    fprintf(stderr, "usage: %s <host>\n", argv[0]);
    exit(0);
  }
  host = argv[1];
  masterfd = Open_clientfd(host, MASTER_PORT);
  listenfd = Open_listenfd(FTP_PORT);
  printf("slave connected to server OS\n");

  Signal(SIGINT, handler);
  Signal(SIGTERM, handler);
  Signal(SIGCHLD, handler_child);

  for(i=0; i<NPROC-1; i++){
    if (Fork() == 0){
      while(1){
        slaveServer(listenfd);
      }
    }
  }
  while(1){
      slaveServer(listenfd);
    }

}
