#include <assert.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "db.h"
#include <pthread.h>
#include <sys/time.h>
#include <signal.h>
#include <time.h>
#include <termios.h>

Node_t *head;
sem_t A;

short Sin_Control_D = 1, Continuar = 1;

int handle_command(char *, char *, int len);

void salir ( int sig ) /* Si se recibe control D se sale del programa */
{
	Sin_Control_D = 0;
}

void* RunClient() { /* Hilo con el cual se trabajara */

	int i = 0;
	char c;
	char command[256] = {0};
	char response[256] = {0};
/*	void *ptr = malloc (sizeof (void) );
	ptr = (void*) -1;
	c = getc ( stdin );
	
	if ( c == EOF ){
		puts ("Control D");
		return ptr;
	}
	ungetc ( c, stdin );
*/
	sem_wait (&A);
	while(1) {
		//read(0,&c,1);
		c = getc (stdin);
		//printf ("%c == %d", c,c);
		if ( c == EOF ){
			puts ("Control-D"); return NULL;}
		if ( c == '\n')
			break;
		command[i]=c;
		i++;
	}
	sem_post(&A);
	handle_command(command, response, sizeof(response));
	memset(command, 0, 256*(sizeof(char)));
	printf("%s\n", response );
	
}

int handle_command(char *command, char *response, int len) {
	if (command[0] == EOF) {
		strncpy(response, "all done", len-1);
		return(0);
	} else {
		interpret_command(command, response, len);
	}
	return(1);
}

int main(int argc, char *argv[]) {

	if (argc != 1) {
		fprintf(stderr, "Usage: server\n");
		exit(1);
	}

	sem_init (&A, 0,1);
	
	pthread_t _ID [6]; /* ID de los hilos usados */	

	 struct termios sin_control, con_control;
	  tcgetattr(0,&sin_control);

	  signal( SIGINT, salir );

	  con_control = sin_control;
	  con_control.c_cc[VINTR] = 4; // ^D
	  tcsetattr(0,TCSANOW,&con_control);

	while (Continuar)
	{
		for (int i = 0; i < 6; i++)
			pthread_create (&_ID[i], NULL, RunClient, NULL);
		
		for (int i = 0; i < 6 ; i++ ){
			pthread_join (_ID[i], NULL);
			if (!Sin_Control_D)
				Continuar = 0;
		}
						
	}

	tcsetattr(0,TCSANOW,&sin_control);

	return(0);
}
