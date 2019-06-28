#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/fcntl.h>
#include "md5.h"
#include <sys/wait.h>



pid_t pid;
char buff[20];
bool Checksize = 0;
int chlidpid = 0;
 std::string msgrevered;
char* ans;

void catchSigT(int sig_num)
{
if(strlen(msgrevered.c_str()) == 32){
    Checksize = 1;
}
};


int main(int argc,char *argv[])
{

int mypipefd[2];
int ret = pipe(mypipefd);
int i =0;
char str1[20]="";

    signal(SIGTERM, catchSigT);

    if(ret == -1){
        perror("pipes fail");
        exit(1);
    }

pid = fork();

if (pid == 0){
        /* Chlid Process */

    read(mypipefd[0], buff, 20);
    sleep(1);    

    // using md5
    msgrevered = md5(buff);
    char answer[msgrevered.length() + 1];
    strcpy(answer, msgrevered.c_str());
    write(mypipefd[1],answer,strlen(answer) + 1);
    kill(pid,SIGTERM);
    sleep(3);


}else{

        /* Parent Process */

    for(i=1;i<argc;i++){  

    char temp[20]="";
    strncpy(temp, argv[i], 20);
    strcat(temp," ");
    strcat(str1,temp);
    }
    if(strlen(str1) >20){
        perror("size fail");
         kill(0,SIGKILL);
        exit(1);
    }else {

    write(mypipefd[1], str1,20);
    sleep(5);
}
    read(mypipefd[0], buff, 32);
    sleep(5);
    if(Checksize == 1){
        printf("praent: %s\n ",buff);
        sleep(1);

        // kill(0,SIGKILL);
    }

}

