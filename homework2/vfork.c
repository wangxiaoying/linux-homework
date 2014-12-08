#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

int global = 333;

void err_sys(const char* info) { 
    fprintf(stderr, "ERROR: %s: %s\n", info, strerror(errno)); 
    exit(EXIT_FAILURE); 
}

void show_status(pid_t pid, int global, int local){
	printf("pid = %d, global = %d, local = %d\n", pid, global, local);
	printf("---------------------------------------\n");
}

int main(int argc, char const *argv[]){
	int local = 999;
	pid_t pid;

	printf("before vfork\n");
	show_status(getpid(), global, local);

	pid = vfork();

	if(pid < 0){
		err_sys("vfork error");
	}else if (0 == pid){
		printf("child process\n");
		global += 111;
		local -= 111;
		show_status(getpid(), global, local);
		exit(EXIT_SUCCESS);
	}else{
		printf("parent process\n");
		show_status(getpid(), global, local);
	}

	exit(EXIT_SUCCESS);
}

