#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

void firstprocessgrep(char * str)
{
    char string[255];
    pid_t pid;
    int status;
    int file_in;
    int file_out;

    


    pid=fork();
    switch(pid)
    { 
        case -1 :
            perror("fork()");
            return;
        case 0 :

            printf("process [son] function \n");    
            file_in=open("toto",O_RDONLY);
            file_out=open("/dev/null",O_RDONLY | O_WRONLY);
            dup2(file_in,0);
            dup2(file_out,1);
            close(file_in);
            close(file_out);
   
            strcpy(string,str);
            strcat(string, "^");

            execlp("grep","grep", string, NULL); // if avec wite pour afficher string = echo "$1 .."        
            exit(0); // non nécessaire, assurer que le processus se termine  
        default :
	    
            printf("process [father] function \n");
    	    pid=fork();
            switch(pid)
	        {
	             case -1 :
                      perror("fork()");
                      return;
           	     case 0 :
		            file_out=open("toto",O_RDONLY | O_WRONLY | O_TRUNC | O_CREAT);
           	        dup2(file_out, 1);
            	    close(1);
           	        execlp("ps","ps",NULL);
           	        exit(0);
		         default : 
		             break;
   	        }
    }
    close(1);
    close(0);
}

int main(int argc, char ** argv)
{
    pid_t pid;
    int status;
    pid=fork();
    if(argc =! 2)
    {
        printf("wrong arguments \n");
        return 1;
    }
    switch(pid)
    {
        case -1 :
            perror("fork()");
            return 0;
        case 0 :
            printf("process [son] main \n");
            firstprocessgrep(argv[1]);
            exit(0);
        default :
            printf("process [father] main \n");
            if((pid=wait(&status)) == -1) 
                perror("wait(&status)");
            if(WIFEXITED(status))
                printf("command execute success \n");
            exit(0);
    }   
}
