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
  Player player = PLAYER_2;
  Pos *pos = malloc(sizeof *pos);
  
  (void)argc;
  (void)argv;

  reversi = reversi_new();
  reversi_print(reversi);
  
  /*
   * test.
    */
  for(;;)
  {
    ia_eval(reversi, player, pos, 0);
    reversi_set_ia_move(reversi, player, pos);
    reversi_print(reversi);
    printf("L'IA a joué :%c %d\n", pos->y + 'a' , pos->x + 1);

    reversi_set_player_move(reversi, INV_PLAYER(player));
    reversi_print(reversi);
  }

  reversi_free(reversi);
  free(pos);

  exit(EXIT_SUCCESS);
}
