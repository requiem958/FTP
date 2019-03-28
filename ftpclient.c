
#include "csapp.h"
#include "ftpclient_commands.h"



int main(int argc, char **argv)
{
  int serverfd, port;
  int return_code;
  int quit =0;
  char *host;
  command cmd;

  rio_t rio;
  rio_t rioUser;

  if (argc != 2) {
    fprintf(stderr, "usage: %s <host>\n", argv[0]);
    exit(0);
  }
  host = argv[1];
  port = FTP_PORT;

  /*
  * Note that the 'host' can be a name or an IP address.
  * If necessary, Open_clientfd will perform the name resolution
  * to obtain the IP address.
  */
  serverfd = Open_clientfd(host, port);

  /*
  * At this stage, the connection is established between the client
  * and the server OS ... but it is possible that the server application
  * has not yet called "Accept" for this connection
  */
  printf("client connected to server OS\n");

  Rio_readinitb(&rio, serverfd);
  Rio_readinitb(&rioUser, 0);
  redirection(&serverfd, &rio);
  Rio_readinitb(&rio, serverfd);

  while((quit!=1) && return_code  < 500){
    return_code = send_command(serverfd, &rio,&rioUser, &cmd);
    if (return_code <500){
      switch(cmd.type){
        case GET:
          retrievefile(cmd.arg,&rio,serverfd);
          break;
        case BYE:
          quit = 1;
          break;
        default:
        break;
      }
    }else{
      switch (return_code) {
        case 502:
          printf("Commande inconnue : %s\n",cmd.cmdline);
          break;
        case 550:
          printf("Le fichier n'existe pas : %s\n",cmd.arg);
          break;
        }
      }
    }
  Close(serverfd);
  exit(0);
}
