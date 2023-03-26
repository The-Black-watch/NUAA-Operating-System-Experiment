#include<stdio.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>

void mysys(char *str){
        int pid;
        pid=fork();
        if(pid==0)
                execl("/bin/sh","sh","-c",str,NULL);
        wait(NULL);
}

int main(){
        printf("---------------------------------\n");
        mysys("echo HELLO WORLD");
        printf("---------------------------------\n");
        mysys("ls /");
        printf("---------------------------------\n");
        return 0;
}

