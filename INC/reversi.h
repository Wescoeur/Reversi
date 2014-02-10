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

#define INV_PLAYER(P) (P == PLAYER_1 ? PLAYER_2 : PLAYER_1)

/** Grille de jeu. */
typedef struct Reversi Reversi;

/** Crée une grille de jeu. */
/* @return : Une grille ou NULL en cas d'échec. */
Reversi *reversi_new(void);

/** Libère une grille de jeu. */
/* @param reversi : Grille à libérer. */
void reversi_free(Reversi *reversi);

/** Affiche une grille de jeu. */
/* @param reversi : Grille à afficher. */
void reversi_print(Reversi *reversi);

/** Teste si il reste au moins un coup valide. */
/* @param reversi : Grille de jeu. */
/* @param player : Joueur en cours. */
/* @return : 1 si oui, 0 sinon. */
int reversi_exist_moves(Reversi *reversi, Player player);

/** Joue un coup dans la grille de jeu ; demande à l'utilisateur d'entrer son coup sur stdin. */
/* @param reversi : Grille où jouer. */
/* @param player : Joueur en cours. */
/* @return : 0 si un coup est jouable et a été joué, ou -1 si impossible de jouer. */
int reversi_set_player_move(Reversi *reversi, Player player);

/** Joue un coup dans la grille de jeu ; utilise un coup passé en argument. */
/* @param reversi : Grille où jouer. */
/* @param player : Joueur en cours. */
/* @return : 0 si un coup est jouable et a été joué, ou -1 si impossible de jouer. */
int reversi_set_ia_move(Reversi *reversi, Player player, Pos *pos);

#endif /* _REVERSI_H_ INCLUDED */
