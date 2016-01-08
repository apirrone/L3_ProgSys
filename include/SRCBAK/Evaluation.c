#include "../include/Shell.h"
#include "../include/Evaluation.h"
#include <stdio.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>	
#include <unistd.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>




void cmd(char **arguments){

	if(isInterne(arguments[0])){//COMMANDE INTERNE
		evaluer_expr_interne(arguments);
	}
	else{//A FAIRE

	}
	// else{
	// 	execvp(arguments[0], arguments);
	// 	perror("exec");	
	// }
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


		case PIPE:

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