/* ---------------------------------------------------------------------- */
/* Filename: socket.c                                                     */
/* Author: ABHAMON Ronan                                                  */
/* Date: 2013-12-23 - 23:22:10                                            */
/*                                                                        */
/* ---------------------------------------------------------------------- */

#include <string.h> /* memcpy */
#include <stdlib.h>
#include "socket.h"

#ifndef h_addr
  /* Pour compatibilité... */
  #define h_addr h_addr_list[0]
#endif

/* Indique si l'API est lancée. */
static int __is_started = 0;

/* Indique si une socket est une socket serveur. */
#define IS_SERVER_SOCKET(SOCK) ((SOCK)->ip.host == 0 && (SOCK)->ip.port == 0)

struct TCP
{
  SOCKET id; /* Socket. */
  IP ip;     /* Adresse de la socket. */
};

void INIT_SOCKET(void)
{
  #ifdef _WIN32 /* Windows. */
    WSADATA wsa;
  #endif

  if(__is_started)
    return; /* Déjà lancé. */

  #ifdef _WIN32 /* Windows. */
    if(WSAStartup(MAKEWORD(2, 2), &wsa) < 0)
    {
      fprintf(stderr, "WSAStartup failed !");
      exit(EXIT_FAILURE);
    }
  #endif

  __is_started = 1;

  return;
}

void END_SOCKET(void)
{
  if(!__is_started)
    return; /* N'est pas actif. */

  #ifdef _WIN32 /* Windows. */
    WSACleanup();
  #endif

  __is_started = 0;

  return;
}

int server_resolve_host(IP *ip, const char *hostname, in_port_t port)
{
  struct hostent *h;

  /* Si host non spécifié, on utilise n'importe quelle adresse valable de la machine.
     Il s'agit donc d'une éventuelle création de serveur et non de client. */
  if(hostname == NULL)
    ip->host = INADDR_ANY;

  /* Sinon, récupération de l'ip à utiliser pour le client. */
  else if((ip->host = inet_addr(hostname)) == INADDR_NONE)
  {
    if((h = gethostbyname(hostname)) == NULL)
      return -1; /* Echec de la récupération des informations sur l'host. */

    memcpy(&ip->host, h->h_addr, h->h_length);
  }

  /* Port. */
  ip->port = htons(port);

  return 0;
}

TCP *tcp_get(IP *ip)
{
  struct sockaddr_in addr;
  TCP *sock;
  int opt = 1; /* Utilisé pour bloquer le EADDRINUSE et mettre en place TCP_NODELAY */

  if((sock = malloc(sizeof *sock)) == NULL)
    return NULL;

  /* Création d'une socket fondée sur le protocole IP, en mode connecté. */
  if((sock->id = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    free(sock);
    return NULL;
  }

  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = ip->port;

  /* Demande client. */
  if(ip->host != INADDR_ANY && ip->host != INADDR_NONE)
  {
    /* Mise en place de l'host à joindre. */
    addr.sin_addr.s_addr = ip->host;

    /* Connection... */
    if(connect(sock->id, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
      tcp_close(sock);
      return NULL;
    }

    sock->ip.host = ip->host;
    sock->ip.port = ip->port;
  }
  /* Demande serveur. */
  else
  {
    /* Adresse de la machine courante. */
    addr.sin_addr.s_addr = INADDR_ANY;

    /* Evite un eventuel bind: Address already in use */
    setsockopt(sock->id, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    /* Affectation de l'adresse. */
    /* Attentes de la socket. */
    if(bind(sock->id, (struct sockaddr *)&addr, sizeof(addr)) < 0 ||
       listen(sock->id, 5) < 0)
    {
      tcp_close(sock);
      return NULL;
    }

    #ifdef TCP_NODELAY /* Pas disponible partout. Parfait pour des bonnes perfs. */
      setsockopt(sock->id, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt));
    #endif

    sock->ip.host = addr.sin_addr.s_addr;
    sock->ip.port = addr.sin_port;
  }

  return sock;
}

void tcp_get_adress(TCP *sock, IP *res)
{
  if(!IS_SERVER_SOCKET(sock))
    memcpy(res, &sock->ip, sizeof(IP));
  else
    memset(res, 0, sizeof(IP));

  return;
}

TCP *tcp_accept(TCP *server)
{
  struct sockaddr_in addr;
  socklen_t len;
  TCP *sock;

  /* La socket donnée n'est pas une socket serveur. */
  if(!IS_SERVER_SOCKET(server))
    return NULL;

  if((sock = malloc(sizeof *sock)) == NULL)
    return NULL;

  /* Acceptation d'une connexion TCP. */
  len = sizeof(addr);

  if((sock->id = accept(server->id, (struct sockaddr *)&addr, &len)) < 0)
  {
    free(sock);
    return NULL;
  }

  sock->ip.host = addr.sin_addr.s_addr;
  sock->ip.port = addr.sin_port;

  return sock;
}

int tcp_send(TCP *sock, void *data, int len)
{
  /* Depuis quand une socket serveur peut-elle envoyer ? */
  if(IS_SERVER_SOCKET(sock))
    return -1;

  return send(sock->id, data, len, 0);
}

int tcp_recv(TCP *sock, void *data, int len)
{
  /* Depuis quand une socket serveur peut-elle recevoir ? */
  if(IS_SERVER_SOCKET(sock))
    return -1;

  return recv(sock->id, data, len, 0);
}

void tcp_close(TCP *sock)
{
  if(sock == NULL)
    return;

  if(sock->id != INVALID_SOCKET)
    closesocket(sock->id);

  free(sock);
  return;
}
