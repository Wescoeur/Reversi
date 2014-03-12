/* ---------------------------------------------------------------------- */
/* Filename: config.h                                                     */
/* Author: ABHAMON Ronan                                                  */
/* Date: 2014-03-12 - 15:16:54                                            */
/* Site: https://github.com/Wescoeur                                      */
/*                                                                        */
/* ---------------------------------------------------------------------- */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "socket.h"

#define PLAYER_NAME_SIZE 64

typedef struct Config
{
  char name[64]; /* Nom de l'IA. */
  IP ip;         /* Ip à joindre. */
} Config;

/** Charge la configuration du serveur. */
/* @param filename : Nom du fichier. */
/* @return : La configuration du serveur à joindre. */
Config *config_load(const char *filename);

#endif /* _CONFIG_H_ INCLUDED */
