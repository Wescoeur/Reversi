/* ---------------------------------------------------------------------- */
/* Filename: negamax.c                                                    */
/* Author: ABHAMON Ronan                                                  */
/* Date: 2014-04-24 - 00:16:52                                            */
/*                                                                        */
/* ---------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "negamax.h"
#include "reversi.h"
#include "ia.h"

static Reversi *__grid_copy(Reversi *reversi)
{
  Reversi *cpy = reversi_new();
  memcpy(cpy->array, reversi->array, REVERSI_SIZE * REVERSI_SIZE * sizeof(char));
  cpy->n_moves = reversi->n_moves;

  return cpy;
}

int ia_negamax(Reversi *reversi, Player player, int depth, int alpha, int beta, Pos *pos)
{
  int i, j, temp;
  Reversi *reversiCpy = NULL;
  Pos playedPos, bestPos;

  if(depth <= 0 || reversi_game_over(reversi))
  {
    return ia_eval_grid(reversi, player);
  }

  bestPos.x = -1;
  bestPos.y = -1;

  if(depth == 7)
  {
    pos->x = -1;
    pos->y = -1;
  }

  for(j = 0; j < REVERSI_SIZE; j++)
  {
    for(i = 0; i < REVERSI_SIZE; i++)
    {
      playedPos.x = i;
      playedPos.y = j;

      reversiCpy = __grid_copy(reversi);

      if(reversi_set_ia_move(reversiCpy, player, &playedPos))
      {
        temp = ia_negamax(reversiCpy, INV_PLAYER(player), depth - 1, -beta, -alpha, pos);

        if (temp >= alpha)
        {
          alpha = temp;

          bestPos.x = -1;
          bestPos.y = -1;

          if(depth == 7)
          {
            pos->x = i;
            pos->y = j;
          }

          if (alpha >= beta)
          {
            if(depth == 7)
            {
              pos->x = i;
              pos->y = j;
            }

            reversi_free(reversiCpy);
            return alpha;
          }
        }
      }

      reversi_free(reversiCpy);
    }
  }

  return alpha;
}
