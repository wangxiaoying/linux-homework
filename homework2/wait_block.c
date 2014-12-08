#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

void err_sys(const char* info) { 
    fprintf(stderr, "ERROR: %s: %s\n", info, strerror(errno)); 
    exit(EXIT_FAILURE); 
}

void show_status(char *pr, pid_t pid, pid_t ppid, pid_t pgid){
	printf("%s : pid = %d, ppid = %d, pgid = %d\n", pr, pid, ppid, pgid);
}

int main(int argc, const char *argv[]){
	pid_t pid, _pid;
	int i;

	pid = fork();

	if(pid < 0){
		err_sys("fork error");
	}else if(0 == pid){
		printf("child process in\n");
		for(i = 0; i < 5; ++i){
			sleep(1);
			printf(". \n");
		}
		printf("\n");
		show_status("child", getpid(), getppid(), getpgid(getpid()));
		exit(EXIT_SUCCESS);
	}else{
		printf("parent process in\n");
		printf("parent start waiting\n");
		_pid = waitpid(pid, NULL, WUNTRACED);
		
		if(0 == _pid){
			err_sys("child process has not exited");
		}else if(pid == _pid){
			printf("child process has exited\n");
			printf("parent finish waiting\n");
		}else{
			err_sys("waitpid error");
		}
		show_status("parent", getpid(), getppid(), getpgid(getpid()));
	}

	exit(EXIT_SUCCESS);
}