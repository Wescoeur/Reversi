/* ---------------------------------------------------------------------- */
/* Filename: ia.c                                                         */
/* Authors: ABHAMON Ronan - HIVERT Kevin                                  */
/* Date: 2014-02-07 - 08:04:20                                            */
/*                                                                        */
/* ---------------------------------------------------------------------- */

#include <limits.h>
#include reversi.h
#include ia.h

/** Grille d'évaluation de l'IA. */
static const int grid[] = {500, -150, 30, 10, 10, 30, -150,  500,
                          -150, -250,  0,  0,  0,  0, -250, -150,
                            30,    0,  1,  2,  2,  1,    0,   30,
                            10,    0,  2, 16, 16,  2,    0,   10,
                            10,    0,  2, 16, 16,  2,    0,   10,
                            10,    0,  1,  2,  2,  1,    0,   30,
                          -150, -250,  0,  0,  0,  0, -250, -150,
                           500, -150, 30, 10, 10, 30, -150,  500};

void ia_eval(Reversi *reversi, Player player, Pos *pos, int depth)
{
	int i, j, bestScore, score;
	Reversi *reversi_save;
	memcpy(reversi_save, reversi, sizeof(Reversi));


	if (game_over())
	{
		return INT_MAX;
	} 

	if (depth == DEPTH_MAX)
	{
		return ia_eval_grid(reversi, player);
	}
	
	if (depth % 2 == 0)
	{
		bestScore = INT_MIN;
		for(j = 1; j <= REVERSI_HEIGHT; j++)
		{
			for(i = 1; i <= REVERSI_WIDTH; i++)
			{
				if (reversi_set_ia_move(reversi, player, POS(i, j)) == 0)
				{
					score = ia_eval(reversi, INV_PLAYER(player), pos, depth - 1);
					memcpy(reversi, reversi_save, sizeof(Reversi));
					if (score > bestScore)
					{
						bestScore = score;
						&pos = POS(i, j);
					}
				}
			}
		}
	} else {
		bestScore = INT_MAX;
		for(j = 1; j <= REVERSI_HEIGHT; j++)
		{
			for(i = 1; i <= REVERSI_WIDTH; i++)
			{
				if (reversi_set_ia_move(reversi, player, POS(i, j)) == 0)
				{
					score = ia_eval(reversi, INV_PLAYER(player), pos, depth - 1);
					memcpy(reversi, reversi_save, sizeof(Reversi));
					if (score < bestScore)
					{
						bestScore = score;
						&pos = POS(i, j);
					}
				}
			}
		}
	}
}

int ia_eval_grid (Reversi *reversi, Player player)
{
	int i, j, pos, n = 0;
  	Player player2 = INV_PLAYER(player);

	for(j = 1; j <= REVERSI_HEIGHT; j++)
	{
		for(i = 1; i <= REVERSI_WIDTH; i++)
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