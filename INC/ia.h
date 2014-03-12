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

/** Evalue une grille en retournant où jouer. */
/* @param reversi : Grille actuelle de jeu. */
/* @param player : Joueur qui va jouer. */
/* @param pos : Structure remplit par la fonction, contient la position du coup à jouer.
   Ne modifie rien si grille pleine. */
Pos ia_eval(Reversi *reversi, Player player, int depth);
int ia_eval_min(Reversi *reversi, Player player, int depth);
int ia_eval_max(Reversi *reversi, Player player, int depth);
int ia_eval_grid(Reversi *reversi, Player player);

#endif /* _IA_H_ INCLUDED */
