/* ---------------------------------------------------------------------- */
/* Filename: reversi.c                                                    */
/* Authors: ABHAMON Ronan - HIVERT Kevin                                  */
/* Date: 2014-02-06 - 23:58:52                                            */
/*                                                                        */
/* ---------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "strplus.h"
#include "reversi.h"

#define __RVALUE__(R, X, Y) (R)->array[REVERSI_WIDTH * (Y) + X]

/* Une grille de Reversi. */
struct Reversi
{
  char *array;          /* Tableau de cases de la grille. */
  unsigned int score_1; /* Score du joueur 1. */
  unsigned int score_2; /* Score du joueur 2. */

  unsigned int n_moves; /* Nombre de mouvements restants. */
};

Reversi *reversi_new(void)
{
  Reversi *r;

  if((r = malloc(sizeof *r)) == NULL)
    return NULL;

  if((r->array = calloc(REVERSI_WIDTH * REVERSI_HEIGHT, sizeof(char))) == NULL)
  {
    free(r);
    return NULL;
  }

  r->score_1 = 0;
  r->score_2 = 0;
  r->n_moves = REVERSI_WIDTH * REVERSI_HEIGHT;

  return r;
}

void reversi_free(Reversi *reversi)
{
  free(reversi->array);
  free(reversi);

  return;
}

#define __REVERSI_DRAW_LINE__(I)       \
  do {                                 \
    printf("\n    +");                 \
                                       \
    for(I = 1; I < REVERSI_WIDTH; I++) \
      printf("---+");                  \
                                       \
    printf("---+\n");                  \
  } while(0)                           \

#define __REVERSI_DRAW_NUMBER__(I)      \
  do {                                  \
    printf("     ");                    \
                                        \
    for(I = 1; I <= REVERSI_WIDTH; I++) \
      printf(" %d  ", I);               \
  } while(0)

void reversi_print(Reversi *reversi)
{
  unsigned int i, j, k = 0;
  char c = 'A';

  printf("\n");
  __REVERSI_DRAW_NUMBER__(i);
  __REVERSI_DRAW_LINE__(i);

  for(j = 1; j <= REVERSI_HEIGHT; j++)
  {
    printf("  %c |", c);

    for(i = 1; i <= REVERSI_WIDTH; i++, k++)
        printf(" %c |", reversi->array[k] ? reversi->array[k] : ' ');

    printf(" %c", c++);

    if(j == REVERSI_HEIGHT / 2)
      printf("    PLAYER 1: %u", reversi->score_1);
    if(j == REVERSI_HEIGHT / 2 + 1)
      printf("    PLAYER 2: %u", reversi->score_2);

    __REVERSI_DRAW_LINE__(i);
  }

  __REVERSI_DRAW_NUMBER__(i);
  printf("\n\n");

  return;
}

/** Teste si un coup joué dans la grille est valide.
    Modifie le buffer en conséquence auu bon format. */
/* @param buf : Buffer obtenue par le biais de reversi_set_next_move. */
/* @return 0 si le coup est valide, -1 sinon. */
int __is_a_valide_move__(char *buf)
{
  char eval[2];
  char *p, c = 0, l = 0;

  /* Parcours du buffer, on recherche exactement une lettre et un chiffre. */
  for(p = buf; *p != '\0'; p++)
  {
    if(*p >= '1' && *p <= '0' + REVERSI_WIDTH)
    {
      if(c++) return -1; /* Un Chiffre déjà trouvé. */
      eval[1] = *p;
    }
    else if((*p >= 'A' && *p <= 'A' + REVERSI_HEIGHT) ||
            (*p >= 'a' && *p <= 'a' + REVERSI_HEIGHT))
    {
      if(l++) return -1; /* Une lettre déjà trouvée. */
      eval[0] = toupper(*p);
    }
    else if(!isspace(*p))
      return -1;
  }

  if(c && l)
  {
    buf[0] = eval[0] - 'A';
    buf[1] = eval[1] - '0' - 1;

    return 0; /* Format respecté. */
  }

  return -1;
}

int reversi_set_move(Reversi *reversi, Player player)
{
  char buf[10];

  if(reversi->n_moves == 0)
    return -1;

  printf("Move: ");

  for(;;)
  {
    mygets(buf, 10);

    if(__is_a_valide_move__(buf) == 0)
    {
      __RVALUE__(reversi, buf[1], buf[0]) = player;
      break;
    }

    printf("Wrong move ! Re-try: ");
  }

  return 0;
}
