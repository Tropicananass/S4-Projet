#include "testJeu.h"

#include "globals.h"

int testRecursif(int *tab, int i, int J);

int testCaseDessus(int *tab, int i, int J){
  if(tab[i+NBSIDE]==J && testRecursif(tab, i+NBSIDE,J)){//la seconde condition est testée uniquement si la première est vrai donc le testRecursifJ1 sera appelé ssi tab[i+NBSIDE]==J1
    return 1;
  }
  return 0;
}

int testCaseDessous(int *tab, int i, int J){
  if(i/NBSIDE>0 && tab[i-NBSIDE]==J && testRecursif(tab, i-NBSIDE,J)){
    return 1;
  }
  return 0;
}

int testCaseG(int *tab, int i, int J){
  if(i%NBSIDE!=0){
    if(tab[i-1]==J && testRecursif(tab, i-1, J)){
      return 1;
    }
    if(tab[i+NBSIDE-1]==J && testRecursif(tab, i+NBSIDE-1, J)){
      return 1;
    }
  }
  return 0;
}

int testCaseD(int *tab, int i, int J){
  if(i%NBSIDE!=NBSIDE-1){
    if(i/NBSIDE>0 && tab[i-NBSIDE+1]==J && testRecursif(tab, i-NBSIDE+1, J)){
      return 1;
    }
    if(tab[i+1]==J && testRecursif(tab, i+1, J)){
      return 1;
    }
  }
  return 0;
}

int testRecursif(int *tab, int i, int J){// ne peut être appelée qu'avec un indice ou il y a un pion du joueur 1 (J1)
  tab[i]=tab[i]+2;
  if(J==J1){
    if(i/NBSIDE==NBSIDE-1){
      return 1;
    }
    return testCaseDessus(tab,i,J) || testCaseG(tab,i,J) || testCaseD(tab,i,J) || testCaseDessous(tab,i,J);
  }
  else{
    if(i%NBSIDE==NBSIDE-1){
      return 1;
    }
    return testCaseD(tab,i,J) || testCaseDessus(tab,i,J)  ||  testCaseDessous(tab,i,J) || testCaseG(tab,i,J);
  }
}


int testGagne(int *tab, int J){
  int i=0, a=0;
  if(J==J1){
    while(!a && i<NBSIDE){
      if(tab[i]==J1){
	a=testRecursif(tab, i, J);
	//i++;//si une case à été testé la case adjacente aura forcément été testée donc on peut décaler de 2 cases
      }
      i++;
    }
  }
  else{
    while(!a && i<NBSIDE*NBSIDE){
      if(tab[i]==J2){
	a=testRecursif(tab, i, J2);
	//i=i+NBSIDE;//si une case à été testé la case adjacente aura forcément été testée donc on peut décaler de 2 cases
      }
      i=i+NBSIDE;
    }
  }
  for(i=0; i<NBSIDE*NBSIDE; i++){
    if(tab[i]>J2){
      tab[i]=tab[i]-2;
    }
  }
  return a;
}

	
	
