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
  mot[strlen(mot)-1]=(mot[strlen(mot)-1]=='\n')?0:mot[strlen(mot)-1];
  if(STR_EQ(mot,"GET")){
    cmd.type = GET;
    mot = strtok(NULL,delim);
    strcpy(cmd.arg, mot);
    cmd.arg[strlen(cmd.arg)-1]=0;//effacer le '\n'
  }
  else if(STR_EQ(mot,"BYE")){
    cmd.type = BYE;
  }
  return cmd;
}
