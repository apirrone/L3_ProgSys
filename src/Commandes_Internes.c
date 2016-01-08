#include "../include/Commandes_Internes.h"
#include <readline/history.h>





void history(){
  HIST_ENTRY **hist_list;
  hist_list = history_list ();
  for (int i = 0; hist_list[i]; i++)
    printf ("test :%d: %s\n", i + history_base, hist_list[i]->line);
}
