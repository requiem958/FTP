#ifndef COMMANDS
#define COMMANDS

#define MAX_NAME_LEN 256
#define BUF_LEN 1024
#define STR_EQ(var, string) (strcasecmp(var,string)==0)

typedef enum {
  GET,
  BYE,
  ERR
}command_type;

typedef struct {
  command_type type;
  char arg[MAX_NAME_LEN];
  char cmdline[MAXLINE];
}command;

command parse_command(char buff[], int tailleBuff);
#endif
