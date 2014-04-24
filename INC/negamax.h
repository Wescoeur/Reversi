/* ---------------------------------------------------------------------- */
/* Filename: negamax.h                                                    */
/* Authors: ABHAMON Ronan - HIVERT Kevin                                  */
/* Date: 2014-04-14 - 00:16:58                                            */
/*                                                                        */
/* ---------------------------------------------------------------------- */

#ifndef _NEGAMAX_H_
#define _NEGAMAX_H_

#include "pos.h"
#include "reversi.h"

int ia_negamax(Reversi *reversi, Player player, int depth, int alpha, int beta, Pos *pos);

#endif /* _NEGAMAX_H_ INCLUDED */
