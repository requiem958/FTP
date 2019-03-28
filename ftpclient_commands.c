#include "ftpclient_commands.h"

void redirection (int * fd, rio_t *rio){
  char ip_text[15];
  Rio_readnb(rio, &ip_text, 15);

  Close(*fd);
  *fd = Open_clientfd(ip_text,FTP_PORT);

  printf("client connected to slave\n");

}

void tmpFileName(char *src, char * dst, char *suff){
  sprintf(dst,"%s.%s",src,suff);
}

int try_to_reget_file(command * cmd){
  if (cmd->type != GET){
    return 0;
  }
  char tmpname[MAX_NAME_LEN];
  tmpFileName(cmd->arg,tmpname,"tmp");
  if (access(tmpname, F_OK) == -1){
    return 0;
  }
  return 1;
}

void changeToReget(command *cmd){
  char tmpname[MAX_NAME_LEN];
  tmpFileName(cmd->arg,tmpname,"tmp");
  cmd->type = REST;

  struct stat infosFichier;
  int fd = open(tmpname,O_RDONLY,0);
  fstat(fd, &infosFichier);
  cmd->size =infosFichier.st_size;

  close(fd);

  bzero(cmd->cmdline,MAXLINE);
  sprintf(cmd->cmdline, "REST %s %ld\n", cmd->arg, cmd->size);
}

int send_command(int serveurfd,rio_t *rio,rio_t *rioUser, command*cmd){
  static char buff_cmd[MAXLINE];
  char buff[4] = {0,0,0,0};
  printf("ftp>");
  fflush(stdout);

  Rio_readlineb(rioUser,buff_cmd,MAXLINE);

  *cmd = parse_command(buff_cmd,MAXLINE);

  if (try_to_reget_file(cmd)){
    changeToReget(cmd);
  }

  Rio_writen(serveurfd, cmd->cmdline, strlen(cmd->cmdline));
  int n=Rio_readnb(rio, buff, 3);

  if(n!=3){
    return -1;
  }
  return asciiToInt(buff[0])*100+asciiToInt(buff[1])*10+asciiToInt(buff[2]);
}

void retrievefile(char *name, rio_t *rio, int connfd){
  char tmpname[MAX_NAME_LEN];
  tmpFileName(name,tmpname,"tmp");
  getfile(name,tmpname,rio,connfd);
}

void regetfile(char *name, rio_t *rio, int connfd){
  double time;
  int fd = 0;
  int n = 1;
  int octetsRecu = 0;
  static char buf[MAXLINE];
  int32_t tailleFichier;

  char tmpname[MAX_NAME_LEN];
  tmpFileName(name,tmpname,"tmp");

  fd = Open(tmpname,O_WRONLY | O_APPEND,DEF_MODE & ~DEF_UMASK);

  Rio_readnb(rio, &tailleFichier,4);
  tailleFichier = ntohl(tailleFichier);
  printf("Taille fichier restante : %d\n", tailleFichier);

  CLOCK_START(tstart,tend);

  while (octetsRecu + MAXLINE <= tailleFichier &&
    (n=rio_readnb(rio, buf, MAXLINE)) > 0) {
      Rio_writen(fd,buf,n);
      octetsRecu += n;
    }

  if ( (n > 0) && octetsRecu < tailleFichier){
    bzero(buf,MAXLINE);
    n=rio_readnb(rio, buf, tailleFichier - octetsRecu);
    if (n > 0){
      Rio_writen(fd,buf,n);
      octetsRecu += n;
    }
  }

  Close(fd);

  CLOCK_END(tstart, tend);
  time = (double) ELAPSED_TIME(tstart,tend) / CLOCKS_PER_SEC;

  if (n <= 0){
    printf("ERREUR Transfert fini trop tôt !!\n");
  }
  else{
    rename(tmpname,name);
    printf("Transfert fini\n");
  }

  printf( "%d octets reçus sur %d en %lf secondes (%d KB/s).\n",
  octetsRecu,tailleFichier, time,
  (int) (octetsRecu / (time * 1000)));
}

void getfile(char *name, char *tmpname, rio_t *rio, int connfd){
  double time;
  int fd = 0;
  int n = 1;
  int octetsRecu = 0;
  static char buf[MAXLINE];
  int32_t tailleFichier;

  fd = Open(tmpname,O_WRONLY | O_CREAT,DEF_MODE & ~DEF_UMASK);

  Rio_readnb(rio, &tailleFichier,4);
  tailleFichier = ntohl(tailleFichier);
  printf("Taille fichier : %d\n", tailleFichier);

  CLOCK_START(tstart,tend);

  while (octetsRecu + MAXLINE <= tailleFichier &&
    (n=rio_readnb(rio, buf, MAXLINE)) > 0) {
      Rio_writen(fd,buf,n);
      octetsRecu += n;
    }
  if ( (n > 0) && octetsRecu < tailleFichier){
    bzero(buf,MAXLINE);
    n=rio_readnb(rio, buf, tailleFichier - octetsRecu);
    if (n > 0){
      Rio_writen(fd,buf,n);
      octetsRecu += n;
    }
  }

  Close(fd);

  CLOCK_END(tstart, tend);
  time = (double) ELAPSED_TIME(tstart,tend) / CLOCKS_PER_SEC;

  if (n <= 0){
    printf("ERREUR Transfert fini trop tôt !!\n");
  }
  else{
    rename(tmpname,name);
    printf("Transfert fini\n");
  }

  printf( "%d octets reçus sur %d en %lf secondes (%d KB/s).\n",
  octetsRecu,tailleFichier, time,
  (int) (octetsRecu / (time * 1000)));

  }
