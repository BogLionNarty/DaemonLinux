
#include "function.h"
#define LEN 1000

int time = 20; int R = 0; 
char zrodlo[LEN], cel[LEN];

int main(int argc, char **argv) 
{
	pid_t pid, sid;
	pid = fork();

	if (pid < 0) 
		exit(EXIT_FAILURE);

	if (pid > 0) 
		exit(EXIT_SUCCESS);

	umask(0);   	

	openlog(argv[0], LOG_PID | LOG_CONS, LOG_DAEMON | LOG_USER);
	syslog(LOG_INFO, "Deamon started");
           
	sid = setsid();

	if (sid < 0) {
		syslog(LOG_ERR, "Unsuccesfuly creation a new SID for the child process.");
		exit(EXIT_FAILURE);
	}

	if ((chdir("/")) < 0) 
		exit(EXIT_FAILURE);

	if(argc < 3) {
		syslog(LOG_ERR, "Unsuccesfuly changing the current directory.");
		exit(EXIT_FAILURE);
	}	

	strcpy(zrodlo,argv[1]);
	strcpy(cel, argv[2]);

	struct stat checkS, checkD;
	stat(zrodlo, &checkS);
	stat(cel, &checkD);

	if(!S_ISDIR(checkS.st_mode) || !S_ISDIR(checkD.st_mode))
		exit(EXIT_FAILURE);
	
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	if(argc > 2)
	{
		int i;
		for(i=2; i<argc; i++)
		{
			if(!strcmp(argv[i], "-T")) {
				sscanf(argv[i+1], "%d", &time);	
				syslog(LOG_INFO, "Czas synchronizacji: %d", time);
			}
			
			if(!strcmp(argv[i], "-R")) {
				R = 1;
				syslog(LOG_INFO, "Synchronizacja katalogow");
			}

			
			}
		}
		
	(void) signal(SIGUSR1, sygnal);
	
	while(1) 
	{
		syslog(LOG_INFO, "Synchronizacja");
		usunPliki(zrodlo, cel);
		synchronizacja(zrodlo, cel);
		syslog(LOG_INFO, "Spij");
		sleep(time);
	}
	syslog(LOG_INFO, "Koniec");
	exit(EXIT_SUCCESS);
}








