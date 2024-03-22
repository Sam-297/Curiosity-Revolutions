#include "terrain.h"
#include <stdio.h>

int main(int argc, char **argv) {
  FILE *f;
  Terrain t;
  int x, y;
  erreur_terrain res;
  char filename[DIM_MAX];

  if (argc < 2) {
    printf("Usage : %s <fichier>\n", argv[0]);
    return 1;
  }

  f = fopen(argv[1], "r");
  res = lire_terrain(f, &t, &x, &y);

  while (res == ERREUR_FICHIER){
    printf("Donner un nouveau fichier:\n");
    scanf("%s", filename);
    f = fopen(filename, "r");
    res = lire_terrain(f, &t, &x, &y);
  
  }
  if (res == OK){
    afficher_terrain(&t);
    printf("Position initiale du robot : (%d, %d)\n", x, y);
  }
  fclose(f);
  // nos tests:
  // printf("-----------------------------------\n");
  // printf("Nos tests a partir d'ici: \n");
  // FILE *f2;
  // f2 = fopen(argv[2], "w");
  // ecrire_terrain(f2, &t, x, y);
  // fclose(f2);
  // f2 = fopen(argv[2], "r");
  // Terrain mt;
  // int x2, y2;
  // lire_terrain(f2, &mt, &x2, &y2);
  // fclose(f2);
  // printf("Hauteur: %d\nLargeur: %d\n", hauteur(&mt), largeur(&mt));
  // afficher_terrain(&mt);
  // printf("Position initiale du robot (test 2): (%d, %d)\n",x2,y2);
}
