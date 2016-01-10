#ifndef COMINTERN_H
#define COMINTERN_H
#include "Shell.h"
#include <stdbool.h>



bool isInterne(char* commande);
int evaluer_expr_interne(char** arguments);
void handleRemoteShell(char** arguments);

void createMiniShell(int idMachine);
void makeListMachines(char** arguments);
bool machineExistante(char* nomMachine);
void executeCommandOnMachine(int idMachine, char* commande);
#endif
