/* ---------------------------------------------------------------------- */
/* Filename: strplus.h                                                    */
/* Authors: ABHAMON Ronan - HIVERT Kevin                                  */
/* Date: 2014-02-09 - 02:59:22                                            */
/*                                                                        */
/* ---------------------------------------------------------------------- */

#ifndef _STRPLUS_H_
#define _STRPLUS_H_

/** Vide le buffer. */
void cleanbuf(void);

/** Récupère une chaine. */
/* @param s : Buffer acceuillant la chaine. */
/* @param size : Taille du buffer. */
/* @param f : Descripteur de fichier. */
/* @return : 0 en cas de réussite, -1 sinon. */
int mygets(char *s, int size, FILE *f);

#endif /* _STRPLUS_H_ INCLUDED */
