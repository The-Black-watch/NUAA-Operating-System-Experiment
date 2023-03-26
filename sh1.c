#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>

#define MAX_CMD 200
#define MAX_PATH 200

// ���ո��и���������������ַ��� 
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

		if(buf[count] == '\n'){	// һ��������� 
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
	argv[argc] = NULL;		// ���һ��Ԫ��ΪNULL 
	return 0;
}

// �ж��Ƿ�Ϊ�������� 
int built_in_cmd(char **argv){
	
	// ���ݵ�һ�����������ж� 
	if(strcmp(argv[0], "cd")==0){		
		if(chdir(argv[1])){	// ʹ��chdir�����л���Ŀ��·�� 
			printf("> cd:%s: No such directory!\n",argv[1]);
		}
		return 1;
	}

	if(strcmp(argv[0], "pwd")==0){
		char buf[MAX_PATH];
		printf("%s\n",getcwd(buf, sizeof(buf)));	// ʹ��getcwd������ʾ��ǰ·�� 
		return 1;
	}

	if(strcmp(argv[0], "exit")==0){
		exit(0);
	}

	return 0;	
}

// ����ָ�� 
void eval(char *cmd_str){	

	char *argv[MAX_CMD];		// �����зִ������ַ��� 
	char buf[MAX_CMD];			// �������������ַ��� 

	strcpy(buf, cmd_str);

	split(buf, argv);			// �з��ַ��� 
	
	if(argv[0] == NULL)			// ������ 
		return;					
	if(built_in_cmd(argv)) 		// �ж��Ƿ�Ϊ�������� 
		return;
		
	int pid;
	pid = fork();				// �����ӽ��� 
	if(pid == 0)				// �����ӽ����� 
	{
		if(execvp(argv[0], argv) < 0){	// ʹ��execvp����ִ������ 
			printf("Non-existent command:%s.\n", argv[0]);
			exit(0);
		}
	}
	else
	{
		wait(NULL);				// �ȴ��ӽ��̽��� 
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