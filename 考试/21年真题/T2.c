// 实现命令 mygrep string file
// + 该命令逐行读取文件，如果行包括目标字符串，则打印该行
// + 该命令有两个命令行参数
//   - 参数 string，要搜索的字符串
//   - 参数 file，要查找的文件名
// 
// 例子，在文件 /etc/passwd 中查找字符串 root，打印包含该字符串的行
// $ ./mygrep root /etc/passwd
// root:x:0:0:root:/root:/bin/bash
//
// 提示，可以使用函数 strstr 在字符串中查找字符串
// https://www.runoob.com/cprogramming/c-function-strstr.html
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char *argv[]){

    FILE *fp;
    int fsize;
    char *buffer;
    fp = fopen(argv[2], "r");
    if (!fp)
    {
        printf("Error:open file wrong!\n");
        exit(0);
    }
    fseek(fp, 0, SEEK_END);
    fsize = ftell(fp);
    rewind(fp);
    buffer = (char *)malloc((1 + fsize) * sizeof(char));
    if (!buffer)
    {
        printf("Error:memory wrong!\n");
        exit(0);
    }
    if (!fread(buffer, 1, fsize, fp))
    {
        printf("Error:read file wrong!\n");
        exit(0);
    }
    printf("%s\n", buffer);

   char *haystack = buffer;
   char *needle = argv[1];
   char *ret;
 
   ret = strstr(haystack, needle);
   
   printf("%s\n", ret);
   
   return(0);
}
