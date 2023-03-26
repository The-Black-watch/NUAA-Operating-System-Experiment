#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{

    FILE *fp;
    int fsize;
    char *buffer;
    fp = fopen(argv[1], "r");
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
}
