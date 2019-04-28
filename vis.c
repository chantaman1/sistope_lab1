#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h> //Unncoment in Linux
#include <math.h>
#define TRUE 1
#define FALSE 0
#define SIZE 512

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

void* obtenerVisibilidadRecibida(char* visibilidad){
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

int main(int argc, char* argv[])
{
    mediaReal = 0;
    mediaImaginaria = 0;
    ruido = 0;
    totalVisibilidades = 0;
    int fd[2]; //PIPE PARA RECIBIR INFORMACIÓN DEL PADRE.
    int fn[2]; //PIPE PARA ENVIAR INFORMACIÓN AL PADRE.
    char buffer[SIZE];
    char* ejemplo = (char*)malloc(128*sizeof(char));
    ejemplo = "46.75563,-160.447845,-0.014992,0.005196,0.005011";
    obtenerVisibilidadRecibida(ejemplo);
    ejemplo = "119.08387,-30.927526,0.016286,-0.001052,0.005888";
    obtenerVisibilidadRecibida(ejemplo);
    printf("MediaR: %lf, MediaI: %lf, Ruido: %lf", mediaReal, mediaImaginaria, ruido);
    return 0;
}