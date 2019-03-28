#ifndef FTPC_COM
#define FTPC_COM
#include <stdio.h>
#include "csapp.h"
#include "commands.h"

#define MAX_NAME_LEN 256
#define FTP_PORT 2121

#define asciiToInt(x) ((x)-'0')

#define CLOCK_START(tsart,tend) clock_t tstart, tend; tstart = clock()
#define CLOCK_END(tstart,tend) tend = clock();
#define ELAPSED_TIME(tstart,tend) ((unsigned long long) tend - (unsigned long long) tstart)


void tmpFileName(char *src, char * dst, char *suff);

int send_command(int serveurfd,rio_t *rio,rio_t * rioUser, command* cmd);

void getfile(char *name, char * tmpname, rio_t *rio, int connfd);

void regetfile(char *name, rio_t *rio, int connfd);
void redirection (int * fd, rio_t *rio);

void retrievefile(char *name, rio_t *rio, int connfd);



#endif
