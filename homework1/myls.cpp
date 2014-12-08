#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>

#include <vector>
#include <algorithm>
#include <string>

using namespace std;

#define BUF_SIZE 1024

#define COLOR_R	(char*)"\33[0m"
#define COLOR_D	(char*)"\33[01m\33[34m"
#define COLOR_L	(char*)"\33[01m\33[36m"
#define COLOR_P	(char*)"\33[40m\33[33m"
#define COLOR_B	(char*)"\33[40m\33[33m"
#define COLOR_C	(char*)"\33[40m\33[33m"
#define COLOR_S	(char*)"\33[02m\33[35m"
#define RESET_CLOLR	(char*)"\33[0m"

vector<string> g_paths;
char *g_color;
bool g_paral = false;

char get_file_type(mode_t st_mode)
{
	if (S_ISREG(st_mode))
	{
		g_color = COLOR_R;
		return '-';
	}
	if (S_ISDIR(st_mode))
	{
		g_color = COLOR_D;
		return 'd';
	}
	if (S_ISCHR(st_mode))
	{
		g_color = COLOR_C;
		return 'c';
	}
	if (S_ISBLK(st_mode))
	{
		g_color = COLOR_B;
		return 'b';
	}
	if (S_ISFIFO(st_mode))
	{
		g_color = COLOR_P;
		return 'p';
	}
	if (S_ISLNK(st_mode))
	{
		g_color = COLOR_L;
		return 'l';
	}
	if (S_ISSOCK(st_mode))
	{
		g_color = COLOR_S;
		return 's';
	}
	g_color = COLOR_R;
	return '-';
}


void get_mode(char *buf, mode_t file_mode){
	buf[0] = get_file_type(file_mode);

	mode_t bit;
	for(int i = 3; i > 0; --i){
		bit = file_mode & 0x01;
		if(1 == bit){
			buf[i*3] = 'x';
		}else{
			buf[i*3] = '-';
		}
		file_mode >>= 1;

		bit = file_mode & 0x01;
		if(1 == bit){
			buf[i*3-1] = 'w';
		}else{
			buf[i*3-1] = '-';
		}
		file_mode >>= 1;

		bit = file_mode & 0x01;
		if(1 == bit){
			buf[i*3-2] = 'r';
		}else{
			buf[i*3-2] = '-';
		}
		file_mode >>= 1;
	}

	buf[10] = '\0';
}

int show_l_part(struct stat *file_stat){
	//文件类型和权限
	char buf[BUF_SIZE];
	get_mode(buf, file_stat->st_mode);
	char file_type = buf[0];
	fprintf(stdout, "%s", buf);

	//硬连接
	fprintf(stdout, "%6d", file_stat->st_nlink);

	//用户名和组名
	struct passwd *user = getpwuid(file_stat->st_uid);
	fprintf(stdout, "%8s", user->pw_name);
	struct group *group = getgrgid(file_stat->st_gid);
	fprintf(stdout, "%8s", group->gr_name);

	if('c' == file_type || 'b' == file_type){
		//设备号
		int major = 0xFF00 & file_stat->st_rdev;
		major >>= 8;
		int sub = 0x00FF & file_stat->st_rdev;
		fprintf(stdout, "\t%4d,%4d", major, sub);
	}else{
		//文件大小
		fprintf(stdout, "  %9ld", file_stat->st_size);
	}

	//时间
	char ftime[BUF_SIZE] = {'\0'};
	snprintf(ftime, 13, "%s", 4+ctime(&file_stat->st_ctime));
	fprintf(stdout, "  %s", ftime);

	return 0;
}

int do_file_ls(const char *path, const char *name){
	char full_path[BUF_SIZE];
	snprintf(full_path, BUF_SIZE, "%s/%s", path, name);

	struct stat file_stat;
	if(-1 == lstat(full_path, &file_stat)){
		perror(full_path);
		return -1;
	}

	if(false == g_paral){
		get_file_type(file_stat.st_mode);
	}else{
		show_l_part(&file_stat);
	}

	fprintf(stdout, "%s", g_color);
	fprintf(stdout, "  %s", name);
	fprintf(stdout, RESET_CLOLR);
	if(true == g_paral && 'l' == get_file_type(file_stat.st_mode)){
		//链接文件，显示指向文件
		char real_file[BUF_SIZE];
		int path_size = readlink(full_path, real_file, BUF_SIZE);
		real_file[path_size] = '\0';
		fprintf(stdout, "->  %s", real_file);
	}

	if(true == g_paral){
		fprintf(stdout, "\n");
	}
	return 0;
}

int do_specific_ls(const char *path){
	DIR *dir = opendir(path);
	if(NULL == dir){
		//路径不是目录
		if(ENOTDIR == errno){
			char *tmp_pos = rindex((char *)path, '/');
			if(NULL == tmp_pos){
				do_file_ls("./", path);
			}else{
				char tmp_dir[BUF_SIZE] = {'\0'};
				strncpy(tmp_dir, path, tmp_pos-path);
				do_file_ls(tmp_dir, tmp_pos+1);
			}
			printf("\n");
			return 0;
		}
		perror(path);
		return -1;
	}

	if(g_paths.size() > 1){
		fprintf(stdout, "%s:\n", path);
	}

	struct dirent *entry;
	vector<string> v_names;
	while(true){
		entry = readdir(dir);
		if(NULL == entry) break;

		if('.' != entry->d_name[0]){
			//排除隐藏文件		
			v_names.push_back(entry->d_name);
		}
	}

	//按字母顺序排列文件
	sort(v_names.begin(), v_names.end());
	for(vector<string>::iterator it = v_names.begin(); it != v_names.end(); ++it){
		do_file_ls(path, it->c_str());
	}
	fprintf(stdout, "\n");

	closedir(dir);
	return 0;
}

int do_ls(){
	for(vector<string>::iterator it = g_paths.begin(); it != g_paths.end(); ++it){
		do_specific_ls(it->c_str());
	}
	return 0;
}

int main(int argc, char const *argv[]){
	for(int i = 1; i < argc; ++i){
		if('-' == argv[i][0]){
			//得到命令参数
			const char *cur = argv[i]+1;
			while(*cur != '\0'){
				if('l' == *cur){
					g_paral = true;
				}else{
					fprintf(stderr, "invalid option -- \'%c\'\n", *cur);
					return 1;
				}
				cur++;
			}
		}else{
			g_paths.push_back(argv[i]);
		}
	}

	if(0 == g_paths.size()){
		g_paths.push_back(".");		//默认为当前目录
	}

	do_ls();

	return 0;
}