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
static const int grid[] = {600, -150, 30, 10, 10, 30, -150, 600,
                           -150, -250, 0, 0, 0, 0, -250, -150,
                           30, 0, 1, 2, 2, 1, 0, 30,
                           10, 0, 2, 16, 16, 2, 0, 10,
                           10, 0, 2, 16, 16, 2, 0, 10,
                           10, 0, 1, 2, 2, 1, 0, 30,
                           -150, -250, 0, 0, 0, 0, -250, -150,
                           600, -150, 30, 10, 10, 30, -150, 600};

/** Copie une grille de jeu. */
/* @param reversi : Grille à copier. */
/* @return : Une copie de grille. */
static Reversi *__grid_copy(Reversi *reversi)
{
  Reversi *cpy = reversi_new();
  memcpy(cpy->array, reversi->array, REVERSI_SIZE * REVERSI_SIZE * sizeof(char));
  return cpy;
}

/*
 * Grid evaluation with the grid[] table (see above).
  */
int ia_eval_grid(Reversi *reversi, Player player)
{
  int i, n = 0;

  for(i = 0; i < REVERSI_SIZE * REVERSI_SIZE; i++)
    if(reversi->array[i] == player)
      n += grid[i];
    else if(reversi->array[i] == INV_PLAYER(player))
      n -= grid[i];

  return n;
}

/*
 * AI function but with alpha-beta pruning.
  */
Pos ia_alphabeta (Reversi *reversi, Player player, int depth)
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
        temp = ia_alphabeta_bis(reversiCpy, depth - 1, alpha, beta, INV_PLAYER(player), 0);
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

int ia_alphabeta_bis (Reversi *reversi, int depth, int alpha, int beta, Player player, int maximizingPlayer)
{
  int i,j;
  Reversi *reversiCpy = NULL;
  Pos playedPos;

  if (depth == 0 || reversi_game_over(reversi))
  {
    return ia_eval_grid(reversi, player);
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
      reversi_free(reversiCpy);
    }
  }
  return beta;
}


/*
 * Grid evaluation v 2.0
  */
int ia_eval_grid_2(Reversi *reversi, Player player)
{
  int pos, pos_bis, n = 0, c;
  int GGood = 512, Ggood = 32, /*ggood = 16,*/ good = 8, /*not_bad = 0,*/ bad = -256, Bad = -1024;
  Player player2 = INV_PLAYER(player);

  for (pos = 0; pos < REVERSI_SIZE * REVERSI_SIZE; ++pos)
  {
    if (reversi->array[pos] == player)
    {
      /*
       * If the cell is a corner, it's super good if the player own it.
        */
      if (pos == TOP_LEFT_CORNER || pos == TOP_RIGHT_CORNER || pos == BOTTOM_LEFT_CORNER || pos == BOTTOM_RIGHT_CORNER)
      {
        n += 500;
      }

      /*
       * Else, we need to do some calculation pos is a position just before a corner.
       * 'Cause it's a dangerous place. (Very dangerous... really)
        */

      /*
       * TOP LEFT CORNER.
       * Line.
        */
      else if (pos == TOP_LEFT_CORNER + 1)
      {
        /*
         * We need to check the corner.
          */
        if (reversi->array[TOP_LEFT_CORNER] == player)
        {
          n += Ggood;
        } else {
          /*
           * In that case we need to check what is at the end of the line.
           * c count the number of disk between pos end the end of the line.
            */
          for (pos_bis = pos + 1, c = 1;
            pos_bis <= TOP_RIGHT_CORNER && reversi->array[pos_bis] == player;
            ++pos_bis, ++c);

          /*
           * If the player own all the line, it's good.
            */
          if (pos_bis > TOP_RIGHT_CORNER)
          {
            n += GGood;
          } else {
            /*
             * If at the end of the line there is a disk owned by the other player
             * we need to check if the corner is empty or not.
              */
            if (reversi->array[pos_bis] == player2)
            {
              if (reversi->array[TOP_LEFT_CORNER] == player2)
              {
                n += GGood;
              } else {
                n += (c * Bad);
              }
            } else {
              /*
               * Else, it's just not a good position but
               * we check if it's a very bad position or just a bad position.
                */
              if (reversi->array[TOP_LEFT_CORNER] == player2)
              {
                n += (c * Bad);
              } else {
                n += (c * bad);
              }
            }
          }
        }
      }

      /*
       * TOP LEFT CORNER.
       * Column.
        */
      else if (pos == TOP_LEFT_CORNER + REVERSI_SIZE)
      {
        if (reversi->array[TOP_LEFT_CORNER] == player)
        {
          n += Ggood;
        } else {
          for (pos_bis = pos + REVERSI_SIZE, c = 1;
            pos_bis <= BOTTOM_LEFT_CORNER && reversi->array[pos_bis] == player;
            pos_bis += REVERSI_SIZE, ++c);

          if (pos_bis > BOTTOM_LEFT_CORNER)
          {
            n += GGood;
          } else {
            if (reversi->array[pos_bis] == player2)
            {
              if (reversi->array[TOP_LEFT_CORNER] == player2)
              {
                n += GGood;
              } else {
                n += (c * Bad);
              }
            } else {
              if (reversi->array[TOP_LEFT_CORNER] == player2)
              {
                n += (c * Bad);
              } else {
                n += (c * bad);
              }
            }
          }
        }
      }

      /*
       * TOP LEFT CORNER.
       * Diagonal.
        */
      else if (pos == TOP_LEFT_CORNER + REVERSI_SIZE + 1)
      {
        if (reversi->array[TOP_LEFT_CORNER] == player)
        {
          n += good;
        } else {
          n += Bad;
        }
      }

      /*
       * TOP RIGHT CORNER.
       * Line.
        */
      else if (pos == TOP_RIGHT_CORNER - 1)
      {
        if (reversi->array[TOP_RIGHT_CORNER] == player)
        {
          n += Ggood;
        } else {
          for (pos_bis = pos - 1, c = 1;
            pos_bis >= TOP_LEFT_CORNER && reversi->array[pos_bis] == player;
            --pos_bis, ++c);

          if (pos_bis < TOP_LEFT_CORNER)
          {
            n += GGood;
          } else {
            if (reversi->array[pos_bis] == player2)
            {
              if (reversi->array[TOP_RIGHT_CORNER] == player2)
              {
                n += GGood;
              } else {
                n += (c * Bad);
              }
            } else {
              if (reversi->array[TOP_RIGHT_CORNER] == player2)
              {
                n += (c * Bad);
              } else {
                n += (c * bad);
              }
            }
          }
        }
      }

      /*
       * TOP RIGHT CORNER.
       * Column.
        */
      else if (pos == TOP_RIGHT_CORNER + REVERSI_SIZE)
      {
        if (reversi->array[TOP_RIGHT_CORNER] == player)
        {
          n += Ggood;
        } else {
          for (pos_bis = pos + REVERSI_SIZE, c = 1;
            pos_bis <= BOTTOM_RIGHT_CORNER && reversi->array[pos_bis] == player;
            pos_bis += REVERSI_SIZE , ++c);

          if (pos_bis > BOTTOM_RIGHT_CORNER)
          {
            n += GGood;
          } else {
            if (reversi->array[pos_bis] == player2)
            {
              if (reversi->array[TOP_RIGHT_CORNER] == player2)
              {
                n += GGood;
              } else {
                n += (c * Bad);
              }
            } else {
              if (reversi->array[TOP_RIGHT_CORNER] == player2)
              {
                n += (c * Bad);
              } else {
                n += (c * bad);
              }
            }
          }
        }
      }

      /*
       * TOP RIGHT CORNER.
       * Diagonal.
        */
      else if (pos == TOP_RIGHT_CORNER + REVERSI_SIZE - 1)
      {
        if (reversi->array[TOP_RIGHT_CORNER] == player)
        {
          n += good;
        } else {
          n += Bad;
        }
      }

      /*
       * BOTTOM LEFT CORNER
       * Line.
        */
      else if (pos == BOTTOM_LEFT_CORNER + 1)
      {
        if (reversi->array[BOTTOM_LEFT_CORNER] == player)
        {
          n += Ggood;
        } else {
          for (pos_bis = pos + 1, c = 1;
            pos_bis <= BOTTOM_RIGHT_CORNER && reversi->array[pos_bis] == player;
            ++pos_bis, ++c);

          if (pos_bis > BOTTOM_RIGHT_CORNER)
          {
            n += GGood;
          } else {
            if (reversi->array[pos_bis] == player2)
            {
              if (reversi->array[BOTTOM_LEFT_CORNER] == player2)
              {
                n += GGood;
              } else {
                n += (c * Bad);
              }
            } else {
              if (reversi->array[BOTTOM_LEFT_CORNER] == player2)
              {
                n += (c * Bad);
              } else {
                n += (c * bad);
              }
            }
          }
        }
      }

      /*
       * BOTTOM LEFT CORNER.
       * Column.
        */
      else if (pos == BOTTOM_LEFT_CORNER - REVERSI_SIZE)
      {
        if (reversi->array[BOTTOM_LEFT_CORNER] == player)
        {
          n += Ggood;
        } else {
          for (pos_bis = pos - REVERSI_SIZE, c = 1;
            pos_bis >= TOP_LEFT_CORNER && reversi->array[pos_bis] == player;
            pos_bis -= REVERSI_SIZE, ++c);

          if (pos_bis < BOTTOM_LEFT_CORNER)
          {
            n += GGood;
          } else {
            if (reversi->array[pos_bis] == player2)
            {
              if (reversi->array[TOP_LEFT_CORNER] == player2)
              {
                n += GGood;
              } else {
                n += (c * Bad);
              }
            } else {
              if (reversi->array[TOP_LEFT_CORNER] == player2)
              {
                n += (c * Bad);
              } else {
                n += (c * bad);
              }
            }
          }
        }
      }

      /*
       * BOTTOM LEFT CORNER.
       * Diagonal.
        */
      else if (pos == BOTTOM_LEFT_CORNER - REVERSI_SIZE + 1)
      {
        if (reversi->array[BOTTOM_LEFT_CORNER] == player)
        {
          n += good;
        } else {
          n += Bad;
        }
      }

      /*
       * BOTTOM RIGHT CORNER
       * Line.
        */
      else if (pos == BOTTOM_RIGHT_CORNER - 1)
      {
        if (reversi->array[BOTTOM_RIGHT_CORNER] == player)
        {
          n += Ggood;
        } else {
          for (pos_bis = pos - 1, c = 1;
            pos_bis >= BOTTOM_LEFT_CORNER && reversi->array[pos_bis] == player;
            --pos_bis, ++c);

          if (pos_bis < BOTTOM_LEFT_CORNER)
          {
            n += GGood;
          } else {
            if (reversi->array[pos_bis] == player2)
            {
              if (reversi->array[BOTTOM_RIGHT_CORNER] == player2)
              {
                n += GGood;
              } else {
                n += (c * Bad);
              }
            } else {
              if (reversi->array[BOTTOM_RIGHT_CORNER] == player2)
              {
                n += (c * Bad);
              } else {
                n += (c * bad);
              }
            }
          }
        }
      }

      /*
       * BOTTOM RIGHT CORNER.
       * Column.
        */
      else if (pos == BOTTOM_RIGHT_CORNER - REVERSI_SIZE)
      {
        if (reversi->array[BOTTOM_RIGHT_CORNER] == player)
        {
          n += Ggood;
        } else {
          for (pos_bis = pos - REVERSI_SIZE, c = 1;
            pos_bis >= TOP_RIGHT_CORNER && reversi->array[pos_bis] == player;
            pos_bis -= REVERSI_SIZE, ++c);

          if (pos_bis < TOP_RIGHT_CORNER)
          {
            n += GGood;
          } else {
            if (reversi->array[pos_bis] == player2)
            {
              if (reversi->array[BOTTOM_RIGHT_CORNER] == player2)
              {
                n += GGood;
              } else {
                n += (c * Bad);
              }
            } else {
              n += (c * Bad);
            }
          }
        }
      }

      /*
       * BOTTOM RIGHT CORNER.
       * Diagonal.
        */
      else if (pos == BOTTOM_RIGHT_CORNER - REVERSI_SIZE - 1)
      {
        if (reversi->array[BOTTOM_RIGHT_CORNER] == player)
        {
          n += good;
        } else {
          n += Bad;
        }
      }

      /*
       * Else we need to some calculation if it's on a border.
       * And if not we need to calculate the Degree of freedom.
        */
      else {
        n += good;
      }

      /*
       * End of the loop.
        */
    }
  }
  return n;
}
