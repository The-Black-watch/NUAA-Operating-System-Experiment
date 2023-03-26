#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>

#define MAX_CMD 200
#define MAX_PATH 200

// 按空格切割输入的命令行中字符串 
int split(char *buf,char**argv){

	while(*buf == ' '){
		buf++;
	}

	int count = 0;
	int argc = 0;

	while(*buf != '\n'){
		while(buf[count] != '\n' && buf[count] != ' '){
			count++;
		}

		if(buf[count] == '\n'){	// 一行命令结束 
			buf[count] = '\0';
			argv[argc++] = buf;
			break;
		}
		
		buf[count] = '\0';	
		argv[argc++] = buf; 
		buf += count+1;
		count = 0;
		
		while(*buf==' '){
			buf++;
		}
	}
	argv[argc] = NULL;		// 最后一个元素为NULL 
	return 0;
}

// 判断是否为内置命令 
int built_in_cmd(char **argv){
	
	// 根据第一个参数类型判断 
	if(strcmp(argv[0], "cd")==0){		
		if(chdir(argv[1])){	// 使用chdir函数切换到目标路径 
			printf("> cd:%s: No such directory!\n",argv[1]);
		}
		return 1;
	}

	if(strcmp(argv[0], "pwd")==0){
		char buf[MAX_PATH];
		printf("%s\n",getcwd(buf, sizeof(buf)));	// 使用getcwd函数显示当前路径 
		return 1;
	}

	if(strcmp(argv[0], "exit")==0){
		exit(0);
	}

	return 0;	
}

// 解析指令 
void eval(char *cmd_str){	

	char *argv[MAX_CMD];		// 储存切分处理后的字符串 
	char buf[MAX_CMD];			// 储存命令行中字符串 

	strcpy(buf, cmd_str);

	split(buf, argv);			// 切分字符串 
	
	if(argv[0] == NULL)			// 空命令 
		return;					
	if(built_in_cmd(argv)) 		// 判断是否为内置命令 
		return;
		
	int pid;
	pid = fork();				// 创建子进程 
	if(pid == 0)				// 处于子进程中 
	{
		if(execvp(argv[0], argv) < 0){	// 使用execvp函数执行命令 
			printf("Non-existent command:%s.\n", argv[0]);
			exit(0);
		}
	}
	else
	{
		wait(NULL);				// 等待子进程结束 
	}
}

int main(int argc, char *argv[]){

	char cmd_str[MAX_CMD];
	int n;
	while(1){
		printf("> ");
		fflush(stdout);						
		
		if((n = read(0, cmd_str, MAX_CMD)) < 0){	/*read command*/
			printf("Falied to read command!");
		}								
		eval(cmd_str);
	}
	return 0;
}