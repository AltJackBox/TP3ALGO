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

void reset_arc(pgraphe_t g) {
  psommet_t sommet_act = g;
  parc_t arc_act;
  while (sommet_act != NULL) {
    arc_act = sommet_act->liste_arcs;
    while (arc_act != NULL) {
      arc_act->parcourus = 0;
      arc_act = arc_act->arc_suivant;
    }
    sommet_act = sommet_act->sommet_suivant;
  }
}

pchemin_t creer_chemin(pgraphe_t g, int *liste_label, int nb_label) {
  psommet_t sommet_act = g;
  while (sommet_act->label != liste_label[0])
    sommet_act = sommet_act->sommet_suivant;
  pchemin_t c = (pchemin_t) malloc(sizeof(chemin_t));
  c->nb_arc = nb_label - 1;
  c->list_arc = malloc(sizeof(arc_t) * c->nb_arc);
  c->debut = sommet_act;
  parc_t arc;
  for (int i = 1; i < nb_label; i++) {
    arc = sommet_act->liste_arcs;
    while (arc->dest->label != liste_label[i]) {
      arc = arc->arc_suivant;
    }
    c->list_arc[i-1] = arc;
    sommet_act = arc->dest;
  }
  return c;

}

void afficher_graphe_largeur (pgraphe_t g, int r)
{
  reset_parcours(g);
  pfile_t sommets_a_parcourir = creer_file();
  pgraphe_t a_traiter;
  parc_t a_ajouter;
  a_traiter = chercher_sommet(g, r);
  enfiler(sommets_a_parcourir, a_traiter);
  while(!file_vide(sommets_a_parcourir)){
    a_traiter = defiler(sommets_a_parcourir);
    if(!a_traiter->parcourus){
      a_traiter->parcourus = 1;
      printf("Label : %d, Couleur : %d\n",a_traiter->label,a_traiter->couleur);
      a_ajouter = a_traiter->liste_arcs;
      while(a_ajouter != NULL){
        if(!a_ajouter->dest->parcourus){
          enfiler(sommets_a_parcourir,a_ajouter->dest);
        }
        a_ajouter = a_ajouter->arc_suivant;
      }
    }
  }
  return;
}


void afficher_graphe_profondeur (pgraphe_t g, int r)
{
  reset_parcours(g);
  psommet_t actuel = chercher_sommet(g, r);
  ppile_t sommets_parcourus = creer_pile();
  parc_t listeArcs;
  empiler(sommets_parcourus, actuel);
  while (!pile_vide(sommets_parcourus)) {
    actuel = depiler(sommets_parcourus);
    if (!actuel->parcourus){
      printf("Label : %d, Couleur : %d\n", actuel->label, actuel->couleur);
      actuel->parcourus = 1;
    }
    listeArcs = actuel->liste_arcs;
    while ((listeArcs != NULL) && (listeArcs->dest->parcourus == 1)){
      listeArcs = listeArcs->arc_suivant;
    }
    if (listeArcs != NULL){
      empiler(sommets_parcourus, actuel);
      empiler(sommets_parcourus, listeArcs->dest);
    }
  }
}

int isAllScanned(pgraphe_t g) {
  psommet_t s = g;
  while (s != NULL) {
    if (!(s->parcourus))
      return 0;
    s = s->sommet_suivant;
  }
  return 1;
}

int plus_petite_distance(int *d, psommet_t *f , int nb_sommets) {
  int min = -1;
  for (int i = 0; i < nb_sommets; i ++) {
    if (!(f[i]->parcourus) && (((min == -1) && (d[i] != -1)) || ((d[i] != -1) && (min != -1) && (d[i] <= d[min])))){
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

int indice(psommet_t u, psommet_t *f, int nb_sommets) {
  for (int i = 0; i < nb_sommets; i ++) {
    if (f[i] == u)
      return i;
  }
  exit(0);
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
    if ((arc == NULL) || (distance[indice_plus_petit_sommet] == -1)) {
      distance[indice_sommet_dest] = -1; // sommet non relié
    } else {
      while (arc != NULL) {
        indice_sommet_dest = indice(arc->dest, f, nb_sommets);
        //relacher
        if ((distance[indice_sommet_dest] == -1) || (distance[indice_sommet_dest] > distance[indice_plus_petit_sommet] + arc->poids)) {
          distance[indice_sommet_dest] = distance[indice_plus_petit_sommet] + arc->poids;
          parents[indice_sommet_dest] = f[indice_plus_petit_sommet];
        }
        arc = arc->arc_suivant;
      }
    }

  }

  /*
    ##############################
                Affichage
    ##############################
  */

  printf("ALGORITHME de Dijkstra \n label distance parents\n");
  for (int i = 0; i < nb_sommets; i ++) {
    if (parents[i] != NULL) {
      printf("  %d        %d        %d\n", f[i]->label, distance[i], parents[i]->label);
    } else if (distance[i] == -1) {
      printf("  %d        INF        X\n", f[i]->label);
    } else {
      printf("  %d        %d        X\n", f[i]->label, distance[i]);
    }

  }
  /*
    ##############################
                FIN
    ##############################
  */
  free(distance);
  free(parents);
  free(f);

  return ;
}

int simple(pgraphe_t g, pchemin_t c) {
  reset_arc(g);
  for(int i = 0; i<c->nb_arc; i++){
    if(c->list_arc[i]->parcourus == 1) {
      return 0;
    }
    c->list_arc[i]->parcourus = 1;
  }
  return 1;
}

int elementaire(pgraphe_t g, pchemin_t c) {
  reset_parcours(g);
  c->debut->parcourus = 1;
  for(int i = 0; i<c->nb_arc; i++){
    if(c->list_arc[i]->dest->parcourus == 1) {
      return 0;
    }
    c->list_arc[i]->dest->parcourus = 1;
  }
  return 1;
}

int eulerien (pgraphe_t g, pchemin_t c) {
  reset_arc(g);
  // on parcours le chemin
  for (int i =0; i < c->nb_arc; i++)
    c->list_arc[i]->parcourus = 1;
  // on vérifie que tout les arcs on été parcourus dans le graphe
  psommet_t sommet_act = g;
  parc_t arc;
  while (sommet_act != NULL) {
    arc = sommet_act->liste_arcs;
    while (arc != NULL) {
      if (arc->parcourus == 0)
        return 0;
      arc = arc->arc_suivant;
    }
    sommet_act = sommet_act->sommet_suivant;
  }
  return 1;
}

int hamiltonien(pgraphe_t g, pchemin_t c) {
  reset_parcours(g);
  c->debut->parcourus = 1;
  for(int i = 0; i<c->nb_arc; i++){
    c->list_arc[i]->dest->parcourus = 1;
  }
  psommet_t sommet_act = g;
  while(sommet_act != NULL){
    if(sommet_act->parcourus == 0){
      return 0;
    }
    sommet_act = sommet_act->sommet_suivant;
  }
  return 1;
}
/*
int graphe_eulerien(pgraphe_t g) {

  reset_arc(g);
  reset_parcours(g);
  pgraphe_t sommet_en_traitement = g;
  pgraphe_t sommet_act;
  pchemin_t chemin;
  while(sommet_en_traitement != NULL){
    chemin = (pchemin_t) malloc(sizeof(chemin_t));
    chemin->list_arc = malloc(sizeof(parc_t) * 128);
    chemin->nb_arc = 0;
    chemin->debut = sommet_en_traitement;
    chemin->longueur = 0;

    reset_arc(g);
    reset_parcours(g);
    sommet_act = sommet_en_traitement;
    while(sommet_act != NULL){
      sommet_act = trouver_sommet_suivant(g, sommet_act, chemin);
    }

    free(chemin->list_arc);
    free(chemin);
  }
}

pgraphe_t trouver_sommet_suivant(pgraphe_t g, pgraphe_t act, pchemin_t c){
  parc_t arc_act = act->liste_arcs;
  while(arc_act != NULL) {
    if (arc_act->parcourus == 0) {
      arc_act->parcourus = 1;
      if(!verif_pont(g)){
        c->longueur += arc_act->poids;
        c->list_arc[c->nb_arc] = arc_act;
        c->nb_arc++;
        return act->sommet_suivant;
      }
      arc_act->parcourus = 0;
    }
    arc_act = arc_act->arc_suivant;
  }
  return NULL;
}*/


// ======================================================================




int degre_sortant_sommet (pgraphe_t g, psommet_t s)
{
  int nbArcs = 0;
  parc_t listeArcs = s->liste_arcs;
  while (listeArcs != NULL){
    listeArcs = listeArcs->arc_suivant;
    nbArcs++;
  }
  return nbArcs;
}

int degre_entrant_sommet (pgraphe_t g, psommet_t s)
{
  int nbArcs = 0;
  psommet_t sommet = g;
  parc_t listeArcs = g->liste_arcs;
  while (sommet != NULL){
    while (listeArcs != NULL){
      if (listeArcs->dest == s){
        nbArcs++;
      }
      listeArcs = listeArcs->arc_suivant;
    }
    sommet = sommet->sommet_suivant;
    if (sommet != NULL){
      listeArcs = sommet->liste_arcs;
    }
  }
  return nbArcs;
}

int degre_maximal_graphe (pgraphe_t g)
{
  int degMax = 0;
  int degCalcul = 0;
  psommet_t sommet = g;
  while (sommet != NULL){
    degCalcul = degre_entrant_sommet(g, sommet) + degre_sortant_sommet(g, sommet);
    if (degMax < degCalcul){
      degMax = degCalcul;
    }
    sommet = sommet->sommet_suivant;
  }
  return degMax;
}


int degre_minimal_graphe (pgraphe_t g)
{
  psommet_t sommet = g;
  int degMin = degre_entrant_sommet(g, sommet) + degre_sortant_sommet(g, sommet);
  int degCalcul = 0;
  while (sommet != NULL){
    degCalcul = degre_entrant_sommet(g, sommet) + degre_sortant_sommet(g, sommet);
    if (degMin > degCalcul){
      degMin = degCalcul;
    }
    sommet = sommet->sommet_suivant;
  }
  return degMin;
}


int independant (pgraphe_t g)
{
  reset_parcours(g);
  int indep = 1;
  psommet_t sommet = (psommet_t) g;
  parc_t listeArcs = g->liste_arcs;
  while (sommet != NULL){
    while (listeArcs != NULL){
      if (listeArcs->dest->parcourus == 1){
        indep = 0;
        break;
      }
      listeArcs->dest->parcourus = 1;
      listeArcs = listeArcs->arc_suivant;
    }
    sommet = sommet->sommet_suivant;
    if (sommet != NULL) {
      listeArcs = sommet->liste_arcs;
    }
  }
  return indep;
}



int complet (pgraphe_t g)
{
  int nbSommet = nombre_sommets(g);
  psommet_t sommet = g;
  while (sommet != NULL){
    if (degre_sortant_sommet(g, sommet) != nbSommet -1){
      return 0;
    }
    sommet = sommet->sommet_suivant;
  }
  return 1;
}

int regulier (pgraphe_t g)
{
  int degre = degre_maximal_graphe(g);
  psommet_t sommet = g->sommet_suivant;
  while (sommet != NULL){
    if (degre_entrant_sommet(g, sommet) + degre_sortant_sommet(g, sommet) != degre){
      return 0;
    }
    sommet = sommet->sommet_suivant;
  }
  return 1;
}




/*
  placer les fonctions de l'examen 2017 juste apres
*/

// int simple (pgraphe_t g, chemin_t c){

// }
