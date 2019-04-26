#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h> //Unncoment in Linux
#include <math.h>
#define TRUE 1
#define FALSE 0

void readLine(FILE* file, char* line){
    int i;
    int limit = 128;
    int read;

    read = fread(line, sizeof(char), limit, file);
    line[read] = '\0';

    for(i = 0; i <= read;i++)
    {
        if('\0' == line[i] || '\n' == line[i] || '\r' == line[i])
        {
            line[i] = '\0';
            break;
        }
    }

    if(i != read)
    {
        fseek(file, i - read + 1, SEEK_CUR);
    }
}

int readFile(char* filename){
    FILE* fs;
    int i = 0;
    fs = fopen(filename, "r");
    if(fs == NULL){
       printf("File %s does not exist.\r\n", filename);
       exit(0);
    }
    while(!feof(fs)){
       char *line = (char*)malloc(128*sizeof(char));
       readLine(fs, line);
       printf("linea %d: %s\r\n", i, line);
       i++;
    }
    return 0;
}

//Retorna el int correspondiente al disco que le corresponde la coordenada de entrada.
int checkDestination(double coordV, double coordU, int discWidth, int discCant){
    double distanceUV;
    double maxRadius = discWidth*discCant;
    distanceUV = sqrt(pow(coordV,2)+ pow(coordU,2));
    int disc = (distanceUV/discWidth) + 1;
    printf("Coordenada V: %lf , Coordenada U: %lf, y Distancia: %lf, Pertenecen al disco: %d\r\n", coordV, coordU, distanceUV, disc);
    return disc;
}

void writeFile(char** data){

}

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
    readFile(fileIn);
    checkDestination(0.021, 19.0, discWidth, discCant);
    checkDestination(0.0545, 0.1545, discWidth, discCant);
    checkDestination(27.0, 38.0, discWidth, discCant);

    printf("\n\n##### Fin de la ejecucion #####\n\n");
    return 0;
}
