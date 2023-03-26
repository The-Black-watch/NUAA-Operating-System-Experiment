#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{

    FILE *fp;
	  FILE *fw;
    int fsize;
    char *buffer;
    fp = fopen(argv[1], "r");
    if (!fp)
    {
        printf("Error:open file1 wrong!\n");
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
    fw = fopen(argv[2], "w");
    if (!fw)
    {
        printf("Error:open file2 wrong!\n");
        exit(0);
    }
    fwrite(buffer, 1, fsize, fw);
}
