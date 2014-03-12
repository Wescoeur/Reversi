/* ---------------------------------------------------------------------- */
/* Filename: ia.c                                                         */
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

/** Grille d'évaluation de l'IA. */
static const int grid[] = {500, -150, 30, 10, 10, 30, -150, 500,
                           -150, -250, 0, 0, 0, 0, -250, -150,
                           30, 0, 1, 2, 2, 1, 0, 30,
                           10, 0, 2, 16, 16, 2, 0, 10,
                           10, 0, 2, 16, 16, 2, 0, 10,
                           10, 0, 1, 2, 2, 1, 0, 30,
                           -150, -250, 0, 0, 0, 0, -250, -150,
                           500, -150, 30, 10, 10, 30, -150, 500};


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

  /*if (game_over())
    {
    return INT_MAX;
    }*/

  /*
   * Copie du plateau pour simuler les coup.
   */
  reversiCpy = __grid_copy(reversi);

  for(j = 0; j < REVERSI_SIZE; j++)
  {
    for(i = 0; i < REVERSI_SIZE; i++)
    {
      playedPos.x = i;
      playedPos.y = j;
      if (reversi_set_ia_move(reversiCpy, player, &playedPos))
      {
        if((score = ia_eval_min(reversiCpy, player, depth)) > bestScore)
        {
          bestScore = score;
          bestPos.x = i;
          bestPos.y = j;
        }
      }
    }
  }

  reversi_free(reversiCpy);
  return bestPos;
}

int ia_eval_min (Reversi *reversi, Player player, int depth)
{
  int i, j, minScore = INT_MAX, score;
  Pos playedPos;
  Reversi *reversiCpy = NULL;
  if (depth == 0)
  {
    return ia_eval_grid(reversi, player);
  }

  reversiCpy = __grid_copy(reversi);

  for(j = 0; j < REVERSI_SIZE; j++)
  {
    for(i = 0; i < REVERSI_SIZE; i++)
    {
      playedPos.x = i;
      playedPos.y = j;
      if (reversi_set_ia_move(reversiCpy, player, &playedPos))
      {
        if((score = ia_eval_max(reversiCpy, INV_PLAYER(player), depth - 1)) < minScore)
        {
          minScore = score;
        }
      }
    }
  }
  return minScore;
}

int ia_eval_max(Reversi *reversi, Player player, int depth)
{
  int i, j, bestScore = INT_MAX, score;
  Pos playedPos;
  Reversi *reversiCpy = NULL;
  if (depth == 0)
  {
    return ia_eval_grid(reversi, player);
  }

  reversiCpy = __grid_copy(reversi);

  for(j = 0; j < REVERSI_SIZE; j++)
  {
    for(i = 0; i < REVERSI_SIZE; i++)
    {
      playedPos.x = i;
      playedPos.y = j;
      if (reversi_set_ia_move(reversiCpy, player, &playedPos))
      {
        if((score = ia_eval_min(reversiCpy, INV_PLAYER(player), depth - 1)) > bestScore)
        {
          bestScore = score;
        }
      }
    }
  }
  return bestScore;
}

int ia_eval_grid(Reversi *reversi, Player player)
{
  int i, j, pos, n = 0;
  Player player2 = INV_PLAYER(player);

  for(j = 0; j < REVERSI_SIZE; j++)
  {
    for(i = 0; i < REVERSI_SIZE; i++)
    {
      pos = POS(i, j);
      if (reversi->array[pos] == player)
      {
        n += grid[pos];
      } else if (reversi->array[pos] == player2) {
        n -= grid[pos];
      }
    }
  }
  return n;
}
