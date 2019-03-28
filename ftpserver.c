#include "csapp.h"
#include "ftpserver_utils.h"

#define NPROC 4
#define NSLAVE 1
#define FTP_PORT 2121
#define SLAVE_PORT 2122

void handler(int sig){
  kill(0,sig);
  exit(0);
}

void handler_child(int sig){
  while(waitpid(-1, NULL, 0) > 0);
}

int main(int argc, char **argv){
  int listenfd, client_port, i;
  int slavefd;
  slaveStruct tabSlave[NSLAVE];
  Signal(SIGINT, handler);
  Signal(SIGTERM, handler);
  Signal(SIGCHLD, handler_child);

  client_port = FTP_PORT;
  listenfd = Open_listenfd(client_port);
  slavefd = Open_listenfd(SLAVE_PORT);
  discoverSlave(tabSlave,SLAVE_PORT, NSLAVE,slavefd);
  Close(slavefd);
  i=0;
  while(1){
    server(listenfd,tabSlave[i]);
    i=(i+1)%NSLAVE;
  }

}
