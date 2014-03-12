/* ---------------------------------------------------------------------- */
/* Filename: main.c                                                       */
/* Authors: ABHAMON Ronan - HIVERT Kevin                                  */
/* Date: 2014-02-06 - 23:58:19                                            */
/*                                                                        */
/* ---------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "reversi.h"
#include "ia.h"

/** Nom du fichier de configuration. */
#define CONFIG_FILENAME "config"

#ifdef __unix__
  #include <signal.h>
  #include <unistd.h>
  #include <string.h> /* memset */

  volatile int run = 1;

  void handler(int sig)
  {
    (void)sig;
    run = 0;
    printf("End...\n");

    return;
  }

  void set_handler(void)
  {
    struct sigaction sig;

    memset(&sig, 0, sizeof(sig));
    sig.sa_handler = handler;
    sigaction(SIGINT, &sig, NULL);

    return;
  }
#else
  int run = 1;
#endif

int main(void)
{
  Reversi *reversi;
  Player player = PLAYER_1;
  Pos pos;

  /* Initialisation des sockets. */
  INIT_SOCKET();

  /* Création d'une grille de jeu. */
  reversi = reversi_new();

  /* Gestion de la sortie du programme. Pour les systèmes UNIX.
     Windows n'a qu'à proposer une solution pour gérer les signaux posix... */
  #ifdef __unix__
    set_handler();
  #endif

  /* Ouverture d'une socket client. */

  while(run)
  {
    reversi_print(reversi);
    reversi_set_player_move(reversi, player);
    reversi_print(reversi);

    pos = ia_eval(reversi, INV_PLAYER(player), 5);
    reversi_set_ia_move(reversi, INV_PLAYER(player), &pos);
    printf("L'IA a joué : %c%d\n", pos.y + 'A' , pos.x + 1); run = 0;
  }

  printf("SORTIE\n");

  /* Libération de la grille. */
  reversi_free(reversi);

  /* Fermeture des sockets. */
  END_SOCKET();

  exit(EXIT_SUCCESS);
}
