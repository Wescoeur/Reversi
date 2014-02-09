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

  (void)argc;
  (void)argv;

  reversi = reversi_new();
  reversi_print(reversi);
  reversi_set_move(reversi, 'X');
  reversi_print(reversi);

  reversi_free(reversi);

  exit(EXIT_SUCCESS);
}
