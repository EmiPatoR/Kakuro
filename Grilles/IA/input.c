#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"
#include "input.h"

STATUS_FICHIER LectureFichier(char* nomDuFichier, GrilleKakuro *grille)
{
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
    if((fichier = fopen(nomDuFichier,"r")) == NULL)
        return (erreur = ERREUR_F_INTROUVABLE);
    while(caractereActuel != EOF)
    {
        if(caractereActuel != 0)
            caracterePrecedent = caractereActuel;
        if(caractereSuivant == 0)
        {
            caractereActuel = fgetc(fichier);
            caractereSuivant = fgetc(fichier);
            positionCurseur+=2;
        }
        else
        {
            caractereActuel = caractereSuivant;
            caractereSuivant = fgetc(fichier);
            positionCurseur++;
        }

        // if(verifFormat(caracterePrecedent,caractereActuel,caractereSuivant) == 0)
        //   return (erreur = ERREUR_F_LECTURE);

        if(caractereActuel == '/')
        {
            //val_sup = isNumber(caracterePrecedent);
            //val_inf = isNumber(caractereSuivant);
            //printf("precedent : %c , suivant : %c,  VAL_SUP : %d VAL_INF : %d \n ",caracterePrecedent,caractereSuivant,val_sup,val_inf );
            nbCasesNoires++;
            if(!isNumber(caracterePrecedent))
                val_sup = 0;
            if(!isNumber(caractereSuivant))
                val_inf = 0;

            if(!isNumber(caractereSuivant))
            {
                cn.pos.X = pos.X;
                cn.pos.Y = pos.Y;
                if(pos.X > max.X)
                    max.X = pos.X;
                if(pos.Y > max.Y)
                    max.Y = pos.Y;
                cn.val_sup = val_sup;
                cn.val_inf = val_inf;
                ListeCn_push_back(&CasesNoires,cn);
            }

        }
        if(caractereActuel == '*')
        {
            nbCasesBlanches++;
            if(pos.X > max.X)
                max.X = pos.X;
            if(pos.Y > max.Y)
                max.Y = pos.Y;
            ListePositions_push_back(&positionCasesBlanches,pos);
        }
        if(caractereActuel == '\n')
        {
            pos.Y++;
            pos.X = 0;
        }
        if(isNumber(caractereActuel))
        {
            if(isNumber(caractereSuivant))
            {
                if(caracterePrecedent == '/')
                {
                    vInf[0] = (char)caractereActuel;
                    vInf[1] = (char)caractereSuivant;
                    val_inf = (vInf[0] - '0')*10 + (vInf[1] - '0');

                    cn.pos.X = pos.X - 1;
                    cn.pos.Y = pos.Y;
                    if(pos.X - 1 > max.X)
                        max.X = pos.X - 1;
                    if(pos.Y > max.Y)
                        max.Y = pos.Y;
                    cn.val_sup = val_sup;
                    cn.val_inf = val_inf;
                    ListeCn_push_back(&CasesNoires,cn);

                }
                else
                {
                    vSup[0] = (char)caractereActuel;
                    vSup[1] = (char)caractereSuivant;
                    val_sup = (vSup[0] - '0')*10 + (vSup[1] - '0');
                }

                caractereActuel = caractereSuivant;
                caractereSuivant = fgetc(fichier);
                positionCurseur++;
            }
            else if(caractereSuivant == '/')
                val_sup = isNumber(caractereActuel);
            else if(caracterePrecedent == '/')
            {
                val_inf = isNumber(caractereActuel);
                cn.pos.X = pos.X - 1;
                cn.pos.Y = pos.Y;
                if(pos.X > max.X)
                    max.X = pos.X;
                if(pos.Y > max.Y)
                    max.Y = pos.Y;
                cn.val_sup = val_sup;
                cn.val_inf = val_inf;
                ListeCn_push_back(&CasesNoires,cn);
            }

        }


        if((isNumber(caractereActuel) == 0) && caractereActuel != EOF && caractereActuel != '\n' && caractereActuel !=' ')
            pos.X++;

        // printf(" %li ", ftell(fichier));
    }


    //printf("cases blanches : %d\ncases noires : %d \nLignes : %d \n",nbCasesBlanches,nbCasesNoires,pos.Y);
    //printf("Nombre de cases noires : %d \n",sizeCn(CasesNoires));
    if(!init_Grille(grille,nbCasesBlanches,nbCasesNoires,positionCasesBlanches,CasesNoires,max)) return (erreur = ERREUR_F_LECTURE);




    fclose(fichier);
    return (erreur = OK);
}

int isNumber(int caractere)
{
    if(caractere > '0' && caractere <='9')
        return (caractere - '0');
    return 0;
}

int verifFormat(int c,int c2, int c3)
{

    return 0;
}
