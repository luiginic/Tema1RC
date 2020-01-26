#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <dirent.h>
#include "Info.h"

struct Command{
    char *command;
    char *argument;
};

char *pathFound;

const char userPath[50] = "/home/david/CLionProjects/Retele/users.txt"; //TODO

int loginAction( char username[]){
    int fd1[2],fd2[2];
    char usernameFound[30];
    pid_t process;

    if(pipe(fd1)==-1) {
        perror("Pipe1 failed\n");
        return;
    }
    if(pipe(fd2)==-1){
        perror("Pipe2 failed\n");
        return;
    }

    process = fork();

    if(process <0){

      //in case the fork fails
      perror("Fork failed\n");
      exit(1);
    }

    else if(process > 0){

       //parent process
      close(fd1[0]);

      write(fd1[1],username,strlen(username)+1);
      close(fd1[1]);

      while(wait(NULL) != -1);

      close(fd2[1]);

      char foundParent[4];
      read(fd2[0],foundParent,sizeof(foundParent));
      printf("Value of foundParent: '%s'\n",foundParent);

      if(strcmp(foundParent,"OK")==0){

          printf("Access granted\n");
          return 1;
        }
      else{

          printf("Access denided\n");
          return 0;
        }


    } else {

      //child process
      close(fd1[1]);

      char found[4] = "NOT";

      char usernameToFind[30];
      read(fd1[0],usernameToFind,sizeof(usernameToFind));

      FILE * users;
      users = fopen("/home/david/CLionProjects/Retele/Resources/users.txt", "r");
      if(users != NULL){
          while(fgets(usernameFound, 100, users)>0) {
              usernameFound[strlen(usernameFound)-1]='\0'; //eliminate the '\n' charachter
              if(strcmp(usernameToFind,usernameFound)==0) {
                  strcpy(found,"OK");
                  printf("The usernames found in file '%s' value of found is '%s' \n", usernameFound, found);

                  fclose(users);
              }
          }
      } else perror("File not found");

      close(fd1[0]);
      close(fd2[0]);

      printf("Value of found: '%s'\n",found);
      write(fd2[1],found, strlen(found)+1);
      close(fd2[1]);

      exit(1);
    }

}

void search(const char *name,char filename[], char **filepath){

    DIR *dir;
    struct dirent *direntp;

    if (!(dir = opendir(name)))
        return;

    while ((direntp = readdir(dir)) != NULL) {
        if (direntp->d_type == DT_DIR) {
            char path[1024];
            if (strcmp(direntp->d_name, ".") == 0 || strcmp(direntp->d_name, "..") == 0)
                continue;
            snprintf(path, sizeof(path), "%s/%s", name, direntp->d_name);
            // printf("[%s]\n", entry->d_name);
            search(path,filename,filepath);
        } else {
            // printf("- %s\n", entry->d_name);
            if(strcmp(direntp->d_name,filename) == 0){
              if(strlen(pathFound)==0){ //comment the condition of if an leave just the block whitin to display the last file found with this name
              *filepath = malloc(strlen(name)+strlen(direntp->d_name)+2); //alocates memory to the address of pathFound to store the path
              memset(*filepath,0,strlen(name)+strlen(direntp->d_name)+2); //cleans memory to the address of pathFound
              snprintf(*filepath, 1024, "%s/%s", name, direntp->d_name); //this can be writen as strcpy(*filepath,name); strcat(*filepath,"/"); strcat(filepath,direntp->d_name);
            }//this if guarantees i will get the first file found with this name
            // printf("!!!!!%s!!!!!\n",*filepath); //my check
            break;
            }
        }
    }
    closedir(dir);
    // printf("!!!!!%s!!!!!\n",filepath);
}

char *findFile(char filename[]){

  printf("[findFIle] Searching for '%s'\n",filename);
  ///////////////////////////////////////////////////////////////////
  //actual searching
  pathFound = malloc(0); //allocates 0 memory to pathFound before search() to make sure that if i don't find anything the function will return "File not found"
  memset(pathFound,0,0); //cleans the pathFound before search() to duble check the fact that it is empty
  search("/home/david/CLionProjects/Retele",filename,&pathFound); //modify the first parameter to change the start directory
  // printf("***%s****\n",pathFound); //my check
  if(strlen(pathFound)!=0){
    return pathFound;
  }

  //////////////////////////////////////////////////////////////////
  return "File not found!";
}

int myfindAction(char filename[]){

  int socket[2];
  pid_t process;

  if(socketpair(AF_UNIX,SOCK_STREAM,0,socket)<0){

    perror("Error on socketpair\n");
    exit(1);
    }

    process = fork();

    if(process <0){

      //in case the fork fails
      perror("Fork failed\n");
      exit(1);
    }

    else if(process > 0){

       //parent process
      // char response[1024];
      close(socket[0]);

      write(socket[1],filename,strlen(filename)+1);

      while(wait(NULL) != -1);

      char* response;
      response = malloc(sizeof(char)*sizeof(struct info));

      read(socket[1],response,sizeof(struct info));

      struct info *fileInfo = (struct info*)response;
      printInfo(*(fileInfo));
      // printf("Your file is located at:\n'%s'\n",response);

      close(socket[1]);

    } else {

      //child process
      char *answer;
      close(socket[1]);

      char fileToFind[30];
      read(socket[0],fileToFind,sizeof(fileToFind));
      printf("I must find the file: '%s'\n",fileToFind);

      //this is where findFile is called to search for the path of the file
      answer = findFile(fileToFind);
      /////////////////////////////////
      struct info infos;

      getInfo(answer,fileToFind,&infos); //here we gather the info for the file and save it as a struct
      // printInfo(infos);
      void * ptr = &infos; //a pointer to pass the struct to the parent
      /////////////////////////////////

      write(socket[0],(char*)ptr,sizeof(infos));
      close(socket[0]);

      exit(1);
    }

  return 1;
}

int mystatAction(char filename[]){

  int fd;

  char* FIFO = "/tmp/myfifo";

  mkfifo(FIFO,0666);

  char response[1024];

  while(1){

    fd = open(FIFO,O_WRONLY);

    write(fd,filename,strlen(filename)+1);
    close(fd);

    fd = open(FIFO,O_RDONLY);

    read(fd,response,sizeof(response));
    close(fd);

    printf("Attributes are: '%s'\n",response);

    break;

  }

  return 1;
}

struct Command setCommand(char input[]){
    struct Command myCommand;

    myCommand.command = strtok(input," ");
    myCommand.argument = strtok(NULL," ");

    printf("Command: '%s' with arguments '%s'\n",myCommand.command, myCommand.argument);
    return myCommand;
}

int action(struct Command command, _Bool *running){
    //what to do if command is 'quit'
    *(running) = strcmp(command.command,"quit");
    if(*(running)==0)
      exit(0);

    //what to do if command in 'login'
    if(strcmp(command.command,"login")==0)
        return loginAction(command.argument);

    //what to do if command is  'myfind'
    if(strcmp(command.command,"myfind")==0)
      return myfindAction(command.argument);

    //what to do if command is  'myfind'
    if(strcmp(command.command,"mystat")==0)
      return mystatAction(command.argument);

    else printf("!Wrong command, try again!!\n");

}
