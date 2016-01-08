#include "../include/Shell.h"
#include "../include/Evaluation.h"
#include "../include/Commandes_Internes.h"
#include <stdio.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>	
#include <unistd.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>

void verifier(int cond, char *s){
  if (!cond){
      perror(s);
      exit(1);
    }
}


void cmd(char **arguments){
	if(strcmp(arguments[0], "history") == 0){
		HIST_ENTRY **hist_list;
		history();
	}

		/*	hist_list = history_list ();
      	if (hist_list)
        	for (int i = 0; hist_list[i]; i++)
        		printf ("%d: %s\n", i + history_base, hist_list[i]->line);
			}*/
	else{
		execvp(arguments[0], arguments);
	}
	perror("exec");
	exit(1);
}


int evaluer_expr(Expression *e){

	switch(e->type){
		
		case SIMPLE:
			if(fork() == 0)//FILS
				cmd(e->arguments);
			else
				wait(NULL);
			break;


		case REDIRECTION_O:
			if(fork() == 0){
				int fd = open(e->arguments[0], O_CREAT | O_WRONLY, 0644);
				dup2(fd, 1); 
				close(fd);
				cmd(e->gauche->arguments);
			}
			else
				wait(NULL);
			break;

		case BG:
			if(fork == 0 )
				cmd(e->gauche->arguments);
			else
				wait(NULL);
			break;
	}

	int status;
	int pid;

	while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
		printf("[%d] status = %d\n", pid, status);

  return 1;
}
