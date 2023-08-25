#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

void secondprocessps(void)
{
    int file_out;
    if ((file_out=open("toto",O_WRONLY | O_TRUNC | O_CREAT, 0666)) == -1)
    {
        perror(" open");
        exit(1);
    }
    if ((dup2(file_out, 1)) ==-1) perror("dup2");
    if (close(file_out)==-1) perror("close(1)");
    execlp("ps","ps","eaux",NULL);
    perror("execlp");
    exit(1);
}


void firstprocessgrep(char * str)
{
    char string[256];
    int file_in, file_out;
    int i,j;
    file_in=open("toto",O_RDONLY);
    if (file_in == -1) 
    {
        perror("open1");
        exit(1);
    }
    file_out=open("/dev/null", O_WRONLY);
    if (file_out == -1) 
    {
        perror("open2");
        exit(1);
    }
    
    if ((dup2(file_in,0)) == -1) perror("dup2");
    if ((dup2(file_out,1)) == -1) perror("dup2");
    if (close(file_in)== -1) perror("close(file_in)");
    if (close(file_out)==-1) perror("close(file_out)");
    string[0]='^';
    for(i=1,j=0;str[j] != '\0';i++,j++) string[i]=str[j];
    printf(" chaine a afficher => %s \n\n",string);
    execlp("grep","grep",string, NULL); 
    perror("execlp");
    exit(1);
}

int main(int argc, char ** argv)
{
    pid_t pid_ps, pid_grep;
    int status, status2;
    char msg_final[256];
    if(argc != 2)
    {
        printf("wrong arguments \n");
        return 1;
    }
    pid_ps=fork();
    switch(pid_ps)
    {
        case -1 : 
            perror("fork()");
            exit(1);
        case 0 :
            secondprocessps();
            exit(1);
        default : 
            if((pid_ps=waitpid(pid_ps,&status,0)) == -1) 
                perror("wait(&status)");
            if(WIFEXITED(status) && (WEXITSTATUS(status) ==0))
            {
                pid_grep=fork();
                switch(pid_grep)
                {
                    case -1 :
                        perror("fork()");
                        exit(1);
                    case 0 :
                        firstprocessgrep(argv[1]);
                        exit(1);
                    default :
                        if((pid_grep=waitpid(pid_grep,&status2,0)) == -1) 
                            perror("wait(&status2)");
                        if(WIFEXITED(status2)&&(WEXITSTATUS(status2)==0))
                        {
                            strcpy(msg_final, " est bien connecte \n");
                            write(1,argv[1],strlen(argv[1]));
                            write(1,msg_final,strlen(msg_final));
                        }
   
                }
            }
            exit(0);
    }
    return 0;  
}
