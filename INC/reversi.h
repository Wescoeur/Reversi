/* ---------------------------------------------------------------------- */
/* Filename: reversi.h                                                    */
/* Authors: ABHAMON Ronan - HIVERT Kevin                                  */
/* Date: 2014-02-06 - 23:59:01                                            */
/*                                                                        */
/* ---------------------------------------------------------------------- */

#ifndef _REVERSI_H_
#define _REVERSI_H_

#include "pos.h"

/** Longueur d'un côté de la grille. */
#define REVERSI_SIZE 8

/** Joueurs. */
typedef char Player;

#define PLAYER_1 'X'
#define PLAYER_2 'O'

/** Inverse le joueur courant. */
#define INV_PLAYER(P) (P == PLAYER_1 ? PLAYER_2 : PLAYER_1)

/** Teste le début/fin d'une ligne/colonne. */
#define END_LEFT(P)  (P % REVERSI_SIZE == 0)
#define END_RIGHT(P) (P % REVERSI_SIZE == REVERSI_SIZE - 1)
#define END_UP(P)    (P / REVERSI_SIZE == 0)
#define END_DOWN(P)  (P / REVERSI_SIZE == REVERSI_SIZE - 1)

/** Valeurs des coins. */
#define TOP_LEFT_CORNER     0
#define TOP_RIGHT_CORNER    (REVERSI_SIZE - 1)
#define BOTTOM_LEFT_CORNER  (REVERSI_SIZE * (REVERSI_SIZE - 1))
#define BOTTOM_RIGHT_CORNER ((REVERSI_SIZE * REVERSI_SIZE) -1)

/** Position d'un pion en (x, y) */
#define POS(X, Y) (REVERSI_SIZE * (Y) + X)

/** Centre de la grille. */
#define REVERSI_CENTER (POS(REVERSI_SIZE / 2 - 1, REVERSI_SIZE / 2 - 1))

/* Une grille de Reversi. */
typedef struct Reversi
{
  char *array;          /* Tableau de cases de la grille. */
  unsigned int n_moves; /* Nombre de mouvements restants. */
} Reversi;

/** Crée une grille de jeu. */
/* @return : Une grille ou NULL en cas d'échec. */
Reversi *reversi_new(void);

/** Libère une grille de jeu. */
/* @param reversi : Grille à libérer. */
void reversi_free(Reversi *reversi);

/** Affiche une grille de jeu. */
/* @param reversi : Grille à afficher. */
void reversi_print(Reversi *reversi);

/** Teste si un coup est possible. */
/* @param reversi : Grille de jeu. */
/* @param player : Joueur courant. */
/* @param pos : Position du coup à tester. */
/* @return : 1 si le coup est possible, 0 sinon. */
int reversi_is_a_right_move(Reversi *reversi, Player player, Pos *pos);

/** Teste si il reste au moins un coup valide. */
/* @param reversi : Grille de jeu. */
/* @param player : Joueur en cours. */
/* @return : 1 si oui, 0 sinon. */
int reversi_exists_moves(Reversi *reversi, Player player);

/** Joue un coup dans la grille de jeu ; demande à l'utilisateur d'entrer son coup sur stdin. */
/* @param reversi : Grille où jouer. */
/* @param player : Joueur en cours. */
/* @return : 0 si un coup est jouable et a été joué, ou -1 si impossible de jouer. */
int reversi_set_player_move(Reversi *reversi, Player player);

/** Joue un coup dans la grille de jeu ; utilise un coup passé en argument. */
/* @param reversi : Grille où jouer. */
/* @param player : Joueur en cours. */
/* @return : 1 si un coup est jouable et a été joué, ou 0 si impossible de jouer. */
int reversi_set_ia_move(Reversi *reversi, Player player, Pos *pos);

/** Teste si la parti est terminée (plus de coup disponible pour les 2 joueurs) */
/* @param reversi : Grille de jeu. */
int reversi_game_over(Reversi *reversi);

#endif /* _REVERSI_H_ INCLUDED */
