#ifndef INPUT_H
#define INPUT_H

#include "structures.h"

typedef enum Fichiers {ERREUR_F_INTROUVABLE, ERREUR_F_LECTURE, OK} STATUS_FICHIER;

STATUS_FICHIER LectureFichier(char* nomDuFichier, GrilleKakuro *grille);
int isNumber(int caractere);
int verifFormat(int c,int c2, int c3);

#endif
