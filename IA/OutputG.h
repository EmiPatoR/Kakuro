#ifndef OUTPUTG_H
#define OUTPUTG_H

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "structures.h"
#include "csp2.h"

typedef enum couleur {BLANC,NOIR,ROUGE,VERT} COULEUR;
typedef struct surfPos
{
    SDL_Rect* pos;
    SDL_Surface** surface;
    COULEUR *bgColor;
} surfacePos;

int initGraphique(GrilleKakuro grille);
int mainLoop(SDL_Surface* screen,GrilleKakuro grille);
int DessinGrille(SDL_Surface* screen,GrilleKakuro grille, surfacePos Cases[2],int focus);
int gestionClic(int X, int Y, GrilleKakuro grille, SDL_Surface* screen,SDL_Rect* position);
void gestionClavier(GrilleKakuro grille, int *focus,int *focusPrecedent,int *appui, SDLKey key);
Uint32 grossirCase(Uint32 interval, void* param);
void dessinerCadre(SDL_Surface* Case, Uint32 couleur);
void Ecrire(char* chaine, int taille,Uint32 couleurPolice, Uint32 couleurFond,SDL_PixelFormat *format, SDL_Surface** texte);
void EcrireTitre(char* chaine, int taille,Uint32 couleurPolice, Uint32 couleurFond,SDL_PixelFormat *format, SDL_Surface* screen);
int init_SurfacesCases(GrilleKakuro grille, surfacePos Cases[2],SDL_Surface* screen);
void dessinerCurseur(GrilleKakuro grille,int numCase, SDL_Surface* Case, Uint32 couleur);
int AnimCurseur(GrilleKakuro grille,surfacePos* Cases, int focus, int *focusPrecedent, SDL_Surface* screen);
void BlitGrille(surfacePos* Cases,GrilleKakuro grille ,SDL_Surface* screen);
int coloreGrille(GrilleKakuro grille, surfacePos* Cases,SDL_Surface* screen);
int backtrack_Graphique(GrilleKakuro jeu, CSP* csp,int log,surfacePos *Cases,int *focus, SDL_Surface *screen,int vitesse);
#endif
