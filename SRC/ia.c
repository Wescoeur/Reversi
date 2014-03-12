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

#define MIN(A,B) (((A) < (B)) ? (A) : (B))
#define MAX(A,B) (((A) > (B)) ? (A) : (B))

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


/*
 * AI function but with alpha-beta pruning. 
  */
Pos ia_alphabeta (Reversi *reversi, int depth, Player player)
{
  int i, j, temp;
  Reversi *reversiCpy = NULL;
  Pos playedPos, bestPos;

  int alpha = INT_MIN, beta = INT_MAX;

  if (depth == 0)
  {
    fprintf(stderr, "How the fuck you want I return to you anything, if I can't do any fucking calculation ?!\n");
    exit(-1);
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
        temp = ia_alphabeta_bis(reversiCpy, depth - 1, alpha, beta, INV_PLAYER(player), 0);
        if (temp > alpha)
        {
          alpha = temp;
          bestPos.x = i;
          bestPos.y = j;
        }
      }
    }
  }
  reversi_free(reversiCpy);
  return bestPos;
}

int ia_alphabeta_bis (Reversi *reversi, int depth, int alpha, int beta, Player player, int maximizingPlayer)
{
  int i,j;
  Reversi *reversiCpy = NULL;
  Pos playedPos;

  if (depth == 0)
  {
    return ia_eval_grid(reversi, player);
  }

  reversiCpy = __grid_copy(reversi);

  if (maximizingPlayer)
  {
    for(j = 0; j < REVERSI_SIZE; j++)
    {
      for(i = 0; i < REVERSI_SIZE; i++)
      {
        playedPos.x = i;
        playedPos.y = j;
        if (reversi_set_ia_move(reversiCpy, player, &playedPos))
        {
          /*
           * If the position is playable, then we put a disk on this position and do
           * the recursiv call.
            */
          alpha = MAX(alpha, ia_alphabeta_bis(reversiCpy, depth - 1, alpha, beta, INV_PLAYER(player), 0));
          if (beta <= alpha)
          {
            /*
             * Beta cut-off.
              */
            reversi_free(reversiCpy);
            return alpha;
          }
        }
      }
    }
    reversi_free(reversiCpy);
    return alpha;
  }

  for(j = 0; j < REVERSI_SIZE; j++)
  {
    for(i = 0; i < REVERSI_SIZE; i++)
    {
      playedPos.x = i;
      playedPos.y = j;
      if (reversi_set_ia_move(reversiCpy, player, &playedPos))
      {
        beta = MIN(beta, ia_alphabeta_bis(reversiCpy, depth - 1, alpha, beta, INV_PLAYER(player), 1));
        if (beta <= alpha)
        {
          /*
           * Alpha cut-off
            */
          reversi_free(reversiCpy);
          return beta;
        }
      }
    }
  }
  reversi_free(reversiCpy);
  return beta;
}