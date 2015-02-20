#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "structures.h"
#include "csp2.h"
#include "input.h"
#include "OutputG.h"

char nomFichierLog[300] = "";
char repAbsolu[300] = "";

int main(int argc, char* argv[])
{
    int i;
    GrilleKakuro jeu;
    CSP csp;
    char* pch;
    char nomFichier[300] = "";
    char chemin[300] = "";
    char nomGrille[30] = "";
    char log[100] = "";
    time_t temps;


    pch = strrchr(argv[0],'/');
    for(i=0;i<pch-argv[0]+1;i++)
        repAbsolu[i] = argv[0][i];
    repAbsolu[(pch-argv[0]+1)] = '\0';

    strcat(nomFichier,repAbsolu);
    strcat(nomFichier,"Logs/");
    strcat(chemin,repAbsolu);
    strcat(chemin,"Grilles/");
    time(&temps);
    strcat(nomFichier,ctime(&temps));
    for(i=0; i<strlen(nomFichier)+1; i++)
        nomFichierLog[i] = nomFichier[i];


    printf("Nom du fichier de la grille a ouvrir: (le fichier doit se trouver dans le repertoire /Grilles) \n");
    if(!scanf("%s",nomGrille))
        return 0;

    strcat(chemin,nomGrille);

    switch(LectureFichier(chemin,&jeu))
    {
        case ERREUR_F_LECTURE:
            printf("Erreur de lecture, format du fichier incorrect ! \n \n");

            sprintf(log,"Grille : %s  - format incorrect ! \n\n",chemin);
            if((Ecrire_log(log,nomFichierLog)))
                printf("\n Fichier Log correctement ecrit. \n");
            else
                printf("Erreur lors de l\'ecriture du fichier log. \n");

        break;

        case ERREUR_F_INTROUVABLE:
            printf("Fichier : %s introuvable ! \n \n",chemin);

            sprintf(log,"Grille : %s  - fichier introuvable ! \n\n",chemin);
            if((Ecrire_log(log,nomFichierLog)))
                printf("\n Fichier Log correctement ecrit. \n");
            else
                printf("Erreur lors de l\'ecriture du fichier log. \n");

        break;

        case OK:
            printf("Fichier %s ouvert et lu avec success ! \n\n",chemin);

            sprintf(log,"Grille ouverte : %s \n\n",chemin);
            if((Ecrire_log(log,nomFichierLog)))
                printf("\n Fichier Log correctement ecrit. \n");
            else
                printf("Erreur lors de l\'ecriture du fichier log. \n");


            afficherInfos_Grille(jeu);

            if((Ecrire_log_Infos_Grille(jeu,nomFichierLog)))
                printf("\n Fichier Log correctement ecrit. \n");
            else
                printf("Erreur lors de l\'ecriture du fichier log. \n");

            init_CSP(jeu,&csp);
            infos_CSP(jeu,csp);

            if(Ecrire_log_infos_CSP(jeu,csp,nomFichierLog))
                printf("\n Fichier Log correctement ecrit. \n");
            else
                printf("Erreur lors de l\'ecriture du fichier log. \n");

            printf("\n FILTRAGE : \n");
            if(Ecrire_log("\n FILTRAGE :\n",nomFichierLog))
                printf("\n Fichier Log correctement ecrit. \n");
            else
                printf("Erreur lors de l\'ecriture du fichier log. \n");

            filtrer(jeu,&csp);
            infos_CSP(jeu,csp);

            if(Ecrire_log_infos_CSP(jeu,csp,nomFichierLog))
                printf("\n Fichier Log correctement ecrit. \n");
            else
                printf("Erreur lors de l\'ecriture du fichier log. \n");

            if(Ecrire_log("\n LANCEMENT DE BACKTRACK :\n",nomFichierLog))
                printf("\n Fichier Log correctement ecrit. \n");
            else
                printf("Erreur lors de l\'ecriture du fichier log. \n");

			//Mettez UNE des deux lignes suivantes en commentaire selon si vous voulez utiliser backtrack ou forxard checking. Le dernier parametre correspond au mode verbose(ecriture detaillé de l'algo dans les log, attention les fichier log peuvent grossir significativement si activé) , 0 = desactivé, 1 = activé . 

            //printf("\n SOLUTION = %d\n",backtrack(jeu, &csp,0)); //utilisation de backtrack,
            printf("\n SOLUTION : %d\n",forward_checking(jeu, &csp,0)); // utilisation de forward checking

            if(Ecrire_log("\n FIN DE BACKTRACK :\n",nomFichierLog))
                printf("\n Fichier Log correctement ecrit. \n");
            else
                printf("Erreur lors de l\'ecriture du fichier log. \n");

            infos_CSP(jeu,csp);
            if(Ecrire_log_infos_CSP(jeu,csp,nomFichierLog))
                printf("\n Fichier Log correctement ecrit. \n");
            else
                printf("Erreur lors de l\'ecriture du fichier log. \n");

            initGraphique(jeu);

            destroy_CSP(jeu,&csp);

            destroy_Grille(&jeu);
        break;
    }


    return EXIT_SUCCESS;
}
