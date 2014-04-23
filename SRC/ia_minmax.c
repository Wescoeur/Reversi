/* ---------------------------------------------------------------------- */
/* Filename: ia_minmax.c                                                         */
/* Authors: ABHAMON Ronan - HIVERT Kevin                                  */
/* Date: 2014-02-07 - 08:04:20                                            */
/*                                                                        */
/* ---------------------------------------------------------------------- */

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include "reversi.h"
#include "ia.h"

#define MIN(A,B) (((A) < (B)) ? (A) : (B))
#define MAX(A,B) (((A) > (B)) ? (A) : (B))

/** Copie une grille de jeu. */
/* @param reversi : Grille à copier. */
/* @return : Une copie de grille. */
static Reversi *__grid_copy(Reversi *reversi)
{
  Reversi *cpy = reversi_new();
  memcpy(cpy->array, reversi->array, REVERSI_SIZE * REVERSI_SIZE * sizeof(char));
  return cpy;
}

Pos ia_eval(Reversi *reversi, Player player, int depth)
{
  int i, j, bestScore = INT_MIN, score;
  Pos playedPos;
  Pos bestPos;
  Reversi *reversiCpy = NULL;

  for(j = 0; j < REVERSI_SIZE; j++)
  {
    for(i = 0; i < REVERSI_SIZE; i++)
    {
      playedPos.x = i;
      playedPos.y = j;
      /*
       * Copie du plateau pour simuler les coup.
       */
      reversiCpy = __grid_copy(reversi);

      if (reversi_set_ia_move(reversiCpy, player, &playedPos))
      {
        if((score = ia_eval_min(reversiCpy, player, depth)) > bestScore)
        {
          bestScore = score;
          bestPos.x = i;
          bestPos.y = j;
        }
      }

      /*
       * Annulation du coup.
        */
      reversi_free(reversiCpy);
    }
  }

  return bestPos;
}

int ia_eval_min (Reversi *reversi, Player player, int depth)
{
  int i, j, minScore = INT_MAX, score;
  Pos playedPos;
  Reversi *reversiCpy = NULL;
  if (depth == 0 || reversi_game_over(reversi))
  {
    return ia_eval_grid(reversi, player);
  }

  for(j = 0; j < REVERSI_SIZE; j++)
  {
    for(i = 0; i < REVERSI_SIZE; i++)
    {
      playedPos.x = i;
      playedPos.y = j;

      reversiCpy = __grid_copy(reversi);
      if (reversi_set_ia_move(reversiCpy, player, &playedPos))
      {
        if((score = ia_eval_max(reversiCpy, INV_PLAYER(player), depth - 1)) < minScore)
        {
          minScore = score;
        }
      }
      reversi_free(reversiCpy);
    }
  }
  return minScore;
}

int ia_eval_max(Reversi *reversi, Player player, int depth)
{
  int i, j, bestScore = INT_MIN, score;
  Pos playedPos;
  Reversi *reversiCpy = NULL;
  if (depth == 0 || reversi_game_over(reversi))
  {
    return ia_eval_grid(reversi, player);
  }

  for(j = 0; j < REVERSI_SIZE; j++)
  {
    for(i = 0; i < REVERSI_SIZE; i++)
    {
      playedPos.x = i;
      playedPos.y = j;

      reversiCpy = __grid_copy(reversi);
      if (reversi_set_ia_move(reversiCpy, player, &playedPos))
      {
        if((score = ia_eval_min(reversiCpy, INV_PLAYER(player), depth - 1)) > bestScore)
        {
          bestScore = score;
        }
      }
      reversi_free(reversiCpy);
    }
  }
  return bestScore;
}
