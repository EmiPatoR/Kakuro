#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "OutputG.h"

#define TAILLE_POLICE 8

#define NOIRES 0
#define BLANCHES 1

#define LARGEUR_FEN 640
#define HAUTEUR_FEN 480

#define LARGEUR_CASE 30
#define HAUTEUR_CASE 30

int initGraphique(GrilleKakuro grille)
{
    SDL_Surface* screen;

    Uint32 BgColor;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) == -1)
    {
        fprintf(stderr, "Erreur d'initialisation de la SDL");
        exit(EXIT_FAILURE);
    }

    if(TTF_Init() == -1)
    {
        fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    screen = SDL_SetVideoMode(LARGEUR_FEN, HAUTEUR_FEN, 32, SDL_HWSURFACE | SDL_DOUBLEBUF); // On tente d'ouvrir une fenêtre
    if (screen == NULL) // Si l'ouverture a échoué, on écrit l'erreur et on arrête
    {
        fprintf(stderr, "Impossible de charger le mode vidéo : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    BgColor = SDL_MapRGB(screen->format,150,150,150);
    SDL_FillRect(screen, NULL, BgColor);

    SDL_WM_SetCaption("Graphic mode", NULL);
    SDL_Flip(screen);
    mainLoop(screen,grille);

    TTF_Quit();

    SDL_Quit();

    return EXIT_SUCCESS;
}

int mainLoop(SDL_Surface* screen, GrilleKakuro grille)
{

    SDL_Event event;


    CSP csp;
    init_CSP(grille,&csp);
    filtrer(grille,&csp);

    int i;
    int focus = -1, focusPrecedent = -1;
    int appui = 0;

    surfacePos Cases[2];

    init_SurfacesCases(grille,Cases,screen);

    EcrireTitre("Kakuro", 60 ,SDL_MapRGB(screen->format,0,0,0), SDL_MapRGB(screen->format,150,150,150),screen->format,screen);

    int fin = 0;

    calculerSommes(&grille);

    //SDL_EnableKeyRepeat(500,500);

   /* for(i=0;i<grille.nbCasesBlanches;i++)
    {
        switch(Cases[BLANCHES].bgColor[i])
        {
            case BLANC:
                printf("case[%d] = BLANC \n",i);
                break;

            case VERT:
                printf("case[%d] = VERT \n",i);
                break;

            default:
                printf("case[%d] = AUTRE \n",i);
                break;
        }
    }
    */

    coloreGrille(grille,Cases,screen);


    //backtrack_Graphique(grille, &csp,1,Cases,&focus,screen,0);

    while (!fin)
    {
        SDL_PollEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            fin = 1;
            printf("\nFenetre fermee, fin du mode graphique. \n\n");
            break;

        case SDL_MOUSEBUTTONUP: /* Clic de la souris */
            focus = gestionClic(event.button.x, event.button.y,grille,screen,Cases[BLANCHES].pos);
            break;

        case SDL_KEYDOWN:
            if(appui == 0)
            {
                gestionClavier(grille,&focus,&focusPrecedent,&appui,event.key.keysym.sym);
                coloreGrille(grille,Cases,screen);

            }
            break;

        case SDL_KEYUP:
            appui = 0;
            break;

        case SDL_MOUSEMOTION:

            break;
        }

        DessinGrille(screen,grille,Cases,focus); //Dessin de la grille
        AnimCurseur(grille,Cases,focus,&focusPrecedent,screen); // Dessin du curseur sur la grille
        BlitGrille(Cases,grille,screen); // Blit de la grille

        SDL_Flip(screen); //Actualisation de l'ecran

    }

    for(i=0; i<grille.nbCasesNoires; i++)
        SDL_FreeSurface(Cases[NOIRES].surface[i]);

    for(i=0; i<grille.nbCasesBlanches; i++)
        SDL_FreeSurface(Cases[BLANCHES].surface[i]);

    free(Cases[NOIRES].surface);
    free(Cases[BLANCHES].surface);
    free(Cases[NOIRES].pos);
    free(Cases[BLANCHES].pos);
    free(Cases[NOIRES].bgColor);
    free(Cases[BLANCHES].bgColor);
    return 1;
}

int DessinGrille(SDL_Surface* screen,GrilleKakuro grille, surfacePos Cases[2],int focus)
{
    int i = 0;
    char nombre[3];
    SDL_Surface *valeur = NULL;
    SDL_Rect posMasque;
    SDL_Rect posValeur;


    Uint32 couleurNoir = SDL_MapRGB(screen->format,0,0,0);
    Uint32 couleurBlanc = SDL_MapRGB(screen->format,255,255,255);
    Uint32 couleurRouge = SDL_MapRGB(screen->format,255,0,0);
    Uint32 couleurVert = SDL_MapRGB(screen->format,205,255,205);
    Uint32 couleurRouge2 = SDL_MapRGB(screen->format,255,205,205);


    for(i=0; i<grille.nbCasesBlanches; i++)
    {
        //fprintf(stderr,"val =%d \n",grille.cb[i].valeur);

        if(grille.cb[i].valeur != 0)
        {
            sprintf(nombre,"%d",grille.cb[i].valeur);
            if(Cases[BLANCHES].bgColor[i] == BLANC)
                Ecrire(nombre,10,couleurNoir,couleurBlanc,screen->format,&valeur);
            if(Cases[BLANCHES].bgColor[i] == VERT)
                Ecrire(nombre,10,couleurNoir,couleurVert,screen->format,&valeur);
            if(Cases[BLANCHES].bgColor[i] == ROUGE)
                Ecrire(nombre,10,couleurNoir,couleurRouge2,screen->format,&valeur);
            posValeur.x = (LARGEUR_CASE / 2) - (valeur->w/2);
            posValeur.y = (HAUTEUR_CASE / 2) - (valeur->h/2);
            SDL_BlitSurface(valeur,NULL,Cases[BLANCHES].surface[i],&posValeur);
            SDL_FreeSurface(valeur);

            posMasque.x = 0;
            posMasque.y = 0;
            posMasque.w = posValeur.x;
            posMasque.h = HAUTEUR_CASE;

            if(Cases[BLANCHES].bgColor[i] == BLANC)
                SDL_FillRect(Cases[BLANCHES].surface[i], &posMasque, couleurBlanc);
            if(Cases[BLANCHES].bgColor[i] == VERT)
                SDL_FillRect(Cases[BLANCHES].surface[i], &posMasque, couleurVert);
            if(Cases[BLANCHES].bgColor[i] == ROUGE)
                SDL_FillRect(Cases[BLANCHES].surface[i], &posMasque, couleurRouge2);

        }
        else
        {
            posMasque.x = (LARGEUR_CASE/2) - 3;
            posMasque.y = 0;
            posMasque.w = LARGEUR_CASE - ((LARGEUR_CASE/2) - 3);
            posMasque.h = HAUTEUR_CASE;

            if(Cases[BLANCHES].bgColor[i] == BLANC)
                SDL_FillRect(Cases[BLANCHES].surface[i], &posMasque, couleurBlanc);
            if(Cases[BLANCHES].bgColor[i] == VERT)
                SDL_FillRect(Cases[BLANCHES].surface[i], &posMasque, couleurVert);
            if(Cases[BLANCHES].bgColor[i] == ROUGE)
                SDL_FillRect(Cases[BLANCHES].surface[i], &posMasque, couleurRouge2);
        }


        if(i == focus)
            dessinerCadre(Cases[BLANCHES].surface[i], couleurRouge);
        else
            dessinerCadre(Cases[BLANCHES].surface[i], couleurNoir);

    }
    return 1;
}

void dessinerCurseur(GrilleKakuro grille,int numCase, SDL_Surface* Case, Uint32 couleur)
{
    SDL_Surface* barre = {NULL};
    barre = SDL_CreateRGBSurface(SDL_HWSURFACE, 1, HAUTEUR_CASE - 16 , 32, 0, 0, 0, 0);

    SDL_Rect posBarre;

    if(grille.cb[numCase].valeur == 0)
        posBarre.x = (LARGEUR_CASE/2) - 4;
    else
        posBarre.x = (LARGEUR_CASE/2) + 4;

    posBarre.y = (HAUTEUR_CASE/2) - (barre->h/2);

    SDL_FillRect(barre, NULL, couleur);

    SDL_BlitSurface(barre,NULL,Case,&posBarre);
    SDL_FreeSurface(barre);

}

void dessinerCadre(SDL_Surface* Case, Uint32 couleur)
{
    int j = 0;
    SDL_Surface* cotes[2] = {NULL};
    cotes[0] = SDL_CreateRGBSurface(SDL_HWSURFACE, LARGEUR_CASE, 1, 32, 0, 0, 0, 0);
    cotes[1] = SDL_CreateRGBSurface(SDL_HWSURFACE, 1, HAUTEUR_CASE, 32, 0, 0, 0, 0);

    SDL_FillRect(cotes[0], NULL, couleur); // Remplissage
    SDL_FillRect(cotes[1], NULL, couleur); // Remplissage

    SDL_Rect posCote[3];
    posCote[0].x = 0;
    posCote[0].y = 0;
    posCote[1].x = LARGEUR_CASE - 1;
    posCote[1].y = 0;
    posCote[2].x = 0;
    posCote[2].y = HAUTEUR_CASE - 1;

    for(j=0; j<4; j++)
    {
        switch(j)
        {
        case 0:
            SDL_BlitSurface(cotes[0],NULL,Case,&posCote[0]);
            break;

        case 1:
            SDL_BlitSurface(cotes[1],NULL,Case,&posCote[1]);
            break;

        case 2:
            SDL_BlitSurface(cotes[0],NULL,Case,&posCote[2]);
            break;

        case 3:
            SDL_BlitSurface(cotes[1],NULL,Case,&posCote[0]);
            break;
        }
    }
    SDL_FreeSurface(cotes[0]);
    SDL_FreeSurface(cotes[1]);
}

/*

SDL_TimerID gestionClic(int X, int Y, GrilleKakuro grille,surfacePos *s)
{
    int i = 0;
    static SDL_TimerID timer = NULL;

    SDL_Rect positionSouris;
        positionSouris.x = X;
        positionSouris.y = Y;

    SDL_Rect posInit;
        posInit.x = (LARGEUR_FEN / 2) - ((grille.max.X * LARGEUR_CASE)/2);
        posInit.y = (HAUTEUR_FEN / 2) - ((grille.max.Y * HAUTEUR_CASE)/2);


    for(i=0;i<grille.nbCasesBlanches;i++)
    {
        if((positionSouris.x >= ((grille.cb[i].pos.X)*LARGEUR_CASE)) && (positionSouris.x <= ((grille.cb[i].pos.X + 1)*LARGEUR_CASE)))
        {
            if((positionSouris.y >= ((grille.cb[i].pos.Y)*HAUTEUR_CASE)) && (positionSouris.y <= ((grille.cb[i].pos.Y + 1)*HAUTEUR_CASE)))
            {
                if(timer == NULL)
                   {
                        s->pos.x = (grille.cb[i].pos.X * LARGEUR_CASE) + posInit.x;
                        s->pos.y = (grille.cb[i].pos.Y * LARGEUR_CASE) + posInit.y;
                        timer = SDL_AddTimer(30, grossirCase, s);
                   }
            }
            else
            {
                if(timer != NULL)
                {
                    SDL_RemoveTimer(timer);
                    timer = NULL;
                }
            }
        }
        else
        {
            if(timer != NULL)
            {
                SDL_RemoveTimer(timer);
                timer = NULL;
            }
        }
    }
    return timer;

}

Uint32 grossirCase(Uint32 interval, void* param)
{
    static int dx = 0;
    static int dy = 0;
    surfacePos *s = param;
    free(s->surface);
    s->surface = SDL_CreateRGBSurface(SDL_HWSURFACE, LARGEUR_CASE + dx, HAUTEUR_CASE + dy, 32, 0, 0, 0, 0);
    s->pos.x -= dx/2;
    s->pos.y -= dy/2;
    dx += 2;
    dy += 2;
    SDL_BlitSurface(s->surface,NULL,s->screen,&(s->pos));
    SDL_Flip(s->screen);
    return interval;
}
*/

int gestionClic(int X, int Y, GrilleKakuro grille, SDL_Surface* screen,SDL_Rect* position)
{
    int i = 0;
    int focus = -1;

    for(i=0; i<grille.nbCasesBlanches; i++)
    {
        //fprintf(stderr,"Xmin = %d , X = %d Xmax = %d \n Ymin = %d, Y = %d, Ymax = %d \n\n",position[i].x,X,position[i].x + LARGEUR_CASE,position[i].y,Y,position[i].y + HAUTEUR_CASE);
        if((X >= position[i].x) && (X <= position[i].x + LARGEUR_CASE) && (Y >= position[i].y) && (Y <= position[i].y + HAUTEUR_CASE))
        {
            focus = i;
            break;
        }
        else
            focus = -1;
    }

    return focus;

}

void gestionClavier(GrilleKakuro grille, int *focus,int *focusPrecedent,int *appui, SDLKey key)
{

    int i;
    int valeur = 0;
    *appui = 1;
    switch (key)
    {
    case SDLK_RETURN: /* Appui sur la touche Echap, on arrête le programme */
        *focus = -1;
        break;

    case SDLK_TAB:
        if(*focus != -1)
        {
            *focusPrecedent = *focus;
            if(*focus < grille.nbCasesBlanches - 1)
                *focus = *focus + 1;
            else
                *focus = 0;
        }
        break;

    case SDLK_ESCAPE: /* Appui sur la touche Echap, on arrête le programme */
        *focus = -1;
        break;

    case SDLK_BACKSPACE:
        if(*focus != -1)
            setVal_cb(&(grille.cb[*focus]),0);
        break;

    case SDLK_DELETE:
        if(*focus != -1)
        {
            setVal_cb(&(grille.cb[*focus]),0);
        }
        else
        {
            for(i=0;i<grille.nbCasesBlanches;i++);
                setVal_cb(&(grille.cb[i]),0);
        }
        break;

    case SDLK_KP1:
        if(*focus != -1 && grille.cb[*focus].valeur == 0)
            setVal_cb(&(grille.cb[*focus]),1);
        break;

    case SDLK_KP2:
        if(*focus != -1 && grille.cb[*focus].valeur == 0)
            setVal_cb(&(grille.cb[*focus]),2);
        break;

    case SDLK_KP3:
        if(*focus != -1 && grille.cb[*focus].valeur == 0)
            setVal_cb(&(grille.cb[*focus]),3);
        break;

    case SDLK_KP4:
        if(*focus != -1 && grille.cb[*focus].valeur == 0)
            setVal_cb(&(grille.cb[*focus]),4);
        break;

    case SDLK_KP5:
        if(*focus != -1 && grille.cb[*focus].valeur == 0)
            setVal_cb(&(grille.cb[*focus]),5);
        break;

    case SDLK_KP6:
        if(*focus != -1 && grille.cb[*focus].valeur == 0)
            setVal_cb(&(grille.cb[*focus]),6);
        break;

    case SDLK_KP7:
        if(*focus != -1 && grille.cb[*focus].valeur == 0)
            setVal_cb(&(grille.cb[*focus]),7);
        break;

    case SDLK_KP8:
        if(*focus != -1 && grille.cb[*focus].valeur == 0)
            setVal_cb(&(grille.cb[*focus]),8);
        break;

    case SDLK_KP9:
        if(*focus != -1 && grille.cb[*focus].valeur == 0)
            setVal_cb(&(grille.cb[*focus]),9);
        break;

    case SDLK_LEFT:
        if(*focus != -1)
        {
            if(grille.cb[*focus -1].pos.Y == grille.cb[*focus].pos.Y && *focus > 0)
            {
                 *focusPrecedent = *focus;
                 *focus = *focus - 1 ;
            }
        }
        break;

    case SDLK_RIGHT:
        if(*focus != -1)
        {
            if(grille.cb[*focus +1].pos.Y == grille.cb[*focus].pos.Y && *focus < (grille.nbCasesBlanches - 1))
            {
                 *focusPrecedent = *focus;
                 *focus = *focus + 1 ;
            }
        }
        break;

    case SDLK_UP:
        if(*focus != -1)
        {
            for(i=0;i<(*focus);i++)
            {
                if(grille.cb[i].pos.X == grille.cb[*focus].pos.X && grille.cb[*focus].pos.Y > grille.cb[0].pos.Y)
                    valeur = i;
            }
            //printf("VALEUR = %d \n",valeur);
            if(valeur != 0)
            {
                *focusPrecedent = *focus;
                *focus = valeur;
            }

        }
        break;

    case SDLK_DOWN:
        if(*focus != -1)
        {
            for(i=(*focus)+1;i<grille.nbCasesBlanches;i++)
            {
                if(grille.cb[i].pos.X == grille.cb[*focus].pos.X && grille.cb[*focus].pos.Y < grille.cb[grille.nbCasesBlanches - 1].pos.Y)
                {
                    valeur = i;
                    break;
                }
            }
            //printf("VALEUR = %d \n",valeur);
            if(valeur != 0)
            {
                *focusPrecedent = *focus;
                *focus = valeur;
            }

        }
        break;

    default:
        calculerSommes(&grille);
        for(i=0; i<grille.nbCasesNoires; i++)
            printf("case noire %d : Somme sup = %d , somme inf = %d \n",i,grille.cn[i].sommeSup,grille.cn[i].sommeInf);
        break;

    }

    calculerSommes(&grille);

}

void Ecrire(char* chaine, int taille,Uint32 couleurPolice, Uint32 couleurFond,SDL_PixelFormat *format, SDL_Surface** texte)
{
    TTF_Font *police = NULL;
    Uint8 rouge, vert, bleu;
    SDL_GetRGB(couleurFond,format,&rouge,&vert,&bleu);
    SDL_Color coulFond = {rouge,vert,bleu};
    SDL_GetRGB(couleurPolice,format,&rouge,&vert,&bleu);
    SDL_Color coulTexte = {rouge,vert,bleu};
    char repertoirePolice[300] = "";
    strcat(repertoirePolice,repAbsolu);
    strcat(repertoirePolice,"Polices/police.ttf");
    if((police = TTF_OpenFont(repertoirePolice, taille)) == NULL)
        exit(1);
    *texte = TTF_RenderText_Shaded(police, chaine, coulTexte,coulFond);
    if(*texte == NULL)
        exit(2);
    TTF_CloseFont(police);
}

void EcrireTitre(char* chaine, int taille,Uint32 couleurPolice, Uint32 couleurFond,SDL_PixelFormat *format, SDL_Surface* screen)
{
    TTF_Font *police = NULL;
    SDL_Rect posTitre;
    SDL_Surface* texte = NULL;
    Uint8 rouge, vert, bleu;
    SDL_GetRGB(couleurFond,format,&rouge,&vert,&bleu);
    SDL_Color coulFond = {rouge,vert,bleu};
    SDL_GetRGB(couleurPolice,format,&rouge,&vert,&bleu);
    SDL_Color coulTexte = {rouge,vert,bleu};
    char repertoirePolice[300] = "";
    strcat(repertoirePolice,repAbsolu);
    strcat(repertoirePolice,"Polices/policeTitre.ttf");
    if((police = TTF_OpenFont(repertoirePolice, taille)) == NULL)
        exit(1);
    texte = TTF_RenderText_Shaded(police, chaine, coulTexte,coulFond);
    if(texte == NULL)
        exit(2);

    posTitre.x = (screen->w/2) - (texte->w/2);
    posTitre.y = 30;

    SDL_BlitSurface(texte,NULL,screen,&posTitre);
    SDL_FreeSurface(texte);
    TTF_CloseFont(police);
}


int init_SurfacesCases(GrilleKakuro grille, surfacePos Cases[2],SDL_Surface* screen)
{
    int i;
    SDL_Surface* mots = NULL;
    Uint32 couleurNoir = SDL_MapRGB(screen->format,0,0,0);
    Uint32 couleurBlanc = SDL_MapRGB(screen->format,255,255,255);
    char nombre[3];

    Cases[NOIRES].surface = (SDL_Surface**) malloc(sizeof(SDL_Surface*) * (grille.nbCasesNoires));
    Cases[NOIRES].pos = (SDL_Rect*) malloc(sizeof(SDL_Rect) * (grille.nbCasesNoires));
    Cases[NOIRES].bgColor = (COULEUR*) malloc(sizeof(COULEUR) * (grille.nbCasesNoires));
    Cases[BLANCHES].surface = (SDL_Surface**) malloc(sizeof(SDL_Surface*) * (grille.nbCasesBlanches));
    Cases[BLANCHES].pos = (SDL_Rect*) malloc(sizeof(SDL_Rect) * (grille.nbCasesBlanches));
    Cases[BLANCHES].bgColor = (COULEUR*) malloc(sizeof(COULEUR) * (grille.nbCasesBlanches));


    int largeur = LARGEUR_CASE * grille.max.X;
    int hauteur = HAUTEUR_CASE * grille.max.Y;

    if(largeur > LARGEUR_FEN)
        return 0;
    if(hauteur > HAUTEUR_FEN)
        return 0;

    SDL_Rect posMot;
    SDL_Rect posInit;
    posInit.x = LARGEUR_FEN/2 - largeur/2;
    posInit.y = HAUTEUR_FEN/2 - hauteur/2;

    for(i=0; i<grille.nbCasesNoires; i++)
    {
        Cases[NOIRES].surface[i] = SDL_CreateRGBSurface(SDL_HWSURFACE, LARGEUR_CASE, HAUTEUR_CASE, 32, 0, 0, 0, 0);
        SDL_FillRect(Cases[NOIRES].surface[i], NULL, couleurNoir); // Remplissage
        Cases[NOIRES].bgColor[i] = NOIR;
        Cases[NOIRES].pos[i].x = (grille.cn[i].pos.X * LARGEUR_CASE) + posInit.x;
        Cases[NOIRES].pos[i].y = (grille.cn[i].pos.Y * HAUTEUR_CASE) + posInit.y;

        switch(caseNoire_nbVal(grille.cn[i]))
        {
        case sup_value:
            //nombre[0] = (grille.cn[i].val_sup / 10) + '0';
            //nombre[1] = (grille.cn[i].val_sup % 10) + '0';
            sprintf(nombre,"%d",grille.cn[i].val_sup);
            Ecrire(nombre,TAILLE_POLICE,couleurBlanc,couleurNoir,screen->format,&mots);
            posMot.x = LARGEUR_CASE - (mots->w + 2);
            posMot.y = HAUTEUR_CASE / 7;
            SDL_BlitSurface(mots,NULL,Cases[NOIRES].surface[i],&posMot);
            break;

        case inf_value:
            //nombre[0] = (char)(grille.cn[i].val_inf / 10) + '0';
            //nombre[1] = (char)(grille.cn[i].val_inf % 10) + '0';
            sprintf(nombre,"%d",grille.cn[i].val_inf);
            Ecrire(nombre,TAILLE_POLICE,couleurBlanc,couleurNoir,screen->format,&mots);
            posMot.x = LARGEUR_CASE / 7;
            posMot.y = HAUTEUR_CASE - (mots->h + 2);
            SDL_BlitSurface(mots,NULL,Cases[NOIRES].surface[i],&posMot);
            break;

        case both:
            //nombre[0] = (grille.cn[i].val_sup / 10) + '0';
            //nombre[1] = (grille.cn[i].val_sup % 10) + '0';
            sprintf(nombre,"%d",grille.cn[i].val_sup);
            Ecrire(nombre,TAILLE_POLICE,couleurBlanc,couleurNoir,screen->format,&mots);
            posMot.x = LARGEUR_CASE - (mots->w + 2);
            posMot.y = HAUTEUR_CASE / 7;
            SDL_BlitSurface(mots,NULL,Cases[NOIRES].surface[i],&posMot);


            // nombre[0] = (grille.cn[i].val_inf / 10) + '0';
            // nombre[1] = (grille.cn[i].val_inf % 10) + '0';
            sprintf(nombre,"%d",grille.cn[i].val_inf);
            Ecrire(nombre,TAILLE_POLICE,couleurBlanc,couleurNoir,screen->format,&mots);
            posMot.x = LARGEUR_CASE / 7;
            posMot.y = HAUTEUR_CASE - (mots->h + 2);
            SDL_BlitSurface(mots,NULL,Cases[NOIRES].surface[i],&posMot);
            break;

        case none:

            break;
        }

    }


    for(i=0; i<grille.nbCasesBlanches; i++)
    {
        Cases[BLANCHES].surface[i] = SDL_CreateRGBSurface(SDL_HWSURFACE, LARGEUR_CASE, HAUTEUR_CASE, 32, 0, 0, 0, 0);
        SDL_FillRect(Cases[BLANCHES].surface[i], NULL, couleurBlanc); // Remplissage
        Cases[BLANCHES].bgColor[i] = BLANC;
        Cases[BLANCHES].pos[i].x = (grille.cb[i].pos.X * LARGEUR_CASE) + posInit.x;
        Cases[BLANCHES].pos[i].y = (grille.cb[i].pos.Y * HAUTEUR_CASE) + posInit.y;
    }

    SDL_FreeSurface(mots);
    return 0;
}

int AnimCurseur(GrilleKakuro grille,surfacePos* Cases, int focus, int *focusPrecedent, SDL_Surface* screen)
{
    Uint32 couleurNoir = SDL_MapRGB(screen->format,0,0,0);
    Uint32 couleurBlanc = SDL_MapRGB(screen->format,255,255,255);
   // Uint32 couleurRouge = SDL_MapRGB(screen->format,255,0,0);
    Uint32 couleurRouge2 = SDL_MapRGB(screen->format,255,205,205);
    Uint32 couleurVert = SDL_MapRGB(screen->format,205,255,205);

    if(focus == -1)
    {
        if(*focusPrecedent == -1)
            return 0;
        else
        {
            if(Cases[BLANCHES].bgColor[*focusPrecedent] == BLANC)
                dessinerCurseur(grille,*focusPrecedent,Cases[BLANCHES].surface[*focusPrecedent],couleurBlanc);
            if(Cases[BLANCHES].bgColor[*focusPrecedent] == VERT)
                dessinerCurseur(grille,*focusPrecedent,Cases[BLANCHES].surface[*focusPrecedent],couleurVert);
            if(Cases[BLANCHES].bgColor[*focusPrecedent] == ROUGE)
                dessinerCurseur(grille,*focusPrecedent,Cases[BLANCHES].surface[*focusPrecedent],couleurRouge2);
        }

    }
    static int tempsPrecedent = 0,tempsActuel = 0;
    static int visible = 1;
    tempsActuel = SDL_GetTicks();
    if (tempsActuel - tempsPrecedent > 500)
    {

        if(focus != -1)
        {
            if(visible == 1)
            {
                dessinerCurseur(grille,focus,Cases[BLANCHES].surface[focus],couleurNoir);
                visible = 0;
            }
            else
            {
                if(*focusPrecedent != -1)
                {
                    if(Cases[BLANCHES].bgColor[*focusPrecedent] == BLANC)
                        dessinerCurseur(grille,*focusPrecedent,Cases[BLANCHES].surface[*focusPrecedent],couleurBlanc);
                    if(Cases[BLANCHES].bgColor[*focusPrecedent] == VERT)
                        dessinerCurseur(grille,*focusPrecedent,Cases[BLANCHES].surface[*focusPrecedent],couleurVert);
                    if(Cases[BLANCHES].bgColor[*focusPrecedent] == ROUGE)
                        dessinerCurseur(grille,*focusPrecedent,Cases[BLANCHES].surface[*focusPrecedent],couleurRouge2);
                }
                visible = 1;
            }


            //fprintf(stderr,"focus : %d , focusP : %d \n",focus,focusPrecedent);
            *focusPrecedent = focus;

        }

        tempsPrecedent = tempsActuel;
    }
    else
    {

    }
    return 1;
}


void BlitGrille(surfacePos* Cases,GrilleKakuro grille ,SDL_Surface* screen)
{
    int i;

    for(i=0; i<grille.nbCasesNoires; i++)
        SDL_BlitSurface(Cases[NOIRES].surface[i],NULL,screen,&(Cases[NOIRES].pos[i])); //Collage

    for(i=0; i<grille.nbCasesBlanches; i++)
        SDL_BlitSurface(Cases[BLANCHES].surface[i],NULL,screen,&(Cases[BLANCHES].pos[i]));
}

int coloreGrille(GrilleKakuro grille, surfacePos* Cases,SDL_Surface* screen)
{
    Uint32 couleurBlanc = SDL_MapRGB(screen->format,255,255,255);
    Uint32 couleurVert = SDL_MapRGB(screen->format,205,255,205);
    Uint32 couleurRouge = SDL_MapRGB(screen->format,255,205,205);
    int i,j,k;
    pile parents = NULL;
    int occurence = 0;
    int ok_sup = 1, ok_inf = 1;

    for(i=0;i<grille.nbCasesNoires;i++)
    {
        switch(caseNoire_nbVal(grille.cn[i]))
        {
            case none:

                break;

            case sup_value:

                for(j=0;j<grille.cn[i].nb_cb_valSup;j++)
                {
                    if(grille.cn[i].mot_sup[j]->valeur == 0)
                        ok_sup = 0;
                }

                if(ok_sup == 1)
                {
                    for(j=0;j<grille.cn[i].nb_cb_valSup - 1;j++)
                    {
                        for(k=j+1;k<grille.cn[i].nb_cb_valSup;k++)
                        {
                            if(grille.cn[i].mot_sup[j]->valeur == grille.cn[i].mot_sup[k]->valeur)
                                occurence = 1;
                        }
                    }
                }


                if(grille.cn[i].sommeSup == grille.cn[i].val_sup && ok_sup == 1 && occurence == 0)
                {
                    for(j=0;j<grille.cn[i].nb_cb_valSup;j++)
                    {
                       // if(!Liste_existe(parents,grille.cn[i].mot_sup[j]->id))
                        {
                            Cases[BLANCHES].bgColor[grille.cn[i].mot_sup[j]->id] = VERT;
                           // Liste_push_back(&parents,grille.cn[i].mot_sup[j]->id);
                        }
                    }
                }
                else if(ok_sup == 0)
                {
                    for(j=0;j<grille.cn[i].nb_cb_valSup;j++)
                    {
                        //if(!Liste_existe(parents,grille.cn[i].mot_sup[j]->id))
                        {
                            Cases[BLANCHES].bgColor[grille.cn[i].mot_sup[j]->id] = BLANC;
                            //Liste_push_back(&parents,grille.cn[i].mot_sup[j]->id);
                        }
                    }
                }
                else if( (grille.cn[i].sommeSup != grille.cn[i].val_sup && ok_sup == 1) || occurence == 1)
                {
                    for(j=0;j<grille.cn[i].nb_cb_valSup;j++)
                    {
                       // if(!Liste_existe(parents,grille.cn[i].mot_sup[j]->id))
                        {
                            Cases[BLANCHES].bgColor[grille.cn[i].mot_sup[j]->id] = ROUGE;
                         //   Liste_push_back(&parents,grille.cn[i].mot_sup[j]->id);
                        }
                    }
                }

                ok_sup = 1;
                occurence = 0;

                break;

                case inf_value:

                    for(j=0;j<grille.cn[i].nb_cb_valInf;j++)
                    {
                        if(grille.cn[i].mot_inf[j]->valeur == 0)
                            ok_inf = 0;
                    }

                    if(ok_inf == 1)
                    {
                        for(j=0;j<grille.cn[i].nb_cb_valInf - 1;j++)
                        {
                            for(k=j+1;k<grille.cn[i].nb_cb_valInf;k++)
                            {
                                if(grille.cn[i].mot_inf[j]->valeur == grille.cn[i].mot_inf[k]->valeur)
                                    occurence = 1;
                            }
                        }
                    }

                    if(grille.cn[i].sommeInf == grille.cn[i].val_inf && ok_inf == 1  && occurence == 0)
                    {
                        for(j=0;j<grille.cn[i].nb_cb_valInf;j++)
                        {
                            //if(!Liste_existe(parents,grille.cn[i].mot_inf[j]->id))
                            {
                                Cases[BLANCHES].bgColor[grille.cn[i].mot_inf[j]->id] = VERT;
                            //    Liste_push_back(&parents,grille.cn[i].mot_inf[j]->id);
                            }
                        }
                    }
                    else if(ok_inf == 0)
                    {
                        for(j=0;j<grille.cn[i].nb_cb_valInf;j++)
                        {
                            //if(!Liste_existe(parents,grille.cn[i].mot_inf[j]->id))
                            {
                                Cases[BLANCHES].bgColor[grille.cn[i].mot_inf[j]->id] = BLANC;
                                //Liste_push_back(&parents,grille.cn[i].mot_inf[j]->id);
                            }
                        }
                    }
                    else if( (grille.cn[i].sommeInf != grille.cn[i].val_inf && ok_inf == 1) || occurence == 1)
                    {
                        for(j=0;j<grille.cn[i].nb_cb_valInf;j++)
                        {
                           // if(!Liste_existe(parents,grille.cn[i].mot_inf[j]->id))
                            {
                                Cases[BLANCHES].bgColor[grille.cn[i].mot_inf[j]->id] = ROUGE;
                             //   Liste_push_back(&parents,grille.cn[i].mot_inf[j]->id);
                            }
                        }
                    }
                    ok_inf = 1;
                    occurence = 0;
                    break;

                case both:

                    for(j=0;j<grille.cn[i].nb_cb_valInf;j++)
                    {
                        if(grille.cn[i].mot_inf[j]->valeur == 0)
                            ok_inf = 0;
                    }

                    if(ok_inf == 1)
                    {
                        for(j=0;j<grille.cn[i].nb_cb_valInf - 1;j++)
                        {
                            for(k=j+1;k<grille.cn[i].nb_cb_valInf;k++)
                            {
                                if(grille.cn[i].mot_inf[j]->valeur == grille.cn[i].mot_inf[k]->valeur)
                                    occurence = 1;
                            }
                        }
                    }

                    if(grille.cn[i].sommeInf == grille.cn[i].val_inf && ok_inf == 1  && occurence == 0)
                    {
                        for(j=0;j<grille.cn[i].nb_cb_valInf;j++)
                        {
                            //if(!Liste_existe(parents,grille.cn[i].mot_inf[j]->id))
                            {
                                Cases[BLANCHES].bgColor[grille.cn[i].mot_inf[j]->id] = VERT;
                              //  Liste_push_back(&parents,grille.cn[i].mot_inf[j]->id);
                            }
                        }
                    }
                    else if(ok_inf == 0)
                    {
                        for(j=0;j<grille.cn[i].nb_cb_valInf;j++)
                        {
                            //if(!Liste_existe(parents,grille.cn[i].mot_inf[j]->id))
                            {
                                Cases[BLANCHES].bgColor[grille.cn[i].mot_inf[j]->id] = BLANC;
                              //  Liste_push_back(&parents,grille.cn[i].mot_inf[j]->id);
                            }
                        }
                    }
                    else if( (grille.cn[i].sommeInf != grille.cn[i].val_inf && ok_inf == 1) || occurence == 1)
                    {
                        for(j=0;j<grille.cn[i].nb_cb_valInf;j++)
                        {
                            //if(!Liste_existe(parents,grille.cn[i].mot_inf[j]->id))
                            {
                                Cases[BLANCHES].bgColor[grille.cn[i].mot_inf[j]->id] = ROUGE;
                                //Liste_push_back(&parents,grille.cn[i].mot_inf[j]->id);
                            }
                        }
                    }


                occurence = 0;

                for(j=0;j<grille.cn[i].nb_cb_valSup;j++)
                {
                    if(grille.cn[i].mot_sup[j]->valeur == 0)
                        ok_sup = 0;
                }


                if(ok_sup == 1)
                {
                    for(j=0;j<grille.cn[i].nb_cb_valSup - 1;j++)
                    {
                        for(k=j+1;k<grille.cn[i].nb_cb_valSup;k++)
                        {
                            if(grille.cn[i].mot_sup[j]->valeur == grille.cn[i].mot_sup[k]->valeur)
                                occurence = 1;
                        }
                    }
                }



                if(grille.cn[i].sommeSup == grille.cn[i].val_sup  && occurence == 0)
                {
                    for(j=0;j<grille.cn[i].nb_cb_valSup;j++)
                    {
                        //if(!Liste_existe(parents,grille.cn[i].mot_sup[j]->id))
                        {
                            Cases[BLANCHES].bgColor[grille.cn[i].mot_sup[j]->id] = VERT;
                          //  Liste_push_back(&parents,grille.cn[i].mot_sup[j]->id);
                        }
                    }
                }
                else if(ok_sup == 0)
                {
                    for(j=0;j<grille.cn[i].nb_cb_valSup;j++)
                    {
                        //if(!Liste_existe(parents,grille.cn[i].mot_sup[j]->id))
                        {
                            Cases[BLANCHES].bgColor[grille.cn[i].mot_sup[j]->id] = BLANC;
                            //Liste_push_back(&parents,grille.cn[i].mot_sup[j]->id);
                        }
                    }
                }
                else if( (grille.cn[i].sommeSup != grille.cn[i].val_sup && ok_sup == 1) || occurence == 1 )
                {
                    for(j=0;j<grille.cn[i].nb_cb_valSup;j++)
                    {
                        //if(!Liste_existe(parents,grille.cn[i].mot_sup[j]->id))
                        {
                            Cases[BLANCHES].bgColor[grille.cn[i].mot_sup[j]->id] = ROUGE;
                            //Liste_push_back(&parents,grille.cn[i].mot_sup[j]->id);
                        }
                    }
                }

                ok_sup = 1;
                ok_inf = 1;
                occurence = 0;

                    break;
        }
    }

    for(i=0;i<grille.nbCasesBlanches;i++)
    {
        switch(Cases[BLANCHES].bgColor[i])
        {
            case VERT:
                SDL_FillRect(Cases[BLANCHES].surface[i], NULL, couleurVert);
                break;

            case BLANC:
                SDL_FillRect(Cases[BLANCHES].surface[i], NULL, couleurBlanc);
                break;

            case ROUGE:
                SDL_FillRect(Cases[BLANCHES].surface[i], NULL, couleurRouge);
                break;

            default:

                break;
        }
        Depiler(&parents);
    }



    /*
    for(i=0;i<grille.nbCasesBlanches;i++)
    {
        if(grille.cb[i].case_parent_sup != NULL && grille.cb[i].case_parent_inf == NULL)
        {
            if(!Liste_existe(parents,grille.cb[i].case_parent_sup->val))
            {
                for(j=0;j<grille.cn[grille.cb[i].case_parent_sup->val].nb_cb_valSup;j++)
                {
                    if(grille.cn[grille.cb[i].case_parent_sup->val].mot_sup[j]->valeur == 0)
                        ok_sup = 0;
                }

                if(ok_sup == 1)
                {
                   for(j=0;j<grille.cn[grille.cb[i].case_parent_sup->val].nb_cb_valSup;j++)
                    {
                        for(k=j+1;k<grille.cn[grille.cb[i].case_parent_sup->val].nb_cb_valSup;k++)
                        {
                            if(grille.cn[grille.cb[i].case_parent_sup->val].mot_sup[j]->valeur == grille.cn[grille.cb[i].case_parent_sup->val].mot_sup[k]->valeur)
                                occurence = 1;
                        }

                    }
                }


                if(grille.cn[grille.cb[i].case_parent_sup->val].sommeSup == grille.cn[grille.cb[i].case_parent_sup->val].val_sup && ok_sup == 1)
                {
                    for(j=0;j<grille.cn[grille.cb[i].case_parent_sup->val].nb_cb_valSup;j++)
                    {
                        SDL_FillRect(Cases[BLANCHES].surface[grille.cn[grille.cb[i].case_parent_sup->val].mot_sup[j]->id], NULL, couleurVert); // Remplissage
                        Cases[BLANCHES].bgColor[i] = VERT;
                    }
                }
               else if((grille.cn[grille.cb[i].case_parent_sup->val].sommeSup != grille.cn[grille.cb[i].case_parent_sup->val].val_sup && ok_sup == 1) || occurence == 1)
                {
                    for(j=0;j<grille.cn[grille.cb[i].case_parent_sup->val].nb_cb_valSup;j++)
                    {
                        SDL_FillRect(Cases[BLANCHES].surface[grille.cn[grille.cb[i].case_parent_sup->val].mot_sup[j]->id], NULL, couleurRouge); // Remplissage
                        Cases[BLANCHES].bgColor[i] = ROUGE;
                    }
                }
                else if(ok_sup == 0)
                {
                    for(j=0;j<grille.cn[grille.cb[i].case_parent_sup->val].nb_cb_valSup;j++)
                    {
                        SDL_FillRect(Cases[BLANCHES].surface[grille.cn[grille.cb[i].case_parent_sup->val].mot_sup[j]->id], NULL, couleurBlanc); // Remplissage
                        Cases[BLANCHES].bgColor[i] = BLANC;
                    }
                }
                Liste_push_front(&parents,grille.cb[i].case_parent_sup->val);
            }
        }
        else if(grille.cb[i].case_parent_inf != NULL && grille.cb[i].case_parent_sup == NULL)
        {
            if(!Liste_existe(parents,grille.cb[i].case_parent_inf->val))
            {
                for(j=0;j<grille.cn[grille.cb[i].case_parent_inf->val].nb_cb_valInf;j++)
                {
                    if(grille.cn[grille.cb[i].case_parent_inf->val].mot_inf[j]->valeur == 0)
                        ok_inf = 0;
                }

                if(ok_inf == 1)
                {
                   for(j=0;j<grille.cn[grille.cb[i].case_parent_inf->val].nb_cb_valInf;j++)
                    {
                        for(k=j+1;k<grille.cn[grille.cb[i].case_parent_inf->val].nb_cb_valInf;k++)
                        {
                            if(grille.cn[grille.cb[i].case_parent_inf->val].mot_inf[j]->valeur == grille.cn[grille.cb[i].case_parent_inf->val].mot_inf[k]->valeur)
                                occurence = 1;
                        }

                    }
                }

                if(grille.cn[grille.cb[i].case_parent_inf->val].sommeInf == grille.cn[grille.cb[i].case_parent_inf->val].val_inf && ok_inf == 1)
                {
                    for(j=0;j<grille.cn[grille.cb[i].case_parent_inf->val].nb_cb_valInf;j++)
                    {
                        SDL_FillRect(Cases[BLANCHES].surface[grille.cn[grille.cb[i].case_parent_inf->val].mot_inf[j]->id], NULL, couleurVert); // Remplissage
                        Cases[BLANCHES].bgColor[i] = VERT;
                    }

                }
                else if((grille.cn[grille.cb[i].case_parent_inf->val].sommeInf != grille.cn[grille.cb[i].case_parent_inf->val].val_inf && ok_inf == 1) || occurence == 1)
                {
                    for(j=0;j<grille.cn[grille.cb[i].case_parent_inf->val].nb_cb_valInf;j++)
                    {
                        SDL_FillRect(Cases[BLANCHES].surface[grille.cn[grille.cb[i].case_parent_inf->val].mot_inf[j]->id], NULL, couleurRouge); // Remplissage
                        Cases[BLANCHES].bgColor[i] = ROUGE;
                    }
                }
                else if(ok_inf == 0)
                {
                    for(j=0;j<grille.cn[grille.cb[i].case_parent_inf->val].nb_cb_valInf;j++)
                    {
                        SDL_FillRect(Cases[BLANCHES].surface[grille.cn[grille.cb[i].case_parent_inf->val].mot_inf[j]->id], NULL, couleurBlanc); // Remplissage
                        Cases[BLANCHES].bgColor[i] = BLANC;
                    }
                }

                Liste_push_front(&parents,grille.cb[i].case_parent_inf->val);
            }
                //else
                //{
                 //   SDL_FillRect(Cases[BLANCHES].surface[grille.cn[grille.cb[i].case_parent_sup->val].mot_sup[j]->id], NULL, couleurRouge); // Remplissage
                   // Cases[BLANCHES].bgColor[i] = ROUGE;
                //}
        }
        else if( grille.cb[i].case_parent_inf != NULL && grille.cb[i].case_parent_sup != NULL)
        {
            if(!Liste_existe(parents,grille.cb[i].case_parent_sup->val))
            {
                for(j=0;j<grille.cn[grille.cb[i].case_parent_sup->val].nb_cb_valSup;j++)
                {
                    if(grille.cn[grille.cb[i].case_parent_sup->val].mot_sup[j]->valeur == 0)
                        ok_sup = 0;
                }

                if(ok_sup == 1)
                {
                   for(j=0;j<grille.cn[grille.cb[i].case_parent_sup->val].nb_cb_valSup;j++)
                    {
                        for(k=j+1;k<grille.cn[grille.cb[i].case_parent_sup->val].nb_cb_valSup;k++)
                        {
                            if(grille.cn[grille.cb[i].case_parent_sup->val].mot_sup[j]->valeur == grille.cn[grille.cb[i].case_parent_sup->val].mot_sup[k]->valeur)
                                occurence = 1;
                        }

                    }
                }


                if(grille.cn[grille.cb[i].case_parent_sup->val].sommeSup == grille.cn[grille.cb[i].case_parent_sup->val].val_sup && ok_sup == 1)
                {
                    for(j=0;j<grille.cn[grille.cb[i].case_parent_sup->val].nb_cb_valSup;j++)
                    {
                        SDL_FillRect(Cases[BLANCHES].surface[grille.cn[grille.cb[i].case_parent_sup->val].mot_sup[j]->id], NULL, couleurVert); // Remplissage
                        Cases[BLANCHES].bgColor[i] = VERT;
                    }
                }
               else if((grille.cn[grille.cb[i].case_parent_sup->val].sommeSup != grille.cn[grille.cb[i].case_parent_sup->val].val_sup && ok_sup == 1) || occurence == 1)
                {
                    for(j=0;j<grille.cn[grille.cb[i].case_parent_sup->val].nb_cb_valSup;j++)
                    {
                        SDL_FillRect(Cases[BLANCHES].surface[grille.cn[grille.cb[i].case_parent_sup->val].mot_sup[j]->id], NULL, couleurRouge); // Remplissage
                        Cases[BLANCHES].bgColor[i] = ROUGE;
                    }
                }
                else if(ok_sup == 0)
                {
                    for(j=0;j<grille.cn[grille.cb[i].case_parent_sup->val].nb_cb_valSup;j++)
                    {
                        SDL_FillRect(Cases[BLANCHES].surface[grille.cn[grille.cb[i].case_parent_sup->val].mot_sup[j]->id], NULL, couleurBlanc); // Remplissage
                        Cases[BLANCHES].bgColor[i] = BLANC;
                    }
                }
                Liste_push_front(&parents,grille.cb[i].case_parent_sup->val);
            }

            if(!Liste_existe(parents,grille.cb[i].case_parent_inf->val))
            {
                for(j=0;j<grille.cn[grille.cb[i].case_parent_inf->val].nb_cb_valInf;j++)
                {
                    if(grille.cn[grille.cb[i].case_parent_inf->val].mot_inf[j]->valeur == 0)
                        ok_inf = 0;
                }

                if(ok_inf == 1)
                {
                   for(j=0;j<grille.cn[grille.cb[i].case_parent_inf->val].nb_cb_valInf;j++)
                    {
                        for(k=j+1;k<grille.cn[grille.cb[i].case_parent_inf->val].nb_cb_valInf;k++)
                        {
                            if(grille.cn[grille.cb[i].case_parent_inf->val].mot_inf[j]->valeur == grille.cn[grille.cb[i].case_parent_inf->val].mot_inf[k]->valeur)
                                occurence = 1;
                        }

                    }
                }

                if(grille.cn[grille.cb[i].case_parent_inf->val].sommeInf == grille.cn[grille.cb[i].case_parent_inf->val].val_inf && ok_inf == 1)
                {
                    for(j=0;j<grille.cn[grille.cb[i].case_parent_inf->val].nb_cb_valInf;j++)
                    {
                        SDL_FillRect(Cases[BLANCHES].surface[grille.cn[grille.cb[i].case_parent_inf->val].mot_inf[j]->id], NULL, couleurVert); // Remplissage
                        Cases[BLANCHES].bgColor[i] = VERT;
                    }

                }
                else if((grille.cn[grille.cb[i].case_parent_inf->val].sommeInf != grille.cn[grille.cb[i].case_parent_inf->val].val_inf && ok_inf == 1) || occurence == 1)
                {
                    for(j=0;j<grille.cn[grille.cb[i].case_parent_inf->val].nb_cb_valInf;j++)
                    {
                        SDL_FillRect(Cases[BLANCHES].surface[grille.cn[grille.cb[i].case_parent_inf->val].mot_inf[j]->id], NULL, couleurRouge); // Remplissage
                        Cases[BLANCHES].bgColor[i] = ROUGE;
                    }
                }
                else if(ok_inf == 0)
                {
                    for(j=0;j<grille.cn[grille.cb[i].case_parent_inf->val].nb_cb_valInf;j++)
                    {
                        SDL_FillRect(Cases[BLANCHES].surface[grille.cn[grille.cb[i].case_parent_inf->val].mot_inf[j]->id], NULL, couleurBlanc); // Remplissage
                        Cases[BLANCHES].bgColor[i] = BLANC;
                    }
                }

                Liste_push_front(&parents,grille.cb[i].case_parent_inf->val);
            }

        }

        occurence = 0;
        ok_sup = 1;
        ok_inf = 1;

    }*/
    return 1;
}

int backtrack_Graphique(GrilleKakuro jeu, CSP* csp,int log,surfacePos *Cases,int *focus, SDL_Surface *screen,int vitesse)
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
            if(vitesse != 0)
                SDL_Delay(vitesse);

            v = Depiler(x.d); // on prend une valeur du sommet de la pile du domaine et on la retire de celui-ci
            solution = 0; // on part du fait que la valeur choisie n'est pas une solution
            if(log)
            {
                sprintf(chaineLog,"caseBlanche[%d][%d] = %d, TAILLE de CSP->V = %d TAILLE de CSP->A = %d \n",x.cb->pos.X,x.cb->pos.Y,v,sizeVar(csp->V),sizeVar(csp->A));
                Ecrire_log(chaineLog,nomFichierLog);
            }
            //x.cb->valeur = v;
            calculerSommes(&jeu);
            //AnimCurseur(grille,Cases,focus,&focusPrecedent,screen); // Dessin du curseur sur la grille
            DessinGrille(screen,jeu,Cases,*focus); //Dessin de la grille
            BlitGrille(Cases,jeu,screen); // Blit de la grille
            coloreGrille(jeu,Cases,screen);
            SDL_Flip(screen); //Actualisation de l'ecran

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

