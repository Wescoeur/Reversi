/* ---------------------------------------------------------------------- */
/* Filename: strplus.c                                                    */
/* Author: ABHAMON Ronan                                                  */
/* Date: 2014-02-09 - 02:59:18                                            */
/*                                                                        */
/* ---------------------------------------------------------------------- */

#include <stdio.h>
#include <string.h>
#include "strplus.h"

void cleanbuf()
{
  int c = 0;

  while(c != '\n' && c != EOF)
    c = getchar();

  return;
}

int mygets(char *s, int size, FILE *f)
{
  char *p = NULL;

  if(fgets(s, size, f) != NULL)
  {
    if((p = strchr(s, '\n')) != NULL)
      *p = '\0';
    else
      cleanbuf();

    return 0;
  }

  return -1;
}
