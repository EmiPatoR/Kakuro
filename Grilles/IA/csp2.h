#ifndef CSP2_H_INCLUDED
#define CSP2_H_INCLUDED

#include "structures.h"

typedef char domaines;

typedef struct _var
{
    caseBlanche* cb;
    domaines* d;
} variable;

typedef struct _vars
{
    variable v;
    struct _vars* suivant;
} variables;
typedef variables* listeVariables;
typedef variables* Affectation;
typedef variables* iteratorVar;

void listeVar_push_back(listeVariables* liste, variable v);
void listeVar_push_front(listeVariables* liste, variable v);
variable DepilerVar(listeVariables* liste);
int sizeVar(listeVariables liste);
int Liste_existeVal(listeVariables liste,variable v);

typedef struct _CSP
{
    listeVariables V;
    domaines** D;
    Affectation A;
} CSP;

void init_CSP(GrilleKakuro jeu, CSP* csp);
void infos_CSP(GrilleKakuro jeu, CSP csp);
int Ecrire_log_infos_CSP(GrilleKakuro jeu, CSP csp,char* nomDuFichier);
int filtrer(GrilleKakuro jeu, CSP *csp);
int backtrack(GrilleKakuro jeu, CSP* csp,int log);
int forward_checking(GrilleKakuro jeu, CSP* csp, int log);
int Consistant(GrilleKakuro jeu,Affectation A, variable x, int v);
void destroy_CSP(GrilleKakuro jeu,CSP* csp);

#endif // CSP2_H_INCLUDED
