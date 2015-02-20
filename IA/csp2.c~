#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "csp2.h"


/* Fonction qui ajoute un element à la fin d'une liste chainée de structure "variable" */

void ListeVar_push_back(listeVariables* liste, variable v)
{
	// on initialise les variables
    int i = 0; 
    iteratorVar it = NULL;
    
    variables* nouveau = (variables*) malloc(sizeof(variables)); // on alloue l'espace memoire pour le nouvel element
    nouveau->v = v; // on initialise les valeurs du nouvel element avec celle de l'element passé en parametre de la fonction.
    nouveau->suivant = NULL; // on place l'element en fin de liste dont il n'y a pas d'élement suivant.
    if(*liste == NULL) // si la liste est vide 
        *liste = nouveau; // on ajoute directement l'element
    else // sinon
    {
        for(it=(*liste); it->suivant!=NULL; it=it->suivant) // on se place à la fin de la liste
            i++;
        it->suivant = nouveau; // on ajoute l'element
    }
}

/* Fonction qui ajoute un element au debut d'une liste chainée de structure "variable" */

void listeVar_push_front(listeVariables* liste, variable v)
{
    variables* nouveau = (variables*) malloc(sizeof(variables));// on alloue l'espace memoire pour le nouvel element
    nouveau->v = v; //on initialise les valeurs du nouvel element avec celle de l'element passé en parametre de la fonction.
    nouveau->suivant = *liste; // comme on place l'element en debut de liste , le suivant de l'element qu'on souhaite ajouté devient le premier element de la liste.
    *liste = nouveau; // on place le nouvel element en tete de liste.

}


/* Fonction qui retourne et supprime le premier element d'une liste chainée de structure "variable" */

variable DepilerVar(listeVariables* liste)
{
	//initisalisation des variables
    variables* save = NULL; //variable pour sauvegarder l'element suivant de l'element que nous allons supprimer.
    variable retour; // variable de retour de la fonction.
    if(*liste != NULL) // on verifie que la liste n'est pas vide.
    {
        retour = (*liste)->v;//on place dans la valeur de retour le premier element de la liste.
        save = (*liste)->suivant; // on sauvegarde dans save le second element de la liste.
        free(*liste);// on libere la mémoire alouée au premier element de la liste.
    }
    (*liste) = save; // on place le second element en tete de liste
    return retour; // on retourne la valeur du premier element
}

/* Fonction qui retourne le nombre d'element que contient une liste chainée de structure "variable" */

int sizeVar(listeVariables liste)
{
	//initialisation des variables
    int compteur = 0;
    iteratorVar it = NULL;
    for(it=liste; it!=NULL; it=it->suivant)// pour chaque element de la liste
        compteur++; //on incremente compteur
    return compteur; //on retourne compteur qui contient desormais le nombre d'elements de la liste.
}


/* Fonction qui initialise la structure "csp" */

void init_CSP(GrilleKakuro jeu, CSP* csp)
{
	//initialisation des variables
    int i,j;
    csp->V = NULL; // liste des variables
    csp->A = NULL; // liste des affectations
    //on alloue en memoire une matrice, pour chaque case blanche (variable) on associe un tableau de 9 boolean dont chaque indice represente une valeur possible.
    csp->D = (domaines**)malloc(sizeof(domaines*)*jeu.nbCasesBlanches); // domaines des variables
    for(i=0;i<jeu.nbCasesBlanches;i++)
        csp->D[i] = (domaines*)malloc(sizeof(domaines)*9);
    variable v;
    for(i=0; i<jeu.nbCasesBlanches; i++) // pour chacune des variables
    {
        for(j=0; j<9; j++)
            csp->D[i][j] = 1; // on initialise tous les domaines à [ 1 2 3 4 5 6 7 8 9 ]
        v.cb = &(jeu.cb[i]); // on associe a chaque variable l'adresse de la case blanche qu'elle represente
        v.d = csp->D[i]; // on lui associe son domaine.
        ListeVar_push_back(&(csp->V),v); // on place la variable dans la liste des variables.
    }

    printf("\nCSP initialisé. \n");

}

/* Fonction qui libere l'espace mémoire alouée par la creation d'une structure CSP */

void destroy_CSP(GrilleKakuro jeu,CSP* csp)
{
    int i , j ;
    for(i=0;i<jeu.nbCasesBlanches;i++)//pour chacune des variables.
    {
        for(j=0;j<sizeVar(csp->V);j++) // pour chacune des variables contenues dans la liste des variables.
            DepilerVar(&(csp->V)); // on suprime la tete de liste.

        for(j=0;j<sizeVar(csp->A);j++) // pour chacune des variables contenues dans la liste des affectations.
            DepilerVar(&(csp->A));// on supprime la tete de liste.
    }

    for(i=0;i<jeu.nbCasesBlanches;i++) // pour chacune des variables
        free(csp->D[i]); // on libere la memoire alouée pour les domaines
    free(csp->D);
    printf("\nCSP detruit. \n");

}

/* Fonction de filtrage qui permet d'enlever certaines valeurs impossibles dans certains domaines */

int filtrer(GrilleKakuro jeu, CSP *csp)
{
	//initialisation des variables
    int i;
    int v;
    int somme = 0;
    iteratorVar it = NULL;

            for(it=csp->V;it!=NULL;it=it->suivant)
            {
                if(it->v.cb->case_parent_sup != NULL) //pour chacune des variables formant un mot superieur
                {
                    v = jeu.cn[it->v.cb->case_parent_sup->val].val_sup - (jeu.cn[it->v.cb->case_parent_sup->val].nb_cb_valSup - 1); // on stocke dans v la valeur que doit contenir le mot superieur et on lui soustrait le nombre de case composant le mot -1.
                    if (v < 9) // si cette valeur est inférieure a la valeur max des domaines qui est 9
                    {
                        for(i=v;i<9;i++)
                            it->v.d[i] = 0; // alors on retire toutes les valeurs supérieures a celle-ci du domaine.
                    }
                }

                if(it->v.cb->case_parent_inf != NULL)//pour chacune des variables formant un mot inférieur
                {
                    v = jeu.cn[it->v.cb->case_parent_inf->val].val_inf - (jeu.cn[it->v.cb->case_parent_inf->val].nb_cb_valInf - 1); // on stocke dans v la valeur que doit contenir le mot inférieur et on lui soustrait le nombre de case composant le mot -1.
                    if (v < 9)
                    {
                        for(i=v;i<9;i++)
                            it->v.d[i] = 0; // alors on retire toutes les valeurs supérieures a celle-ci du domaine.
                    }
                }
            }


            for(it=csp->V;it!=NULL;it=it->suivant)
            {
                if(it->v.cb->case_parent_sup != NULL)//pour chacune des variables formant un mot superieur
                {
                    for(i=0;i<jeu.cn[it->v.cb->case_parent_sup->val].nb_cb_valSup - 1;i++)
                        somme = somme + 9 - i; // on calcule somme = les valeurs max des cases qui forment le mot

                    for(i=0;i<9;i++)
                    {
                        if(somme + (i+1) < jeu.cn[it->v.cb->case_parent_sup->val].val_sup)// pour chacune des valeur d'un domaine, si somme + i( une valeur du domaine) est inférieure a la valeur du mot supérieur
                            it->v.d[i] = 0; // on retire i du domaine.
                    }
                }

                if(it->v.cb->case_parent_inf != NULL)//pour chacune des variables formant un mot inférieur
                {
                    for(i=0;i<jeu.cn[it->v.cb->case_parent_inf->val].nb_cb_valInf - 1;i++)
                        somme = somme + 9 - i;// on calcule somme = les valeurs max des cases qui forment le mot

                    for(i=0;i<9;i++)
                    {
                        if(somme + (i+1) < jeu.cn[it->v.cb->case_parent_inf->val].val_inf)// pour chacune des valeur d'un domaine, si somme + i( une valeur du domaine) est inférieure a la valeur du mot inférieur
                            it->v.d[i] = 0;// on retire i du domaine.
                    }
                }

            }

            return 1;
}

// Fonction qui permet d'afficher tous les domaines de chacune des variables a l'écran

void infos_CSP(GrilleKakuro jeu, CSP csp)
{
    int i;
    int j;
    for(i=0; i<jeu.nbCasesBlanches; i++)
    {
        printf("Domaine d%d = [",i);
        for(j=0;j<9;j++)
        {
            if(csp.D[i][j])
                printf("%d ",j+1);
        }

        printf(" ] \n");
    }
}

// Fonction qui permet d'afficher tous les domaines de chacune des variables dans un fichier

int Ecrire_log_infos_CSP(GrilleKakuro jeu, CSP csp,char* nomDuFichier)
{
    int i;
    int j;
    FILE* fichier;
    fichier = fopen(nomDuFichier,"a+");
    if(fichier == NULL)
        return 0;
    fprintf(fichier,"\n");
    for(i=0; i<jeu.nbCasesBlanches; i++)
    {
        fprintf(fichier,"Domaine d%d = [",i);
        for(j=0;j<9;j++)
        {
            if(csp.D[i][j])
                fprintf(fichier, "%d ",j+1);
        }
        fprintf(fichier," ] \n");
    }
    fclose(fichier);
    return 1;
}

// Fonction de resolution d'une grille donnée a l'aide de l'algorithme backtrack, retourne 1 si solution trouvée , 0 sinon.

int backtrack(GrilleKakuro jeu, CSP* csp,int log)
{
    int i,j;
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

    char chaineLog[150];
    int solution = 0;
    int num = 0;
    for(it2=csp->V;it2!=NULL;it2=it2->suivant)
    {
         for(i=0;i<9;i++)
            copieDomaine[it2->v.cb->id][i] = it2->v.d[i];
    }



    while(csp->V != NULL) // Tant qu'il y a des variables a tester
    {
        x = DepilerVar(&(csp->V)); // on choisit la variable au sommet de la pileet on la retire de la pile
        solution = 0;

        while((v = domaineVide(x.d)) != -1) // tant qu'il y a des valeur dans le domaine de la variable choisie precedemment
        {
           // v = num[x.cb->id]; // on prend une valeur du sommet de la pile du domaine et on la retire de celui-ci
            csp->D[x.cb->id][v-1] = 0;

            solution = 0; // on part du fait que la valeur choisie n'est pas une solution
            if(log)
            {
                sprintf(chaineLog,"caseBlanche[%d][%d] = %d, TAILLE de CSP->V = %d TAILLE de CSP->A = %d \n ",x.cb->pos.X,x.cb->pos.Y,v,sizeVar(csp->V),sizeVar(csp->A));
                //Ecrire_log_infos_CSP(jeu,*csp,nomFichierLog);
                Ecrire_log(chaineLog,nomFichierLog);
            }
            //
            num++;
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
            x.cb->valeur = 0; // on remet la valeur de la case à 0
            if(csp->A == NULL) //si on est revenu a la premiere valeur et on a essayé toutes les possibilités
            {

                for(i=0;i<sizeVar(csp->V);i++)
                    free(copieDomaine[i]);
                free(copieDomaine);
                return 0; // alors il n'y a pas de solution
            }

            for(i=0;i<9;i++)
            {
                if(copieDomaine[x.cb->id][i] == 1)
                    x.d[i] = 1;
            }

            listeVar_push_front(&(csp->V),x); //on remet la variable a parcourir au sommet de la pile des variables a parcourir
            listeVar_push_front(&(csp->V),csp->A->v); // on remet la variable precedemment parcourue au sommet de la pile
            DepilerVar(&(csp->A)); // on enleve la case actuelle de la liste des affectations
        }
    }

    for(i=0;i<sizeVar(csp->A);i++)
        free(copieDomaine[i]);
    free(copieDomaine);

        sprintf(chaineLog,"\n Nombre de tests : %d \n SOLUTION : \n", num);
        Ecrire_log(chaineLog,nomFichierLog);
        for(it2=csp->A;it2!=NULL;it2=it2->suivant)
        {
            sprintf(chaineLog,"\t Case blanche %d = %d \n",it2->v.cb->id,it2->v.cb->valeur);
            Ecrire_log(chaineLog,nomFichierLog);
        }
    return 1;
}

// Fonction de resolution d'une grille donnée a l'aide de l'algorithme forward checking, retourne 1 si solution trouvée , 0 sinon.

int forward_checking(GrilleKakuro jeu, CSP* csp,int log)
{
    int i,j;
    int oki = 1;
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
    int num = 0;
    for(it2=csp->V;it2!=NULL;it2=it2->suivant)
    {
         for(i=0;i<9;i++)
            copieDomaine[it2->v.cb->id][i] = it2->v.d[i];
    }



    while(csp->V != NULL) // Tant qu'il y a des variables a tester
    {
        x = DepilerVar(&(csp->V)); // on choisit la variable au sommet de la pileet on la retire de la pile
        solution = 0;

        while((v = domaineVide(x.d)) != -1) // tant qu'il y a des valeur dans le domaine de la variable choisie precedemment
        {
           // v = num[x.cb->id]; // on prend une valeur du sommet de la pile du domaine et on la retire de celui-ci
            csp->D[x.cb->id][v-1] = 0;

            solution = 0; // on part du fait que la valeur choisie n'est pas une solution
            if(log)
            {
                sprintf(chaineLog,"caseBlanche %d[%d][%d] = %d, TAILLE de CSP->V = %d TAILLE de CSP->A = %d \n \t domaine d%d = [",x.cb->id,x.cb->pos.X,x.cb->pos.Y,v,sizeVar(csp->V),sizeVar(csp->A),x.cb->id);
                Ecrire_log(chaineLog,nomFichierLog);
                for(i=0;i<9;i++)
                {
                    if(csp->D[x.cb->id][i] == 1)
                    {
                                sprintf(chaineLog,"%d ",i+1);
                                Ecrire_log(chaineLog,nomFichierLog);
                    }
                }
                sprintf(chaineLog,"] \n");
                Ecrire_log(chaineLog,nomFichierLog);
               // Ecrire_log_infos_CSP(jeu,*csp,nomFichierLog);

            }
            //
             num++;

             oki = 1;

             for(it2=csp->V;it2!=NULL;it2=it2->suivant)
             {
                    if(tailleDomaine(csp->D[it2->v.cb->id]) == 1)
                    {
                        if(domaineVide(csp->D[it2->v.cb->id]) == v)
                        {
                            if(it2->v.cb->case_parent_sup != NULL && x.cb->case_parent_sup!=NULL)
                            {
                                if(it2->v.cb->case_parent_sup->val == x.cb->case_parent_sup->val)
                                {
                                    oki=0;
                                }

                            }

                            if(it2->v.cb->case_parent_inf != NULL && x.cb->case_parent_inf!=NULL)
                            {
                                if(it2->v.cb->case_parent_inf->val == x.cb->case_parent_inf->val)
                                    oki=0;
                            }
                        }
                    }
            }

            if (Consistant(jeu,csp->A,x,v) == 1 && oki ==1) // si la valeur est consistante
            {

                if(log)
                {
                    sprintf(chaineLog,"Valeur OK -> caseBlanche[%d][%d] = %d \n",x.cb->pos.X,x.cb->pos.Y,v);
                    Ecrire_log(chaineLog,nomFichierLog);
                }
     
                    solution = 1; // alors la valeur choisie est une solution
                    x.cb->valeur = v; // on affecte cette valeur a la case correspondante de la grille
                    listeVar_push_front(&(csp->A),x); // on etend progressivement la liste des affectations
                    if(x.cb->case_parent_sup != NULL)
                    {
                        for(i=0;i<jeu.cn[x.cb->case_parent_sup->val].nb_cb_valSup;i++)
                        {
                                if(jeu.cn[x.cb->case_parent_sup->val].mot_sup[i]->id > x.cb->id)
                                    csp->D[jeu.cn[x.cb->case_parent_sup->val].mot_sup[i]->id][v-1] = 0;
                        }
                    }
                    if(x.cb->case_parent_inf != NULL)
                    {
                        for(i=0;i<jeu.cn[x.cb->case_parent_inf->val].nb_cb_valInf;i++)
                        {
                                if(jeu.cn[x.cb->case_parent_inf->val].mot_inf[i]->id > x.cb->id)
                                    csp->D[jeu.cn[x.cb->case_parent_inf->val].mot_inf[i]->id][v-1] = 0;
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
            x.cb->valeur = 0; // on remet la valeur de la case à 0
            if(csp->A == NULL) //si on est revenu a la premiere valeur et on a essayé toutes les possibilités
            {

                for(i=0;i<sizeVar(csp->V);i++)
                    free(copieDomaine[i]);
                free(copieDomaine);
                return 0; // alors il n'y a pas de solution
            }

            for(i=0;i<9;i++)
            {
                if(copieDomaine[x.cb->id][i] == 1)
                    x.d[i] = 1;

            }

                for(it2=csp->A->suivant;it2!=NULL;it2=it2->suivant)
                {
                    if(x.cb->case_parent_sup != NULL && it2->v.cb->case_parent_sup != NULL)
                    {
                        if(x.cb->case_parent_sup->val == it2->v.cb->case_parent_sup->val)
                        {
   
                            x.d[it2->v.cb->valeur - 1] = 0;
                        }
                    }


                    if(x.cb->case_parent_inf != NULL && it2->v.cb->case_parent_inf != NULL)
                    {
                        if(x.cb->case_parent_inf->val == it2->v.cb->case_parent_inf->val)
                        {
                            x.d[it2->v.cb->valeur - 1] = 0;
                        }
                    }

                }



            if(csp->A->v.cb->case_parent_sup != NULL)
            {
                    for(i=0;i<jeu.cn[csp->A->v.cb->case_parent_sup->val].nb_cb_valSup;i++)
                    {
                        if(jeu.cn[csp->A->v.cb->case_parent_sup->val].mot_sup[i]->id > csp->A->v.cb->id)
                            csp->D[jeu.cn[csp->A->v.cb->case_parent_sup->val].mot_sup[i]->id][csp->A->v.cb->valeur -1] = 1;
                    }
            }

            if(csp->A->v.cb->case_parent_inf != NULL)
            {
                for(i=0;i<jeu.cn[csp->A->v.cb->case_parent_inf->val].nb_cb_valInf;i++)
                {
                        if(jeu.cn[csp->A->v.cb->case_parent_inf->val].mot_inf[i]->id > csp->A->v.cb->id)
                            csp->D[jeu.cn[csp->A->v.cb->case_parent_inf->val].mot_inf[i]->id][csp->A->v.cb->valeur -1] = 1;
                }
            }


 

            listeVar_push_front(&(csp->V),x); //on remet la variable a parcourir au sommet de la pile des variables a parcourir
            listeVar_push_front(&(csp->V),csp->A->v); // on remet la variable precedemment parcourue au sommet de la pile
            DepilerVar(&(csp->A)); // on enleve la case actuelle de la liste des affectations
        }
    }

    for(i=0;i<sizeVar(csp->A);i++)
        free(copieDomaine[i]);
    free(copieDomaine);

        sprintf(chaineLog,"\n Nombre de tests : %d \n SOLUTION : \n", num);
        Ecrire_log(chaineLog,nomFichierLog);
        for(it2=csp->A;it2!=NULL;it2=it2->suivant)
        {
            sprintf(chaineLog,"\t Case blanche %d = %d \n",it2->v.cb->id,it2->v.cb->valeur);
            Ecrire_log(chaineLog,nomFichierLog);
        }

    return 1;
}

//Fonction qui permet de calculer si une valeur pour une certaine variable est correcte et ne viole aucune contrainte, renvoie 1 si la valeur est acceptée ,0 sinon.
int Consistant(GrilleKakuro jeu,Affectation A, variable x, char v)
{
    int i;
    iteratorVar it = NULL;
    iterator it2 = NULL;
    int ok_sup = 1, ok_inf = 1;
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

    free(sommeSup);
    free(sommeInf);
    return 1;
}

// Fonction qui verifie si un domaine est vide , si oui retourne -1 sinon retourne la premiere valeur non vide du domaine 
int domaineVide(domaines* d)
{
        int i;
        for(i=0;i<9;i++)
        {
            if(d[i] != 0)
                return i+1;
        }
        return -1;
}

//fonction qui retourne le nombre d'elements dans le domaine
int tailleDomaine(domaines *d)
{
    int i;
    int c = 0;
    for(i=0;i<9;i++)
    {
        if(d[i] == 1)
            c++;
    }
    return c;
}
