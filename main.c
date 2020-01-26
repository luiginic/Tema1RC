#include <stdio.h>
#include "Command.h"

struct Command newCommand;
char user[100];

_Bool running = 1;

void listenLoginMenuCommand(){
    char command[100];
    char *comm;
    fgets(command,100,stdin);
    fflush(stdin);
    comm = strtok(command,"\n");
    strcpy(command,comm);
    printf(" Your command was: '%s' no. of letters: %d\n",command,strlen(command));
    fflush(stdout);
    newCommand = setCommand(comm);
    if(action(newCommand, &running)==1){
      strcpy(user,newCommand.argument);
      printMainMenu();
    }
}

void listenMainMenuCommand(){
  char command[100];
  char *comm;
  fgets(command,100,stdin);
  fflush(stdin);
  comm = strtok(command,"\n");
  strcpy(command,comm);
  printf(" Your command was: '%s' no. of letters: %d\n",command,strlen(command));
  fflush(stdout);
  newCommand = setCommand(comm);
  action(newCommand, &running);
  printMainMenu();
}

void printMainMenu(){

  printf("----------WELCOME %s!----------\n",user);
  printf("1. Type 'myfind [file name]' to display the path of the specified file.\n" );
  printf("2. Type 'mystat [file name]' to display the stats of the specified file.\n");
  printf("3. Type 'quit' to exit the program.\n");
  listenMainMenuCommand();
}

void printLoginMenu(){
    printf("1. Type 'login [username]' to log in.\n");
    printf("2. Type 'quit' to exit the program\n");
    fflush(stdout);
    listenLoginMenuCommand();
}

int main() {
    while(running){
        printLoginMenu();
    }
    return 0;
}
