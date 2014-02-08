/* ---------------------------------------------------------------------- */
/* Filename: reversi.h                                                    */
/* Authors: ABHAMON Ronan - HIVERT Kevin                                  */
/* Date: 2014-02-06 - 23:59:01                                            */
/*                                                                        */
/* ---------------------------------------------------------------------- */

#ifndef _REVERSI_H_
#define _REVERSI_H_

/** Largeur et hauteur d'une grille de jeu. */
#define REVERSI_WIDTH 8
#define REVERSI_HEIGHT 8

/** Joueurs. */
typedef enum Player {
  PLAYER_1, PLAYER_2
} Player;

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

#endif /* _REVERSI_H_ INCLUDED */
