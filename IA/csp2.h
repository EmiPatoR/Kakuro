#ifndef CSP2_H_INCLUDED
#define CSP2_H_INCLUDED

#include "structures.h"

typedef char domaines;

//definition d'un objet de type "variable"
typedef struct _var
{
    caseBlanche* cb; //Adresse de la case blanche referant a cette variable
    domaines* d; //domaine de la variable
} variable;

//definition de la structure d'une liste chainée d'objet de type "variable"
typedef struct _vars
{
    variable v;
    struct _vars* suivant;
} variables;
// autres synonymes 
typedef variables* listeVariables;
typedef variables* Affectation;
typedef variables* iteratorVar;

//prototypes de fonctions liées à la liste chainée
void listeVar_push_back(listeVariables* liste, variable v);
void listeVar_push_front(listeVariables* liste, variable v);
variable DepilerVar(listeVariables* liste);
int sizeVar(listeVariables liste);
int Liste_existeVal(listeVariables liste,variable v);

//definition de la stucture d'un CSP
typedef struct _CSP
{
    listeVariables V; // Liste chainée de type "variable" représentant la liste des variables non affectées.
    domaines** D; // Tableau des domaines de chaque variable
    Affectation A; // Liste chainée de type "variable" représentant la liste des affectations actuelle.
} CSP;

//prototypes des autres fonctions
void init_CSP(GrilleKakuro jeu, CSP* csp);
void infos_CSP(GrilleKakuro jeu, CSP csp);
int Ecrire_log_infos_CSP(GrilleKakuro jeu, CSP csp,char* nomDuFichier);
int filtrer(GrilleKakuro jeu, CSP *csp);
int backtrack(GrilleKakuro jeu, CSP* csp,int log);
int forward_checking(GrilleKakuro jeu, CSP* csp, int log);
int Consistant(GrilleKakuro jeu,Affectation A, variable x, char v);
int domaineVide(domaines* d);
int tailleDomaine(domaines *d);
void destroy_CSP(GrilleKakuro jeu,CSP* csp);

#endif // CSP2_H_INCLUDED
