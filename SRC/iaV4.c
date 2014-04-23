/* ---------------------------------------------------------------------- */
/* Filename: iaV4.c                                                         */
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

/** Copie une grille de jeu. */
/* @param reversi : Grille à copier. */
/* @return : Une copie de grille. */
static Reversi *__grid_copy(Reversi *reversi)
{
  Reversi *cpy = reversi_new();
  memcpy(cpy->array, reversi->array, REVERSI_SIZE * REVERSI_SIZE * sizeof(char));
  cpy->n_moves = reversi->n_moves;
  return cpy;
}

Pos iaV4_alphabeta (Reversi *reversi, Player player, int depth)
{
  int i, j, temp;
  Reversi *reversiCpy = NULL;
  Pos playedPos, bestPos;
  int alpha = INT_MIN, beta = INT_MAX;

  /*
   * (-1,-1) initialisation in case of AI have no right move.
   * So the returned position is still a valid position (we can access to x and y members).
    */
  bestPos.x = -1;
  bestPos.y = -1;

  for(j = 0; j < REVERSI_SIZE; j++)
  {
    for(i = 0; i < REVERSI_SIZE; i++)
    {
      playedPos.x = i;
      playedPos.y = j;

      reversiCpy = __grid_copy(reversi);
      if (reversi_set_ia_move(reversiCpy, player, &playedPos))
      {
        if (IS_CORNER(POS(i,j)))
        {
          bestPos.x = i;
          bestPos.y = j;
          reversi_free(reversiCpy);
          return bestPos;
        }
        temp = iaV4_alphabeta_bis(reversiCpy, depth - 1, alpha, beta, INV_PLAYER(player), 0);
        if (temp >= alpha)
        {
          alpha = temp;
          bestPos.x = i;
          bestPos.y = j;
        }
      }
      reversi_free(reversiCpy);
    }
  }
  return bestPos;
}


int iaV4_alphabeta_bis (Reversi *reversi, int depth, int alpha, int beta, Player player, int maximizingPlayer)
{
  int i,j;
  Reversi *reversiCpy = NULL;
  Pos playedPos;

  if (depth == 0 || reversi_game_over(reversi))
  {
    return iaV4_eval_grid(reversi, player);
  }

  if (maximizingPlayer)
  {
    for(j = 0; j < REVERSI_SIZE; j++)
    {
      for(i = 0; i < REVERSI_SIZE; i++)
      {
        playedPos.x = i;
        playedPos.y = j;

        reversiCpy = __grid_copy(reversi);
        if (reversi_set_ia_move(reversiCpy, player, &playedPos))
        {
          /*
           * If the position is playable, then we put a disk on this position and do
           * the recursiv call.
            */
          alpha = MAX(alpha, iaV4_alphabeta_bis(reversiCpy, depth - 1, alpha, beta, INV_PLAYER(player), 0));
          if (beta <= alpha)
          {
            /*
             * Beta cut-off.
              */
            reversi_free(reversiCpy);
            return alpha;
          }
        }
        reversi_free(reversiCpy);
      }
    }
    return alpha; 
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
        beta = MIN(beta, iaV4_alphabeta_bis(reversiCpy, depth - 1, alpha, beta, INV_PLAYER(player), 1));
        if (beta <= alpha)
        {
          /*
           * Alpha cut-off
            */
          reversi_free(reversiCpy);
          return beta;
        }
      }
      reversi_free(reversiCpy);
    }
  }
  return beta;
}

int __liberty_level(Reversi *reversi, int pos)
{
  int n = 0;
  if (reversi->array[pos - REVERSI_SIZE - 1] == 0)
    ++n;
  if (reversi->array[pos - REVERSI_SIZE] == 0)
    ++n;
  if (reversi->array[pos - REVERSI_SIZE + 1] == 0)
    ++n;
  if (reversi->array[pos - 1] == 0)
    ++n;
  if (reversi->array[pos + 1] == 0)
    ++n;
  if (reversi->array[pos + REVERSI_SIZE - 1] == 0)
    ++n;
  if (reversi->array[pos + REVERSI_SIZE] == 0)
    ++n;
  if (reversi->array[pos + REVERSI_SIZE + 1] == 0)
    ++n;
return n;
}

int iaV4_eval_grid(Reversi *reversi, Player player)
{
	int nb_moves = 1, x, nb_pawn = 1;
	Pos pos;

	if (reversi->n_moves <= 18 )
	{
		/* 10 derniers coups */
		for(pos.x = 0; pos.x < REVERSI_SIZE; pos.x++)
			for(pos.y = 0; pos.y < REVERSI_SIZE; pos.y++)
				if(reversi_is_a_right_move(reversi, INV_PLAYER(player), &pos))
					++nb_moves;

		return 1000 / nb_moves;
	} else if (reversi->n_moves >= REVERSI_SIZE * REVERSI_SIZE - 18 ) {
		/* 10 premiers coups */
		for(x = 0; x < REVERSI_SIZE * REVERSI_SIZE; x++)
			if(reversi->array[x] == player)
        nb_pawn += __liberty_level(reversi, x);

		return 1000 / nb_pawn;
	} else {
		for(x = 0; x < REVERSI_SIZE * REVERSI_SIZE; x++)
        if(reversi->array[x] == INV_PLAYER(player))
          ++nb_pawn;

    return ia_eval_grid(reversi, player) - nb_pawn;
	}
	return 1;
}