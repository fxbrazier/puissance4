//
//  main.c
//  PROJET
//
//  Created by Guillaume Fradeira on 15/06/2016.
//  Copyright © 2016 Guillaume Fradeira. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

static void affiche_grille(unsigned char (*)[6]);
static int coup_valide(unsigned, unsigned char (*)[6]);
static unsigned long_colonne(unsigned, unsigned, unsigned, unsigned char (*)[6]);
static unsigned long_ligne(unsigned, unsigned, unsigned, unsigned char (*)[6]);
static unsigned long_oblique(unsigned, unsigned, unsigned, unsigned char (*)[6]);
static int grille_complete(unsigned char (*)[6]);
static int statut_du_jeu(unsigned, unsigned, unsigned char (*)[6]);


static void affiche_grille(unsigned char (*grille)[6])
{
    unsigned i;
    unsigned j;
    
    putchar('\n');
    printf("  1   2   3   4   5   6   7  \n");
    printf("+---+---+---+---+---+---+---+\n");
    
    for (i = 0; i < 6; ++i)
    {
        putchar('|');
        
        for (j = 0; j < 7; ++j)
        {
            char c;
            
            switch (grille[j][i])
            {
                case 0:
                    c = ' ';
                    break;
                    
                case 1:
                    c = 'O';
                    break;
                    
                case 2:
                    c = 'X';
                    break;
            }
            
            printf(" %c |", c);
        }
        
        putchar('\n');
        printf("+---+---+---+---+---+---+---+\n");
    }
    
    printf("  1   2   3   4   5   6   7  \n");
    putchar('\n');
}


static int coup_valide(unsigned col, unsigned char (*grille)[6])
{
    if (!col || col > 7 || grille[col - 1][0] != 0)
        return 0;
    
    return 1;
}


static unsigned long_colonne(unsigned joueur, unsigned col, unsigned ligne, unsigned char (*grille)[6])
{
    unsigned i;
    unsigned n = 1;
    
    for (i = ligne + 1; i < 6; ++i)
    {
        if (grille[col][i] == joueur)
            ++n;
        else
            break;
    }
    
    return n;
}


static unsigned long_ligne(unsigned joueur, unsigned col, unsigned ligne, unsigned char (*grille)[6])
{
    unsigned i;
    unsigned n = 1;
    
    i = col;
    
    while (i > 0)
    {
        --i;
        
        if (grille[i][ligne] == joueur)
            ++n;
        else
            break;
    }
    
    i = col;
    
    while (i < 7)
    {
        ++i;
        
        if (grille[i][ligne] == joueur)
            ++n;
        else
            break;
    }
    
    return n;
}


static unsigned long_oblique(unsigned joueur, unsigned col, unsigned ligne, unsigned char (*grille)[6])
{
    unsigned i;
    unsigned j;
    unsigned n = 1;
    unsigned m = 1;
    
    i = col;
    j = ligne;
    
    while (i > 0 && j < 5)
    {
        --i;
        ++j;
        
        if (grille[i][j] == joueur)
            ++n;
        else
            break;
    }
    
    i = col;
    j = ligne;
    
    while (i < 7 && j > 0)
    {
        ++i;
        --j;
        
        if (grille[i][j] == joueur)
            ++n;
        else
            break;
    }
    
    i = col;
    j = ligne;
    
    while (i > 0 && j > 0)
    {
        --i;
        --j;
        
        if (grille[i][j] == joueur)
            ++m;
        else
            break;
    }
    
    i = col;
    j = ligne;
    
    while (i < 7 && j < 6)
    {
        ++i;
        ++j;
        
        if (grille[i][j] == joueur)
            ++m;
        else
            break;
    }
    
    return (n > m) ? n : m;
}


static int grille_complete(unsigned char (*grille)[6])
{
    unsigned i;
    unsigned j;
    
    for (i = 0; i < 7; ++i)
        for (j = 0; j < 6; ++j)
            if (grille[i][j] == 0)
                return 0;
    
    return 1;
}


static int statut_du_jeu(unsigned joueur, unsigned col, unsigned char (*grille)[6])
{
    unsigned ligne;
    unsigned i;
    
    for (i = 0; i < 6; ++i)
        if (grille[col][i] != 0)
            break;
    
    ligne = i - 1;
    grille[col][ligne] = joueur;
    
    if (long_colonne(joueur, col, ligne, grille) >= 4)
        return joueur;
    else if (long_ligne(joueur, col, ligne, grille) >= 4)
        return joueur;
    else if (long_oblique(joueur, col, ligne, grille) >= 4)
        return joueur;
    else if (grille_complete(grille))
        return 3;
    
    return 0;
}


int main(void)
{
    char choix[255];
    char *ptr;
    unsigned char grille[7][6] = { { 0 } };
    unsigned joueur = 1;
    unsigned col;
    int n;
    
    affiche_grille(grille);
    
    while (1)
    {
        printf("Joueur %u : ", joueur);
        
        if (scanf("%255s", choix) != 1)
        {
            fprintf(stderr, "Erreur lors de la saisie\n");
            return EXIT_FAILURE;
        }
        else if (scanf("%*[^\n]") < 0 || scanf("%*c") < 0)
        {
            fprintf(stderr, "Erreur lors de l'utilisation de scanf\n");
            return EXIT_FAILURE;
        }
        
        if (choix[0] == 'q' || choix[0] == 'Q')
            return 0;
        
        col = strtol(choix, &ptr, 10);
        
        if (*ptr != '\0')
        {
            fprintf(stderr, "Vous devez entrer un numéro de colonne\n");
            continue;
        }
        else if (!coup_valide(col, grille))
        {
            fprintf(stderr, "Vous ne pouvez pas jouer à cet endroit\n");
            continue;
        }
        
        --col;
        n = statut_du_jeu(joueur, col, grille);
        affiche_grille(grille);
        
        if (n == 1 || n == 2)
        {
            printf("Le joueur %u a gagné\n", joueur);
            break;
        }
        else if (n == 3)
        {
            printf("Égalité\n");
            break;
        }
        
        joueur = (joueur == 1) ? 2 : 1;
    }
    
    return 0;
}
