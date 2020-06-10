#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graphe.h"

int main (int argc, char **argv)
{
  pgraphe_t g ;
  int nc ;

  if (argc != 2)
    {
      fprintf (stderr, "erreur parametre \n") ;
      exit (-1) ;
    }

  /*
    la fonction lire_graphe alloue le graphe (matrice,...)
    et lit les donnees du fichier passe en parametre
  */


  lire_graphe (argv [1], &g) ;

  /*
    la fonction ecrire_graphe affiche le graphe a l'ecran
  */

  printf ("nombre de sommets du graphe %d nombre arcs %d \n", nombre_sommets (g), nombre_arcs (g)) ;
  fflush (stdout) ;

  ecrire_graphe (g) ;

  nc = colorier_graphe (g) ;

  printf ("nombre chromatique graphe = %d\n", nc) ;

  ecrire_graphe_colorie (g) ;

  afficher_graphe_largeur(g,1);
  algo_dijkstra(g,1);

  if (!strcmp(argv[1], "data/gr4")) {
    pchemin_t c;
    int label[4] = {1, 6, 2 , 3};
    c = creer_chemin(g, label, 4);
    if (elementaire(g, c)) {
      printf("Le chemin ");
      for (int i = 0; i < 4; i++)
        printf("%d -> ", label[i]);
      printf(" est elementaire\n");
    } else {
      printf("Le chemin ");
      for (int i = 0; i < 4; i++)
        printf("%c -> \n", label[i]);
      printf(" n'est pas elementaire\n");
    }

    label[0] = 1;
    label[1] =  6;
    label[2] =  2;
    label[3] =  1;
    c = creer_chemin(g, label, 4);
    if (elementaire(g, c)) {
      printf("Le chemin ");
      for (int i = 0; i < 4; i++)
        printf("%d -> ", label[i]);
      printf(" est elementaire\n");
    } else {
      printf("Le chemin ");
      for (int i = 0; i < 4; i++)
        printf("%c -> \n", label[i]);
      printf(" n'est pas elementaire\n");
    }

  } else {
    printf("Les test des chemins se font sur le graphe 4\n");
  }

  //pchemin_t c = creer_chemin(g)
}
