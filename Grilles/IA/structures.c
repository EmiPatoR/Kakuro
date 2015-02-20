#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"

void ListeCn_push_front(pileCn* Pile, caseNoire val)
{
    elementCn* nouveau = (elementCn*) malloc(sizeof(elementCn));
    nouveau->val = val;
    nouveau->precedent = NULL;
    nouveau->suivant = *Pile;
    if(*Pile == NULL)
        nouveau->dernier = nouveau;
    *Pile = nouveau;
}

void ListeCn_push_back(pileCn* Pile, caseNoire val)
{
    int i = 0;
    iteratorCn it = NULL;
    elementCn* nouveau = (elementCn*) malloc(sizeof(elementCn));
    nouveau->val = val;
    nouveau->suivant = NULL;
    if(*Pile == NULL)
    {
        nouveau->dernier = nouveau;
        nouveau->precedent = NULL;
        *Pile = nouveau;
    }
    else
    {
        for(it=(*Pile); it->suivant!=NULL; it=it->suivant)
            i++;
        //Parcours
        nouveau->precedent = it;
        it->suivant = nouveau;
    }

}

caseNoire DepilerCn(pileCn *Pile)
{
    elementCn* save = NULL;
    caseNoire retour;
    if(*Pile != NULL)
    {
        retour = (*Pile)->val;
        save = (*Pile)->suivant;
        if(save != NULL)
            save->precedent = NULL;
        free(*Pile);
    }
    (*Pile) = save;
    return retour;
}

int sizeCn(pileCn Pile)
{
    int compteur = 0;
    iteratorCn it = NULL;
    for(it=Pile; it!=NULL; it=it->suivant)
        compteur++;
    return compteur;
}

void ListeCb_push_front(pileCb* Pile, caseBlanche* val)
{
    elementCb* nouveau = (elementCb*) malloc(sizeof(elementCb));
    nouveau->val = val;
    nouveau->precedent = NULL;
    nouveau->suivant = *Pile;
    if(*Pile == NULL)
        nouveau->dernier = nouveau;
    *Pile = nouveau;
}

void ListeCb_push_back(pileCb* Pile, caseBlanche* val)
{
    int i = 0;
    iteratorCb it = NULL;
    elementCb* nouveau = (elementCb*) malloc(sizeof(elementCb));
    nouveau->val = val;
    nouveau->suivant = NULL;
    if(*Pile == NULL)
    {
        nouveau->dernier = nouveau;
        nouveau->precedent = NULL;
        *Pile = nouveau;
    }
    else
    {
        for(it=(*Pile); it->suivant!=NULL; it=it->suivant)
            i++;
        //Parcours
        nouveau->precedent = it;
        it->suivant = nouveau;
    }

}

caseBlanche* DepilerCb(pileCb *Pile)
{
    elementCb* save = NULL;
    caseBlanche* retour = NULL;
    if(*Pile != NULL)
    {
        retour = (*Pile)->val;
        save = (*Pile)->suivant;
        if(save != NULL)
            save->precedent = NULL;
        free(*Pile);
    }
    (*Pile) = save;
    return retour;
}

int sizeCb(pileCb Pile)
{
    int compteur = 0;
    iteratorCb it = NULL;
    for(it=Pile; it!=NULL; it=it->suivant)
        compteur++;
    return compteur;
}


void ListePositions_push_front(pilePos* Pile, position val)
{
    elementPos* nouveau = (elementPos*) malloc(sizeof(elementPos));
    nouveau->val = val;
    nouveau->precedent = NULL;
    nouveau->suivant = *Pile;
    if(*Pile == NULL)
        nouveau->dernier = nouveau;
    *Pile = nouveau;
}

void ListePositions_push_back(pilePos* Pile, position val)
{
    int i = 0;
    iteratorPos it = NULL;
    elementPos* nouveau = (elementPos*) malloc(sizeof(elementPos));
    nouveau->val = val;
    nouveau->suivant = NULL;
    if(*Pile == NULL)
    {
        nouveau->dernier = nouveau;
        nouveau->precedent = NULL;
        *Pile = nouveau;
    }
    else
    {
        for(it=(*Pile); it->suivant!=NULL; it=it->suivant)
            i++;
        //Parcours
        nouveau->precedent = it;
        it->suivant = nouveau;
    }
}

position DepilerPos(pilePos *Pile)
{
    elementPos* save = NULL;
    position retour;
    if(*Pile != NULL)
    {
        retour = (*Pile)->val;
        save = (*Pile)->suivant;
        if(save != NULL)
            save->precedent = NULL;
        free(*Pile);
    }
    (*Pile) = save;
    return retour;
}

int sizePos(pilePos Pile)
{
    int compteur = 0;
    iteratorPos it = NULL;
    for(it=Pile; it!=NULL; it=it->suivant)
        compteur++;
    return compteur;
}


void Liste_push_front(pile* Pile, int val)
{
    element* nouveau = (element*) malloc(sizeof(element));
    nouveau->val = val;
    nouveau->precedent = NULL;
    nouveau->suivant = *Pile;
    if(*Pile == NULL)
        nouveau->dernier = nouveau;
    else
        nouveau->dernier = (*Pile)->dernier;
    *Pile = nouveau;
}


void Liste_push_back(pile* Pile, int val)
{
    int i = 0;
    iterator it = NULL;
    element* nouveau = (element*) malloc(sizeof(element));
    nouveau->val = val;
    nouveau->suivant = NULL;
    if(*Pile == NULL)
    {
        nouveau->dernier = nouveau;
        nouveau->precedent = NULL;
        *Pile = nouveau;
    }
    else
    {
        for(it=(*Pile); it->suivant!=NULL; it=it->suivant)
            i++;
        //Parcours
        nouveau->precedent = it;
        it->suivant = nouveau;
        (*Pile)->dernier = nouveau;
    }
}

int Depiler(pile *Pile)
{
    element* save = NULL;
    int retour;
    if(*Pile != NULL)
    {
        retour = (*Pile)->val;
        save = (*Pile)->suivant;
        if(save != NULL)
            save->precedent = NULL;
        free(*Pile);
    }
    else
    {
        return -1;
    }
    (*Pile) = save;
    return retour;
}

int Liste_suprimer(pile *Pile,int valeur)
{
    int compteur = 0;
    iterator it = NULL;
    iterator precedent = NULL;

    for(it=*Pile;it!=NULL;it=it->suivant)
    {
        compteur++;
        if(valeur == it->val)
        {
            if(precedent != NULL)
                precedent->suivant = it->suivant;
            else
                *Pile = it->suivant;

            free(it);
            return compteur;
        }
        precedent = it;

    }

    return 0;
}

 int Liste_inserer(pile* Pile,int valeur, int position)
 {
    int i;
    element* nouveau = (element*) malloc(sizeof(element));
    iterator it = *Pile;
    for(i=1;i<position;i++)
         it = it->suivant;
    nouveau->val = valeur;
    nouveau->suivant = it->suivant;
    it->suivant = nouveau;
    return 1;
 }
int size(pile Pile)
{
    int compteur = 0;
    iterator it = NULL;
    for(it=Pile; it!=NULL; it=it->suivant)
        compteur++;
    return compteur;
}

int Liste_existe(pile Pile,int v)
{
    iterator it = NULL;
    for(it=Pile; it!=NULL; it=it->suivant)
    {
        if(it->val == v)
            return 1;
    }
    return 0;
}

int Liste_compare(pile Pile,pile Pile2)
{
    iterator it = NULL;
    iterator it2 = NULL;
    for(it=Pile; it!=NULL; it=it->suivant)
    {
        for(it2=Pile2; it2!=NULL ; it2=it2->suivant)
        {
            if(it->val == it2->val)
                return it->val;
        }
    }
    return -1;
}

void setVal_cb(caseBlanche* Case, char val)
{
    Case->valeur = val;
}

VAL caseNoire_nbVal(caseNoire Case)
{
    VAL valeur;
    if(Case.val_sup != 0)
    {
        if(Case.val_inf != 0)
            valeur = both;
        else
            valeur = sup_value;
    }
    else
    {
        if(Case.val_inf != 0)
            valeur = inf_value;
        else
            valeur = none;
    }
    return valeur;
}

int init_Grille(GrilleKakuro* grille,int nbCasesBlanches, int nbCasesNoires,pilePos caseBlanches,pileCn caseNoires,position max)
{
    VAL valeursCase;

    int posPrecedente = -1;

    int nb_cb = 0;
    int i = 0, j = 0, k = 0;

    grille->nbCasesNoires = nbCasesNoires;
    grille->nbCasesBlanches = nbCasesBlanches;
    grille->nbMots = 0;
    grille->max = max;
    grille->cb = (caseBlanche*) malloc(sizeof(caseBlanche)*(nbCasesBlanches));
    grille->cn = (caseNoire*) malloc(sizeof(caseNoire)*(nbCasesNoires));
    if(sizePos(caseBlanches) != nbCasesBlanches)
        return 0;
    if(sizeCn(caseNoires) != nbCasesNoires)
        return 0;
    for(i=0; i<nbCasesBlanches; i++)
    {
        grille->cb[i].pos = DepilerPos(&caseBlanches);
        grille->cb[j].cases_parents = NULL;
        grille->cb[j].case_parent_sup = NULL;
        grille->cb[j].case_parent_inf = NULL;
        grille->cb[i].valeur = 0;
        grille->cb[i].id = i;
    }
    for(i=0; i<nbCasesNoires; i++)
    {
        grille->cn[i] = DepilerCn(&caseNoires);
        grille->cn[i].sommeSup = 0;
        grille->cn[i].sommeInf = 0;
        grille->cn[i].id = i;
    }



    //assignation des mots et copmptage des mots

    for(i=0; i<nbCasesNoires; i++)
    {
        posPrecedente = -1;
        valeursCase = caseNoire_nbVal(grille->cn[i]);
        switch(valeursCase)
        {
        case none:
            grille->cn[i].mot_sup = NULL;
            grille->cn[i].mot_inf = NULL;
            grille->cn[i].nb_cb_valSup = 0;
            grille->cn[i].nb_cb_valInf = 0;
            break;

        case sup_value:
            for(j=0; j<nbCasesBlanches; j++)
            {
                if(grille->cb[j].pos.Y == grille->cn[i].pos.Y)
                {
                    if(posPrecedente == -1)
                        posPrecedente = grille->cn[i].pos.X;

                    if((posPrecedente - grille->cb[j].pos.X) == -1)
                    {
                        nb_cb++;
                        posPrecedente = grille->cb[j].pos.X;
                    }



                }
            }
            posPrecedente = -1;

            grille->cn[i].mot_sup = (caseBlanche**) malloc(sizeof(caseBlanche*) * nb_cb);

            for(j=0; j<nbCasesBlanches; j++)
            {
                if(grille->cb[j].pos.Y == grille->cn[i].pos.Y)
                {
                    if(posPrecedente == -1)
                        posPrecedente = grille->cn[i].pos.X;

                    if(i==12 && j==5)
                        printf("posPrecedente : %d / cn[i] = %d , pos cb : %d \n",posPrecedente, grille->cn[i].pos.X,grille->cb[j].pos.X);

                    if((posPrecedente - grille->cb[j].pos.X) == -1)
                    {
                        Liste_push_back(&(grille->cb[j].case_parent_sup), i);
                        Liste_push_back(&(grille->cb[j].cases_parents), i);
                        grille->cn[i].mot_sup[k] = &(grille->cb[j]);
                        k++;
                        posPrecedente = grille->cb[j].pos.X;
                    }



                }
            }
            posPrecedente = -1;
            grille->cn[i].nb_cb_valSup = k;
            grille->cn[i].nb_cb_valInf = 0;
            k=0;
            nb_cb = 0;
            grille->cn[i].mot_inf = NULL;
            grille->nbMots++;
            break;

        case inf_value:

            for(j=0; j<nbCasesBlanches; j++)
            {
                if(grille->cb[j].pos.X == grille->cn[i].pos.X)
                {
                    if(posPrecedente == -1)
                        posPrecedente = grille->cn[i].pos.Y;

                    if((posPrecedente - grille->cb[j].pos.Y) == -1)
                    {
                        nb_cb++;
                        posPrecedente = grille->cb[j].pos.Y;
                    }




                }
            }
            posPrecedente = -1;

            grille->cn[i].mot_inf = (caseBlanche**) malloc(sizeof(caseBlanche*) * nb_cb);

            for(j=0; j<nbCasesBlanches; j++)
            {
                if(grille->cb[j].pos.X == grille->cn[i].pos.X)
                {

                    if(posPrecedente == -1)
                        posPrecedente = grille->cn[i].pos.Y;
                    if((posPrecedente - grille->cb[j].pos.Y) == -1)
                    {
                        Liste_push_back(&(grille->cb[j].case_parent_inf), i);
                        Liste_push_back(&(grille->cb[j].cases_parents), i);
                        grille->cn[i].mot_inf[k] = &(grille->cb[j]);
                        k++;
                        posPrecedente = grille->cb[j].pos.Y;
                    }




                }
            }
            posPrecedente = -1;
            grille->cn[i].nb_cb_valSup = 0;
            grille->cn[i].nb_cb_valInf = k;
            k=0;
            nb_cb = 0;
            grille->cn[i].mot_sup = NULL;
            grille->nbMots++;
            break;

        case both:
            for(j=0; j<nbCasesBlanches; j++)
            {
                if(grille->cb[j].pos.Y == grille->cn[i].pos.Y)
                {
                    if(posPrecedente == -1)
                        posPrecedente = grille->cn[i].pos.X;

                    if((posPrecedente - grille->cb[j].pos.X) == -1)
                    {
                        nb_cb++;
                        posPrecedente = grille->cb[j].pos.X;
                    }



                }
            }
            posPrecedente = -1;

            grille->cn[i].mot_sup = (caseBlanche**) malloc(sizeof(caseBlanche*) * nb_cb);

            for(j=0; j<nbCasesBlanches; j++)
            {
                if(grille->cb[j].pos.Y == grille->cn[i].pos.Y)
                {
                    if(posPrecedente == -1)
                        posPrecedente = grille->cn[i].pos.X;
                    if((posPrecedente - grille->cb[j].pos.X) == -1)
                    {
                        Liste_push_back(&(grille->cb[j].case_parent_sup), i);
                        Liste_push_back(&(grille->cb[j].cases_parents), i);
                        grille->cn[i].mot_sup[k] = &(grille->cb[j]);
                        k++;
                        posPrecedente = grille->cb[j].pos.X;
                    }



                }
            }
            posPrecedente = -1;
            grille->cn[i].nb_cb_valSup = k;
            k=0;
            nb_cb = 0;

            for(j=0; j<nbCasesBlanches; j++)
            {
                if(grille->cb[j].pos.X == grille->cn[i].pos.X)
                {
                    if(posPrecedente == -1)
                        posPrecedente = grille->cn[i].pos.Y;

                    if((posPrecedente - grille->cb[j].pos.Y) == -1)
                    {
                        nb_cb++;
                        posPrecedente = grille->cb[j].pos.Y;
                    }




                }
            }
            posPrecedente = -1;

            grille->cn[i].mot_inf = (caseBlanche**) malloc(sizeof(caseBlanche*) * nb_cb);

            for(j=0; j<nbCasesBlanches; j++)
            {
                if(grille->cb[j].pos.X == grille->cn[i].pos.X)
                {

                    if(posPrecedente == -1)
                        posPrecedente = grille->cn[i].pos.Y;
                    if((posPrecedente - grille->cb[j].pos.Y) == -1)
                    {
                        Liste_push_back(&(grille->cb[j].case_parent_inf), i);
                        Liste_push_back(&(grille->cb[j].cases_parents), i);
                        grille->cn[i].mot_inf[k] = &(grille->cb[j]);
                        k++;
                        posPrecedente = grille->cb[j].pos.Y;
                    }



                }
            }
            posPrecedente = -1;
            grille->cn[i].nb_cb_valInf = k;
            k=0;
            nb_cb = 0;
            grille->nbMots+=2;
            break;
        }

    }

    printf("\nGrille initialisée. \n");

    return 1;
}

void afficherInfos_Grille(GrilleKakuro jeu)
{
    int i,j;
    iterator it = NULL;

    printf("Nombre de cases noires : %d \n",jeu.nbCasesNoires);
    printf("Nombre de cases blanches : %d \n",jeu.nbCasesBlanches);
    printf("Nombre de mots : %d \n\n",jeu.nbMots);
    for(i=0; i<jeu.nbCasesNoires; i++)
    {
        printf("Case noire num %d :\n\t - val_sup = %d / val_inf = %d \n",i,jeu.cn[i].val_sup,jeu.cn[i].val_inf);
        printf("\t - posX = %d / posY = %d \n",jeu.cn[i].pos.X , jeu.cn[i].pos.Y);
        printf("\t - mot_sup : %d case(s) : ", jeu.cn[i].nb_cb_valSup);
        for(j=0; j<jeu.cn[i].nb_cb_valSup; j++)
        {
            printf("[%d][%d]",jeu.cn[i].mot_sup[j]->pos.X, jeu.cn[i].mot_sup[j]->pos.Y);
            if(j!=jeu.cn[i].nb_cb_valSup - 1)
                printf(" -> ");
        }
        printf("\n");
        printf("\t - mot_inf : %d case(s) : ",jeu.cn[i].nb_cb_valInf);
        for(j=0; j<jeu.cn[i].nb_cb_valInf; j++)
        {
            printf("[%d][%d]",jeu.cn[i].mot_inf[j]->pos.X, jeu.cn[i].mot_inf[j]->pos.Y);
            if(j!=jeu.cn[i].nb_cb_valInf - 1)
                printf(" -> ");
        }
        printf("\n\n");
    }
    printf("\n");

    printf("Position des cases Blanches : \n");
    for(i=0; i<jeu.nbCasesBlanches; i++)
    {
        printf(" - Case blanche %d : [%d][%d] \n \t nbParents : %d \n \t Case(s) noire(s) parent(s) :",i,jeu.cb[i].pos.X, jeu.cb[i].pos.Y,size(jeu.cb[i].cases_parents));
        for(it=jeu.cb[i].cases_parents; it!=NULL; it = it->suivant)
            printf(" %d",it->val);
        printf("\n");
        /*if(i==6)
        {
            for(it=jeu.cb[i].case_parent_sup; it!=NULL; it=it->suivant)
                printf(" VAL = %d \n",it->val);
        }*/
        //printf("\n \t parent sup : %d \n \t parent inf : %d \n",jeu.cb[i].case_parent_sup->val,jeu.cb[i].case_parent_inf->val);
    }


}

int Ecrire_log(const char* phrase, char* nomDuFichier)
{
    FILE* fichier;

    if((fichier = fopen(nomDuFichier,"a+")) == NULL)
        return 0;

    fprintf(fichier,phrase);

    fclose(fichier);
    return 1;

}

int Ecrire_log_Infos_Grille(GrilleKakuro jeu,char* nomDuFichier)
{
    FILE* fichier;
    int i,j;
    iterator it = NULL;

    if((fichier = fopen(nomDuFichier,"a+")) == NULL)
        return 0;

    fprintf(fichier,"\n");

    fprintf(fichier,"Nombre de cases noires : %d \n",jeu.nbCasesNoires);
    fprintf(fichier,"Nombre de cases blanches : %d \n",jeu.nbCasesBlanches);
    fprintf(fichier,"Nombre de mots : %d \n\n",jeu.nbMots);
    for(i=0; i<jeu.nbCasesNoires; i++)
    {
        fprintf(fichier,"Case noire num %d :\n\t - val_sup = %d / val_inf = %d \n",i,jeu.cn[i].val_sup,jeu.cn[i].val_inf);
        fprintf(fichier,"\t - posX = %d / posY = %d \n",jeu.cn[i].pos.X , jeu.cn[i].pos.Y);
        fprintf(fichier,"\t - mot_sup : %d case(s) : ", jeu.cn[i].nb_cb_valSup);
        for(j=0; j<jeu.cn[i].nb_cb_valSup; j++)
        {
            fprintf(fichier,"case[%d][%d]",jeu.cn[i].mot_sup[j]->pos.X, jeu.cn[i].mot_sup[j]->pos.Y);
            if(j!=jeu.cn[i].nb_cb_valSup - 1)
                fprintf(fichier," -> ");
        }
        fprintf(fichier,"\n");
        fprintf(fichier,"\t - mot_inf : %d case(s) : ",jeu.cn[i].nb_cb_valInf);
        for(j=0; j<jeu.cn[i].nb_cb_valInf; j++)
        {
            fprintf(fichier,"case[%d][%d]",jeu.cn[i].mot_inf[j]->pos.X, jeu.cn[i].mot_inf[j]->pos.Y);
            if(j!=jeu.cn[i].nb_cb_valInf - 1)
                fprintf(fichier," -> ");
        }
        fprintf(fichier,"\n\n");
    }
    fprintf(fichier,"\n");

    fprintf(fichier,"Position des cases Blanches : \n");
    for(i=0; i<jeu.nbCasesBlanches; i++)
    {
        fprintf(fichier," - Case blanche %d : [%d][%d] \n \t nbParents : %d \n \t Case(s) noire(s) parent(s) :",i,jeu.cb[i].pos.X, jeu.cb[i].pos.Y,size(jeu.cb[i].cases_parents));
        for(it=jeu.cb[i].cases_parents; it!=NULL; it = it->suivant)
            fprintf(fichier," %d",it->val);
        fprintf(fichier,"\n");
        /*if(i==6)
        {
            for(it=jeu.cb[i].case_parent_sup; it!=NULL; it=it->suivant)
                printf(" VAL = %d \n",it->val);
        }*/
        //printf("\n \t parent sup : %d \n \t parent inf : %d \n",jeu.cb[i].case_parent_sup->val,jeu.cb[i].case_parent_inf->val);
    }

    fclose(fichier);
    return 1;
}

void calculerSommes(GrilleKakuro *jeu)
{
    int i,j;
    for(i=0; i<jeu->nbCasesNoires; i++)
    {
        jeu->cn[i].sommeSup = 0;
        jeu->cn[i].sommeInf = 0;

        for(j=0; j<jeu->cn[i].nb_cb_valSup; j++)
            jeu->cn[i].sommeSup = jeu->cn[i].sommeSup + jeu->cn[i].mot_sup[j]->valeur;

        for(j=0; j<jeu->cn[i].nb_cb_valInf; j++)
            jeu->cn[i].sommeInf = jeu->cn[i].sommeInf + jeu->cn[i].mot_inf[j]->valeur;

    }
}

void destroy_Grille(GrilleKakuro* newGrille)
{
    int i = 0;
    for(i=0; i<newGrille->nbCasesNoires; i++)
    {
        free(newGrille->cn[i].mot_sup);
        free(newGrille->cn[i].mot_inf);
    }
    free(newGrille->cn);
    free(newGrille->cb);
    printf("\nGrille detruite. \n");
}
