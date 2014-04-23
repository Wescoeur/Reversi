/* ---------------------------------------------------------------------- */
/* Filename: ia.h                                                         */
/* Authors: ABHAMON Ronan - HIVERT Kevin                                  */
/* Date: 2014-02-07 - 08:04:25                                            */
/*                                                                        */
/* ---------------------------------------------------------------------- */

#ifndef _IA_H_
#define _IA_H_

#include "reversi.h"
#include "pos.h"


#define MIN(A,B) (((A) < (B)) ? (A) : (B))
#define MAX(A,B) (((A) > (B)) ? (A) : (B))

/** Evalue une grille en retournant où jouer. */
/* @param reversi : Grille actuelle de jeu. */
/* @param player : Joueur qui va jouer. */
/* @param pos : Structure remplit par la fonction, contient la position du coup à jouer.
   Ne modifie rien si grille pleine. */
Pos ia_eval(Reversi *reversi, Player player, int depth);
int ia_eval_min(Reversi *reversi, Player player, int depth);
int ia_eval_max(Reversi *reversi, Player player, int depth);

/*
 * AI function but with alpha-beta pruning.
  */
Pos ia_alphabeta (Reversi *reversi, Player player, int depth);
int ia_alphabeta_bis (Reversi *reversi, int depth, int alpha, int beta, Player player, int maximizingPlayer);


/*
 * Grid evaluation, giving a value to each cells.
  */
int ia_eval_grid(Reversi *reversi, Player player);

int ia_eval_grid_2(Reversi *reversi, Player player);

/*
 * V3.
  */
Pos iaV3_alphabeta (Reversi *reversi, Player player, int depth);
int iaV3_alphabeta_bis (Reversi *reversi, int depth, int alpha, int beta, Player player, int maximizingPlayer);
int iaV3_eval_grid(Reversi *reversi, Player player);


Pos iaV4_alphabeta (Reversi *reversi, Player player, int depth);
int iaV4_alphabeta_bis (Reversi *reversi, int depth, int alpha, int beta, Player player, int maximizingPlayer);
int iaV4_eval_grid(Reversi *reversi, Player player);
#endif /* _IA_H_ INCLUDED */
