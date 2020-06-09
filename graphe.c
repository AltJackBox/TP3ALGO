/*
  Structures de type graphe
  Structures de donnees de type liste
  (Pas de contrainte sur le nombre de noeuds des  graphes)
*/


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "graphe.h"
#include "pile.h"
#include "file.h"

psommet_t chercher_sommet (pgraphe_t g, int label)
{
  psommet_t s ;

  s = g ;

  while ((s!= NULL) && (s->label != label))
    {
      s = s->sommet_suivant ;
    }
  return s ;
}

parc_t existence_arc (parc_t l, psommet_t s)
{
  parc_t p = l ;

  while (p != NULL)
    {
      if (p->dest == s)
	return p ;
      p = p->arc_suivant ;
    }
  return p ;

}


void ajouter_arc (psommet_t o, psommet_t d, int distance)
{
  parc_t parc ;

  parc = (parc_t) malloc (sizeof(arc_t)) ;

  if (existence_arc (o->liste_arcs, d) != NULL)
    {
      fprintf(stderr, "ajout d'un arc deja existant\n") ;
      exit (-1) ;
    }

  parc->poids = distance ;
  parc->dest = d ;
  parc->arc_suivant = o->liste_arcs ;
  o->liste_arcs = parc ;
  return ;
}



// ===================================================================

int nombre_sommets (pgraphe_t g)
{
  psommet_t p = g ;
  int nb = 0 ;

  while (p != NULL)
    {
      nb = nb + 1 ;
      p = p->sommet_suivant ;
    }

  return nb ;
}

int nombre_arcs (pgraphe_t g)
{

  psommet_t p = g ;
  int nb_arcs = 0 ;

  while (p != NULL)
    {
      parc_t l = p->liste_arcs ;

      while (l != NULL)
	{
          nb_arcs = nb_arcs + 1 ;
	  l = l->arc_suivant ;
	}

      p = p->sommet_suivant ;
    }
  return nb_arcs ;
}

void init_couleur_sommet (pgraphe_t g)
{
  psommet_t p = g ;

  while (p != NULL)
    {
      p->couleur = 0 ; // couleur indefinie
      p = p->sommet_suivant ; // passer au sommet suivant dans le graphe
    }

  return ;
}

int colorier_graphe (pgraphe_t g)
{
  /*
    coloriage du graphe g

    datasets
    graphe data/gr_planning
    graphe data/gr_sched1
    graphe data/gr_sched2
  */

  psommet_t p = g ;
  parc_t a ;
  int couleur ;
  int max_couleur = INT_MIN ; // -INFINI

  int change ;

  init_couleur_sommet (g) ;

  while (p != NULL)
    {
      couleur = 1 ; // 1 est la premiere couleur

      // Pour chaque sommet, on essaie de lui affecter la plus petite couleur

      // Choix de la couleur pour le sommet p

      do
	{
	  a = p->liste_arcs ;
	  change = 0 ;

	  while (a != NULL)
	    {
	      if (a->dest->couleur == couleur)
		{
		  couleur = couleur + 1 ;
		  change = 1 ;
		}
	      a = a->arc_suivant ;
	    }

	} while (change == 1) ;

      // couleur du sommet est differente des couleurs de tous les voisins

      p->couleur = couleur ;
      if (couleur > max_couleur)
	max_couleur = couleur ;

      p = p->sommet_suivant ;
    }

  return max_couleur ;
}

void reset_parcours(pgraphe_t g) {
  psommet_t sommet_act = g;
  while (sommet_act != NULL) {
    sommet_act->parcourus = 0;
    sommet_act = sommet_act->sommet_suivant;
  }
}
void afficher_graphe_largeur (pgraphe_t g, int r)
{
  /*
    afficher les sommets du graphe avec un parcours en largeur
  */

  return ;
}


void afficher_graphe_profondeur (pgraphe_t g, int r)
{
  /*
    afficher les sommets du graphe avec un parcours en profondeur
  */

  return ;
}

int isAllScanned(pgraphe_t g) {
  psommet_t s = g;
  while (s != NULL) {
    if (s->parcourus)
      return 0;
    s = s->sommet_suivant;
  }
  return 1;
}

int plus_petite_distance(int *d, psommet_t *f , int nb_sommets) {
  int min = -1;
  for (int i = 0; i < nb_sommets; i ++) {
    if (!(f[i]->parcourus) && (((min == -1) && (d[i] != -1)) || ((d[i] != -1) && (min != -1) && (d[i] < d[min])))){
      min = i;
    }
  }
  if (min != -1)
    return min;


  //ici, on est dans le cas d'un sommet non relier, sa distance est "infini"

  for (int i = 0; i < nb_sommets; i ++) {
    if (!(f[i]->parcourus))
      return i;
  }
  return 0;
}

void relacher(int u, int v, int poids, int *d, psommet_t *f, psommet_t *parents, int nb_sommets) {
  if (d[v] > d[u] + poids) {
    d[v] = d[u] + poids;
    parents[v] = f[u];
  }
}

int indice(psommet_t u, psommet_t *f, int nb_sommets) {
  for (int i = 0; i < nb_sommets; i ++) {
    if (f[i] == u)
      return i;
  }
  return 0;
}

void algo_dijkstra (pgraphe_t g, int r)
{
  /*
    ##############################
    INITIALISATION de l'algorithme
    ##############################
  */

  reset_parcours(g); //on reset le champ parcourus des sommets du graphes
  int nb_sommets = nombre_sommets(g);
  int *distance = (int *) malloc(sizeof(int) * nb_sommets);
  // initialisation du tableau des distance
  for (int i = 0; i < nb_sommets; i ++)
    distance[i] = -1;

  // initialisation des parents
  psommet_t* parents = (psommet_t *) malloc(sizeof(psommet_t) * nb_sommets);
  for (int i = 0; i < nb_sommets; i ++)
    parents[i] = NULL;

  psommet_t *f = (psommet_t *) malloc(sizeof(psommet_t) * nb_sommets);
  psommet_t sommet_act = g;

  for (int i = 0; i < nb_sommets; i ++) {
    f[i] = sommet_act;
    if (sommet_act->label == r)
      distance[i] = 0;
    sommet_act = sommet_act->sommet_suivant;
  }

  /*
    ##############################
              ALGORITHME
    ##############################
  */
  int indice_plus_petit_sommet;
  int indice_sommet_dest;
  parc_t arc;
  while (!isAllScanned(g)) {
    indice_plus_petit_sommet = plus_petite_distance(distance, f, nb_sommets);
    f[indice_plus_petit_sommet]->parcourus = 1;
    arc = f[indice_plus_petit_sommet]->liste_arcs;
    while (arc != NULL) {
      indice_sommet_dest = indice(arc->dest, f, nb_sommets);
      relacher(indice_plus_petit_sommet, indice_sommet_dest, arc->poids ,distance, f, parents, nb_sommets);
      arc = arc->arc_suivant;
    }
  }
  /*
    ##############################
                FIN
    ##############################
  */
  reset_parcours(g);
  free(distance);
  free(parents);
  free(f);

  return ;
}





// ======================================================================




int degre_sortant_sommet (pgraphe_t g, psommet_t s)
{
  /*
    Cette fonction retourne le nombre d'arcs sortants
    du sommet n dans le graphe g
  */

  return 0 ;
}

int degre_entrant_sommet (pgraphe_t g, psommet_t s)
{
  /*
    Cette fonction retourne le nombre d'arcs entrants
    dans le noeud n dans le graphe g
  */

  return 0 ;
}

int degre_maximal_graphe (pgraphe_t g)
{
  /*
    Max des degres des sommets du graphe g
  */

  return 0 ;
}


int degre_minimal_graphe (pgraphe_t g)
{
  /*
    Min des degres des sommets du graphe g
  */

  return 0 ;
}


int independant (pgraphe_t g)
{
  /* Les aretes du graphe n'ont pas de sommet en commun */

  return 0 ;
}



int complet (pgraphe_t g)
{
  /* Toutes les paires de sommet du graphe sont jointes par un arc */

  return 0 ;
}

int regulier (pgraphe_t g)
{
  /*
     graphe regulier: tous les sommets ont le meme degre
     g est le ponteur vers le premier sommet du graphe
     renvoie 1 si le graphe est régulier, 0 sinon
  */

  return 0 ;
}




/*
  placer les fonctions de l'examen 2017 juste apres
*/
