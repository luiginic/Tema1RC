#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/msdos_fs.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <linux/fs.h>
#include <linux/fsmap.h>
#include "Command.h"

char fileAttributes[15];

void attributes(char filename[]){



  int attr;
  int fd;

  fd = open(findFile(filename),"r");


  ioctl(fd,FS_IOC_GETFLAGS,&attr);
  fileAttributes[0] = (attr & FS_APPEND_FL) ? 'a' : '-';
  fileAttributes[1]= (attr & FS_COMPR_FL) ? 'c' : '-';
  fileAttributes[2]=(attr & FS_DIRSYNC_FL) ? 'D' : '-';
  fileAttributes[3]=(attr & FS_IMMUTABLE_FL) ? 'i' : '-';
  fileAttributes[4]=(attr & FS_JOURNAL_DATA_FL) ? 'j' : '-';
  fileAttributes[5]=(attr & FS_NOATIME_FL) ? 'A' : '-';
  fileAttributes[6]=(attr & FS_NOCOW_FL) ? 'C' : '-';
  fileAttributes[7]=(attr & FS_NODUMP_FL) ? 'd' : '-';
  fileAttributes[8]=(attr & FS_NOTAIL_FL) ? 't' : '-';
  fileAttributes[9]=(attr & FS_PROJINHERIT_FL) ? 'P' : '-';
  fileAttributes[10]=(attr & FS_SECRM_FL) ? 's' : '-';
  fileAttributes[11]=(attr & FS_SYNC_FL) ? 'S' : '-';
  fileAttributes[12]=(attr & FS_TOPDIR_FL) ? 'T' : '-';
  fileAttributes[13]=(attr & FS_UNRM_FL) ? 'u' : '-';

  printf("Attributes are: '%d'\n",attr);

  // struct stat fileStat;
  // cahr attributes[10];
  //
  // if(stat(path,&fileStat) < 0)
  //     return;
  //
  // strcpy(atttributes,(fileStat.st_mode & S_IRUSR) ? "r" : "-");
  // strcat(atttributes,(fileStat.st_mode & S_IWUSR) ? "w" : "-");
  // strcat(atttributes,(fileStat.st_mode & S_IXUSR) ? "x" : "-");
  // strcat(atttributes,(fileStat.st_mode & S_IRGRP) ? "r" : "-");
  // strcat(atttributes,(fileStat.st_mode & S_IWGRP) ? "w" : "-");
  // strcat(atttributes,(fileStat.st_mode & S_IXGRP) ? "x" : "-");
  // strcat(atttributes,(fileStat.st_mode & S_IROTH) ? "r" : "-");
  // strcat(atttributes,(fileStat.st_mode & S_IWOTH) ? "w" : "-");
  // strcat(atttributes,(fileStat.st_mode & S_IXOTH) ? "x" : "-");

}


int main(){

  int fd;
  char fileToDisplay[1024];

  char* FIFO = "/tmp/myfifo";

  mkfifo(FIFO,0666);

  char response[1024];

  while(1){

    fd = open(FIFO,O_RDONLY);

    read(fd,fileToDisplay,sizeof(fileToDisplay));
    close(fd);

    printf("The file is: '%s'\n",fileToDisplay);
    attributes(fileToDisplay);

    fd = open(FIFO,O_WRONLY);

    write(fd,fileAttributes,strlen(fileAttributes)+1);
    close(fd);
  }

}
