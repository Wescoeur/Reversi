/* ---------------------------------------------------------------------- */
/* Filename: negamax.h                                                    */
/* Author: ABHAMON Ronan                                                  */
/* Date: 2014-04-24 - 00:16:58                                            */
/* Site: https://github.com/Wescoeur                                      */
/*                                                                        */
/* ---------------------------------------------------------------------- */

#ifndef _NEGAMAX_H_
#define _NEGAMAX_H_

#include "pos.h"
#include "reversi.h"

#include "pos.h"
#include "reversi.h"

int ia_negamax(Reversi *reversi, Player player, int depth, int alpha, int beta, Pos *pos);

#endif /* _NEGAMAX_H_ INCLUDED */
