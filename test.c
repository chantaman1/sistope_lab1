#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h> //Unncoment in Linux
#include <math.h>
#define TRUE 1
#define FALSE 0

int main(int argc, char* argv[])
{
    printf("Se ha copiado satisfactoriamente el código en un nuevo proceso\n");
    printf("argc: %s\n", argv[1]);
    printf("#### Fin del proceso hijo ####\n");
    return 0;
}