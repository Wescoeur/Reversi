/* ---------------------------------------------------------------------- */
/* Filename: reversi.c                                                    */
/* Authors: ABHAMON Ronan - HIVERT Kevin                                  */
/* Date: 2014-02-06 - 23:58:52                                            */
/*                                                                        */
/* ---------------------------------------------------------------------- */

#include <ctype.h>
#include "alloc.h"
#include "strplus.h"
#include "reversi.h"

#define __RVALUE__(R, X, Y) (R)->array[POS(X, Y)]

Reversi *reversi_new(void)
{
  Reversi *r;
  int pos;

  pmalloc(r, sizeof *r);
  pcalloc(r->array, REVERSI_SIZE * REVERSI_SIZE, sizeof(char));

  r->score_1 = 0;
  r->score_2 = 0;
  r->n_moves = REVERSI_SIZE * REVERSI_SIZE;

  pos = REVERSI_CENTER;
  r->array[pos] = PLAYER_2;
  r->array[pos + 1] = PLAYER_1;
  r->array[pos + REVERSI_SIZE] = PLAYER_1;
  r->array[pos + REVERSI_SIZE + 1] = PLAYER_2;

  return r;
}

void reversi_free(Reversi *reversi)
{
  free(reversi->array);
  free(reversi);

  return;
}

#define __REVERSI_DRAW_LINE__(I)       \
  do {                                 \
    printf("\n    +");                 \
                                       \
    for(I = 1; I < REVERSI_SIZE; I++)  \
      printf("---+");                  \
                                       \
    printf("---+\n");                  \
  } while(0)                           \

#define __REVERSI_DRAW_NUMBER__(I)      \
  do {                                  \
    printf("     ");                    \
                                        \
    for(I = 1; I <= REVERSI_SIZE; I++)  \
      printf(" %d  ", I);               \
  } while(0)

void reversi_print(Reversi *reversi)
{
  unsigned int i, j, k = 0;
  char c = 'A';

  printf("\n");
  __REVERSI_DRAW_NUMBER__(i);
  __REVERSI_DRAW_LINE__(i);

  for(j = 1; j <= REVERSI_SIZE; j++)
  {
    printf("  %c |", c);

    for(i = 1; i <= REVERSI_SIZE; i++, k++)
        printf(" %c |", reversi->array[k] ? reversi->array[k] : ' ');

    printf(" %c", c++);

    if(j == REVERSI_SIZE / 2)
      printf("    PLAYER 1: %u", reversi->score_1);
    if(j == REVERSI_SIZE / 2 + 1)
      printf("    PLAYER 2: %u", reversi->score_2);

    __REVERSI_DRAW_LINE__(i);
  }

  __REVERSI_DRAW_NUMBER__(i);
  printf("\n\n");

  return;
}

int reversi_is_a_right_move(Reversi *reversi, Player player, Pos *pos)
{
  /* Position du pion potentiellement ajouté. */
  const int cpos = POS(pos->x, pos->y);
  int i;

  /* Joueur adverse. */
  Player player2 = INV_PLAYER(player);

  if(reversi->array[cpos] != 0)
    return 0;

  /* Horizontale 1. */
  if(!END_LEFT(cpos) && reversi->array[i = cpos - 1] == player2)
  {
    for(; !END_LEFT(i) && reversi->array[i] == player2; i--);
    if(reversi->array[i] == player) return 1;
  }

  /* Horizontale 2. */
  if(!END_RIGHT(cpos) && reversi->array[i = cpos + 1] == player2)
  {
    for(; !END_RIGHT(i) && reversi->array[i] == player2; i++);
    if(reversi->array[i] == player) return 1;
  }

  /* Verticale 1. */
  if(!END_UP(cpos) && reversi->array[i = cpos - REVERSI_SIZE] == player2)
  {
    for(; !END_UP(i) && reversi->array[i] == player2; i -= REVERSI_SIZE);
    if(reversi->array[i] == player) return 1;
  }

  /* Verticale 2. */
  if(!END_DOWN(cpos) && reversi->array[i = cpos + REVERSI_SIZE] == player2)
  {
    for(; !END_DOWN(i) && reversi->array[i] == player2; i += REVERSI_SIZE);
    if(reversi->array[i] == player) return 1;
  }

  /* Diagonale 1. */
  if(!END_LEFT(cpos) && !END_UP(cpos) && reversi->array[i = cpos - REVERSI_SIZE - 1] == player2)
  {
    for(; !END_LEFT(i) && !END_UP(i) && reversi->array[i] == player2; i -= REVERSI_SIZE + 1);
    if(reversi->array[i] == player) return 1;
  }

  /* Diagonale 2. */
  if(!END_RIGHT(cpos) && !END_DOWN(cpos) && reversi->array[i = cpos + REVERSI_SIZE + 1] == player2)
  {
    for(; !END_RIGHT(i) && !END_DOWN(i) && reversi->array[i] == player2; i += REVERSI_SIZE + 1);
    if(reversi->array[i] == player) return 1;
  }

  /* Diagonale 3. */
  if(!END_LEFT(cpos) && !END_DOWN(cpos) && reversi->array[i = cpos + REVERSI_SIZE - 1] == player2)
  {
    for(; !END_LEFT(i) && !END_DOWN(i) && reversi->array[i] == player2; i += REVERSI_SIZE - 1);
    if(reversi->array[i] == player) return 1;
  }

  /* Diagonale 4. */
  if(!END_RIGHT(cpos) && !END_UP(cpos) && reversi->array[i = cpos - REVERSI_SIZE + 1] == player2)
  {
    for(; !END_RIGHT(i) && !END_UP(i) && reversi->array[i] == player2; i -= REVERSI_SIZE - 1);
    if(reversi->array[i] == player) return 1;
  }

  return 0;
}

int reversi_exists_moves(Reversi *reversi, Player player)
{
  Pos pos;

  if(reversi->n_moves == 0)
    return 0;

  for(pos.x = 0; pos.x < REVERSI_SIZE; pos.x++)
    for(pos.y = 0; pos.y < REVERSI_SIZE; pos.y++)
      if(reversi_is_a_right_move(reversi, player, &pos))
        return 1;

  return 0;
}

/** Met à jour la grille de jeu. */
/* @param reversi : Grille de jeu. */
/* @param player : Joueur en cours. */
/* @param pos : Position où jouer. */
static void __update_game(Reversi *reversi, Player player, Pos *pos)
{
  /* Position du pion ajouté. */
  const int cpos = POS(pos->x, pos->y);
  int i;

  /* Joueur adverse. */
  Player player2 = INV_PLAYER(player);

  /* Placement du premier jeton. */
  reversi->array[cpos] = player;

  /* Horizontale 1. */
  if(!END_LEFT(cpos))
  {
    for(i = cpos - 1; !END_LEFT(i) && reversi->array[i] == player2; i--);

    if(reversi->array[i] == player)
      for(; i < cpos; reversi->array[++i] = player);
  }

  /* Horizontale 2. */
  if(!END_RIGHT(cpos))
  {
    for(i = cpos + 1; !END_RIGHT(i) && reversi->array[i] == player2; i++);

    if(reversi->array[i] == player)
      for(; i > cpos; reversi->array[--i] = player);
  }

  /* Verticale 1. */
  if(!END_UP(cpos))
  {
    for(i = cpos - REVERSI_SIZE; !END_UP(i) && reversi->array[i] == player2; i -= REVERSI_SIZE);

    if(reversi->array[i] == player)
      for(; i < cpos; reversi->array[i += REVERSI_SIZE] = player);
  }

  /* Verticale 2. */
  if(!END_DOWN(cpos))
  {
    for(i = cpos + REVERSI_SIZE; !END_DOWN(i) && reversi->array[i] == player2; i += REVERSI_SIZE);

    if(reversi->array[i] == player)
      for(; i > cpos; reversi->array[i -= REVERSI_SIZE] = player);
  }

  /* Diagonale 1. */
  if(!END_LEFT(cpos) && !END_UP(cpos))
  {
    for(i = cpos - REVERSI_SIZE - 1; !END_LEFT(i) && !END_UP(i) &&
          reversi->array[i] == player2; i -= REVERSI_SIZE + 1);

    if(reversi->array[i] == player)
      for(; i < cpos; reversi->array[i += REVERSI_SIZE + 1] = player);
  }

  /* Diagonale 2. */
  if(!END_RIGHT(cpos) && !END_DOWN(cpos))
  {
    for(i = cpos + REVERSI_SIZE + 1; !END_RIGHT(i) && !END_DOWN(i) &&
          reversi->array[i] == player2; i += REVERSI_SIZE + 1);

    if(reversi->array[i] == player)
      for(; i > cpos; reversi->array[i -= REVERSI_SIZE + 1] = player);
  }

  /* Diagonale 3. */
  if(!END_LEFT(cpos) && !END_DOWN(cpos))
  {
    for(i = cpos + REVERSI_SIZE - 1; !END_LEFT(i) && !END_DOWN(i) &&
          reversi->array[i] == player2; i += REVERSI_SIZE - 1);

    if(reversi->array[i] == player)
      for(; i > cpos; reversi->array[i -= REVERSI_SIZE - 1] = player);
  }

  /* Diagonale 4. */
  if(!END_RIGHT(cpos) && !END_UP(cpos))
  {
    for(i = cpos - REVERSI_SIZE + 1; !END_RIGHT(i) && !END_UP(i) &&
          reversi->array[i] == player2; i -= REVERSI_SIZE - 1);

    if(reversi->array[i] == player)
      for(; i < cpos; reversi->array[i += REVERSI_SIZE - 1] = player);
  }

  return;
}

/** Teste si un coup joué dans la grille est valide.
    Modifie le buffer en conséquence auu bon format. */
/* @param buf : Buffer obtenue par le biais de reversi_set_next_move. */
/* @return 0 si le coup est valide, -1 sinon. */
static int __is_a_valid_str(char *buf)
{
  char eval[2];
  char *p, c = 0, l = 0;

  /* Parcours du buffer, on recherche exactement une lettre et un chiffre. */
  for(p = buf; *p != '\0'; p++)
  {
    if(*p >= '1' && *p <= '0' + REVERSI_SIZE)
    {
      if(c++) return -1; /* Un Chiffre déjà trouvé. */
      eval[1] = *p;
    }
    else if((*p >= 'A' && *p < 'A' + REVERSI_SIZE) ||
            (*p >= 'a' && *p < 'a' + REVERSI_SIZE))
    {
      if(l++) return -1; /* Une lettre déjà trouvée. */
      eval[0] = toupper(*p);
    }
    else if(!isspace(*p))
      return -1;
  }

  if(c && l)
  {
    buf[0] = eval[0] - 'A';
    buf[1] = eval[1] - '0' - 1;

    return 0; /* Format respecté. */
  }

  return -1;
}

int reversi_set_player_move(Reversi *reversi, Player player)
{
  char buf[10];
  Pos pos;

  if(!reversi_exists_moves(reversi, player))
    return -1;

  printf("Move %c: ", player);

  for(;;)
  {
    mygets(buf, 10, stdin);

    if(__is_a_valid_str(buf) == 0)
    {
      pos.y = buf[0];
      pos.x = buf[1];

      /* Si le mouvement est possible, on joue. */
      if(reversi_is_a_right_move(reversi, player, &pos))
      {
        __update_game(reversi, player, &pos);
        break;
      }
    }

    printf("Wrong move ! Re-try: ");
  }

  return 0;
}

int reversi_set_ia_move(Reversi *reversi, Player player, Pos *pos)
{
/*  if(!reversi_exists_moves(reversi, player))
    return 0;*/

  /* Si le mouvement est possible, on joue. */
  if(reversi_is_a_right_move(reversi, player, pos))
  {
    __update_game(reversi, player, pos);
    return 1;
  }

  return 0;
}
