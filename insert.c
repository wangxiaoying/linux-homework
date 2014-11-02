#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <malloc.h>

#define BUFFER_SIZE 1024

char* getContent(int f, char *temp, off_t index, off_t *length){
	int read_length, offset = 0;
	char *buffer = malloc(BUFFER_SIZE);
	
	lseek(f, index, SEEK_SET);
	while((read_length=read(f, buffer, 2)) > 0){
		if(offset + read_length > sizeof(temp)){
			temp = realloc(temp, 2*offset+BUFFER_SIZE);
		}
		strncpy(temp + offset, buffer, read_length);
		offset += read_length;
	}
	
	temp[offset] = '\0';
	*length = offset;
	
	free(buffer);
	return temp;
}

int main(int argc, char const *argv[]){
	off_t index;
	const char *file_src, *file_des;
	char *write_temp, *read_temp;
	int fs = 0, fd = 0;
	off_t write_length, read_length;
	
	if(argc != 4){
		fprintf(stderr, "invalid option\n");
		return 1;	
	}
	
	index = atoi(argv[1]);
	file_src = argv[2];
	file_des = argv[3];
	
	fs = open(file_src, O_RDONLY|O_EXCL, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
	fd = open(file_des, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
	
	if(-1 == fs || -1 == fd){
		if(ENOENT == errno) fprintf(stderr, "file: %s dose not exist\n", file_src);
		else fprintf(stderr, "file open error\n");
		return 1;
	}

	write_temp = malloc(BUFFER_SIZE);
	write_temp = getContent(fs, write_temp, 0, &write_length);
	off_t currpos = lseek(fd, 0, SEEK_END);
	if(currpos < index) index = currpos;
	read_temp = malloc(BUFFER_SIZE);
	read_temp = getContent(fd, read_temp, index, &read_length);
	
	lseek(fd, index, SEEK_SET);
	write(fd, write_temp, write_length-1);
	write(fd, read_temp, read_length);	

	free(write_temp);
	free(read_temp);

	close(fs);
	close(fd);
	
	return 0;
}
