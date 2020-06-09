#include <stdlib.h>
#include "pile.h"

ppile_t creer_pile ()
{
  ppile_t Pile = malloc( sizeof( pile_t ) ) ;
  Pile->sommet = 0;

  return Pile;
}

int detruire_pile (ppile_t p)
{

  if ( p == NULL ) return 0;
  else {
    free( p );
    return 1;
  }
}

int pile_vide (ppile_t p)
{
  return ( p->sommet == 0 );
}

int pile_pleine (ppile_t p)
 {
  return ( p->sommet == MAX_PILE_SIZE );
}

pgraphe_t depiler (ppile_t p)
{
  if ( !( pile_vide( p ) ) ){
    p -> sommet--;
    pgraphe_t elem = p -> Tab[ p->sommet ];
    return elem;
  }
  else return NULL;
}

int empiler (ppile_t p, pgraphe_t pn)
  {
    if ( !( pile_pleine( p ) ) ){

      p -> Tab[ p -> sommet ] = pn;
      p -> sommet++;
      return 1;
    }
    else return 0;
}
