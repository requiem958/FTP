#ifndef FTPS_UTILS
#define FTPS_UTILS

#include "csapp.h"


#define MAX_NAME_LEN 256
#define BUF_LEN 1024
#define STR_EQ(var, string) (strcasecmp(var,string)==0)

typedef struct {
  struct sockaddr_in adr;
  int fd;
}slaveStruct;

void redirectToSlave(int clientfd, slaveStruct slave);

void server(int listenfd, slaveStruct slave);

void discoverSlave(slaveStruct tabSlave[], int SLAVE_PORT, int taille, int slavefd);


#endif
