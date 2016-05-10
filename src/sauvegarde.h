#ifndef _SAVEGARDE_H_
#define _SAVEGARDE_H_

#define ERROR_IA 1
#define ERROR_SIZE 2
#define ERROR_NB_VALUE 3
#define ERROR_OPENFILE 4
#define ERROR_CREATE_DIR 11

int listeSauvegarde(char*** nomSave );
void free_liste (char** liste, int n);
int sauvegarde (char * nom , int * tab, int IA);
int chargement (char *nom , int **tab, int *IA);
void supprimer(char * nom);

#endif /* _SAVEGARDE_H_ */
