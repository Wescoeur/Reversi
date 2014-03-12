/* ---------------------------------------------------------------------- */
/* Filename: socket.h                                                     */
/* Author: ABHAMON Ronan                                                  */
/* Date: 2013-12-23 - 23:22:17                                            */
/*                                                                        */
/* ---------------------------------------------------------------------- */

#ifndef _SOCKET_
#define _SOCKET_

/* Initialise l'utilisation des sockets. */
void INIT_SOCKET(void);

/* Ferme l'utilisation des sockets. */
void END_SOCKET(void);

#ifdef _WIN32 /* Windows */
  #include <winsock2.h>

  /* Si un jour Windows devient propre, je le deviendrais aussi pour lui,
     mais en attendant... */
  typedef unsigned long in_addr_t;
  typedef unsigned short in_port_t;
  typedef int socklen_t;
  typedef struct SOCKADDR_IN sockaddr_in;
  typedef struct SOCKADDR sockaddr;
  typedef struct IN_ADDR in_addr;
#elif defined __unix__ /* UNIX */
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <unistd.h>
  #include <netdb.h>

  #define INVALID_SOCKET -1
  #define SOCKET_ERROR -1

  #define closesocket(s) close(s)
  typedef int SOCKET;
#else
  #error socket is not defined for this platform
#endif

/* Une IP de la machine. */
typedef struct IP
{
  in_addr_t host; /* Host - IPv4. */
  in_port_t port; /* Port. */
} IP;

/* Une socket TCP. */
typedef struct TCP TCP;

/* Remplir une structure IP avec les bonnes données. */
/* @param ip : IP à remplir. */
/* @param hostname : Hôte demandé. Mettre à NULL si le but est de créer un serveur. */
/* @param port : Port à utiliser. */
/* @return : -1 en cas d'échec, 0 sinon. */
int server_resolve_host(IP *ip, const char *hostname, in_port_t port);

/* Obtenir une socket relative à une ip. Que ce soit une socket cliente ou serveur. */
/* @param ip : Host et port correspondants. */
/* @return : NULL en cas d'échec ou une socket sinon. */
TCP *tcp_get(IP *ip);

/* Obtenir l'ip et le port d'une socket. */
/* @param sock : Socket contenant les infos recherchées. */
/* @param res : Structure IP servant à récupérer les infos. */
void tcp_get_adress(TCP *sock, IP *res);

/* Accepte une connexion TCP. */
/* @param server : Serveur faisant la liaison avec les demandes de connexions. */
/* @return : Une socket ayant fait une demande. */
TCP *tcp_accept(TCP *server);

/* Envoit des données à partir d'une socket. */
/* @param sock : Socket source. */
/* @param date : Données à envoyer. */
/* @param len : Taille des données. */
/* @return : -1 en cas d'échec ou le nombre d'octets reçus. */
int tcp_send(TCP *sock, void *data, int len);

/* Reçoit des données à partir d'une socket. */
/* @param sock : Socket source. */
/* @param data : Zone mémoire recevant les données. */
/* @param len : Taille max des données. */
/* @return : -1 en cas d'échec ou le nombre d'octets envoyés. */
int tcp_recv(TCP *sock, void *data, int len);

/* Ferme une socket. */
/* @param sock : Socket à fermer. */
void tcp_close(TCP *sock);

#endif /* _SOCKET_ INCLUDED */
