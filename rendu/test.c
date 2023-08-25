#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char** argv)
{
    if(argc !=2)
    {
        write(1,"usage: ./exec nom_util\n",23);
        return 1;
    }
    
    int fd1;
    fd1=open("toto",O_WRONLY | O_CREAT | O_TRUNC);
    write(fd1,"on est la aa\nbobobobobter\nty\nla iop le\n",39);
    close(fd1);
    fd1=open("toto",O_RDONLY); 
    dup2(fd1,0);
    close(fd1);
    char str[4]="^la";
    execlp("grep","grep",str,(char*)NULL);
    printf("no bro \n");
    return 0;
}
