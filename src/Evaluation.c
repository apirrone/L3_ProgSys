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



	int pipefd[2];

int cmd(char **arguments){

	if(isInterne(arguments[0])){//COMMANDE INTERNE
		evaluer_expr_interne(arguments);
	}
	else{//A FAIRE
	  if(fork() == 0){//FILS
	    execvp(arguments[0], arguments);
	    perror("exec");
	    return 0;
	  }
	  else{
	    wait(NULL);
	  }
	}
	return 1;
}


int evaluer_expr(Expression *e){

	switch(e->type){
		
		case SIMPLE:
			if(cmd(e->arguments) == 0)
				return 0;
			break;


		case PIPE:
			pipe(pipefd);
			int stdout = dup(1);
			if(fork()==0){//FILS
				close(pipefd[0]);
				dup2(pipefd[1], 1);//Redirection de la sortie standard dans le pipe
				close(pipefd[1]);
				evaluer_expr(e->gauche);
				// perror("pipe fils\n");

			}
			
			else{//PERE
				close(pipefd[1]);
				dup2(pipefd[0], 0);//redirection du pipe dans l'entrée standard
				close(pipefd[0]);
				evaluer_expr(e->droite);
				// perror("pipe pere\n");				
				wait(NULL);
			}
			dup2(stdout, 1);


			break;
			
		case SEQUENCE:
			evaluer_expr(e->gauche);
			evaluer_expr(e->droite);
			break;
			
		case SEQUENCE_OU:
			if(evaluer_expr(e->gauche) != 1)
				evaluer_expr(e->droite);
			break;
			
		case SEQUENCE_ET:
			if(evaluer_expr(e->gauche) == 1)
				evaluer_expr(e->droite);
			break;
			
		case SOUS_SHELL:
			pipe(pipefd);
			if(fork() == 0){//FILS
				int pipeFils [2];
				pipe(pipeFils);
				pid_t pid = fork();
				if(pid == 0){//PETIT FILS
					// en ecriture avec le PERE
					close(pipefd[0]);
					dup2(pipefd[1], 1);
					close(pipefd[1]);
					// en lecture avec le FILS
					close(pipeFils[1]);
					dup2(pipeFils[0], 0);
					close(pipeFils[0]);
					execlp("Shell", "Shell", NULL);
					perror("exec");
				}
				else{//FILS
					// se tait face au PERE
					close(pipefd[0]);
					close(pipefd[1]);
					// en ecriture avec le PETIT FILS
					close(pipeFils[0]);
					dup2(pipeFils[1], 1);
					close(pipeFils[1]);
					// lance la commande, pour l'instant on réalise les tests avec "ls -l"
					printf("ls -l ; kill %d\n", pid);
					wait(NULL);
				}
			}
			else{//PERE
				// en ecriture avec le PETIT FILS
				close(pipefd[1]);
				dup2(pipefd[0], 0);
				close(pipefd[0]);
				char buffer [1024];
				int nbRead = 0;
				while ((nbRead = read(0, buffer, 1024))> 0)
					write(1, buffer, nbRead);
				wait(NULL);
			}
			break;
			
		case REDIRECTION_O:
			if(fork() == 0){
				int fd = open(e->arguments[0], O_CREAT | O_WRONLY | O_TRUNC, 0644);
				if(fd == -1)
					perror("open");
				dup2(fd, 1); 
				close(fd);
				evaluer_expr(e->gauche);
				exit(1);
			}
			else
				wait(NULL);
			break;
			
		case REDIRECTION_A:
			if(fork() == 0){
				int fd = open(e->arguments[0], O_CREAT | O_WRONLY | O_APPEND, 0644);
				if(fd == -1)
					perror("open");
				dup2(fd, 1); 
				close(fd);
				evaluer_expr(e->gauche);
				exit(1);
			}
			else
				wait(NULL);
			break;
			
		case REDIRECTION_I:
			if(fork() == 0){
				int fd = open(e->arguments[0], O_RDONLY);
				if(fd == -1)
					perror("open");
				dup2(fd, 0); 
				close(fd);
				evaluer_expr(e->gauche);
				exit(1);
			}
			else
				wait(NULL);
			break;
			
		case BG:
		  if(fork() == 0 ){
		    cmd(e->gauche->arguments);
		    exit(0);
		  }
		  break;
	}

	int status;
	int pid;

	while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
		printf("[%d] status = %d\n", pid, status);

	return 1;
}
