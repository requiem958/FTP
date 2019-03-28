#include "csapp.h"
#include "commands.h"

command parse_command(char buff[], int bufSize){
  command cmd;
  const char delim[] = {' ','\t','\0','\n'};
  bzero(cmd.cmdline,bufSize);
  bzero(cmd.arg,MAX_NAME_LEN);
  cmd.type = ERR;

  strcpy(cmd.cmdline,buff);
  char * mot = strtok(buff,delim);
  ER_EOL(mot);
  if(STR_EQ(mot,"GET")){
    mot = strtok(NULL,delim);
    if (mot==NULL){
      return cmd;
    }
    strcpy(cmd.arg, mot);
    cmd.arg[strlen(cmd.arg)-1]=0;

    cmd.type = GET;
  }
  else if (STR_EQ(mot,"REST")){
    mot = strtok(NULL,delim);
    if (mot==NULL){
      return cmd;
    }
    strcpy(cmd.arg, mot);

    mot = strtok(NULL,delim);
    if (mot==NULL){
      return cmd;
    }
    ER_EOL(mot);
    cmd.size = atol(mot);

    cmd.type = REST;
  }
  else if(STR_EQ(mot,"BYE")){
    cmd.type = BYE;
  }
  return cmd;
}
