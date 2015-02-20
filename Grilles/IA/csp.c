#include <stdio.h>
#include <stdlib.h>

#include "csp.h"

void ListeVar_push_back(listeVariables* liste, variable v)
{
    int i = 0;
    iteratorVar it = NULL;
    variables* nouveau = (variables*) malloc(sizeof(variables));
    nouveau->v = v;
    nouveau->suivant = NULL;
    if(*liste == NULL)
        *liste = nouveau;
    else
    {
        for(it=(*liste); it->suivant!=NULL; it=it->suivant)
            i++;
        it->suivant = nouveau;
    }
}

void listeVar_push_front(listeVariables* liste, variable v)
{
    variables* nouveau = (variables*) malloc(sizeof(variables));
    nouveau->v = v;
    nouveau->suivant = *liste;
    *liste = nouveau;

}


variable DepilerVar(listeVariables* liste)
{
    variables* save = NULL;
    variable retour;
    if(*liste != NULL)
    {
        retour = (*liste)->v;
        save = (*liste)->suivant;
        free(*liste);
    }
    (*liste) = save;
    return retour;
}


int sizeVar(listeVariables liste)
{
    int compteur = 0;
    iteratorVar it = NULL;
    for(it=liste; it!=NULL; it=it->suivant)
        compteur++;
    return compteur;
}


void init_CSP(GrilleKakuro jeu, CSP* csp)
{
    int i,j;
    csp->V = NULL;
    csp->A = NULL;
    csp->D = (domaines*)malloc(sizeof(domaines)*jeu.nbCasesBlanches);
    variable v;
    for(i=0; i<jeu.nbCasesBlanches; i++)
    {
        csp->D[i] = NULL;
        //for(j=1;j<=9;j++) // Ordre 1 2 3 4 5 6 7 8 9
        // Liste_push_back(&(csp->D[i]),j);
        for(j=9; j>0; j--) // Ordre 9 8 7 6 5 4 3 2 1
            Liste_push_back(&(csp->D[i]),j);
        v.cb = &(jeu.cb[i]);
        v.d = &(csp->D[i]);
        ListeVar_push_back(&(csp->V),v);
    }

    printf("\nCSP initialisé. \n");

}

void destroy_CSP(GrilleKakuro jeu,CSP* csp)
{
    int i , j ;
    for(i=0;i<jeu.nbCasesBlanches;i++)
    {
        for(j=0;j<size(csp->D[i]);j++)
            Depiler(&(csp->D[i]));

        for(j=0;j<sizeVar(csp->V);j++)
            DepilerVar(&(csp->V));

        for(j=0;j<sizeVar(csp->A);j++)
            DepilerVar(&(csp->A));
    }

    free(csp->D);

    printf("\nCSP detruit. \n");
}

int backtrack(GrilleKakuro jeu, CSP* csp,int log)
{
    int i,j;
    iterator it = NULL;
    iteratorVar it2 = NULL;
    variable x;
    int v;

    int** copieDomaine = (int**)malloc(sizeof(int*) * sizeVar(csp->V));
    for(i=0;i<sizeVar(csp->V);i++)
        copieDomaine[i] = (int*)malloc(sizeof(int) * 9);

    for(i=0;i<sizeVar(csp->V);i++)
    {
        for(j=0;j<9;j++)
        {
            copieDomaine[i][j] = 0;
        }
    }

    char chaineLog[100];
    int solution = 0;

    for(it2=csp->V;it2!=NULL;it2=it2->suivant)
    {
         for(it=*(it2->v.d);it!=NULL;it=it->suivant)
            copieDomaine[it2->v.cb->id][it->val - 1] = 1;
    }

   /* for(i=0;i<sizeVar(csp->V);i++)
    {
        printf("\t domaineCopie d%d = [ ",i);
        for(j=8;j>=0;j--)
        {
            if(copieDomaine[i][j] == 1)
                printf(" %d ", j+1);
        }
        printf("]\n");
    }
    printf("\n"); */

    while(csp->V != NULL) // Tant qu'il y a des variables a tester
    {
        x = DepilerVar(&(csp->V)); // on choisit la variable au sommet de la pileet on la retire de la pile
        //solution = 0;


        while(*(x.d) != NULL) // tant qu'il y a des valeur dans le domaine de la variable choisie precedemment
        {
            v = Depiler(x.d); // on prend une valeur du sommet de la pile du domaine et on la retire de celui-ci
            solution = 0; // on part du fait que la valeur choisie n'est pas une solution
            if(log)
            {
                sprintf(chaineLog,"caseBlanche[%d][%d] = %d, TAILLE de CSP->V = %d TAILLE de CSP->A = %d \n",x.cb->pos.X,x.cb->pos.Y,v,sizeVar(csp->V),sizeVar(csp->A));
                Ecrire_log(chaineLog,nomFichierLog);
            }
            //
            if (Consistant(jeu,csp->A,x,v) == 1) // si la valeur est consistante
            {
                if(log)
                {
                    sprintf(chaineLog,"Valeur OK -> caseBlanche[%d][%d] = %d \n",x.cb->pos.X,x.cb->pos.Y,v);
                    Ecrire_log(chaineLog,nomFichierLog);
                }
                //printf("OK \n");
                solution = 1; // alors la valeur choisie est une solution
                x.cb->valeur = v; // on affecte cette valeur a la case correspondante de la grille
                listeVar_push_front(&(csp->A),x); // on etend progressivement la liste des affectations
                break;
            }
        }

        if( solution == 0) // si aucune valeur trouvee consistante
        {
            if(log)
            {
                sprintf(chaineLog,"Pas de valeurs , --> RETOUR \n");
                Ecrire_log(chaineLog,nomFichierLog);
            }
            //printf(" --> RETOUR\n");
            x.cb->valeur = 0; // on remet la valeur de la case à 0
            if(csp->A == NULL) //si on est revenu a la premiere valeur et on a essayé toutes les possibilités
            {

                for(i=0;i<sizeVar(csp->V) + 1;i++)
                    free(copieDomaine[i]);
                free(copieDomaine);
                return 0; // alors il n'y a pas de solution
            }

            for(i=8;i>=0;i--)
            {
                if(copieDomaine[x.cb->id][i] == 1)
                    Liste_push_back(x.d,i+1);
            }

           // for(i=9;i>0;i--) // on reinitialise le domaine de la variable
             //   Liste_push_back(x.d,i);

            listeVar_push_front(&(csp->V),x); //on remet la variable a parcourir au sommet de la pile des variables a parcourir
            listeVar_push_front(&(csp->V),csp->A->v); // on remet la variable precedemment parcourue au sommet de la pile
            DepilerVar(&(csp->A)); // on enleve la case actuelle de la liste des affectations
        }
        //printf("domaine TEMP taille = %d \n",size(temp));
        //printf("domaine taille = %d \n",size(*(x.d)));
    }

    for(i=0;i<sizeVar(csp->A);i++)
        free(copieDomaine[i]);
    free(copieDomaine);

    return 1;
}

int forward_checking(GrilleKakuro jeu, CSP* csp, int log)
{
    int i,j;
    int decalage = 0;
    int test = 1;
    iterator it = NULL;
    iteratorVar it2 = NULL;
    iteratorVar it3 = NULL;
    variable x;
    int v;

    pile* positions = (pile*)malloc(sizeof(pile) * sizeVar(csp->V));
    for(i=0;i<sizeVar(csp->V);i++)
        positions[i] = NULL;

    int** copieDomaine = (int**)malloc(sizeof(int*) * sizeVar(csp->V));
    for(i=0;i<sizeVar(csp->V);i++)
        copieDomaine[i] = (int*)malloc(sizeof(int) * 9);

    for(i=0;i<sizeVar(csp->V);i++)
    {
        for(j=0;j<9;j++)
        {
            copieDomaine[i][j] = 0;
        }
    }

    char chaineLog[100];
    int solution = 0;

    for(it2=csp->V;it2!=NULL;it2=it2->suivant)
    {
         for(it=*(it2->v.d);it!=NULL;it=it->suivant)
            copieDomaine[it2->v.cb->id][it->val - 1] = 1;
    }

   /* for(i=0;i<sizeVar(csp->V);i++)
    {
        printf("\t domaineCopie d%d = [ ",i);
        for(j=8;j>=0;j--)
        {
            if(copieDomaine[i][j] == 1)
                printf(" %d ", j+1);
        }
        printf("]\n");
    }
    printf("\n"); */

    while(csp->V != NULL) // Tant qu'il y a des variables a tester
    {
        x = DepilerVar(&(csp->V)); // on choisit la variable au sommet de la pileet on la retire de la pile
        //solution = 0;


        while(*(x.d) != NULL) // tant qu'il y a des valeur dans le domaine de la variable choisie precedemment
        {
            v = Depiler(x.d); // on prend une valeur du sommet de la pile du domaine et on la retire de celui-ci
            solution = 0; // on part du fait que la valeur choisie n'est pas une solution
            if(log)
            {
                sprintf(chaineLog,"caseBlanche[%d][%d] = %d, TAILLE de CSP->V = %d TAILLE de CSP->A = %d \n",x.cb->pos.X,x.cb->pos.Y,v,sizeVar(csp->V),sizeVar(csp->A));
                Ecrire_log(chaineLog,nomFichierLog);
            }
            //
            if (Consistant(jeu,csp->A,x,v) == 1) // si la valeur est consistante
            {
                if(log)
                {
                    sprintf(chaineLog,"Valeur OK -> caseBlanche[%d][%d] = %d \n",x.cb->pos.X,x.cb->pos.Y,v);
                    Ecrire_log(chaineLog,nomFichierLog);
                }
                //printf("OK \n");
                solution = 1; // alors la valeur choisie est une solution
                x.cb->valeur = v; // on affecte cette valeur a la case correspondante de la grille
                listeVar_push_front(&(csp->A),x); // on etend progressivement la liste des affectations

//FILTRAGE

                if(x.cb->case_parent_sup != NULL)
                {
                    for(i=0;i<jeu.cn[x.cb->case_parent_sup->val].nb_cb_valSup;i++)
                    {
                        for(it2=csp->V;it2!=NULL;it2=it2->suivant)
                        {
                            if(it2->v.cb == jeu.cn[x.cb->case_parent_sup->val].mot_sup[i])
                            {
                                decalage = Liste_suprimer(it2->v.d,v);
                                Liste_push_back(&(positions[it2->v.cb->id]),decalage);
                            }
                        }
                    }
                }

                if(x.cb->case_parent_inf !=NULL)
                {
                    for(i=0;i<jeu.cn[x.cb->case_parent_inf->val].nb_cb_valInf;i++)
                    {
                        for(it2=csp->V;it2!=NULL;it2=it2->suivant)
                        {
                            if(it2->v.cb == jeu.cn[x.cb->case_parent_inf->val].mot_inf[i])
                            {
                                decalage = Liste_suprimer(it2->v.d,v);
                                Liste_push_back(&(positions[it2->v.cb->id]),decalage);
                            }
                        }
                    }
                }

                break;
            }
        }

        if( solution == 0) // si aucune valeur trouvee consistante
        {
            if(log)
            {
                sprintf(chaineLog,"Pas de valeurs , --> RETOUR \n");
                Ecrire_log(chaineLog,nomFichierLog);
            }
            //printf(" --> RETOUR\n");
            x.cb->valeur = 0; // on remet la valeur de la case à 0
            if(csp->A == NULL) //si on est revenu a la premiere valeur et on a essayé toutes les possibilités
            {

                for(i=0;i<sizeVar(csp->V) + 1;i++)
                    free(copieDomaine[i]);
                free(copieDomaine);
                return 0; // alors il n'y a pas de solution
            }

            for(i=8;i>=0;i--)
            {
                for(it2=csp->A;it2!=NULL;it2=it2->suivant)
                {
                    if(x.cb->case_parent_sup != NULL && it2->v.cb->case_parent_sup != NULL)
                    {
                        if(x.cb->case_parent_sup->val != it2->v.cb->case_parent_sup->val)
                        {
                            test = 1;
                        }
                        else
                        {
                            test = 0;
                        }
                    }

                    if(x.cb->case_parent_inf != NULL && it2->v.cb->case_parent_inf != NULL)
                    {
                        if(x.cb->case_parent_inf->val != it2->v.cb->case_parent_inf->val)
                        {
                            test = 1;
                        }
                        else
                        {
                            test = 0;
                        }
                    }

                }

                if(copieDomaine[x.cb->id][i] == 1 && test == 1)
                    Liste_push_back(x.d,i+1);

                test = 1;
            }

            it3 = csp->A;

            if(it3->v.cb->case_parent_sup != NULL)
            {
                    for(i=0;i<jeu.cn[it3->v.cb->case_parent_sup->val].nb_cb_valSup;i++)
                    {
                        for(it2=csp->V;it2!=NULL;it2=it2->suivant)
                        {
                            if(it2->v.cb == jeu.cn[it3->v.cb->case_parent_sup->val].mot_sup[i])
                            {
                                decalage = Depiler(&positions[it3->v.cb->id]);
                                Liste_inserer(it2->v.d,it3->v.cb->valeur,decalage);
                            }
                        }
                    }
            }

            if(it3->v.cb->case_parent_inf != NULL)
            {
                    for(i=0;i<jeu.cn[it3->v.cb->case_parent_inf->val].nb_cb_valInf;i++)
                    {
                        for(it2=csp->V;it2!=NULL;it2=it2->suivant)
                        {
                            if(it2->v.cb == jeu.cn[it3->v.cb->case_parent_inf->val].mot_inf[i])
                            {
                                decalage = Depiler(&positions[it3->v.cb->id]);
                                Liste_inserer(it2->v.d,it3->v.cb->valeur,decalage);
                            }
                        }
                    }
            }



           // for(i=9;i>0;i--) // on reinitialise le domaine de la variable
             //   Liste_push_back(x.d,i);

            listeVar_push_front(&(csp->V),x); //on remet la variable a parcourir au sommet de la pile des variables a parcourir
            listeVar_push_front(&(csp->V),csp->A->v); // on remet la variable precedemment parcourue au sommet de la pile
            DepilerVar(&(csp->A)); // on enleve la case actuelle de la liste des affectations
        }
        //printf("domaine TEMP taille = %d \n",size(temp));
        //printf("domaine taille = %d \n",size(*(x.d)));
    }

    for(i=0;i<sizeVar(csp->A);i++)
        free(copieDomaine[i]);
    free(copieDomaine);

    return 1;
}

int Consistant(GrilleKakuro jeu,Affectation A, variable x, int v)
{
    int i;
    //x.cb->valeur = v;
    //printf("X = %d \n \n",x.cb->valeur);
    iteratorVar it = NULL;
    iterator it2 = NULL;
    int ok_sup = 1, ok_inf = 1;
    // int parent = -2;
    int* sommeSup = (int*)malloc(sizeof(int)*jeu.nbCasesNoires);
    int* sommeInf = (int*)malloc(sizeof(int)*jeu.nbCasesNoires);

    for(i=0; i<jeu.nbCasesNoires; i++)
    {
        sommeSup[i] = 0;
        sommeInf[i] = 0;
    }

    calculerSommes(&jeu);

    for(it2 = x.cb->cases_parents; it2 != NULL ; it2 = it2->suivant)
    {
        if(x.cb->case_parent_sup != NULL)
        {
            if(x.cb->case_parent_sup->val == it2->val)
            {
                for(i=0; i<jeu.cn[it2->val].nb_cb_valSup; i++)
                {
                    if(x.cb != jeu.cn[it2->val].mot_sup[i])
                    {
                        if(jeu.cn[it2->val].mot_sup[i]->valeur == 0)
                            ok_sup = 0;
                    }
                }
                if(ok_sup == 1)
                {
                    sommeSup[it2->val] = jeu.cn[it2->val].sommeSup + v;
                    if(sommeSup[it2->val] != jeu.cn[it2->val].val_sup)
                    {
                        free(sommeSup);
                        free(sommeInf);
                        return 0;
                    }
                }
            }

        }

        if(x.cb->case_parent_inf != NULL)
        {
            if(x.cb->case_parent_inf->val == it2->val)
            {
                for(i=0; i<jeu.cn[it2->val].nb_cb_valInf; i++)
                {
                    if(x.cb != jeu.cn[it2->val].mot_inf[i])
                    {
                        if(jeu.cn[it2->val].mot_inf[i]->valeur == 0)
                            ok_inf = 0;
                    }
                }
                if(ok_inf == 1)
                {
                    sommeInf[it2->val] = jeu.cn[it2->val].sommeInf + v;
                    if(sommeInf[it2->val] != jeu.cn[it2->val].val_inf)
                    {
                        free(sommeSup);
                        free(sommeInf);
                        return 0;
                    }

                }
            }
        }
    }

    for(it = A; it!=NULL ; it = it->suivant)
    {
        if(it->v.cb->valeur == v)
        {
            if(it->v.cb->case_parent_sup != NULL)
            {
                if(x.cb->case_parent_sup != NULL)
                {
                    if(x.cb->case_parent_sup->val == it->v.cb->case_parent_sup->val)
                    {
                        free(sommeSup);
                        free(sommeInf);
                        return 0;
                    }
                }
            }

            if(it->v.cb->case_parent_inf != NULL)
            {
                if(x.cb->case_parent_inf != NULL)
                {
                    if(x.cb->case_parent_inf->val == it->v.cb->case_parent_inf->val)
                    {
                        free(sommeSup);
                        free(sommeInf);
                        return 0;
                    }
                }
            }
        }
    }


   /* for(i=0; i<jeu.nbCasesNoires; i++)
    {
        sommeSup[i] = 0;
        sommeInf[i] = 0;
    }*/

    free(sommeSup);
    free(sommeInf);
    return 1;
}

int filtrer(GrilleKakuro jeu, CSP *csp)
{
    int v;
    iteratorVar it = NULL;

            for(it=csp->V;it!=NULL;it=it->suivant)
            {
                if(it->v.cb->case_parent_sup != NULL)
                {
                    v = jeu.cn[it->v.cb->case_parent_sup->val].val_sup - (jeu.cn[it->v.cb->case_parent_sup->val].nb_cb_valSup - 1);
                    if (v >= 9) continue;
                    //printf("V = %d \n",v);
                    while((*(it->v.d))->val !=v)
                        Depiler(it->v.d);
                }

                /*if(it->v.cb->case_parent_inf != NULL)
                {
                    v = jeu.cn[it->v.cb->case_parent_inf->val].val_inf - (jeu.cn[it->v.cb->case_parent_inf->val].nb_cb_valInf - 1);
                    if (v >= 9) continue;
                    //printf("V = %d \n",v);
                    while((*(it->v.d))->val !=v)
                        Depiler(it->v.d);
                }*/
            }
            return 1;
}

int filtre2(GrilleKakuro jeu, CSP *csp)
{
    iteratorVar it = NULL;

    for(it=csp->V;it!=NULL;it=it->suivant)
    {
        //FILTRE
    }
    return 1;
}

void infos_CSP(GrilleKakuro jeu, CSP csp)
{
    int i;
    iterator it = NULL;
    for(i=0; i<jeu.nbCasesBlanches; i++)
    {
        printf("Domaine d%d = [",i);
        for(it=csp.D[i]; it!=NULL; it=it->suivant)
            printf("%d ",it->val);
        printf(" ] \n");
    }
   // printf("COMPARAISON : %d \n",Liste_compare(csp.D[1],csp.D[2]));
}

int Ecrire_log_infos_CSP(GrilleKakuro jeu, CSP csp,char* nomDuFichier)
{
    int i;
    iterator it = NULL;
    FILE* fichier;
    fichier = fopen(nomDuFichier,"a+");
    if(fichier == NULL)
        return 0;
    fprintf(fichier,"\n");
    for(i=0; i<jeu.nbCasesBlanches; i++)
    {
        fprintf(fichier,"Domaine d%d = [",i);
        for(it=csp.D[i]; it!=NULL; it=it->suivant)
            fprintf(fichier,"%d ",it->val);
        fprintf(fichier," ] \n");
    }
    fclose(fichier);
    return 1;
   // printf("COMPARAISON : %d \n",Liste_compare(csp.D[1],csp.D[2]));
}
