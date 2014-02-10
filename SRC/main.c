/* ---------------------------------------------------------------------- */
/* Filename: main.c                                                       */
/* Authors: ABHAMON Ronan - HIVERT Kevin                                  */
/* Date: 2014-02-06 - 23:58:19                                            */
/*                                                                        */
/* ---------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include "reversi.h"

int main(int argc, char *argv[])
{
  Reversi *reversi;
  Player player = PLAYER_2;

  (void)argc;
  (void)argv;

  reversi = reversi_new();
  reversi_print(reversi);

  for(;;)
  {
    reversi_set_player_move(reversi, player = INV_PLAYER(player));
    reversi_print(reversi);
  }

  reversi_free(reversi);

  exit(EXIT_SUCCESS);
}
