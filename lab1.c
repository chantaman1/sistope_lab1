#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h> //Unncoment in Linux
#include <math.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <getopt.h>
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
        if('\0' == line[i] || '\n' == line[i] || '\r' == line[i] || line[i] == 0)
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
    int disc = (distanceUV/discWidth);
    if(distanceUV > maxRadius){
       return discCant - 1;
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
  char numArr[512];
  sprintf(numArr, "%d", numDisco);
  char disco[] = "Disco ";
  char endDisc[] = ":\r\n";
  char endLine[] = "\r\n";
  fp = fopen(nombreArchivo, "a+b");
  fwrite(disco, sizeof(char) , strlen(disco), fp);
  fwrite(numArr, sizeof(char) , strlen(numArr), fp);
  fwrite(endDisc, sizeof(char), strlen(endDisc), fp);
  for(i = 0; i < 4; i++){
    fwrite(str[i], sizeof(char), strlen(str[i]), fp);
    sprintf(numArr, "%lf", informacionHijos[i]);
    fwrite(numArr, sizeof(char), strlen(numArr), fp);
    fwrite(endLine, sizeof(char), strlen(endLine), fp);
  }
  fwrite(endLine, sizeof(char), strlen(endLine), fp);
  fclose(fp);
}

int main(int argc, char* argv[])
{
    printf("\n\n##### Inicio de la ejecucion PADRE #####\n\n");
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

    //DEBUG
    printf("Iniciando procesamiento con %i discos...\n", discCant);

    int i;
    pid_t child_pid, wpid;
    int status = 0;
    int * childs = (int*)malloc(sizeof(int)*discCant);
    int ** pipesLectura = (int**)malloc(sizeof(int*)*discCant);         //SE CREA UN ARREGLO DE TAMAÑO DISCCANT PARA LA LECTURA DEL PADRE
    int ** pipesEscritura = (int**)malloc(sizeof(int*)*discCant);       //SE CREA UN ARREGLO DE TAMAÑO DISCCANT PARA LA ESCRITURA DEL PADRE
    for(i=0; i<discCant; i++)                                           //SE CREARÁN TANTOS HIJOS COMOS DISCOS
    {

        pipesLectura[i] = (int*)malloc(sizeof(int)*2);                  //SE CREAN DOS PIPES POR CADA HIJO PARA LEER (que el padre lea)
        pipesEscritura[i] = (int*)malloc(sizeof(int)*2);                //SE CREAN DOS PIPES POR CADA HIJO PARA ESCRIBIR (que el padre escriba)
        pipe(pipesLectura[i]);
        pipe(pipesEscritura[i]);
                                            //EL PADRE...
        //close(pipesLectura[i][ESCRITURA]);    //SE CIERRA EL PIPE DE LECTURA, YA QUE EL PADRE LEERÁ DESDE ESTE PIPE
        //close(pipesEscritura[i][LECTURA]);    //SE CIERRA EL PIPE DE ESCRITURA, YA QUE EL PADRE ESCRIBIRÁ DESDE ESTE PIPE

        child_pid = fork(); //SE CREA EL NUEVO HIJO

        if(child_pid == -1)
        {
            printf("Error en el fork\n");
            return 1;
        }

        if(child_pid == 0) //Si el proceso es el hijo...
        {
            //ENTONCES EL HIJO:
            // - LEERÁ DESDE STDIN
            // - ESCRIBIRÁ POR STDOUT

            //dup2(pipesLectura[i][ESCRITURA], STDOUT_FILENO);
            //close(pipesLectura[i][LECTURA]);        //SE CIERRA EL PIPE DE ESCRITURA, YA QUE EL PADRE LEERÁ DESDE ESTE PIPE

            //dup2(pipesEscritura[i][LECTURA], STDIN_FILENO);
            //close(pipesEscritura[i][ESCRITURA]);    //SE CIERRA EL PIPE DE LECTURA, YA QUE EL PADRE ESCRIBIRÁ DESDE ESTE PIPE


            //ESTE ES EL BUENO
            char * canalLecturaHijo = (char*)malloc(sizeof(char)*10);
            char * canalEscrituraHijo = (char*)malloc(sizeof(char)*10);

            sprintf(canalLecturaHijo, "%i", pipesEscritura[i][LECTURA]);
            sprintf(canalEscrituraHijo, "%i", pipesLectura[i][ESCRITURA]);

            char* args[] = {"./vis", canalLecturaHijo, canalEscrituraHijo, (char*)NULL};

            if(execvp(args[0], args) == -1) //SE COPIARÁ EL EJECUTABLE COMPILADO PREVIAMENTE DEL ARCHIVO VIS.C
            {
                printf("No se ha copiado el nuevo programa\n");
                exit(1);
            }
        }
        else {
            childs[i] = child_pid;
        }
    }
    //EN ESTE PUNTO SE HAN CREADO TODOS LOS HIJOS Y SE HAN COMUNICADO MEDIANTE PIPES
    //TEST: enviando datos a un hijo
    //read(pipesLectura[0][LECTURA],buff,100);
    //Datoshijos:
    //0. media real
    //1. imaginaria
    //2. portencia
    //3. ruido
    //4. vis totales
    fs = fopen(fileIn, "r");
    int j;
    double **dataHijos = (double**)malloc(sizeof(double*)*discCant);
    for(j = 0; j < discCant; j++){
      dataHijos[j] = (double*)malloc(sizeof(double)*512);
    }
    if(fs == NULL){
       printf("File %s does not exist.\r\n", fileIn);
       exit(0);
    }
    int count = 1;
    printf("Procesando linea: \r\n");
    while(!feof(fs)){
       char *line = (char*)malloc(128*sizeof(char));
       readLine(fs, line);
       if(line[0] == '\0'){
         //AQUI ES CUANDO SE AVISA A LOS HIJOS DE FIN
         //Y SE LES PIDE LOS DATOS CALCULADOS.
         //PLAN: RECIBIR LOS DATOS DE LOS HIJOS Y LUEGO ALMACENARLO EN UN ARCHIVO.
         char* endProgram = "FIN";
         for(j = 0; j < discCant; j++){
           write(pipesEscritura[j][ESCRITURA], endProgram, strlen(endProgram));
         }
         for(j = 0; j < discCant; j++){
           read(pipesLectura[j][LECTURA], dataHijos[j], 512);
         }
       }
       else{
        //AQUI SE LES ENTREGA LINEA A LINEA LOS DATOS DE ENTRADA.
        //A CADA HIJO QUE TENGAMOS.
        //PLAN: ENVIAR LINE AL HIJO SELECCIONADO EN DISC MEDIANTE PIPE.
        int disc = obtenerVisibilidadRecibida(line, discWidth, discCant);
        if(disc >= 0)
        {
            write(pipesEscritura[disc][ESCRITURA], line, strlen(line));
            //write(pipesEscritura[disc][ESCRITURA], line, 128);
            //printf("Informacion: %s, Pertenece al disco: %d\r\n", line, disc);
        }
        //Esto permite hacer conocer al usuario que linea del archivo el programa esta leyendo.
        printf("\b\b\b\b\b\b\b\b\b");
        fflush(stdout);
        printf("%.7d", count);
        fflush(stdout);
        count = count + 1;
       }
       usleep(5000);
    }
    while ((wpid = wait(&status)) > 0);
    for(i = 0; i < discCant; i++){
      writeFile(dataHijos[i], fileOut, i + 1);
    }
    //Acá empezamos a guardar
    if(bFlag)
    {
        int total = 0;
        printf("\r\n");
        for(i = 0; i < discCant; i++)
        {
            printf("Soy el hijo, de pid %i y procese %i visibilidades\r\n", childs[i], (int)dataHijos[i][4]);
            total = total + (int)dataHijos[i][4];
        }
        printf("Total de visibilidades procesadas por mis hijos: %i\r\n", total);

    }
    //Antes de finalizar el programa, liberamos la memoria.
    //Liberamos la memoria del arreglo doble de Double.
    for(i = 0; i < discCant; i++){
      free(dataHijos[i]);
    }
    free(dataHijos);
    //Ahora liberamos los pipes.
    for(i = 0; i < discCant; i++){
      free(pipesEscritura[i]);
      free(pipesLectura[i]);
    }
    free(pipesEscritura);
    free(pipesLectura);
    //Liberamos la memoria del resto.
    free(childs);

    printf("\n\n##### Fin de la ejecucion PADRE #####\n\n");
    return 0;
}
