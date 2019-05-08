#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h> //Unncoment in Linux
#include <math.h>
#include <sys/types.h>
#include <sys/wait.h>
#define TRUE 1
#define FALSE 0
#define LECTURA 0
#define ESCRITURA 1
#define SIZE 128

//VARIABLES GLOBALES.
double mediaReal;
double mediaImaginaria;
double ruido;
int totalVisibilidades;

//PERMITE CALCULAR LA MEDIA DE LA PARTE REAL.
double calcularMediaReal(){
    double media = (1/totalVisibilidades)*mediaReal;
    return media;
}

//PERMITE CALCULAR LA MEDIA DE LA PARTE IMAGINARIA.
double calcularMediaImaginaria(){
    double media = (1/totalVisibilidades)*mediaImaginaria;
    return media;
}

//PERMITE CALCULAR LA POTENCIA CON LOS DATOS RECIBIDOS.
double calcularPotencia(){
    double potenciaTotal = sqrt(pow(mediaReal, 2) + pow(mediaImaginaria, 2));
    return potenciaTotal;
}

//DEVUELVE EL RUIDO TOTAL OBTENIDO CON LOS DATOS.
double calcularRuido(){
    double ruidoTotal = ruido;
    return ruidoTotal;
}

//ESTA FUNCION PERMITE DEJAR EL ARREGLO COMPLETO EN 0.
//CON EL OBJETIVO DE QUE NO EXISTA BASURA EN EL ARREGLO DEBIDO A LA INICIALIZACION.
void inicializarCharArray(char* array, int largo){
    int i;
    for(i = 0; i < largo; i++){
      array[i] = 0;
    }
}

//LE ASIGNO LOS DATOS RECIBIDOS A CADA VARIABLE.
//SE REALIZA LA SUMATORIA DE CADA ITEM RECIBIDO.
void asignarVisibilidadRecibida(double* datosVisibilidad){
    mediaReal = mediaReal + datosVisibilidad[2];
    mediaImaginaria = mediaImaginaria + datosVisibilidad[3];
    ruido = ruido + datosVisibilidad[4];
}

void obtenerVisibilidadRecibida(char* visibilidad){
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
    asignarVisibilidadRecibida(data); //ASIGNO LA INFORMACION A SU VARIABLE RESPECTIVA.
}

double* prepararDatosVisibilidad(){
    double* data = (double*)malloc(5*sizeof(double));
    data[0] = calcularMediaReal();
    data[1] = calcularMediaImaginaria();
    data[2] = calcularPotencia();
    data[3] = calcularRuido();
    data[4] = (double) totalVisibilidades;
    return data;
}

int main(int argc, char* argv[])
{
    printf("\n\n##### Inicio de la ejecucion HIJO #####\n\n");
    
    int nro = atoi(argv[1]);


    mediaReal = 0;
    mediaImaginaria = 0;
    ruido = 0;
    totalVisibilidades = 0;

    //printf("Soy el hijo nro: %i\n", nro);
    //printf("Voy a leer \n");

    //read(STDIN_FILENO, buffer, SIZE);
    //write(STDOUT_FILENO, buffer, SIZE);
    
    

    printf("\n\n##### Fin de la ejecucion HIJO #####\n\n");
    return 0;
}
