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

  printf ("degre sortant du sommet = %d\n", degre_sortant_sommet (g,g)) ;
  printf ("degre entrant du sommet = %d\n", degre_entrant_sommet (g, g)) ;
  printf ("degre maximal du graphe = %d\n", degre_maximal_graphe (g)) ;
  printf ("degre minimal du graphe = %d\n", degre_minimal_graphe (g)) ;
  printf ("graphe indep ? = %d\n", independant (g)) ;
  printf ("graphe complet ? = %d\n", complet (g)) ;
  printf ("graphe regulier ? = %d\n", regulier (g)) ;

  if (!strcmp(argv[1], "data/gr4")) {
    pchemin_t c;
    /*
      Test chemin élémentaire
    */
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
        printf("%d -> ", label[i]);
      printf(" n'est pas elementaire\n");
    }

    /*
      Test chemin simple
    */
    int label_simple[5] = {3,4,5,3,4};
    c = creer_chemin(g, label_simple, 5);
    if (simple(g, c)) {
      printf("Le chemin ");
      for (int i = 0; i < 5; i++)
        printf("%d -> ", label_simple[i]);
      printf(" est simple\n");
    } else {
      printf("Le chemin ");
      for (int i = 0; i < 5; i++)
        printf("%d -> ", label_simple[i]);
      printf(" n'est pas simple\n");
    }

    label_simple[0] = 3;
    label_simple[1] =  4;
    label_simple[2] =  5;
    label_simple[3] =  3;
    label_simple[4] =  6;
    c = creer_chemin(g, label_simple, 5);
    if (simple(g, c)) {
      printf("Le chemin ");
      for (int i = 0; i < 5; i++)
        printf("%d -> ", label_simple[i]);
      printf(" est simple\n");
    } else {
      printf("Le chemin ");
      for (int i = 0; i < 5; i++)
        printf("%d -> ", label_simple[i]);
      printf(" n'est pas simple\n");
    }


    /*
      Test chemin eulerien
    */
    label[0] = 1;
    label[1] =  6;
    label[2] =  2;
    label[3] =  1;
    c = creer_chemin(g, label, 4);
    if (eulerien(g, c)) {
      printf("Le chemin ");
      for (int i = 0; i < 4; i++)
        printf("%d -> ", label[i]);
      printf(" est eulerien\n");
    } else {
      printf("Le chemin ");
      for (int i = 0; i < 4; i++)
        printf("%d -> ", label[i]);
      printf(" n'est pas eulerien\n");
    }

    int label_eule[13] = {1,6,2,1,6,2,3,4,5,3,6,2,5};
    c = creer_chemin(g, label_eule, 13);
    if (eulerien(g, c)) {
      printf("Le chemin ");
      for (int i = 0; i < 13; i++)
        printf("%d -> ", label_eule[i]);
      printf(" est eulerien\n");
    } else {
      printf("Le chemin ");
      for (int i = 0; i < 13; i++)
        printf("%d -> ", label_eule[i]);
      printf(" n'est pas eulerien\n");
    }

    /*
      Test chemin hamiltonien
    */
    int label_ham[6] = {1,6,2,3,4,5};
    c = creer_chemin(g, label_ham, 6);
    if (hamiltonien(g, c)) {
      printf("Le chemin ");
      for (int i = 0; i < 6; i++)
        printf("%d -> ", label_ham[i]);
      printf(" est hamiltonien\n");
    } else {
      printf("Le chemin ");
      for (int i = 0; i < 6; i++)
        printf("%d -> ", label_ham[i]);
      printf(" n'est pas hamiltonien\n");
    }
    label_ham[0] = 1;
    label_ham[1] = 6;
    label_ham[2] = 2;
    label_ham[3] = 3;
    label_ham[4] = 6;
    label_ham[5] = 2;
    c = creer_chemin(g, label_ham, 6);
    if (hamiltonien(g, c)) {
      printf("Le chemin ");
      for (int i = 0; i < 6; i++)
        printf("%d -> ", label_ham[i]);
      printf(" est hamiltonien\n");
    } else {
      printf("Le chemin ");
      for (int i = 0; i < 6; i++)
        printf("%d -> ", label_ham[i]);
      printf(" n'est pas hamiltonien\n");
    }

  } else {
    printf("Les test des chemins se font sur le graphe 4\n");
  }

  //pchemin_t c = creer_chemin(g)
}
