#include "function.h"
#include <stdio.h>
#define LEN 1000

void sygnal(int n)
{
	syslog(LOG_INFO, "Synchronizacja");
}

void skopiuj(char *dir1, char *dir2)
{
	int file1, file2;	

	if((file2 = open(dir1, O_RDONLY)) != -1)
	{
		if(creat(dir2, S_IRWXU | S_IRWXG | S_IRWXO) != -1)
		{
		
			{
				int bytes;
				char buf[200];

				if((file1 = open(dir2, O_WRONLY | O_TRUNC)) != -1) 
				{
					while((bytes = read(file2, buf, 200)) > 0)
						write(file1, buf, bytes);
			
					syslog(LOG_INFO, "Skopiowano: %s->%s", dir1, dir2);
					close(file1);
				}
				else
					syslog(LOG_ERR, "Nie mozna otworzyc pliku: %s", dir2);
			}
		
		close(file2);
		}
	}
}

void usunWszystko(char *dir2)
{
	struct dirent *wsk;
	DIR *direct;

	if((direct = opendir(dir2)) != NULL)
	{
		while(wsk = readdir(direct))
		{
			struct stat infoD;
			char temp[LEN];
			strcpy(temp, dir2);
			strcat(temp, "/");
			strcat(temp, (*wsk).d_name);		
			stat(temp, &infoD);
			if(!S_ISDIR(infoD.st_mode)) {
				remove(temp);
				syslog(LOG_INFO, "Plik zosta? usuni?ty: %s", temp);
			}
			else if(S_ISDIR(infoD.st_mode))
			{
				char temp2[LEN], temp3[LEN];
				strcpy(temp2, dir2);
				strcat(temp2, "/.");
				strcpy(temp3, dir2);
				strcat(temp3, "/..");
				if(strcmp(temp, temp2) && strcmp(temp, temp3))
					usunWszystko(temp); //rek
			}
		}
	}   	
	rmdir(dir2);
	syslog(LOG_INFO, "Katalog usuni?tyL: %s", dir2);
}


void usunPliki(char *dir1, char *dir2)
{
	struct dirent *wsk; 
	DIR *direct;

	if((direct = opendir(dir2)) != NULL)
	{
		while(wsk = readdir(direct))
		{
			struct stat infoD;
			char tmp2[LEN];
			strcpy(tmp2, dir2);
			strcat(tmp2, "/");
			strcat(tmp2, (*wsk).d_name);
			stat(tmp2, &infoD);

			if(!S_ISDIR(infoD.st_mode))
			{
				struct stat check;
				char tmp[LEN];
				strcpy(tmp, dir1);
				strcat(tmp, "/");
				strcat(tmp, (*wsk).d_name);
				if(stat(tmp, &check)) {
				    remove(tmp2);
					syslog(LOG_INFO, "Plik został usuniety: %s", tmp2);
				}
			}
			else if(S_ISDIR(infoD.st_mode))
			{
				char tmp[LEN], t1[LEN], t2[LEN];
				strcpy(tmp, dir1);
				strcat(tmp, "/");
				strcat(tmp, (*wsk).d_name);
				strcpy(t1, dir2);
				strcat(t1, "/.");
				strcpy(t2, dir2);
				strcat(t2, "/..");
				if(strcmp(tmp2, t1) && strcmp(tmp2, t2)) {
					if(opendir(tmp) != NULL)
						usunPliki(tmp, tmp2);
					else
						usunWszystko(tmp2);
				}
			}
		}
	}
	else
		syslog(LOG_ERR, "Nie mozna otworzyc katalogu %s", dir2);
}

void synchronizacja(char *dir1, char *dir2)
{
	if(!mkdir(dir2, S_IRWXU | S_IRWXG | S_IRWXO));

	struct dirent *wsk;
	DIR *direct;

	if((direct = opendir(dir1)) != NULL)
	{
		while(wsk = readdir(direct))
		{
			struct stat infoS;
			char tmp[LEN];
			strcpy(tmp, dir1);
			strcat(tmp, "/");
			strcat(tmp, (*wsk).d_name);
			stat(tmp, &infoS);
			if(!S_ISDIR(infoS.st_mode))
			{                
				struct stat infoD;
				char tmp2[LEN];
				strcpy(tmp2, dir2);
				strcat(tmp2, "/");
				strcat(tmp2, (*wsk).d_name);			
				if(stat(tmp2, &infoD))
				{
					skopiuj(tmp, tmp2);
				}
				else
				{
					if(infoS.st_mtime > infoD.st_mtime)
					{
						remove(tmp2);
						syslog(LOG_INFO, "Plik usuniety: %s", tmp2);
						skopiuj(tmp, tmp2);
					}
				}
			}
			else if(S_ISDIR(infoS.st_mode) && R)
			{
				char tmp2[LEN], t1[LEN], t2[LEN];
				strcpy(tmp2, dir2);
				strcat(tmp2, "/");
				strcat(tmp2, (*wsk).d_name);
				strcpy(t1, dir1);
				strcat(t1, "/.");
				strcpy(t2, dir1);
				strcat(t2, "/..");
				if(strcmp(tmp, t1) && strcmp(tmp, t2))
					synchronizacja(tmp, tmp2);
			}
		}
	}
	else
		syslog(LOG_ERR, "Nie mozna otworzyc katalogu: %s", dir1);
}







