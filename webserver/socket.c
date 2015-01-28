#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>

/*struct sockaddr_in {
  sa_family_t sin_family; 
  in_port_t sin_port; 
  struct in_addr sin_addr ; 
};

struct in_addr {
  uint32_t s_addr ;
};
*/

int creer_serveur (int port)
{
  int socket_serveur ;
  socket_serveur = socket(AF_INET, SOCK_STREAM, 0);
  if ( socket_serveur == -1)
  {
    perror ("socket_serveur");
    return -1;
  }
  struct sockaddr_in saddr ;
  saddr.sin_family = AF_INET ; /* Socket ipv4 */
  saddr.sin_port = htons(port); /* Port d’écoute */
  saddr.sin_addr.s_addr = INADDR_ANY ; /*écoute sur toutes les interfaces*/
  if ( bind(socket_serveur, (struct sockaddr *)& saddr, sizeof(saddr)) == -1)
  {
    perror("bind socker_serveur");
    return -1;
  }
  if (listen(socket_serveur, 10) == -1)
  {
    perror ("listen socket_serveur");
    return -1;
  }
  int socket_client ;
	while(1){
		socket_client = accept(socket_serveur, NULL, NULL);
		if (socket_client == -1)
		{
		  perror("accept");
		  return -1;
		}
		sleep(1);
		const char* message_bienvenue = "Bonjour, bienvenue sur mon serveur\n" ;
		write(socket_client, message_bienvenue, strlen(message_bienvenue));
	}
  return socket_serveur;
}
