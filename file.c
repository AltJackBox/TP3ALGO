#include <stdlib.h>
#include "file.h"

pfile_t creer_file ()
{
  pfile_t newFile = malloc(sizeof(file_t));
  newFile->tete = 0;
  newFile->queue = -1;
  return newFile;
}

int detruire_file (pfile_t f)
{
  free(f);
  return 0 ;
}

int file_vide (pfile_t f)
{
  return (f->queue == -1);
}

int file_pleine (pfile_t f)
  {
  return (f->tete == f->queue);
}

pgraphe_t defiler (pfile_t f)
  {
  if (!file_vide(f)) {
    pgraphe_t newPsommet;
    newPsommet = f->Tab[f->tete];
    f->tete = (f->tete + 1) % MAX_FILE_SIZE;
    if (file_pleine(f)) {
      f->queue = -1;
    }
    return newPsommet;
  }
  return NULL ;
}

int enfiler (pfile_t f, pgraphe_t p)
{
  if (!file_pleine(f)) {
    if (file_vide(f)) {
      f->queue = f->tete;
    }
    f->Tab[f->queue] = p;
    f->queue = (f->queue + 1) % MAX_FILE_SIZE;
    return 1;
  }
  return 0 ;
}
