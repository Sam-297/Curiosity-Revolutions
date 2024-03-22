#include "environnement.h"
#include "interprete.h"
#include "programme.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define LN_MAX 256

void nprint(char *message)
{
    printf("%s\n", message);
}

void gestion_erreur_terrain(erreur_terrain e)
{
    switch (e)
    {
    case OK:
        break;
    case ERREUR_FICHIER:
        printf("Erreur lors de la lecture du terrain : erreur d'ouverture du fichier\n");
        exit(1);
    case ERREUR_LECTURE_LARGEUR:
        printf("Erreur lors de la lecture du terrain : erreur de lecture de la largeur\n");
        exit(1);
    case ERREUR_LECTURE_HAUTEUR:
        printf("Erreur lors de la lecture du terrain : erreur de lecture de la hauteur\n");
        exit(1);
    case ERREUR_LARGEUR_INCORRECTE:
        printf("Erreur lors de la lecture du terrain : largeur incorrecte\n");
        exit(1);
    case ERREUR_HAUTEUR_INCORRECTE:
        printf("Erreur lors de la lecture du terrain : hauteur incorrecte\n");
        exit(1);
    case ERREUR_CARACTERE_INCORRECT:
        printf("Erreur lors de la lecture du terrain : caractère incorrect\n");
        exit(1);
    case ERREUR_LIGNE_TROP_LONGUE:
        printf("Erreur lors de la lecture du terrain : ligne trop longue\n");
        exit(1);
    case ERREUR_LIGNE_TROP_COURTE:
        printf("Erreur lors de la lecture du terrain : ligne trop courte\n");
        exit(1);
    case ERREUR_LIGNES_MANQUANTES:
        printf("Erreur lors de la lecture du terrain : lignes manquantes\n");
        exit(1);
    case ERREUR_POSITION_ROBOT_MANQUANTE:
        printf("Erreur lors de la lecture du terrain : position initiale du robot manquante\n");
        exit(1);
    }
}

void affichage_position_programme(erreur_programme e)
{
    int i;
    printf("Ligne %d, colonne %d :\n", e.num_ligne, e.num_colonne);
    printf("%s\n", e.ligne);
    /* Impression de e.num_colonne-1 espaces */
    for (i = 1; i < e.num_colonne; i++)
    {
        printf(" ");
    }
    /* Impression d'un curseur de position */
    printf("^\n");
}

void gestion_erreur_programme(erreur_programme e)
{
    switch (e.type_err)
    {
    case OK_PROGRAMME:
        break;
    case ERREUR_FICHIER_PROGRAMME:
        printf("Erreur lors de la lecture du programme : erreur d'ouverture du fichier\n");
        exit(2);
    case ERREUR_BLOC_NON_FERME:
        printf("Erreur lors de la lecture du programme : bloc non fermé\n");
        exit(2);
    case ERREUR_FERMETURE_BLOC_EXCEDENTAIRE:
        printf("Erreur lors de la lecture du programme : fermeture de bloc excédentaire\n");
        affichage_position_programme(e);
        exit(2);
    case ERREUR_COMMANDE_INCORRECTE:
        printf("Erreur lors de la lecture du programme : commande incorrecte\n");
        affichage_position_programme(e);
        exit(2);
    }
}

void checknull(bool nul)
{
    if (nul)
        nprint("### Il y a un problème lors de la lecture du Programme ...");
}

int main(int argc, char **argv)
{
    Environnement envt;
    Programme prog;
    erreur_terrain errt;
    erreur_programme errp;
    etat_inter etat;
    resultat_inter res;

    if (argc < 2)
    {
        printf("Usage : %s <fichier test> \n", argv[0]);
        return 1;
    }
    char ligne[LN_MAX] = {0};
    FILE *fp = fopen(argv[1], "r");
    while (fp == NULL)
    {
        nprint("Entrez un nouveau nom correct : ");
        scanf("%s", ligne);
        fp = fopen(ligne, "r");
    }

    char nom_terrain[LN_MAX] = {0};
    char nom_prog[LN_MAX] = {0};
    int pas_max = 0;
    char even_final = 0;
    int x, y = 0;
    Orientation orio = 0;

    checknull(fgets(nom_terrain, LN_MAX, fp) == NULL);
    strtok(nom_terrain, "\n");
    checknull(fgets(nom_prog, LN_MAX, fp) == NULL);
    strtok(nom_prog, "\n");
    checknull(fgets(ligne, LN_MAX, fp) == NULL);
    pas_max = atoi(ligne);
    checknull(fgets(ligne, LN_MAX, fp) == NULL);
    even_final = ligne[0];

    if (even_final == 'N' || even_final == 'F')
    {
        checknull(fgets(ligne, LN_MAX, fp) == NULL);
        sscanf(ligne, "%d %d", &x, &y);
        checknull(fgets(ligne, LN_MAX, fp) == NULL);
        switch (ligne[0])
        {
        case 'N':
            orio = Nord;
            break;
        case 'E':
            orio = Est;
            break;
        case 'S':
            orio = Sud;
            break;
        case 'O':
            orio = Ouest;
            break;
        default:
            nprint("Erreur d'orientation !!!");
            break;
        }
    }

    /* Initialisation de l'environnement : lecture du terrain,
       initialisation de la position du robot */
    errt = initialise_environnement(&envt, nom_terrain);
    gestion_erreur_terrain(errt);

    /* Lecture du programme */
    errp = lire_programme(&prog, nom_prog);
    gestion_erreur_programme(errp);

    /* Initialisation de l'état */
    init_etat(&etat);
    int cont_pas = 0;
    do
    {
        res = exec_pas(&prog, &envt, &etat);
        /* Affichage du terrain et du robot */
        afficher_envt(&envt);
        cont_pas++;
    } while (res == OK_ROBOT && cont_pas <= pas_max);

    /* Affichage du résultat */
    switch (res)
    {
    case OK_ROBOT:
        printf("Robot sur une case libre, programme non terminé (ne devrait pas "
               "arriver)\n");
        break;
    case SORTIE_ROBOT:
        printf("Le robot est sorti :-)\n");
        break;
    case ARRET_ROBOT:
        printf("Robot sur une case libre, programme terminé :-/\n");
        break;
    case PLOUF_ROBOT:
        printf("Le robot est tombé dans l'eau :-(\n");
        break;
    case CRASH_ROBOT:
        printf("Le robot s'est écrasé sur un rocher X-(\n");
        break;
    case ERREUR_PILE_VIDE:
        printf("ERREUR : pile vide\n");
        break;
    case ERREUR_ADRESSAGE:
        printf("ERREUR : erreur d'adressage\n");
        break;
    case ERREUR_DIVISION_PAR_ZERO:
        printf("ERREUR : division par 0\n");
        break;
    }

    if (envt.r.x == x)
        nprint("-> Le robot est sur le bon abscisse.");
    else
        nprint("---> Le robot ne semble pas être obéissant (pas le même abscisse) !!");
    if (envt.r.y == y)
        nprint("-> Le robot est sur le bon ordonnée.");
    else
        nprint("---> Le robot ne semble pas être obéissant (pas le même ordonnée) !!");
    if (envt.r.o == orio)
        nprint("-> Le robot est sur la bonne orientation.");
    else
        nprint("---> Le robot ne semble pas être obéissant (pas la même orientation) !!");
}
