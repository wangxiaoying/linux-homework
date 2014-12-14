#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

void sys_err(const char* info){
	printf("ERROR: %s - %s\n", info, strerror(errno));
}

void sig_handler(int signum){
	if(SIGINT == signum){
		printf("SIGINT interrupted!\n");
	}else if(SIGRTMIN == signum){
		printf("SIGRTMIN interrupted!\n");
	}else if(SIGQUIT == signum){
		printf("SIGQUIT got!\n");
	}
}


int main(void){
	sigset_t newmask, oldmask, zeromask;
	if(signal(SIGINT, sig_handler) == SIG_ERR){
		sys_err("signal SIGINT error\n");
	}
	if(signal(SIGRTMIN, sig_handler) == SIG_ERR){
		sys_err("signal SIGRTMIN error\n");
	}
	if(signal(SIGQUIT, sig_handler) == SIG_ERR){
		sys_err("signal SIGQUIT error\n");
	}
	
	sigemptyset(&newmask);
	sigemptyset(&zeromask);
	
	sigaddset(&zeromask, SIGINT);
	sigaddset(&zeromask, SIGRTMIN);
	
	sigaddset(&newmask, SIGQUIT);

	if(sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0){
		sys_err("sigprocmask error\n");
	}
	
	printf("start suspend\n");
	
	if(sigsuspend(&zeromask) != -1){
		sys_err("sigsuspend error\n");
	}
	
	printf("end suspend\n");
	
	if(sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0){
		sys_err("sigprocmask error\n");
	}
	
	exit(EXIT_SUCCESS);
	
	
}
