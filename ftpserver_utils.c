#include "ftpserver_utils.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


void server(int listenfd, slaveStruct slave){
  int clientfd;
  socklen_t clientlen;
  struct sockaddr_in clientaddr;
  char client_ip_string[INET_ADDRSTRLEN];
  char client_hostname[MAX_NAME_LEN];
  clientlen = (socklen_t)sizeof(clientaddr);

  clientfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
  /* determine the name of the client */
  Getnameinfo((SA *) &clientaddr, clientlen,client_hostname, MAX_NAME_LEN, 0, 0, 0);

  /* determine the textual representation of the client's IP address */
  Inet_ntop(AF_INET, &clientaddr.sin_addr, client_ip_string,INET_ADDRSTRLEN);

  printf(">server connected to %s (%s) port : %u\n", client_hostname,client_ip_string,clientaddr.sin_port);

  redirectToSlave(clientfd, slave);

  Close(clientfd);
  printf("Client sended to slave\n");
}

void redirectToSlave(int clientfd, slaveStruct slave){
    char slave_ip_string[INET_ADDRSTRLEN] = {0};
    char slave_hostname[MAX_NAME_LEN];
    Getnameinfo((SA *) &slave.adr, sizeof(slave.adr),
      slave_hostname, MAX_NAME_LEN, 0, 0, 0);
    /* determine the textual representation of the client's IP address */
    Inet_ntop(AF_INET, &slave.adr.sin_addr, slave_ip_string,INET_ADDRSTRLEN);
    Rio_writen(clientfd, &slave_ip_string,INET_ADDRSTRLEN);
}

void discoverSlave(slaveStruct tabSlave[], int SLAVE_PORT, int taille, int slavefd){

  int i;
  socklen_t slavelen;
  struct sockaddr_in slaveaddr;
  slavelen = (socklen_t)sizeof(slaveaddr);
  for(i=0;i<taille;i++){
    tabSlave[i].fd = Accept(slavefd, (SA *)&tabSlave[i].adr, &slavelen);
    printf("Slave n° %d decouvert\n",i);
  }

}
