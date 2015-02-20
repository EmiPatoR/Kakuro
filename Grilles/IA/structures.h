#ifndef STRUCTURES_H
#define STRUCTURES_H

extern char nomFichierLog[300];
extern char repAbsolu[300];

typedef struct elem
{
    int val;
    struct elem* dernier;
    struct elem* precedent;
    struct elem* suivant;
} element;
typedef element* liste;
typedef element* iterator;
typedef element* pile;

void Liste_push_front(pile* Pile, int val);
void Liste_push_back(pile* Pile, int val);
int size(pile Pile);
int Depiler(pile* Pile);
int Liste_suprimer(pile *Pile,int valeur);
int Liste_inserer(pile* Pile,int valeur, int position);
int Liste_existe(pile Pile,int v);
int Liste_compare(pile Pile,pile Pile2);

typedef struct Position
{
    int X;
    int Y;
} position;

typedef struct caseBlanche
{
    position pos;
    int id;
    pile cases_parents;
    pile case_parent_sup;
    pile case_parent_inf;
    char valeur;
} caseBlanche;
void setVal_cb(caseBlanche* Case,char val);

typedef enum val { none, sup_value, inf_value, both} VAL;

typedef struct caseNoire
{
    position pos;
    int id;
    char val_sup;
    char val_inf;
    char nb_cb_valSup;
    char nb_cb_valInf;
    int sommeSup;
    int sommeInf;
    caseBlanche** mot_sup; // tableau de pointeurs sur les cases blanches qui forment le mot superieur
    caseBlanche** mot_inf; //tableau de pointeurs sur les cases blanches qui forment le mot inferieur
} caseNoire;
VAL caseNoire_nbVal(caseNoire Case);


typedef struct elemCn
{
    caseNoire val;
    struct elemCn* dernier;
    struct elemCn* precedent;
    struct elemCn* suivant;
} elementCn;

typedef struct elemCb
{
    caseBlanche* val;
    struct elemCb* dernier;
    struct elemCb* precedent;
    struct elemCb* suivant;
} elementCb;

typedef struct elemPos
{
    position val;
    struct elemPos* dernier;
    struct elemPos* precedent;
    struct elemPos* suivant;
} elementPos;


typedef elementCn* listeCn;
typedef elementCn* iteratorCn;
typedef elementCn* pileCn;
typedef elementCb* listeCb;
typedef elementCb* iteratorCb;
typedef elementCb* pileCb;
typedef elementPos* listePos;
typedef elementPos* iteratorPos;
typedef elementPos* pilePos;

void ListePositions_push_front(pilePos* Pile, position val);
void ListePositions_push_back(pilePos* Pile, position val);
int sizePos(pilePos Pile);
position DepilerPos(pilePos* Pile);
void ListeCn_push_front(pileCn* Pile, caseNoire val);
void ListeCn_push_back(pileCn* Pile, caseNoire val);
int sizeCn(pileCn Pile);
caseNoire DepilerCn(pileCn* Pile);
void ListeCb_push_front(pileCb* Pile, caseBlanche* val);
void ListeCb_push_back(pileCb* Pile, caseBlanche* val);
int sizeCb(pileCb Pile);
caseBlanche* DepilerCb(pileCb *Pile);

int Ecrire_log(const char* phrase, char* nomDuFichier);

typedef struct Grille
{
    int nbMots;
    int nbCasesNoires;
    int nbCasesBlanches;
    position max;
    position maxCn;
    position maxCb;
    caseBlanche *cb;
    caseNoire *cn;
} GrilleKakuro;
int init_Grille(GrilleKakuro* grille,int nbCasesBlanches, int nbCasesNoires,pilePos caseBlanches,pileCn caseNoires,position max);
void afficherInfos_Grille(GrilleKakuro jeu);
int Ecrire_log_Infos_Grille(GrilleKakuro jeu,char* nomDuFichier);
void calculerSommes(GrilleKakuro* jeu);
void destroy_Grille(GrilleKakuro* newGrille);
#endif
