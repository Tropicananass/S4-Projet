#define ERROR_IA 1
#define ERROR_SIZE 2
#define ERROR_NB_VALUE 3
#define ERROR_OPENFILE 4
#define ERROR_CREATE_DIR 11

int listeSauvegarde(char*** nomSave );
int sauvegarde (char * nom , int * tab, int IA);
int chargement (char *nom , int **tab, int *IA);
