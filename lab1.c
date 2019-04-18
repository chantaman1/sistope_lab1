#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h> //Unncoment in Linux
#define TRUE 1
#define FALSE 0

int main(int argc, char* argv[])
{
    printf("\n\n##### Inicio de la ejecucion #####\n\n");
    //Manejo de las banderas
    //Variables de entrada
    int bFlag = FALSE;          //-b Bandera indicadora de imprimir datos en pantalla
    char * fileIn = NULL;       //-i Nombre del archivo de entrada
    char * fileOut = NULL;      //-o Nombre del archivo de salida
    int discCant = 0;           //-n Cantidad de discos
    int discWidth = 0;          //-d Ancho de cada disco

    if (argc < 9) //Si hay menos de 9 argumentos, termina la ejecución del programa y entrega mensaje de error
    {
        printf("La cantidad de argumentos es menor a la requerida\n");
        return 0;
    }
    else
    {
        int c; //Auxiliar
        while ((c = getopt(argc,argv,"i:o:n:d:b")) != -1)
        {
            switch(c)
            {
                case 'i':
                    fileIn = optarg;
                    break;
                
                case 'o':
                    fileOut = optarg;
                    break;

                case 'n':
                    sscanf(optarg, "%i", &discCant);
                    break;

                case 'd':
                    sscanf(optarg, "%i", &discWidth);
                    break;
                
                case 'b':
                    bFlag = TRUE;
                    break;
            }
        }
    }


    printf("Archivo entrada: %s\n", fileIn);
    printf("Archivo salida: %s\n", fileOut);
    printf("Cantidad de discos: %i\n", discCant);
    printf("Ancho de cada disco: %i\n", discWidth);
    printf("BFlag: %i\n", bFlag);

    printf("\n\n##### Fin de la ejecucion #####\n\n");
    return 0;
}
