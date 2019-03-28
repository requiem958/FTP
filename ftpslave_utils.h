#ifndef FTPS_SLAVE_UTILS
#define FTPS_SLAVE_UTILS

#include "csapp.h"
#include "commands.h"

#define MAX_NAME_LEN 256
#define BUF_LEN 1024
#define STR_EQ(var, string) (strcasecmp(var,string)==0)

int send_file(char * name, int clientfd);

int execute_command(command * cmd, int fd);

void slaveServer(int listenfd);


#endif
