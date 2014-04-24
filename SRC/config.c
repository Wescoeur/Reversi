/* ---------------------------------------------------------------------- */
/* Filename: config.c                                                     */
/* Authors: ABHAMON Ronan - HIVERT Kevin                                  */
/* Date: 2014-03-12 - 15:16:49                                            */
/*                                                                        */
/* ---------------------------------------------------------------------- */

#include <string.h>
#include "alloc.h"
#include "config.h"
#include "strplus.h"

#define BUFFER_SIZE 256

Config *config_load(const char *filename)
{
  Config *config;
  FILE *f;
  char buffer[BUFFER_SIZE];
  in_port_t port;
  int i;

  pmalloc(config, sizeof *config);

  if((f = fopen(filename, "r")) == NULL)
  {
    fprintf(stderr, "Unable to load %s\n", filename);
    exit(EXIT_FAILURE);
  }

  /* Nom. */
  mygets(config->name, PLAYER_NAME_SIZE, f);
  printf("client: \'%s\'\n", config->name);

  /* Remplissage de '\0' */
  for(i = strlen(config->name); i < PLAYER_NAME_SIZE; i++)
    config->name[i] = '\0';

  /* Port. */
  mygets(buffer, BUFFER_SIZE, f);
  port = (in_port_t)atoi(buffer);
  printf("port: \'%d\'\n", port);

  /* IP. */
  mygets(buffer, BUFFER_SIZE, f);
  printf("ip: \'%s\'\n", buffer);

  /* Ip + Port au bon format. */
  server_resolve_host(&config->ip, buffer, port);

  fclose(f);

  return config;
}
