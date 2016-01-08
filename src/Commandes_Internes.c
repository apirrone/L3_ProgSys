#include "../include/Commandes_Internes.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

const int SIZE_LIST_COMMANDES_INTERNES = 8;
const char * const LIST_COMMANDES_INTERNES[] = { "echo",
												"date",
											 	"cd",
											 	"pwd",
											 	"history",
											 	"hostname",
											 	"kill",
											 	"exit" };




bool isInterne(char* cmd){//renvoie true si la commande est interne, faux sinon
	bool ret = false;
	for(int i = 0 ; i < SIZE_LIST_COMMANDES_INTERNES ; i++)
		if(strcmp(cmd, LIST_COMMANDES_INTERNES[i])){
			ret = true; 
			break;
		}
}



int evaluer_expr_interne(char** arguments){

	if(strcmp(arguments[0], "history") == 0){//HISTORY

		HIST_ENTRY **hist_list;

		hist_list = history_list ();
	  	if (hist_list)
	    	for (int i = 0; hist_list[i]; i++)
	    		printf ("%d: %s\n", i + history_base, hist_list[i]->line);

	}
	else if(strcmp(arguments[0], "echo") == 0){//ECHO

		int longueurListe  =LongueurListe(arguments);
		for(int i = 1 ; i < longueurListe ; i++)
			if(i == (longueurListe -1))
				printf("%s\n", arguments[i]);
			else
				printf("%s ", arguments[i]);


	}	
	else if(strcmp(arguments[0], "date") == 0){//DATE
		time_t	currentTime = time(NULL);
		char* sCurrentTime = ctime(&currentTime);
		printf("%s\n", sCurrentTime);

	}	
	else if(strcmp(arguments[0], "cd") == 0){//CD


	}	
	else if(strcmp(arguments[0], "pwd") == 0){//PWD
		char cwd[1024];
		getcwd(cwd, sizeof(cwd));
		printf("%s\n", cwd);

	}	
	else if(strcmp(arguments[0], "hostname") == 0){//HOSTNAME
		char hostname[1024];
		gethostname(hostname, 1024);
		printf("%s\n", hostname) ;

	}	
	else if(strcmp(arguments[0], "kill") == 0){//KILL
		if(!kill(atoi(arguments[1]), SIGKILL))
			printf("PID %d terminé\n", atoi(arguments[1]));
		else
			printf("PID %d non existant\n", atoi(arguments[1]));


	}	
	else if(strcmp(arguments[0], "exit") == 0){//EXIT


	}
}