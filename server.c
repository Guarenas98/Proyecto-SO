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
#include <pthread.h>

Node_t *head;

int handle_command(char *, char *, int len);


void* RunClient() { /* Hilo con el cual se trabajara */

	int i = 0;
	char c;
	char command[256] = {0};
	char response[256] = {0};

	while(1) {
		read(0,&c,1);
		if ( c == '\n')
			break;
		command[i]=c;
		i++;
	}

	handle_command(command, response, sizeof(response));
	memset(command, 0, 256*(sizeof(char)));
	printf("%s\n", response );

	pthread_exit (1);
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

	pthread_t _ID [6]; /* ID de los hilos usados */
	//short Activo = 0; /* hilos activos */

	for (int i = 0; i < 6; i++)
	{
		pthread_create (&_ID[i], NULL, RunClient, NULL);
		pthread_join (_ID[i], NULL);
	}

	//RunClient();

	return(0);
}
