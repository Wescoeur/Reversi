/* ---------------------------------------------------------------------- */
/* Filename: reversi.c                                                    */
/* Authors: ABHAMON Ronan - HIVERT Kevin                                  */
/* Date: 2014-02-06 - 23:58:52                                            */
/*                                                                        */
/* ---------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include "reversi.h"

#define __RVALUE__(R, X, Y) (R)->array[REVERSI_WIDTH * (Y) + X]

/* Une grille de Reversi. */
struct Reversi
{
  char *array;          /* Tableau de cases de la grille. */
  unsigned int score_1; /* Score du joueur 1. */
  unsigned int score_2; /* Score du joueur 2. */
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
