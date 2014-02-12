/* ---------------------------------------------------------------------- */
/* Filename: main.c                                                       */
/* Authors: ABHAMON Ronan - HIVERT Kevin                                  */
/* Date: 2014-02-06 - 23:58:19                                            */
/*                                                                        */
/* ---------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include "reversi.h"
#include "ia.h"

int main(int argc, char *argv[])
{
  Reversi *reversi;
  Player player = PLAYER_1;
  Pos pos;
  
  (void)argc;
  (void)argv;

  reversi = reversi_new();
  
  for(;;)
  {
    reversi_print(reversi);
    reversi_set_player_move(reversi, player);
    reversi_print(reversi);

    pos = ia_eval(reversi, INV_PLAYER(player));
    reversi_set_ia_move(reversi, INV_PLAYER(player), &pos);
    printf("L'IA a joué : %c%d\n", pos.y + 'A' , pos.x + 1);
  }

  reversi_free(reversi);

  exit(EXIT_SUCCESS);
}
