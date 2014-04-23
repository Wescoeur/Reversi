/* ---------------------------------------------------------------------- */
/* Filename: main.c                                                       */
/* Authors: ABHAMON Ronan - HIVERT Kevin                                  */
/* Date: 2014-02-06 - 23:58:19                                            */
/*                                                                        */
/* ---------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <setjmp.h>
#include <errno.h>
#include "config.h"
#include "reversi.h"
#include "ia.h"

/** Nom du fichier de configuration. */
#define CONFIG_FILENAME "config"

/** Niveau de l'IA. */
#define IA_LEVEL 5

static volatile int run = 1;
static jmp_buf jbuf;

/** Attrape le signal SIGINT. */
void handler(int sig);

/** Lance une partie IA contre Joueur. */
void ia_vs_player(void);

/** Lance une partie IA contre IA. */
void ia_vs_ia(void);

/* ---------------------------------------------------------------------- */

int main(int argc, char *argv[])
{
  if(argc > 1)
  {
    if(strcmp(argv[1], "ia") == 0)
      ia_vs_ia();
    else if(strcmp(argv[1], "player") == 0)
      ia_vs_player();
  }

  fprintf(stderr, "Usage: ia | player\n");
  exit(EXIT_FAILURE);
}

/* ---------------------------------------------------------------------- */

void handler(int sig)
{
  (void)sig;
  run = 0;
  longjmp(jbuf, 1);

  return;
}

void ia_vs_player(void)
{
  Player player = PLAYER_1; /* Joueur courant. */
  Pos pos;                  /* Position coup. */

  Reversi *reversi; /* Grille de jeu. */
  int finish;       /* Détermine si le jeu peut continuer. */

  /* Création d'une grille de jeu. */
  reversi = reversi_new();

  /* Mise en place d'un signal d'interruption. */
  signal(SIGINT, handler);

  /* Point de sauvegarde de la pile pour un éventuel SIGINT. */
  setjmp(jbuf);

  while(run)
  {
    finish = 2;
    reversi_print(reversi);

    /* Coup du joueur. */
    if(reversi_set_player_move(reversi, player) == -1)
    {
      printf("Aucun coup n'est jouable pour le joueur %c.\n", player);
      finish--;
    }
    else
      reversi_print(reversi);

    /* Coup de l'IA. */
    pos = ia_alphabeta(reversi, INV_PLAYER(player), IA_LEVEL);

    if(pos.x == -1 && pos.y == -1)
    {
      printf("Aucun coup n'est jouable pour le joueur %c.\n", INV_PLAYER(player));
      finish--;
    }
    else
    {
      printf("L'IA a jouée : %d%c\n", pos.y + 1, pos.x + 'A');
      reversi_set_ia_move(reversi, INV_PLAYER(player), &pos);
    }

    /* Grille pleine ou non ? */
    if(reversi->n_moves == 0 || finish == 0)
      break;
  }

  reversi_print(reversi);
  printf("Partie terminée.\n");

  /* Libération de la grille. */
  reversi_free(reversi);

  exit(EXIT_SUCCESS);
}

void ia_vs_ia(void)
{
  Pos pos; /* Position coup. */

  Reversi reversi;    /* Grille de jeu. */
  Config *config;     /* Config serveur. */
  TCP *socket = NULL; /* Socket client. */

  char buffer[66]; /* Données socket. */

  /* Initialisation des sockets. */
  INIT_SOCKET();

  /* Chargement de la configuration du client. */
  config = config_load(CONFIG_FILENAME);

  /* Ouverture d'une socket client. */
  if((socket = tcp_get(&config->ip)) == NULL)
  {
    perror("Socket");
    free(config);

    exit(EXIT_FAILURE);
  }

  /* Envoie du nom du client. */
  printf("Send client-name... (%d)\n", tcp_send(socket, config->name, PLAYER_NAME_SIZE));

  /* Libération de la configuration. */
  free(config);

  /* Mise en place d'un signal d'interruption. */
  signal(SIGINT, handler);

  /* Point de sauvegarde de la pile pour un éventuel SIGINT. */
  setjmp(jbuf);

  /* Départ de la grille de jeu par rapport au buffer. */
  reversi.array = buffer + 2;

  while(run)
  {
    /* Réception de la grille. */
    if(tcp_recv(socket, buffer, sizeof(buffer)) <= 0)
    {
      fprintf(stderr, "You killed me !\n");
      exit(EXIT_FAILURE);
    }

    if(*buffer == 'F')
      break; /* Fin de partie. */

    /* Coup à jouer. */
    pos = ia_alphabeta(&reversi, *buffer, 5);
    buffer[0] = POS(pos.x, pos.y);
    tcp_send(socket, buffer, 1);

    #ifdef DEBUG
      reversi_print(&reversi);
      printf("L'IA a jouée : %d%c\n", pos.y + 1, pos.x + 'A');
    #endif
  }

  /* Fermeture socket client. */
  tcp_close(socket);

  /* Fermeture des sockets. */
  END_SOCKET();

  exit(EXIT_SUCCESS);
}
