#include "sauvegarde.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "globals.h"

int listeSauvegarde(char*** nomSave){
	DIR* rep = opendir("save/");
	if(rep==NULL){
		return 0;
	}
	struct dirent* d;
	int i=0;
	int nbSave=0;
	int ext;
	while((d=readdir(rep))!=NULL)
	{
		ext=strlen(d->d_name);
		if (ext>3 && !strcmp(&d->d_name[ext-4], ".sav") && ext<13){
			nbSave++;
		}
	}
	rewinddir(rep);
	char ** tmp;
	tmp=malloc(sizeof(char*)*(nbSave));
	int j;
	while((d=readdir(rep))!=NULL)
	{
		ext=strlen(d->d_name);
		if (ext>3 && !strcmp(&d->d_name[ext-4], ".sav") && ext<13){
			tmp[i]=malloc(sizeof(char)*(ext-2));
			for(j=0;j<ext-4;j++){
				tmp[i][j]= d->d_name[j];
			}
			
			tmp[i][ext-4]=0;
			i++;
		}
	}
	closedir(rep);
	*nomSave = tmp;
	return nbSave;
	
}

void free_liste (char** liste, int n)
{
	for (int i = 0; i < n; ++i)
		free (liste[i]);
	free (liste);
}

// Renvoi 0 si la sauvegarde a eu lieu et 1 si il y a eu problème

int sauvegarde (char * nom, int * tab, int IA)
{
	DIR* rep = opendir("save/");

	if(rep==NULL){
		if(mkdir("save/", S_IRWXU)){
			closedir(rep);
			return ERROR_CREATE_DIR;
		}
	}
	closedir(rep);
	
	FILE* f_sauvegarde;
	char desif[strlen(nom)+12];
	strcpy(desif,"save/");
	strcat(desif,nom);
	strcat(desif,".sav");
	f_sauvegarde=fopen(desif,"w+"); 
	fprintf(f_sauvegarde,"\n%d\n",IA);
	fprintf(f_sauvegarde,"%d\n",param->size);
	int i;
	for (i=0; i<param->size*param->size; ++i)
		fprintf(f_sauvegarde,"%d\n", tab[i]);
	fclose(f_sauvegarde);
	return 0;
}


int chargement(char *nom , int **tab, int *IA)
{
	FILE* f_chargement;
	char desif[strlen(nom)+12];
	int *t;
	int a=0, i, taille;
	strcpy(desif,"save/");
	strcat(desif,nom);
	strcat(desif,".sav");
	f_chargement=fopen (desif, "r");
	if (f_chargement == NULL)
		return ERROR_OPENFILE;
	a=fscanf(f_chargement, "%d", IA);

	if(!a || *IA>2 || *IA<0){
		return ERROR_IA;
	}
	fscanf(f_chargement, "%d", &taille);
	if(taille<1 || taille > MAXSIZE){
		return ERROR_SIZE;
	}

	t=malloc(sizeof(int)*taille*taille);
	a=1, i = -1;

	while(a>0){
		i++;
		a=fscanf(f_chargement, "%d" , &t[i]);
	}
	fclose(f_chargement);

	if(i!=taille*taille){
		return ERROR_NB_VALUE;
	}
	param->size = taille;
	*tab = t;
	return 0;
}

void supprimer(char * nom){
	char desif[strlen(nom)+12];
	strcpy(desif,"save/");
	strcat(desif,nom);
	strcat(desif,".sav");
	unlink(desif);
}
