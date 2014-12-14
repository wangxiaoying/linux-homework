#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <setjmp.h>

sigjmp_buf sig_buf;

void sys_err(const char* info){
	printf("ERROR: %s - %s\n", info, strerror(errno));
}

void handler(int signum){
	if(SIGFPE == signum){
		sys_err("can't divided by 0");
		siglongjmp(sig_buf, SIGFPE);
	}
}

int main(void){
	int ans;
	
	signal(SIGFPE, handler);
	int jmpResult = sigsetjmp(sig_buf, 1);
	
	if(0 == jmpResult){
		ans = 5 / 0;
	}else{
		printf("jmp finished + %d\n", jmpResult);
	}
	
	exit(EXIT_SUCCESS);
}


