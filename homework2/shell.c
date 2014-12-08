#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <pwd.h>

#define BUFFER_SIZE 1024


void err_sys(const char* info) { 
    fprintf(stderr, "ERROR: %s: %s\n", info, strerror(errno)); 
    exit(EXIT_FAILURE); 
}

int main(int argc, char const *argv[]){	
	char cmd[BUFFER_SIZE], path[BUFFER_SIZE];
	char *head, *tail;
	struct passwd *pwd;

	pwd = getpwuid(getuid());

	while(1){
		if(NULL == realpath("./", path)) err_sys("can't get current path");
		printf("%s:%s$ ", pwd->pw_name, path);

		gets(cmd);

		if(0 == strcmp(cmd, "")) continue;
		if(0 == strcmp(cmd, "exit")) break;

		head = strtok_r(cmd, " ", &tail);
		if(0 == strcmp(head, "cd")){
			if(NULL != tail){
				chdir(tail);
			} 
			continue;
		}

		system(cmd);
	}
	
	exit(EXIT_SUCCESS);
}

