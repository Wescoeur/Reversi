/* ---------------------------------------------------------------------- */
/* Filename: main.c                                                       */
/* Authors: ABHAMON Ronan - HIVERT Kevin                                  */
/* Date: 2014-02-06 - 23:58:19                                            */
/*                                                                        */
/* ---------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>
#include <errno.h>
#include "config.h"
#include "reversi.h"
#include "ia.h"

/** Nom du fichier de configuration. */
#define CONFIG_FILENAME "config"

static volatile int run = 1;
static jmp_buf jbuf;

void handler(int sig)
{
  (void)sig;
  run = 0;
  longjmp(jbuf, 1);

  return;
}

int main(void)
{
  Player player = PLAYER_1; /* Joueur courant. */
  Pos pos;                  /* Position coup. */

  Reversi *reversi;   /* Grille de jeu. */
  Config *config;     /* Config serveur. */
  TCP *socket = NULL; /* Socket client. */

  /* Initialisation des sockets. */
  INIT_SOCKET();

  /* Création d'une grille de jeu. */
  reversi = reversi_new();

  /* Mise en place d'un signal d'interruption. */
  signal(SIGINT, handler);

  /* Chargement de la configuration du client. */
  config = config_load(CONFIG_FILENAME);

#if 0
  /* Ouverture d'une socket client. */
  if((socket = tcp_get(&config->ip)) == NULL)
  {
    perror("Socket");
    exit(EXIT_FAILURE);
  }

  /* Envoie du nom du client. */
  printf("Send client-name... (%d)\n", tcp_send(socket, config->name, PLAYER_NAME_SIZE));
#endif

  /* Libération de la configuration. */
  free(config);

  /* Point de sauvegarde de la pile pour un éventuel SIGINT. */
  setjmp(jbuf);

  while(run)
  {
    reversi_print(reversi);
    reversi_set_player_move(reversi, player);
    reversi_print(reversi);

    pos = ia_alphabeta(reversi, INV_PLAYER(player), 5);
    reversi_set_ia_move(reversi, INV_PLAYER(player), &pos);
    printf("L'IA a joué : %c%d\n", pos.y + 'A' , pos.x + 1);
  }

  /* Libération de la grille. */
  reversi_free(reversi);

  /* Fermeture socket client. */
  tcp_close(socket);

  /* Fermeture des sockets. */
  END_SOCKET();

  exit(EXIT_SUCCESS);
}
