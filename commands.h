#ifndef COMMANDS
#define COMMANDS

#include "csapp.h"
#define MAX_NAME_LEN 256
#define BUF_LEN 1024
#define STR_EQ(var, string) (strcasecmp(var,string)==0)
#define ER_EOL(mot) mot[strlen(mot)-1]=(mot[strlen(mot)-1]=='\n')?0:mot[strlen(mot)-1]

typedef enum {
  GET,
  REST,
  BYE,
  ERR
}command_type;

typedef struct {
  command_type type;
  char arg[MAX_NAME_LEN];
  char cmdline[MAXLINE];
  long size;
}command;

command parse_command(char buff[], int tailleBuff);
#endif
