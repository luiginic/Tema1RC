#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <langinfo.h>


struct info{
  char fileName[1024];
  char filePath[1024];
  char dateModified[256];
  int size;
  char rights[10];

};

// char* serializeInfo(struct info infos){
// int  nameL= 0;
// int tLen = 0;
// char* serialized = 0;
// char* metadata = 0;
// char* infoName = 0;
// // char* infoPath = 0;
// // char* infoDatestring = 0;
// char* infoSize = 0;
// // char* infoRights = 0;
//
// tLen = sizeof(int) + strlen(infos.fileName);//+strlen(infos.filePath)+strlen(infos.datestring)+strlen(info.rights);
// serialized = malloc(sizeof(char) * (tLen + sizeof(int)));
// metadata = serialized;
// infoSize = serialized + sizeof(int);
// infoName = infoSize + sizeof(int);
// *((int*)metadata) = strlen(infos.fileName);
// *((int*)infoSize) = infos.size;
// strncpy(infoName, infos.fileName,strlen(infos.fileName));
// // infoPath = infoName + sizeof(int);
// printf("&&&&&&%s&&&&&&&\n",serialized);
// return serialized;
// }

// struct info deserialize(char* serialized){
//
//   struct info inf;
//   char* metadata = serialized;
//   char* infoSize = metadata + sizeof(int);
//   char* infoName = infoSize + sizeof(int);
//   int nameL = *((int*)metadata);
//   inf.size = *((int*)infoSize);
//   inf.fileName = malloc((nameL+1) * sizeof(char));
//   strncpy(inf.fileName, infoName,nameL);
//   inf.fileName[nameL] = '\0';
//   return inf;
// }

void printInfo(struct info infos){
  if(strcmp(infos.fileName,"")==0)
      printf("File not found!!\n");
  else{
    printf("/////////////////////\n");
    printf("The file '%s' is located in\n%s\n",infos.fileName,infos.filePath);
    printf("Last modified on: %s\n",infos.dateModified);
    printf("And has a size of: %d bytes\n",infos.size);
    printf("permissions: %s\n",infos.rights);
    printf("/////////////////////\n");
  }
}


void getInfo(char* path,char* filename, struct info* infos){
  
    struct tm *tm;
    struct stat fileStat;

    if(stat(path,&fileStat) < 0)
        return;

    strcpy(infos->fileName,filename);
    strcpy(infos->filePath, path);

    infos->size = fileStat.st_size;

    tm=localtime(&fileStat.st_mtim);
    strftime(infos->dateModified, sizeof(infos->dateModified), nl_langinfo(D_T_FMT), tm);

    strcpy(infos->rights,(fileStat.st_mode & S_IRUSR) ? "r" : "-");
    strcat(infos->rights,(fileStat.st_mode & S_IWUSR) ? "w" : "-");
    strcat(infos->rights,(fileStat.st_mode & S_IXUSR) ? "x" : "-");
    strcat(infos->rights,(fileStat.st_mode & S_IRGRP) ? "r" : "-");
    strcat(infos->rights,(fileStat.st_mode & S_IWGRP) ? "w" : "-");
    strcat(infos->rights,(fileStat.st_mode & S_IXGRP) ? "x" : "-");
    strcat(infos->rights,(fileStat.st_mode & S_IROTH) ? "r" : "-");
    strcat(infos->rights,(fileStat.st_mode & S_IWOTH) ? "w" : "-");
    strcat(infos->rights,(fileStat.st_mode & S_IXOTH) ? "x" : "-");

}
