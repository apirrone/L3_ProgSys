#include "../include/Commandes_Internes.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

const int SIZE_LIST_COMMANDES_INTERNES = 9;
const char * const LIST_COMMANDES_INTERNES[] = { "echo",
												"date",
											 	"cd",
											 	"pwd",
											 	"history",
											 	"hostname",
											 	"kill",
											 	"exit",
											 	"remote" };

char** listMachines;
int nbMachines;
int *pidsProcessus;

bool isInterne(char* cmd){//renvoie true si la commande est interne, faux sinon
	bool ret = false;
	for(int i = 0 ; i < SIZE_LIST_COMMANDES_INTERNES ; i++)
		if(strcmp(cmd, LIST_COMMANDES_INTERNES[i]) == 0){
			ret = true; 
			break;
		}
	return ret;
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
		chdir(arguments[1]);
		char cwd[1024];
		getcwd(cwd, sizeof(cwd));
		printf("Current root : %s\n", cwd);
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
	  wait(NULL);
	  printf("notre PID est %d\n", getpid());
	  kill(getpid(), SIGKILL);

	}
	else if(strcmp(arguments[0], "remote") == 0){//REMOTE SHELL
		handleRemoteShell(arguments);
	}
}


void handleRemoteShell(char** arguments){
	char* argument1 = arguments[1];

	if(!strcmp(argument1, "add")){//remote add liste-de-machines
		if(arguments[2] == NULL){
			printf("USAGE : remote add <machine1> <machine2> ...\n");
		}	
		else{
			makeListMachines(arguments);	
			pidsProcessus = malloc(sizeof(int)*nbMachines);	
			
			for(int i = 0 ; i < nbMachines ; i++){
				createMiniShell(i);
			}
		}
	}
	else if(!strcmp(argument1, "remove")){//remote remove
		if(listMachines == NULL){
			printf("Aucune machine\n");
		}
		else{

		}
	}
	else if(!strcmp(argument1, "list")){//remote list
		if(listMachines == NULL){
			printf("Aucune machine\n");
		}
		else{
			for(int i = 0; i < nbMachines ; i++){
				printf("%s\n", listMachines[i]);
			}
		}
	}
	else if(!strcmp(argument1, "all")){//remote all commande-simple
		if(arguments == NULL){
			printf("USAGE : remote all <commande>\n");
		}
		else if(listMachines[0] == NULL){
			printf("Aucune machine\n");
		}
		else{
			for(int i = 0 ; i < nbMachines ; i++){
				executeCommandOnMachine(i, arguments[2]);
			}
		}
	}
	else{//remote nom-machine commande-simple
		char* nom_machine = arguments[1];
		if(machineExistante(nom_machine)){

		}
		else{
			printf("Machine non existante sur le réseau\n");
		}
	}
}

void makeListMachines(char** arguments){
	nbMachines = LongueurListe(arguments)-2; //-2 car nom commande et add
	printf("nbMachines : %d\n", nbMachines);
	listMachines = malloc(sizeof(char*)*nbMachines);
	for(int i = 0 ; i < nbMachines ; i++){
		listMachines[i] = malloc(sizeof(char)*strlen(arguments[i+2]));
	}
	for(int i = 0 ; i < nbMachines ; i++){
		strcpy(listMachines[i], arguments[i+2]);
	}
}


//A FAIRE
void createMiniShell(int idMachine){
	
}


//A FAIRE
bool machineExistante(char* nomMachine){
	return false;
}

//A FAIRE
void executeCommandOnMachine(int idMachine, char* commande){

}
