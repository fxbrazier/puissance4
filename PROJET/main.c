#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Affiche la grille de jeu
static void affiche_grille(unsigned char (*grille)[6])
{
    unsigned i;
    unsigned j;

    putchar('\n');
    printf("  1   2   3   4   5   6   7  \n");
    printf("+---+---+---+---+---+---+---+\n");

    // Parcours de toute la grille en colonne
    for (i = 0; i < 6; ++i)
    {
        putchar('|');

        // Parcours de toute la grille en ligne
        for (j = 0; j < 7; ++j)
        {
            char c;

            switch (grille[j][i])
            {
            //La case est vide
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

// option echange jetons
static void echanger_grille(unsigned char (*grille)[6])
{
    unsigned i;
    unsigned j;

    for (i = 0; i < 6; ++i)
    {
        for (j = 0; j < 7; ++j)
        {
            int tmp;

            switch (grille[j][i])
            {
            case 0:
                tmp = 0;
                break;
            // le jeton du joueur 1 devient celui du joueur 2
            case 1:
                tmp = 2;
                break;
            // le jeton du joueur 2 devient celui du joueur 1
            case 2:
                tmp = 1;
                break;
            }
            grille[j][i] = tmp;
        }
    }
}

// verifie si le nombre rentré est valide (entre 1 et 6 inclus)
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


static unsigned ia(unsigned char (*grille)[6])
{
    unsigned coups[7] = { 0 };
    unsigned i;
    unsigned j;
    unsigned max = 0;
    unsigned nmax = 0;
    unsigned col = 0;

    for (i = 0; i < 7; ++i)
    {
        unsigned n = 0;

        if (grille[i][0] != 0)
            continue;

        for (j = 0; j < 6; ++j)
            if (grille[i][j] != 0)
                break;

        n = long_colonne(1, i, j - 1, grille);

        if (n > coups[i])
            coups[i] = n;

        n = long_ligne(1, i, j - 1, grille);

        if (n > coups[i])
            coups[i] = n;

        n = long_oblique(1, i, j - 1, grille);

        if (n > coups[i])
            coups[i] = n;

        n = long_colonne(2, i, j - 1, grille);

        if (n == 4)
            return i;
        else if (n > coups[i])
            coups[i] = n;

        n = long_ligne(2, i, j - 1, grille);

        if (n == 4)
            return i;
        else if (n > coups[i])
            coups[i] = n;

        n = long_oblique(2, i, j - 1, grille);

        if (n == 4)
            return i;
        else if (n > coups[i])
            coups[i] = n;
    }

    for (i = 0; i < 7; ++i)
    {
        if (coups[i] > max)
        {
            col = i;
            max = coups[i];
            nmax = 1;
        } else if (coups[i] == max)
            ++nmax;
    }

    if (nmax == 1)
        return col;
    else
        nmax = rand() % (nmax + 1);

    for (i = 0; i < 7; ++i)
    {
        if (coups[i] == max)
        {
            if (nmax == 0)
            {
                col = i;
                break;
            }
            else
                --nmax;
        }
    }

    return col;
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


static int sauvegarder_jeu(char *nom, unsigned char (*grille)[6], unsigned joueur, unsigned njoueur)
{
    FILE *fp;
    unsigned i;
    unsigned j;

    printf("Entrez un nom de fichier : ");

    if (scanf("%255s", nom) != 1)
    {
        fprintf(stderr, "Erreur lors de la saisie\n");
        return EXIT_FAILURE;
    }
    else if (scanf("%*[^\n]") < 0 || scanf("%*c") < 0)
    {
        fprintf(stderr, "Erreur lors de l'utilisation de scanf\n");
        return EXIT_FAILURE;
    }

    fp = fopen(nom, "w");

    if (fp == NULL)
    {
        fprintf(stderr, "Impossible d'ouvrir le fichier %s\n", nom);
        return 0;
    }
    else if (fprintf(fp, "%u\n", joueur) < 0)
    {
        fprintf(stderr, "Impossible de sauvegarder le joueur courant\n");
        return 0;
    }
    else if (fprintf(fp, "%u\n", njoueur) < 0)
    {
        fprintf(stderr, "Impossible de sauvegarder le nombre de joueurs\n");
        return 0;
    }

    for (i = 0; i < 7; ++i)
    {
        for (j = 0; j < 6; ++j)
        {
            if (fprintf(fp, "%u ", grille[i][j]) < 0)
            {
                fprintf(stderr, "Impossible de sauvegarder la grille\n");
                return 0;
            }
        }

        if (fputc('\n', fp) == EOF)
        {
            fprintf(stderr, "Impossible de sauvegarder la grille\n");
            return 0;
        }
    }

    if (fclose(fp) != 0)
    {
        fprintf(stderr, "Impossible de fermer le fichier %s\n", nom);
        return 0;
    }

    printf("La partie a bien été sauvegardée dans le fichier %s\n", nom);
    return 1;
}


static int charger_jeu(char *nom, unsigned char (*grille)[6], unsigned *joueur, unsigned *njoueur)
{
    FILE *fp;
    unsigned i;
    unsigned j;

    printf("Entrez un nom de fichier : ");

    if (scanf("%255s", nom) != 1)
    {
        fprintf(stderr, "Erreur lors de la saisie\n");
        return EXIT_FAILURE;
    }
    else if (scanf("%*[^\n]") < 0 || scanf("%*c") < 0)
    {
        fprintf(stderr, "Erreur lors de l'utilisation de scanf\n");
        return EXIT_FAILURE;
    }

    fp = fopen(nom, "r");

    if (fp == NULL)
    {
        fprintf(stderr, "Impossible d'ouvrir le fichier %s\n", nom);
        return 0;
    }
    else if (fscanf(fp, "%u", joueur) != 1)
    {
        fprintf(stderr, "Impossible de récupérer le joueur courant\n");
        return 0;
    }
    else if (fscanf(fp, "%u", njoueur) != 1)
    {
        fprintf(stderr, "Impossible de récupérer le nombre de joueurs\n");
        return 0;
    }

    for (i = 0; i < 7; ++i)
        for (j = 0; j < 6; ++j)
        {
            if (fscanf(fp, "%u", &grille[i][j]) != 1)
            {
                fprintf(stderr, "Impossible de récupérer la grille\n");
                return 0;
            }
        }

    if (fclose(fp) != 0)
    {
        fprintf(stderr, "Impossible de fermer le fichier %s\n", nom);
        return 0;
    }

    return 1;
}

int main(void)
{
    int choice;
    char choix[256];
    char *ptr;
    unsigned char grille[7][6] = { { 0 } };
    unsigned joueur = 1;
    unsigned njoueur;
    unsigned col;
    time_t t;
    int n;
    int cpt = 0;
    printf("\t\t\t*************************PUISSANCE 4****************************** ");
    printf("\n\n\t\t\t\t\t     1 - Jouer a deux joueurs ou contre l'IA");
    printf("\n\t\t\t\t\t     2 - Quitter");
    printf("\n\t\t\t****************************************************************** ");
    printf("\n\nQuel est votre choix ?");
    scanf("%d",&choice);
    if (choice==1)
    {

        if (time(&t) == (time_t)-1)
        {
            fprintf(stderr, "Impossible d'obtenir la date courante\n");
            return EXIT_FAILURE;
        }
            printf("Entrez le nombre de joueurs (2 joueurs max): ");

            if (scanf("%u", &njoueur) != 1)
            {
                fprintf(stderr, "Erreur lors de la saisie\n");
                return EXIT_FAILURE;
            }

            else{
                printf("Erreur nombre incorrect");
            }

        affiche_grille(grille);

        while (1)
        {
            if (joueur == 1 || (njoueur == 2 && joueur == 2))
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
            } else
                col = ia(grille);

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
            if (cpt == 3){
                echanger_grille(grille);
                cpt = 0;
            }
            else{
                cpt++;
            }

        }

        return 0;
    }
    if (choice==2)
    {
        printf("\t\t\t\t\t\tA bientot");
        return 0;
    }
}
