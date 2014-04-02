// A simple server in the interne domain using TCP the port number is passed as an argument
#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> //For the port
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    //Declaracion de variables
    int sockfd, newsockfd, portno, n;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;

    //Al momento de ejecutar el Archivo, si no se le proporciona el Puerto marcara error.
    if (argc < 2)
    {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
     /***************** Creacion del Socket *************************/
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        error("ERROR opening socket");
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    /*************** Conversion del parametro de entrad PUERTO a Entero **********************************/
    portno = atoi(argv[1]);

    
    /********************Asignacion de protocolo de conexion, el puerto, y Direccion IP ******************/
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    {
        error("ERROR on binding");
    }

    /********************* El Servidor se prepara para cualquier conexion entrante por medio del socket e indica como parametro cuantas conexiones maximas puede haber. 1 en este caso**********************/
    listen(sockfd,1);
    clilen = sizeof(cli_addr);

    
    /*********************** El Servidor acepta la conexion ********************************/
      
    newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);

    if (newsockfd < 0)
    {
        error("ERROR on accept");
    }
    
    
    /******************* CICLO PARA QUE SE SALGA DE LA COMUNICACION SOLO HASTA QUE SE PONGA el CARACTER  '!' **********************************************/
     
    do{
    
      printf("Please write something: ");
      bzero(buffer,256);
      fgets(buffer,255,stdin);
      n = write(newsockfd,buffer,strlen(buffer)-1);

      /*****************************************************************/  
      /***********/                                         /***********/
      /************        Ciclo infinito de lectura        ************/
      /***********/                                         /***********/
      /*****************************************************************/
      
      bzero(buffer,256);
      n = read(newsockfd,buffer,255);
      
      while(buffer[0] == '?')
      {
	n = write(newsockfd,"REC",3);

	if (n < 0)
	{
	    error("ERROR writing to socket");
	}   
	
	bzero(buffer,256);
	n = read(newsockfd,buffer,255);

	if (n < 0)
	{
	    error("ERROR reading from socket");
	}
	
      }
      
      
      
      if((buffer[0] != '?')&&(buffer[0] != 'O'))
      {
	printf("Word deleted, ");
	//n = write(newsockfd,buffer,strlen(buffer));
      }

      printf("Here is the message: %s\n",buffer);
      
	  
    }while(buffer[0] != '!');     
    
      
    return 0;
    
}