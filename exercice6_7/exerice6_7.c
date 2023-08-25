#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>


int main(int argc, char** argv)
{
   if (argc <= 2) 
    {
        printf("wrong arguments \n");
        return 1;
    }
    char **arg;
    int i, j, n, err=0;
    int status;
    pid_t pid;
    
    n= atoi(argv[1]);
    
    char c[]="NULL";
    char* err_cas=c;
    arg= (char **) malloc((argc)*sizeof(char *));

    if (arg == NULL) 
    {
        printf("allocation error \n");
        return 1;
    }
 
    for(i=0;i<(argc-2);i++)
    {
        arg[i]=argv[i+2];
    }
    
    
    for(i=0;i<n;i++)
    {
        pid=fork();
        switch(pid)
        {
            case -1 : 
                {
                perror("fork()");
                exit(1); 
                }
            case 0 :  // fils
                {
                    sprintf(arg[argc-2],"%d",i); 
                    arg[argc-1]=err_cas;
                    for(j=2;j<argc;j++) printf("%s\n",arg[i]);
                    execvp(arg[0],arg);
                    exit(0);
                }
        }
    }
    
    for(j=0;j<n;j++)
    {
        // affichage du code de retour des commandes
        if((pid=wait(&status)) == -1) err=1;
        printf("commande %d return %d \n",j, WEXITSTATUS(status) );
    }
    
    if (err != 1)
        return 0;
    else return 1;

}
