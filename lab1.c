#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h> //Unncoment in Linux
#include <math.h>
#define TRUE 1
#define FALSE 0
#define LECTURA 0
#define ESCRITURA 1

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

//Retorna el int correspondiente al disco que le corresponde la coordenada de entrada.
int checkDestination(double coordV, double coordU, int discWidth, int discCant){
    double distanceUV;
    double maxRadius = discWidth*discCant;
    distanceUV = sqrt(pow(coordV,2)+ pow(coordU,2));
    int disc = (distanceUV/discWidth) + 1;
    if(distanceUV > maxRadius){
       return -1;
    }
    else{
       return disc;
    }
}

void inicializarCharArray(char* array, int largo){
    int i;
    for(i = 0; i < largo; i++){
      array[i] = 0;
    }
}

int obtenerVisibilidadRecibida(char* visibilidad, int discWidth, int discCant){
    int i; //DECLARACION DE i PARA EL CICLO FOR.
    int j = 0; //DECLARACION DE j PARA POSICIONAR DOUBLE EN ARREGLO.
    int k = 0; //DECLARACION DE k PARA POSICIONAR CHAR EN ARREGLO.
    int len = strlen(visibilidad); //OBTENGO EL LARGO DEL STRING RECIBIDO.
    int arrayLength = 32;
    double* data = (double*)malloc(len*sizeof(double));
    char* temp = (char*)malloc(arrayLength*sizeof(char));
    inicializarCharArray(temp, arrayLength); //INICIALIZO EL ARREGLO PARA QUE NO TENGA BASURA.
    for(i = 0; i <= len; i++){
      if(i == len){ //SI LOS LARGOS SON IGUALES, SIGNIFICA QUE TERMINE DE PROCESAR LA INFORMACION.
        data[j] = atof(temp);
        free(temp);
      }
      else if(visibilidad[i] == 44){ //SI EN LA POSICION HAY UNA ',', SIGNIFICA QUE TERMINE DE LEER UN DATO.
        data[j] = atof(temp);
        j++;
        k = 0;
        free(temp); //LIBERO LA MEMORIA PARA EVITAR LEAKS.
        temp = (char*)malloc(32*sizeof(char)); //LE ASIGNO ESPACIO EN LA RAM NUEVAMENTE.
        inicializarCharArray(temp, arrayLength); //LE VUELVO A ASIGNAR CEROS PARA QUE NO HAYA BASURA.
      }
      else{
        temp[k] = visibilidad[i];
        k++;
      }
    }
    return checkDestination(data[0], data[1], discWidth, discCant); //BUSCO EL DISCO AL QUE PERTENECE LA INFO.
}

void writeFile(double* informacionHijos, char* nombreArchivo, int numDisco){
  FILE *fp;
  int i;
  char** str = (char**)malloc(4*sizeof(char*));
  for(i = 0; i < 4 ; i++){
    str[i] = (char*)malloc(20*sizeof(char));
  }
  str[0] = "Media real: ";
  str[1] = "Media imaginaria: ";
  str[2] = "Potencia: ";
  str[3] = "Ruido total: ";
  char disco[] = "Disco ";
  char endDisc[] = ":\r\n";
  char endLine[] = "\r\n";
  fp = fopen(nombreArchivo, "a+");
  fwrite(disco, 1 , 7, fp);
  fwrite(&numDisco, 1 , sizeof(int), fp);
  fwrite(endDisc, 1, 6, fp);
  for(i = 0; i < 4; i++){
    fwrite(str[i], 1, sizeof(str[i]), fp);
    fwrite(&informacionHijos[i], 1, sizeof(double), fp);
    fwrite(endLine, 1, 5, fp);
  }
  fclose(fp);
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
    //Variables de procesamiento.
    FILE* fs;

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


    int i;
    int ** pipesLectura = (int**)malloc(sizeof(int*)*discCant);         //SE CREA UN ARREGLO DE TAMAÑO DISCCANT PARA LA LECTURA DEL PADRE
    int ** pipesEscritura = (int**)malloc(sizeof(int*)*discCant);       //SE CREA UN ARREGLO DE TAMAÑO DISCCANT PARA LA ESCRITURA DEL PADRE
    for(i=0; i<discCant; i++)                                           //SE CREARÁN TANTOS HIJOS COMOS DISCOS
    {
        int pid = fork();

        if(pid == -1)
        {
            printf("Error en el fork\n");
            return 1;
        }

        pipesLectura[i] = (int*)malloc(sizeof(int)*2);                  //SE CREAN DOS PIPES POR CADA HIJO PARA LEER
        pipesEscritura[i] = (int*)malloc(sizeof(int)*2);                //SE CREAN DOS PIPES POR CADA HIJO PARA ESCRIBIR
        pipe(pipesLectura[i]);
        pipe(pipesEscritura[i]);

        if(pid == 0) //Si el proceso es el hijo...
        {
            close(pipesLectura[i][LECTURA]);    //SE CIERRA EL PIPE DE LECTURA, YA QUE EL PADRE LEERÁ DESDE ESTE PIPE
            char* arr[] = {(const char*)NULL};
            int exeint = execv("./vis", arr);   //SE COPIARÁ EL EJECUTABLE COMPILADO PREVIAMENTE DEL ARCHIVO VIS.C
            printf("No se ha copiado el nuevo programa\n");
        }
        else
        {
            close(pipesEscritura[i][ESCRITURA]); //SE CIERRA EL PIPE DE ESCRITURA, YA QUE EL PADRE ESCRIBIRÁ DESDE ESTE PIPE
        }
    }

    int k = 1;
    fs = fopen(fileIn, "r");
    if(fs == NULL){
       printf("File %s does not exist.\r\n", fileIn);
       exit(0);
    }
    while(!feof(fs)){
       char *line = (char*)malloc(128*sizeof(char));
       readLine(fs, line);
       if(line[0] == '\0'){
         //AQUI ES CUANDO SE AVISA A LOS HIJOS DE FIN
         //Y SE LES PIDE LOS DATOS CALCULADOS.
         //PLAN: RECIBIR LOS DATOS DE LOS HIJOS Y LUEGO ALMACENARLO EN UN ARCHIVO.
         printf("FIN");
       }
       else{
         //AQUI SE LES ENTREGA LINEA A LINEA LOS DATOS DE ENTRADA.
         //A CADA HIJO QUE TENGAMOS.
         //PLAN: ENVIAR LINE AL HIJO SELECCIONADO EN DISC MEDIANTE PIPE.
         int disc = obtenerVisibilidadRecibida(line, discWidth, discCant);
         printf("Informacion: %s, Pertenece al disco: %d\r\n", line, disc);
         double* ggwp = (double*)malloc(5*sizeof(double));
         ggwp[0] = 0.514541;
         ggwp[1] = 2.254541;
         ggwp[2] = 5.645127;
         ggwp[3] = 6.544200;
         writeFile(ggwp, fileOut, k);
         k++;
       }
    }

    printf("\n\n##### Fin de la ejecucion #####\n\n");
    return 0;
}
