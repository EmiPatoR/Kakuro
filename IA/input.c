#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"
#include "input.h"

STATUS_FICHIER LectureFichier(char* nomDuFichier, GrilleKakuro *grille)
{
	//initialisation des variables
    STATUS_FICHIER erreur;
    int nbCasesNoires = 0;
    int nbCasesBlanches = 0;
    int positionCurseur = 0;
    int caractereActuel = 0, caracterePrecedent = 0, caractereSuivant = 0;
    char vSup[2];
    char vInf[2];
    int val_sup = 0, val_inf = 0;
    position pos;
    pos.X = 0;
    pos.Y = 0;
    position max;
    max.X = 0;
    max.Y = 0;
    caseNoire cn;
    pileCn CasesNoires = NULL;
    pilePos positionCasesBlanches = NULL;

    FILE* fichier;
    if((fichier = fopen(nomDuFichier,"r")) == NULL)// si on arrive pas a ouvrir le fichier
        return (erreur = ERREUR_F_INTROUVABLE); // on retourne une erreur de type "Fichier introuvable"
    while(caractereActuel != EOF) // si l'ouverture du fichier a réussi, on lit le fichier caractere par caractere jusqu'a la fin du fichier (EOF)
    {
        if(caractereActuel != 0) // si on a deja lu des caracteres
            caracterePrecedent = caractereActuel; // le caractere precedent devient le caractere actuellement lu
        if(caractereSuivant == 0) // si c'est la premiere fois qu'on lit un caractere
        {
            caractereActuel = fgetc(fichier); // on lit un caractere et on le stocke dans caractereActuel
            caractereSuivant = fgetc(fichier); // on lit encore un caractere et on le stocke dans caractereSuivant
            positionCurseur+=2; // on incremente la position du curseur de lecture de 2
        }
        else // sinon
        {
            caractereActuel = caractereSuivant; // le caractere actuel devient le caractere suivant
            caractereSuivant = fgetc(fichier); // on lit un caractere de plus et on le stocke dans caractere suivant
            positionCurseur++; // on incremente la position du curseur de lecture
        }

        if(caractereActuel == '/') // si le caractere actuellement lu est '/'
        {
            nbCasesNoires++; // alors c'est une case noire , on increment le nombre de cases noires
            if(!isNumber(caracterePrecedent)) // si le caractere precedent '/' n'est pas un nombre
                val_sup = 0; // la case noire ne comporte pas de mot supérieur associé
            if(!isNumber(caractereSuivant)) // si le caractere suivant '/' n'est pas un nombre
                val_inf = 0; // la case noire ne comporte pas de mot inférieur associé

            if(!isNumber(caractereSuivant)) // si le caractere suivant n'est pas un nombre
            {
                cn.pos.X = pos.X; //la position X de la case noire correspond a pos.X
                cn.pos.Y = pos.Y; //la position Y de la case noire correspond a pos.Y
                if(pos.X > max.X) // si pos.X est supérieure aux plus grandes pos.X précedentes
                    max.X = pos.X; // on met a jour le pos.X le plus grand dans max.X
                if(pos.Y > max.Y) // si pos.Y est supérieure aux plus grandes pos.Y précedentes
                    max.Y = pos.Y;// on met a jour le pos.Y le plus grand dans max.Y
                cn.val_sup = val_sup; // on stocke dans la valeur du mot supérieur de la case noire , la valeur lue dans le fichier si elle existe , 0 sinon
                cn.val_inf = val_inf; // on stocke dans la valeur du mot inférieur de la case noire , la valeur lue dans le fichier si elle existe , 0 sinon
                ListeCn_push_back(&CasesNoires,cn); // on insere en fin de liste la nouvelle case noire ainsi définie.
            }

        }
        if(caractereActuel == '*') // si le caractere actuellement lu est '*'
        {
            nbCasesBlanches++; // alors c'est une case blanche, on incrémente le nombre de cases blanches
            if(pos.X > max.X) // si pos.X est supérieure aux plus grandes pos.X précedentes
                max.X = pos.X;// on met a jour le pos.X le plus grand dans max.X
            if(pos.Y > max.Y)// si pos.Y est supérieure aux plus grandes pos.Y précedentes
                max.Y = pos.Y;// on met a jour le pos.Y le plus grand dans max.Y
            ListePositions_push_back(&positionCasesBlanches,pos);// on insere en fin de liste la position de la nouvelle case blanche ainsi définie.
        }
        if(caractereActuel == '\n') // si le caractere actuellement lu est la fin d'une ligne ('\n')
        {
            pos.Y++; // on incremente pos.Y
            pos.X = 0; // et on recommence la lecture a partir du premier caractere de la ligne suivante, on remet donc pos.X à 0.
        }
        if(isNumber(caractereActuel)) // si le caractere actuellement lu est un nombre
        {
            if(isNumber(caractereSuivant)) // et que le caractere suivant est aussi un nombre
            {
                if(caracterePrecedent == '/') // on verifie alors si le caractere precedent est une case noire ('/')
                {
                    vInf[0] = (char)caractereActuel; //si oui , la case noire possede une valeur inférieure composée de deux chiffres vInf[0] 
                    vInf[1] = (char)caractereSuivant; // et vInf[1]
                    val_inf = (vInf[0] - '0')*10 + (vInf[1] - '0'); // on convertie ces nombres en entier 

                    cn.pos.X = pos.X - 1; // on stocke dans la position X de cette case noire la position pos.X - 1 car le nombre est composé de deux chiffres (et on incremente pos.X pour chaque caractere lu or ici , on doit considerer les deux caractere comme une seule entité
                    cn.pos.Y = pos.Y; // on stocke dans la position Y de cette case noire la valeur de pos.Y
                    if(pos.X - 1 > max.X)// si pos.X est supérieure aux plus grandes pos.X précedentes
                        max.X = pos.X - 1;// on met a jour le pos.X le plus grand dans max.X
                    if(pos.Y > max.Y)// si pos.Y est supérieure aux plus grandes pos.Y précedentes
                        max.Y = pos.Y;// on met a jour le pos.Y le plus grand dans max.Y
                    cn.val_sup = val_sup;// on stocke dans la valeur du mot supérieur de la case noire , la valeur lue dans le fichier si elle existe , 0 sinon
                    cn.val_inf = val_inf;// on stocke dans la valeur du mot inférieur de la case noire , la valeur lue dans le fichier si elle existe , 0 sinon
                    ListeCn_push_back(&CasesNoires,cn);// on stocke dans la valeur du mot inférieur de la case noire , la valeur lue dans le fichier si elle existe , 0 sinon

                }
                else // sinon c'est que ces deux chiffres forment une valeur d'un mot supérieur
                {
                    vSup[0] = (char)caractereActuel; // on stocke le premier chiffre lu dans vSup[0]
                    vSup[1] = (char)caractereSuivant; // on stocke le second dans vSup[1]
                    val_sup = (vSup[0] - '0')*10 + (vSup[1] - '0'); // on convertie les deux char en int correspondant au nombre qu'ils forment
                }

                caractereActuel = caractereSuivant; // le caractere actuel devient le caractere suivant
                caractereSuivant = fgetc(fichier); // on lit un nouveau caractere qu'on stocke dans caractereSuivant
                positionCurseur++;// on increment la position du curseur de lecture
            }
            else if(caractereSuivant == '/') // sinon si le caractere suivant est une case noire ('/')
                val_sup = isNumber(caractereActuel); // c'est que la valeur du mot supérieur de cette case est contenu sur un seul chiffre et il est dans caractereActuel que l'on convertie a l'aide de la fonction isNumber.
            else if(caracterePrecedent == '/') // sinon si le caractere precedent est une case noire ('/')
            {
                val_inf = isNumber(caractereActuel);// c'est que la valeur du mot supérieur de cette case est contenu sur un seul chiffre et il est dans caractereActuel que l'on convertie a l'aide de la fonction isNumber.
                // on remplit les informations de la case noire.
                cn.pos.X = pos.X - 1;
                cn.pos.Y = pos.Y;
                if(pos.X > max.X)
                    max.X = pos.X;
                if(pos.Y > max.Y)
                    max.Y = pos.Y;
                cn.val_sup = val_sup;
                cn.val_inf = val_inf;
                ListeCn_push_back(&CasesNoires,cn); //on ajoute la case noire en fin de liste de la liste chainée des cases noires.
            }

        }


        if((isNumber(caractereActuel) == 0) && caractereActuel != EOF && caractereActuel != '\n' && caractereActuel !=' ') // si le caractere actuellement lu est un caractere reconnu
            pos.X++; // on increment pos.X

    }

    if(!init_Grille(grille,nbCasesBlanches,nbCasesNoires,positionCasesBlanches,CasesNoires,max)) return (erreur = ERREUR_F_LECTURE);// on initialise la grille avec les valeurs calculés precedemment , en cas d'erreur on retourne une erreur de lecture, comme quoi le fichier est incorrect.

    fclose(fichier); // on ferme le fichier
    return (erreur = OK); // on retourne OK comme quoi tout s'est bien déroulé
}

//Fonction qui renvoie si un caractere lu est un chiffre supérieur strictement a 0 et inférieur ou egal a 9, return le nombre en question si oui , retourne 0 sinon.
int isNumber(int caractere)
{
    if(caractere > '0' && caractere <='9')
       return (caractere - '0');
    return 0;
}
