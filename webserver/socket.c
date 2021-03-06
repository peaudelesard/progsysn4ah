#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>


/*struct sockaddr_in {
  sa_family_t sin_family; 
  in_port_t sin_port; 
  struct in_addr sin_addr ; 
};

struct in_addr {
  uint32_t s_addr ;
};
*/
void traitement_signal ( int sig )
{
	printf ("Signal %d reçu\n", sig );
	if(waitpid(-1, NULL, WNOHANG) == -1)
	{
		perror("waitpid");
	}
}
void initialiser_signaux ( void )
{
	if ( signal ( SIGPIPE , SIG_IGN ) == SIG_ERR )
	{
		perror ( " signal " );
	}	
	struct sigaction sa ;
	sa.sa_handler = traitement_signal ;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1)
	{
		perror ("sigaction(SIGCHLD)");
	}	
}
int creer_serveur (int port)
{
  int socket_serveur ;
	int optval = 1;
	initialiser_signaux();
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
	if ( setsockopt ( socket_serveur, SOL_SOCKET , SO_REUSEADDR , & optval , sizeof(int)) == -1)
	{
		perror ( " Can not set SO_REUSEADDR option " );
		return -1;
	}
  if ( bind(socket_serveur, (struct sockaddr *)& saddr, sizeof(saddr)) == -1)
  {
    perror("bind socket_serveur");
    return -1;
  }
  if (listen(socket_serveur, 10) == -1)
  {
    perror ("listen socket_serveur");
    return -1;
  }
  int socket_client ;
	const char* message_bienvenue = "Bonjour, bienvenue sur mon serveur\n" ;
	while(1)
	{
		/*FILE* message = fdopen(socket_client, "w+");*/
		socket_client = accept(socket_serveur, NULL, NULL);
		if (socket_client == -1)
		{
			perror("accept");
			return -1;
		}
		if (fork()==0)
		{
			sleep(1);
			
			FILE* message = fdopen(socket_client, "w+");
			write(socket_client, message_bienvenue, strlen(message_bienvenue));

			/*fprintf(message, "%s",message_bienvenue);*/
			char reception[255] = "";
			while(fgets(reception,1,message) != NULL)
			{				
			/*read(socket_client, &reception, 254);*/
			/*reception [255] = '\0';*/
				printf("%s",reception);
				fprintf(message, "<zigzag> %s",reception);
				/*sleep(100);	*/
			}
			/*write(socket_client, reception, strlen(reception));*/
		
		}
		close(socket_client);
	}  
	return socket_serveur;
}

