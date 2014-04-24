/* ---------------------------------------------------------------------- */
/* Filename: alloc.h                                                      */
/* Authors: ABHAMON Ronan - HIVERT Kevin                                  */
/* Date: 2014-02-14 - 14:34:41                                            */
/*                                                                        */
/* ---------------------------------------------------------------------- */

#ifndef _ALLOC_H_
#define _ALLOC_H_

#include <stdio.h>
#include <stdlib.h>

#define pmalloc(P, SIZE)                                                \
  while(((P) = malloc(SIZE)) == NULL)                                   \
  {                                                                     \
    fprintf(stderr, "Bad malloc in %s, line %d\n", __FILE__, __LINE__); \
    exit(EXIT_FAILURE);                                                 \
  }

#define pcalloc(P, N, SIZE)                                             \
  while(((P) = calloc(N, SIZE)) == NULL)                                \
  {                                                                     \
    fprintf(stderr, "Bad calloc in %s, line %d\n", __FILE__, __LINE__); \
    exit(EXIT_FAILURE);                                                 \
  }

#define prealloc(P, SIZE)                                               \
  while(((P) = realloc(P, SIZE)) == NULL)                               \
  {                                                                     \
    fprintf(stderr, "Bad calloc in %s, line %d\n", __FILE__, __LINE__); \
    exit(EXIT_FAILURE);                                                 \
  }

#endif /* _ALLOC_H_ INCLUDED */
