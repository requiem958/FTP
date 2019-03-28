#include "ftpslave_utils.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int send_file(char * name, int clientfd){
  size_t n;

  char buf[BUF_LEN];
  int fd = 0;
  rio_t fileAsked;
  struct stat infosFichier;

  printf(">Server received request for file (%s)\n",name);

  if ((fd = open(name,O_RDONLY,0))<0){
    return -1;
  }
  else{
    printf("FILE %s exists.\n",name);
    Rio_writen(clientfd, "150", 3);
    bzero(buf,BUF_LEN);
    Rio_readinitb(&fileAsked, fd);

    puts(">BEGIN");
    fstat(fd, &infosFichier);
    *((int32_t*) buf) = htonl(infosFichier.st_size);
    printf("File size : %d\n", (int32_t)infosFichier.st_size);
    Rio_writen(clientfd, buf,4);

    while ((n=Rio_readnb(&fileAsked,buf,BUF_LEN)) > 0) {
      printf(">Server send %ld bytes\n",n);
      Rio_writen(clientfd, buf, n);
      bzero(buf,BUF_LEN);
    }
    puts(">END");
    return 0;
  }
}

int resend_file(char * name, long size, int clientfd){
  size_t n;

  char buf[BUF_LEN];
  int fd = 0;
  rio_t fileAsked;
  struct stat infosFichier;

  printf(">Server received request to restart transfert for file (%s)\n",name);

  if ((fd = open(name,O_RDONLY,0))<0){
    return -1;
  }
  else{
    printf("FILE %s exists.\n",name);
    bzero(buf,BUF_LEN);

    puts(">BEGIN");
    fstat(fd, &infosFichier);
    *((int32_t*) buf) = htonl(infosFichier.st_size-size);

    if (lseek(fd,size,SEEK_SET) < size){
      close(fd);
      return -1;
    };
    Rio_writen(clientfd, "150", 3);

    printf("Remaining file size : %d\n", (int32_t) (infosFichier.st_size-size));
    Rio_writen(clientfd, buf,4);

    Rio_readinitb(&fileAsked, fd);
    while ((n=Rio_readnb(&fileAsked,buf,BUF_LEN)) > 0) {
      printf(">Server send %ld bytes\n",n);
      Rio_writen(clientfd, buf, n);
    }
    puts(">END");
    return 0;
  }
}

void slaveServer(int listenfd){
  int clientfd;
  socklen_t clientlen;
  struct sockaddr_in clientaddr;
  char client_ip_string[INET_ADDRSTRLEN];
  char client_hostname[MAX_NAME_LEN];
  command cmd;
  char buff_cmd[MAXLINE];
  rio_t rio;
  int n;
  int quit=0;

  clientlen = (socklen_t)sizeof(clientaddr);

  clientfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
  /* determine the name of the client */
  Getnameinfo((SA *) &clientaddr, clientlen,client_hostname, MAX_NAME_LEN, 0, 0, 0);

  /* determine the textual representation of the client's IP address */
  Inet_ntop(AF_INET, &clientaddr.sin_addr, client_ip_string,INET_ADDRSTRLEN);

  printf(">server connected to %s (%s) port : %u\n", client_hostname,client_ip_string,clientaddr.sin_port);

  Rio_readinitb(&rio,clientfd);

  while((quit!=1)&&(n = Rio_readlineb(&rio,buff_cmd,MAXLINE)) > 0){
    cmd = parse_command(buff_cmd,strlen(buff_cmd));
    printf("Commande recue : \"%s\"\n",cmd.cmdline);
    if (execute_command(&cmd,clientfd)==-1){
      quit =1;
    }
    bzero(buff_cmd,MAXLINE);
  }
  Close(clientfd);

  printf(">Connection to %s (%s) port : %u CLOSED\n", client_hostname,client_ip_string,clientaddr.sin_port);
}


int execute_command(command * cmd, int fd){
  if (cmd->type == ERR){
    Rio_writen(fd, "502", 3);
  }

  switch (cmd->type){
    case GET:
    if(send_file(cmd->arg,fd)==-1){
      Rio_writen(fd, "550", 3);
    }
    break;
    case REST:
    if (resend_file(cmd->arg,cmd->size,fd) == -1){
      Rio_writen(fd, "550", 3);
    }
    case BYE:
    Rio_writen(fd, "150", 3);
    return -1;
    case ERR:
    break;
  }
  return 0;
}
