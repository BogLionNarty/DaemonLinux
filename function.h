#ifndef FUNCTION_H_INCLUDED
#define FUNCTION_H_INCLUDED
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <string.h>
#include <utime.h>
#include <fcntl.h>
#include <syslog.h>
#include <stdlib.h>
#include <dirent.h>
#include <signal.h>
void skopiuj(char *a, char *b);
void synchronizacja(char *a, char *b);
void usunWszystko(char *b);
void usunPliki(char *a, char *b);
void sygnal(int n);
int time;
int R;
#endif // FUNCTION_H_INCLUDED
