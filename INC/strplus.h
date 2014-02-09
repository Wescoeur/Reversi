/* ---------------------------------------------------------------------- */
/* Filename: strplus.h                                                    */
/* Author: ABHAMON Ronan                                                  */
/* Date: 2014-02-09 - 02:59:22                                            */
/*                                                                        */
/* ---------------------------------------------------------------------- */

#ifndef _STRPLUS_H_
#define _STRPLUS_H_

/** Vide le buffer. */
void cleanbuf(void);

/** Récupère une chaine sur stdin. */
/* @param s : Buffer acceuillant la chaine. */
/* @param size : Taille du buffer. */
/* @return : 0 en cas de réussite, -1 sinon. */
int mygets(char *s, int size);

#endif /* _STRPLUS_H_ INCLUDED */
