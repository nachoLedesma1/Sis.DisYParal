#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<mpi.h>
#include <time.h>
typedef struct {
        int blanco;
        int azul;
        int rojo;
        int naranja;
        int verde;
    }Salud;
    typedef struct {
        Salud Estado;//Blanco, Azul, Rojo, Verde y Naranja
        int Edad; //en relación a la cantidad de semanas de vida
        int Herida; //ver si no poner de otro tipo
        int Enfermo; //cantidad de semanas que estuvo enferomo siendo 0 que esta sano
        int Podado;
    } Celda;

int aniosVida (int semanas){
    //48 semanas para un año (aunque es 52 en realidad preguntar cual tomar )

    return semanas/48; //como es int va a devolver solo en entero asi que si da 4,9 devuelve 4 nomas o si es 4,01 devuelve 4

}
void inicializar (Celda *ma, int fila, int columna){
    int ran;
    float ran2;
    for (int i = 0; i < fila*columna; i++){
        
        ma[i].Herida = rand() % 2; //random de 0 a 1
        ran = rand() % 101;// random de 0 a 100
        if(ran <= 65){//arboles sanos
            ma[i].Estado.verde = 1;
            ma[i].Estado.naranja = 0;
            ma[i].Estado.rojo = 0;
            ma[i].Estado.azul = 0;
            ma[i].Estado.blanco = 0;
        }
        else {
            ran2 = (rand() % 1001)/10;
            if(ran2 <= 1.5){//enfermo con sintomas
                ma[i].Estado.verde = 0;
                ma[i].Estado.naranja = 0;
                ma[i].Estado.rojo = 1;
                ma[i].Estado.azul = 0;
                ma[i].Estado.blanco = 0;
            }
            else {
                ran2 = (rand() % 1001)/10;
                if(ran2 <= 0.5){//enfermo sin sintomas
                    ma[i].Estado.verde = 0;
                    ma[i].Estado.naranja = 1;
                    ma[i].Estado.rojo = 0;
                    ma[i].Estado.azul = 0;
                    ma[i].Estado.blanco = 0;
                }
                else{
                    ran = rand() % 101;// random de 0 a 100
                    if(ran <= 50){
                        ma[i].Estado.verde = 0;
                        ma[i].Estado.naranja = 0;
                        ma[i].Estado.rojo = 0;
                        ma[i].Estado.azul = 1;
                        ma[i].Estado.blanco = 0;
                    }
                    else {
                        ma[i].Estado.verde = 0;
                        ma[i].Estado.naranja = 0;
                        ma[i].Estado.rojo = 0;
                        ma[i].Estado.azul = 0;
                        ma[i].Estado.blanco = 1;
                    }
                }
            }
        }
        //edades
        ran = rand() % 101;// edad de 0 a 100
        if(ran <= 30){//joven
            ma[i].Edad = rand() % 145; //random de 0 a 144 semanas
        }
        if(ran > 30 && ran<= 80){//adulto
            //se multilica por las 48 porque son la cantidad de semanas para hacer 1 año
            ma[i].Edad = rand() % (35*48 - 4*48 +1) + 4*48; //(N-M+1) + M; n mayor que m
        }
        if(ran > 80){
            ma[i].Edad = rand() % (100*48 - 36*48 + 1) + 36*48; // entre 36 y 100 el random
        }
        //por si geera problemas no incializar
        ma[i].Podado = 0;
        ma[i].Enfermo = 0;
    }//for i
}
void imprimirMatris (Celda *ma, int cant){

    printf ("La matris es la siguiente \n");
    for (int i= 0; i < cant*cant; i++){
        
        if(ma[i].Estado.verde == 1){
            printf("V ");
        }
        if(ma[i].Estado.naranja == 1){
            printf("N ");
        }
        if(ma[i].Estado.rojo == 1){
            printf("R ");
        }
        if(ma[i].Estado.azul == 1){
            printf("A ");
        }
        if(ma[i].Estado.blanco == 1){
            printf("B ");
        }

        printf("\n");
    }//for i
}

void proxEstado (Celda **ma, Celda **ma2,Celda **sup1, Celda **sup2, Celda **inf1, Celda **inf2 ,int cantCelReal, int t, int caso, int fila){// el t es el clock

    Celda *aux; //para cuando se cambia de estado
    int i, vecEnfermo, suceptibilidad, pContagio, cantVecin;
    int cantCeldas = cantCelReal-1; //porque lo hice así sin darme Cuenta
    int ran;
    Salud vecino;
    for(int i = 0; i<fila; i++){//filas Depende de ma
        for(int j = 0; j<cantCelReal; j++){//columnas 
            vecino.blanco = 0;
            vecino.azul = 0;
            vecino.rojo = 0;
            vecino.naranja = 0;
            vecino.verde = 0;
            if((*ma)[(i*cantCelReal)+j].Estado.verde == 1){ // si esta sano
                if((i == 0 || i == cantCeldas) && (j == 0 || j == cantCeldas)){ //Las cuatro esquinas
                    if(j == 0){
                        vecino.rojo = (*ma)[(i*cantCelReal)+j+1].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[(i*cantCelReal)+j+2].Estado.rojo + vecino.rojo;
                    }
                    else {
                        vecino.rojo = (*ma)[(i*cantCelReal)+j-1].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[(i*cantCelReal)+j-2].Estado.rojo + vecino.rojo;
                    }
                    if(i == 0){
                        if(j == 0){
                            vecino.rojo = (*ma)[((i+1)*cantCelReal)+j+1].Estado.rojo + vecino.rojo;
                             if(caso != 1){//nodo != 0
                                vecino.rojo = (*sup1)[i+j].Estado.rojo + vecino.rojo;
                                vecino.rojo = (*sup1)[i+j+1].Estado.rojo + vecino.rojo;
                                vecino.rojo = (*sup2)[i+j].Estado.rojo + vecino.rojo;
                             }// nodo != 0 
                        }
                        else {
                            vecino.rojo = (*ma)[((i+1)*cantCelReal)+j-1].Estado.rojo + vecino.rojo;
                        }
                        vecino.rojo = (*ma)[((i+1)*cantCelReal)+j].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[((i+2)*cantCelReal)+j].Estado.rojo + vecino.rojo;
                                                      
                    }
                    else {
                        if(j == 0){
                            vecino.rojo = (*ma)[((i-1)*cantCelReal)+j+1].Estado.rojo + vecino.rojo;
                        }
                        else {
                            vecino.rojo = (*ma)[((i-1)*cantCelReal)+j-1].Estado.rojo + vecino.rojo;
                            
                        }
                        vecino.rojo = (*ma)[((i-1)*cantCelReal)+j].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[((i-2)*cantCelReal)+j].Estado.rojo + vecino.rojo;
                        if(caso != 3){
                            vecino.rojo = (*inf1)[i+j].Estado.rojo + vecino.rojo;
                            vecino.rojo = (*inf1)[i+j+1].Estado.rojo + vecino.rojo;
                            vecino.rojo = (*inf2)[i+j].Estado.rojo + vecino.rojo;
                        }
                    }
                    if(caso == 1 ){
                        if(i == 0){
                            cantVecin = 5;
                        }
                        else {
                            cantVecin = 8;
                        }
                    }
                    if(caso == 2){
                        cantVecin = 8;
                    }
                    if(caso == 3){
                        if(i == 0){
                            cantVecin = 8;
                        }
                        else {
                            cantVecin = 5;
                        }
                    }
                            
                }//cuatro esquinas
                if((i == 0 || i == cantCeldas) && (j == 1 || j == cantCeldas -1)){ //fila superior e inferior caso 7
                    if(j == 1){
                        vecino.rojo = (*ma)[(i*cantCelReal)+j-1].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[(i*cantCelReal)+j+1].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[(i*cantCelReal)+j+2].Estado.rojo + vecino.rojo;
                    }
                    else {
                        vecino.rojo = (*ma)[(i*cantCelReal)+j+1].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[(i*cantCelReal)+j-1].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[(i*cantCelReal)+j-2].Estado.rojo + vecino.rojo;
                    }
                    if(i == 0){ //fila superior
                        vecino.rojo = (*ma)[((i+1)*cantCelReal)+j+1].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[((i+1)*cantCelReal)+j-1].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[((i+1)*cantCelReal)+j].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[((i+2)*cantCelReal)+j].Estado.rojo + vecino.rojo;
                        if(caso != 1){
                            vecino.rojo = (*sup1)[i+j-1].Estado.rojo + vecino.rojo;
                            vecino.rojo = (*sup1)[i+j+1].Estado.rojo + vecino.rojo;
                            vecino.rojo = (*sup1)[i+j].Estado.rojo + vecino.rojo;
                            vecino.rojo = (*sup2)[i+j].Estado.rojo + vecino.rojo;
                        }
                    }
                    else { //fila inferior
                        vecino.rojo = (*ma)[((i-1)*cantCelReal)+j+1].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[((i-1)+cantCelReal)+j-1].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[((i-1)*cantCelReal)+j].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[((i-2)*cantCelReal)+j].Estado.rojo + vecino.rojo;
                        if(caso != 3){
                            vecino.rojo = (*inf1)[i+j-1].Estado.rojo + vecino.rojo;
                            vecino.rojo = (*inf1)[i+j+1].Estado.rojo + vecino.rojo;
                            vecino.rojo = (*inf1)[i+j].Estado.rojo + vecino.rojo;
                            vecino.rojo = (*inf2)[i+j].Estado.rojo + vecino.rojo;
                        }
                    }
                    if(caso == 1){
                        if(i == 0){
                            cantVecin = 7;
                        }
                        else{
                            cantVecin = 11;
                        }
                    }
                    if(caso == 2){
                        cantVecin = 11;
                    }
                    if(caso == 3){
                        if(i == 0){
                            cantVecin = 11;
                        }
                        else {
                            cantVecin = 7;
                        }
                    }
                    
                }//fila superior e inferior
                if((i == 1 || i == cantCeldas-1) && (j == 0 || j == cantCeldas)){//columna izquierda y derecha caso 7
                    if(i == 1){
                        vecino.rojo = (*ma)[((i-1)*cantCelReal)+j].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[((i+1)*cantCelReal)+j].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[((i+2)*cantCelReal)+j].Estado.rojo + vecino.rojo;
                        if(caso != 1){
                            vecino.rojo = (*sup2)[i+j].Estado.rojo + vecino.rojo;
                        }
                    }
                    else {
                        vecino.rojo = (*ma)[((i+1)*cantCelReal)+j].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[((i-1)*cantCelReal)+j].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[((i-2)*cantCelReal)+j].Estado.rojo + vecino.rojo;
                        if(caso != 3){
                            vecino.rojo = (*inf2)[i+j].Estado.rojo + vecino.rojo;
                        }
                    }
                    if(j == 0){
                        vecino.rojo = (*ma)[((i-1)*cantCelReal)+j+1].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[(i*cantCelReal)+j+1].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[(i*cantCelReal)+j+2].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[((i+1)*cantCelReal)+j+1].Estado.rojo + vecino.rojo;
                    }
                    else {
                        vecino.rojo = (*ma)[((i-1)*cantCelReal)+j-1].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[(i*cantCelReal)+j-1].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[(i*cantCelReal)+j-2].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[((i+1)*cantCelReal)+j-1].Estado.rojo + vecino.rojo;
                    }
                    if(caso == 1){
                        if(i == 1){
                            cantVecin = 7;
                        }
                        else {
                            cantVecin = 8;
                        }
                    }
                    if(caso == 2){
                        cantVecin = 7;
                    }
                    if(caso == 3){
                        if(i == 1){
                            cantVecin = 8;
                        }
                        else{
                            cantVecin = 7;
                        }
                    }                    
                }//columna izquierda y derecha
                if((i == 0 || i == cantCeldas) && (j > 1 && j < cantCeldas-1)){ //demas casos 1ra fila superior e inferior
                    if(i == 0){
                        vecino.rojo = (*ma)[(i*cantCelReal)+j+2].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[(i*cantCelReal)+j+1].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[(i*cantCelReal)+j-2].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[(i*cantCelReal)+j-1].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[((i+2)*cantCelReal)+j].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[((i+1)*cantCelReal)+j].Estado.rojo + vecino.rojo;

                        vecino.rojo = (*ma)[((i+1)*cantCelReal)+j+1].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[((i+1)*cantCelReal)+j-1].Estado.rojo + vecino.rojo;
                        if(caso != 1){
                            vecino.rojo = (*sup1)[i+j-1].Estado.rojo + vecino.rojo;
                            vecino.rojo = (*sup1)[i+j+1].Estado.rojo + vecino.rojo;
                            vecino.rojo = (*sup1)[i+j].Estado.rojo + vecino.rojo;
                            vecino.rojo = (*sup2)[i+j].Estado.rojo + vecino.rojo;
                        }
                    }
                    else {
                        vecino.rojo = (*ma)[(i*cantCelReal)+j+2].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[(i*cantCelReal)+j+1].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[(i*cantCelReal)+j-2].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[(i*cantCelReal)+j-1].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[((i-2)*cantCelReal)+j].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[((i-1)*cantCelReal)+j].Estado.rojo + vecino.rojo;

                        vecino.rojo = (*ma)[((i-1)*cantCelReal)+j+1].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[((i-1)*cantCelReal)+j-1].Estado.rojo + vecino.rojo;
                        if(caso != 3){
                            vecino.rojo = (*inf1)[i+j-1].Estado.rojo + vecino.rojo;
                            vecino.rojo = (*inf1)[i+j+1].Estado.rojo + vecino.rojo;
                            vecino.rojo = (*inf1)[i+j].Estado.rojo + vecino.rojo;
                            vecino.rojo = (*inf2)[i+j].Estado.rojo + vecino.rojo;
                        }
                    }
                    if(caso == 1){
                        if(i == 0){
                            cantVecin = 8;
                        }
                        else {
                            cantVecin = 12;
                        }
                    }
                    if(caso == 2){
                        cantVecin = 12;
                    }
                    if(caso == 3){
                        if(i == 0){
                            cantVecin = 12;
                        }
                        else{
                            cantVecin = 8;
                        }
                    }
                    
                }//demas casos 1ra fila superior e inferior
                if((i > 1 && i < cantCeldas-1 ) && (j == 0 || j == cantCeldas)){//demas casos 1 columna izquierda y derecha
                    if(j == 0){
                        vecino.rojo = (*ma)[(i*cantCelReal)+j+2].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[(i*cantCelReal)+j+1].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[((i+2)*cantCelReal)+j].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[((i+1)*cantCelReal)+j].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[((i-2)*cantCelReal)+j].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[((i-1)*cantCelReal)+j].Estado.rojo + vecino.rojo;

                        vecino.rojo = (*ma)[((i+1)*cantCelReal)+j+1].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[((i-1)*cantCelReal)+j+1].Estado.rojo + vecino.rojo;
                    }
                    else {
                        vecino.rojo = (*ma)[(i*cantCelReal)+j-2].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[(i*cantCelReal)+j-1].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[((i+2)*cantCelReal)+j].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[((i+1)*cantCelReal)+j].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[((i-2)*cantCelReal)+j].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[((i-1)*cantCelReal)+j].Estado.rojo + vecino.rojo;

                        vecino.rojo = (*ma)[((i+1)*cantCelReal)+j-1].Estado.rojo + vecino.rojo;
                        vecino.rojo = (*ma)[((i-1)*cantCelReal)+j-1].Estado.rojo + vecino.rojo;
                    }
                    cantVecin = 8;
                }//demas casos 1 columna izquierda y derecha (leesto de fabrica nomas)
                if((i == 1 || i == cantCeldas-1) && (j == 1 || j == cantCeldas-1)){ //Segunda casos de las esquinas
                    vecino.rojo = (*ma)[((i-1)*cantCelReal)+j-1].Estado.rojo + vecino.rojo;
                    vecino.rojo = (*ma)[((i-1)*cantCelReal)+j].Estado.rojo + vecino.rojo;
                    vecino.rojo = (*ma)[((i-1)*cantCelReal)+j+1].Estado.rojo + vecino.rojo;
                    vecino.rojo = (*ma)[(i*cantCelReal)+j-1].Estado.rojo + vecino.rojo;
                    vecino.rojo = (*ma)[(i*cantCelReal)+j+1].Estado.rojo + vecino.rojo;
                    vecino.rojo = (*ma)[((i+1)*cantCelReal)+j-1].Estado.rojo + vecino.rojo;
                    vecino.rojo = (*ma)[((i+1)*cantCelReal)+j].Estado.rojo + vecino.rojo;
                    vecino.rojo = (*ma)[((i+1)*cantCelReal)+j+1].Estado.rojo + vecino.rojo;
                    if(i == 1){
                        vecino.rojo = (*ma)[((i+2)*cantCelReal)+j].Estado.rojo + vecino.rojo;
                        if(j == 1){
                            vecino.rojo = (*ma)[(i*cantCelReal)+j+2].Estado.rojo + vecino.rojo;
                        }
                        else{
                            vecino.rojo = (*ma)[(i*cantCelReal)+j-2].Estado.rojo + vecino.rojo;
                        }
                        if(caso != 1){
                            vecino.rojo = (*sup2)[i+j].Estado.rojo + vecino.rojo;
                        }
                    }
                    else {
                        vecino.rojo = (*ma)[((i-2)*cantCelReal)+j].Estado.rojo + vecino.rojo;
                        if(j == 1){
                            vecino.rojo = (*ma)[(i*cantCelReal)+j+2].Estado.rojo + vecino.rojo;
                        }
                        else{
                            vecino.rojo = (*ma)[(i*cantCelReal)+j-2].Estado.rojo + vecino.rojo;
                        }
                        if(caso != 3){
                            vecino.rojo = (*inf2)[i+j].Estado.rojo + vecino.rojo;
                        }
                    }
                    if(caso == 1){
                        if(i == 1){
                            cantVecin = 10;
                        }
                        else {
                            cantVecin = 11;
                        }
                    }
                    if(caso == 2){
                        cantVecin = 11;
                    }
                    if(caso == 3){
                        if(i == 1){
                            cantVecin = 11;
                        }
                        else {
                            cantVecin = 10;
                        }
                    }
                    
                }//Segunda casos de las esquinas
                if((i == 1 || i == cantCeldas-1) && (j > 1 && j < cantCeldas-1)){//segundo caso de superior e inferior
                    vecino.rojo = (*ma)[((i-1)*cantCelReal)+j-1].Estado.rojo + vecino.rojo;
                    vecino.rojo = (*ma)[((i-1)*cantCelReal)+j].Estado.rojo + vecino.rojo;
                    vecino.rojo = (*ma)[((i-1)*cantCelReal)+j+1].Estado.rojo + vecino.rojo;
                    vecino.rojo = (*ma)[(i*cantCelReal)+j-1].Estado.rojo + vecino.rojo;
                    vecino.rojo = (*ma)[(i*cantCelReal)+j+1].Estado.rojo + vecino.rojo;
                    vecino.rojo = (*ma)[((i+1)*cantCelReal)+j-1].Estado.rojo + vecino.rojo;
                    vecino.rojo = (*ma)[((i+1)*cantCelReal)+j].Estado.rojo + vecino.rojo;
                    vecino.rojo = (*ma)[((i+1)*cantCelReal)+j+1].Estado.rojo + vecino.rojo;

                    vecino.rojo = (*ma)[(i*cantCelReal)+j+2].Estado.rojo + vecino.rojo;
                    vecino.rojo = (*ma)[(i*cantCelReal)+j-2].Estado.rojo + vecino.rojo;
                    if(i == 1){
                        vecino.rojo = (*ma)[((i+2)*cantCelReal)+j].Estado.rojo + vecino.rojo;
                        if(caso != 1){
                            vecino.rojo = (*sup2)[i+j].Estado.rojo + vecino.rojo;
                        }                        
                    }
                    else {
                        vecino.rojo = (*ma)[((i-2)*cantCelReal)+j].Estado.rojo + vecino.rojo;
                        if(caso != 3){
                            vecino.rojo = (*inf2)[i+j].Estado.rojo + vecino.rojo;
                        }
                    }
                    if(caso == 1){
                        if(i == 1){
                            cantVecin = 11;
                        }
                        else{
                            cantVecin = 12;
                        }
                    }
                    if(caso == 2){
                        cantVecin = 12;
                    }
                    if(caso == 3){
                        if(i == 1){
                            cantVecin = 12;
                        }
                        else{
                            cantVecin = 11;
                        }
                    }
                    
                }//segundo caso de superior e inferior
                if((i > 1 && i < cantCeldas-1) && (j == 1 || j == cantCeldas-1)){//segundo caso izquierda y derecha
                    vecino.rojo = (*ma)[((i-1)*cantCelReal)+j-1].Estado.rojo + vecino.rojo;
                    vecino.rojo = (*ma)[((i-1)*cantCelReal)+j].Estado.rojo + vecino.rojo;
                    vecino.rojo = (*ma)[((i-1)*cantCelReal)+j+1].Estado.rojo + vecino.rojo;
                    vecino.rojo = (*ma)[(i*cantCelReal)+j-1].Estado.rojo + vecino.rojo;
                    vecino.rojo = (*ma)[(i*cantCelReal)+j+1].Estado.rojo + vecino.rojo;
                    vecino.rojo = (*ma)[((i+1)*cantCelReal)+j-1].Estado.rojo + vecino.rojo;
                    vecino.rojo = (*ma)[((i+1)*cantCelReal)+j].Estado.rojo + vecino.rojo;
                    vecino.rojo = (*ma)[((i+1)*cantCelReal)+j+1].Estado.rojo + vecino.rojo;

                    vecino.rojo = (*ma)[((i+2)*cantCelReal)+j].Estado.rojo + vecino.rojo;
                    vecino.rojo = (*ma)[((i-2)*cantCelReal)+j].Estado.rojo + vecino.rojo;

                    if(j == 1){
                        vecino.rojo = (*ma)[(i*cantCelReal)+j+2].Estado.rojo + vecino.rojo;
                    }
                    else {
                        vecino.rojo = (*ma)[(i*cantCelReal)+j-2].Estado.rojo + vecino.rojo;
                    }
                    cantVecin = 11;
                }//segundo caso izquierda y derecha (Bieen como vino de fabrica)
                if(i >= 3 && i<cantCeldas-2 && j>= 3 && j<cantCeldas-2){
                    vecino.rojo = (*ma)[((i-1)*cantCelReal)+j-1].Estado.rojo + vecino.rojo;
                    vecino.rojo = (*ma)[((i-1)*cantCelReal)+j].Estado.rojo + vecino.rojo;
                    vecino.rojo = (*ma)[((i-1)*cantCelReal)+j+1].Estado.rojo + vecino.rojo;
                    vecino.rojo = (*ma)[(i*cantCelReal)+j-1].Estado.rojo + vecino.rojo;
                    vecino.rojo = (*ma)[(i*cantCelReal)+j+1].Estado.rojo + vecino.rojo;
                    vecino.rojo = (*ma)[((i+1)*cantCelReal)+j-1].Estado.rojo + vecino.rojo;
                    vecino.rojo = (*ma)[((i+1)*cantCelReal)+j].Estado.rojo + vecino.rojo;
                    vecino.rojo = (*ma)[((i+1)*cantCelReal)+j+1].Estado.rojo + vecino.rojo;

                    vecino.rojo = (*ma)[((i+2)*cantCelReal)+j].Estado.rojo + vecino.rojo;
                    vecino.rojo = (*ma)[((i-2)*cantCelReal)+j].Estado.rojo + vecino.rojo;

                    vecino.rojo = (*ma)[(i*cantCelReal)+j+2].Estado.rojo + vecino.rojo;
                    vecino.rojo = (*ma)[(i*cantCelReal)+j-2].Estado.rojo + vecino.rojo;

                    cantVecin = 12;
                }

        //---------------------------------------------------------------------------
                vecEnfermo = vecino.rojo / cantVecin; // saca un porcentaje de todos los enfermos
                (*ma2)[(i*cantCelReal)+j].Estado.verde = 1;
                if((*ma)[(i*cantCelReal)+j].Herida == 1 && t != 0){ //  t!= 0 por si en la primera iteración ya tenia un aherida para no curarla porque no paso 1 se(*ma)na
                    // supongo de que ya paso 1 semana de que tuvo la herida asi que ya se sanó
                    (*ma2)[(i*cantCelReal)+j].Herida = 0;
                }
                if(aniosVida((*ma)[(i*cantCelReal)+j].Edad) <= 3 ){//arol joven

                    suceptibilidad = 0.35; //por ser joven
                    if((*ma)[(i*cantCelReal)+j].Herida == 1){
                        suceptibilidad = suceptibilidad + 0.15;
                    }

                    //actualizaciones de estados
                    pContagio = ((vecEnfermo + suceptibilidad)*0.60) + 0.07;
                    ran = rand() % 101;// random de 0 a 100
                    if (ran <= 23 && ran != 0){ // es decir que que tiene una herida
                        (*ma2)[(i*cantCelReal)+j].Herida = 1;
                    }
                    ran = rand() % 101;
                    if (ran <= pContagio){
                        (*ma2)[(i*cantCelReal)+j].Estado.verde = 0;
                        (*ma)[(i*cantCelReal)+j].Estado.verde = 0;

                        (*ma2)[(i*cantCelReal)+j].Estado.naranja = 1;
                        (*ma2)[(i*cantCelReal)+j].Enfermo ++;// aumento 1 porque va a pasar una semana en el proximo clock
                    }
                }
                if(aniosVida((*ma)[(i*cantCelReal)+j].Edad) <= 35 && aniosVida((*ma)[(i*cantCelReal)+j].Edad) > 3){

                    suceptibilidad = 0.17; //por ser adulto chiquito

                    if((*ma)[(i*cantCelReal)+j].Herida == 1){
                        suceptibilidad = suceptibilidad + 0.15;
                    }

                    //actualizaciones de estados
                    pContagio = ((vecEnfermo + suceptibilidad)*0.60) + 0.07;

                    ran = rand() % 101;// random de 0 a 100
                    if (ran <= 8 && ran != 0 ){ // es decir que que tiene una herida
                        (*ma2)[(i*cantCelReal)+j].Herida = 1;
                    }
                    ran = rand() % 101;
                    if (ran <= pContagio){
                        (*ma2)[(i*cantCelReal)+j].Estado.verde = 0;
                        (*ma)[(i*cantCelReal)+j].Estado.verde = 0;                        

                        (*ma2)[(i*cantCelReal)+j].Estado.naranja = 1;
                       

                        (*ma2)[(i*cantCelReal)+j].Enfermo ++;// aumento 1 porque va a pasar una semana en el proximo clock
                   }

                }
                if(aniosVida((*ma)[(i*cantCelReal)+j].Edad) > 35){

                    suceptibilidad = 0.63; //por ser adulto viejo

                    if((*ma)[(i*cantCelReal)+j].Herida == 1){
                        suceptibilidad = suceptibilidad + 0.15;
                    }

                    //actualizaciones de estados
                    pContagio = ((vecEnfermo + suceptibilidad)*0.60) + 0.07;

                    ran = rand() % 101;// random de 0 a 100
                    if (ran <= 37 && ran != 0 ){ // es decir que que tiene una herida
                        (*ma2)[(i*cantCelReal)+j].Herida = 1;
                    }
                    ran = rand() % 101;
                    if (ran <= pContagio){
                        (*ma2)[(i*cantCelReal)+j].Estado.verde = 0;
                        (*ma)[(i*cantCelReal)+j].Estado.verde = 0;                        

                        (*ma2)[(i*cantCelReal)+j].Estado.naranja = 1;               
    
                        (*ma2)[(i*cantCelReal)+j].Enfermo ++;// aumento 1 porque va a pasar una semana en el proximo clock
                    }
                }
                
            }//si esta sano
    //--------------------------------------------------------------------------------------------------------------
            if((*ma)[(i*cantCelReal)+j].Estado.naranja == 1){ //infectado con esporas
                (*ma2)[(i*cantCelReal)+j].Estado.naranja = 1;
                if((*ma)[(i*cantCelReal)+j].Enfermo < 3){
                    (*ma2)[(i*cantCelReal)+j].Enfermo ++;// aumento 1
               }
               else {//de infectado con esporas a enfermo con síntomas
                   (*ma2)[(i*cantCelReal)+j].Estado.naranja = 0;
                   (*ma)[(i*cantCelReal)+j].Estado.naranja = 0;

                   (*ma2)[(i*cantCelReal)+j].Estado.rojo = 1;
                   (*ma2)[(i*cantCelReal)+j].Enfermo ++;// aumento 1
               }
            }
            if((*ma)[(i*cantCelReal)+j].Estado.rojo == 1){//enfermo con síntomas
                (*ma2)[(i*cantCelReal)+j].Estado.rojo = 1;
                ran = rand() % 101;// random de 0 a 100
                if(ran <= 85){// 85% de que esto ocurra paso de enfermo con síntomas a enfermo con tratamiento
                    (*ma2)[(i*cantCelReal)+j].Estado.rojo = 0;
                    (*ma)[(i*cantCelReal)+j].Estado.rojo = 0;

                    (*ma2)[(i*cantCelReal)+j].Estado.azul = 1;
                }
                (*ma2)[(i*cantCelReal)+j].Enfermo ++;// aumento 1
            }
            if((*ma)[(i*cantCelReal)+j].Estado.azul == 1){//enfermo con tratamiento antifúngico
                (*ma2)[(i*cantCelReal)+j].Estado.azul = 1;
                ran = rand() % 101;// random de 0 a 100
                if(aniosVida((*ma)[(i*cantCelReal)+j].Edad)<= 3){ //arbol joven
                    if(ran <= 3){//si tiene resistencia al hongo

                        if((*ma2)[(i*cantCelReal)+j].Enfermo <= 8){
                            (*ma2)[(i*cantCelReal)+j].Estado.azul = 0;
                            (*ma)[(i*cantCelReal)+j].Estado.azul = 0;

                            (*ma2)[(i*cantCelReal)+j].Estado.verde = 1;//pasa a sano con la misma edad
                            (*ma2)[(i*cantCelReal)+j].Enfermo = 0;//0 porque ya no está más enfermo en el proximo clock
                        }
                        else{//se poda el arbol porque ya pasaron 8 o más semanas del contagio
                            (*ma2)[(i*cantCelReal)+j].Estado.azul = 0;
                            (*ma)[(i*cantCelReal)+j].Estado.azul = 0;                

                            (*ma2)[(i*cantCelReal)+j].Estado.blanco = 1;//podado
                            (*ma2)[(i*cantCelReal)+j].Enfermo = 0; //0 porque ya no esta más enfermo en el proximo clock
                            (*ma2)[(i*cantCelReal)+j].Podado = 1; //al proximo clock va a ser 1 semana de ser podado
                        }
                    }
                    else{//en esta semana no tiene resistencia
                        (*ma2)[(i*cantCelReal)+j].Enfermo ++; //pasa el tiempo nomas
                    }
                }
                if(aniosVida((*ma)[(i*cantCelReal)+j].Edad) <= 35 && aniosVida((*ma)[(i*cantCelReal)+j].Edad) > 3){//adulto
                    if(ran <= 15){//si tiene resistencia al hongo
                        if((*ma2)[(i*cantCelReal)+j].Enfermo <= 8){
                            (*ma2)[(i*cantCelReal)+j].Estado.azul = 0;
                            (*ma)[(i*cantCelReal)+j].Estado.azul = 0;

                            (*ma2)[(i*cantCelReal)+j].Estado.verde = 1;//pasa a sano con la misma edad
                            (*ma2)[(i*cantCelReal)+j].Enfermo = 0;//0 porque ya no está más enfermo en el proximo clock
                        }
                        else{//se poda el arbol porque ya pasaron 8 o más semanas del contagio
                            (*ma2)[(i*cantCelReal)+j].Estado.azul = 0;
                            (*ma)[(i*cantCelReal)+j].Estado.azul = 0;

                            (*ma2)[(i*cantCelReal)+j].Estado.blanco = 1;//podado
                            (*ma2)[(i*cantCelReal)+j].Enfermo = 0; //0 porque ya no esta más enfermo en el proximo clock
                            (*ma2)[(i*cantCelReal)+j].Podado = 1; //al proximo clock va a ser 1 semana de ser podado
                        }
                    }
                    else{//En esta semana no tiene resistencia
                        (*ma2)[(i*cantCelReal)+j].Enfermo ++; //pasa el tiempo nomas
                    }

                }
                if(aniosVida((*ma)[(i*cantCelReal)+j].Edad) > 35){//viejo
                    if(ran <= 53){//si tiene resistencia al hongo
                        if((*ma2)[(i*cantCelReal)+j].Enfermo <= 8){
                            (*ma2)[(i*cantCelReal)+j].Estado.azul = 0;
                            (*ma)[(i*cantCelReal)+j].Estado.azul = 0;

                            (*ma2)[(i*cantCelReal)+j].Estado.verde = 1;//pasa a sano con la misma edad
                            (*ma2)[(i*cantCelReal)+j].Enfermo = 0;//0 porque ya no está más enfermo en el proximo clock
                        }
                        else {// se reemplaza el arbol por uno nuevo
                            (*ma2)[(i*cantCelReal)+j].Estado.azul = 0;
                            (*ma)[(i*cantCelReal)+j].Estado.azul = 0;

                            (*ma2)[(i*cantCelReal)+j].Estado.verde = 1;
                            (*ma2)[(i*cantCelReal)+j].Edad = 48;// 1 año de edad
                            (*ma2)[(i*cantCelReal)+j].Enfermo = 0;//0 porque ya no está más enfermo en el proximo clock
                        }
                    }
                    else{//En esta semana no tiene resistencia
                        (*ma2)[(i*cantCelReal)+j].Enfermo ++; //pasa el tiempo nomas
                    }
                }
            }
            if((*ma)[(i*cantCelReal)+j].Estado.blanco == 1){// arbol podado
                (*ma2)[(i*cantCelReal)+j].Estado.blanco = 1;
                if((*ma2)[(i*cantCelReal)+j].Podado == 12){
                    //de podado a sano
                    (*ma2)[(i*cantCelReal)+j].Estado.blanco = 0;
                    (*ma)[(i*cantCelReal)+j].Estado.blanco = 0;

                    (*ma2)[(i*cantCelReal)+j].Estado.verde = 1;
                    (*ma2)[(i*cantCelReal)+j].Podado = 0;//para reiniciar la variable nomas
                }
                else{
                    (*ma2)[(i*cantCelReal)+j].Podado ++;
                }
            }
        }//for j
    }//for i
    

    //se actualizo ma2 y ahora solo tengo que cambiar el ṕuntero
    
    aux = (*ma);
    (*ma) = (*ma2);
    (*ma2) = aux;
    
    

}

void asignoNada (Celda *algo, int cant){
    for(int i = 0; i < cant; i ++){
        algo[i].Estado.blanco = 0; 
        algo[i].Estado.azul = 0;
        algo[i].Estado.rojo = 0;
        algo[i].Estado.naranja = 0;
        algo[i].Estado.verde = 0;
        algo[i].Edad = 0;
        algo[i].Herida = 0;
        algo[i].Enfermo = 0;
        algo[i].Podado = 0;
    }
}
main(int argc, char **argv){

    int node,cantNodos;
    

    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &node);
    MPI_Comm_size(MPI_COMM_WORLD, &cantNodos);//en ejecución en cluster serán 20 nodos acá 10 en pruebas en mi pc
    MPI_Status status;
    

    //variables a usar 
    int cantCelReal = 800;
    int fil = cantCelReal, col = cantCelReal, caso,t=0;
    int cantEnvio = (col*fil)/cantNodos;
    
    double inicio, fin;
    

    Celda *matris, *matris2, *recibo, *rec2, *super1, *super2, *infe1, *infe2; //[cantCeldas][cantCeldas];
    
/*
    matris = (Celda *) malloc (sizeof(Celda)*fil*col);//declaro arreglo de 1 dimensión 
    
    matris2 = (Celda *) malloc (sizeof(Celda)*fil*col);

    recibo = (Celda *) malloc (sizeof(Celda)*cantEnvio*col);//probando

    rec2 = (Celda *) malloc (sizeof(Celda)*cantEnvio*col);

    //auxiliares 
    super1 = (Celda *) malloc (sizeof(Celda)*cantCelReal);
    super2 = (Celda *) malloc (sizeof(Celda)*cantCelReal);
      
    infe1 = (Celda *) malloc (sizeof(Celda)*cantCelReal);
    infe1 = (Celda *) malloc (sizeof(Celda)*cantCelReal);*/
    //inicio tiempo
    inicio = MPI_Wtime();
    //MPI_Barrier(MPI_COMM_WORLD);
    for(int j =0; j<5; j++){
        if(node == 0){ 
            matris = (Celda *) malloc (sizeof(Celda)*fil*col);//declaro arreglo de 1 dimensión 
    
            matris2 = (Celda *) malloc (sizeof(Celda)*fil*col);
            inicializar(matris, fil, col); //cambiar logica

            recibo = (Celda *) malloc (sizeof(Celda)*cantEnvio*col);//probando

            rec2 = (Celda *) malloc (sizeof(Celda)*cantEnvio*col);

            //auxiliares 
            super1 = (Celda *) malloc (sizeof(Celda)*cantCelReal);
            super2 = (Celda *) malloc (sizeof(Celda)*cantCelReal);

            infe1 = (Celda *) malloc (sizeof(Celda)*cantCelReal);
            infe1 = (Celda *) malloc (sizeof(Celda)*cantCelReal);
            //inicio tiempo
        }
        else {
                matris = (Celda *) malloc (sizeof(Celda)*fil*col);//declaro arreglo de 1 dimensión 
    
                    matris2 = (Celda *) malloc (sizeof(Celda)*fil*col);

                    recibo = (Celda *) malloc (sizeof(Celda)*cantEnvio*col);//probando

                    rec2 = (Celda *) malloc (sizeof(Celda)*cantEnvio*col);

                    //auxiliares 
                    super1 = (Celda *) malloc (sizeof(Celda)*cantCelReal);
                    super2 = (Celda *) malloc (sizeof(Celda)*cantCelReal);

                    infe1 = (Celda *) malloc (sizeof(Celda)*cantCelReal);
                    infe1 = (Celda *) malloc (sizeof(Celda)*cantCelReal);
                    //inicio tiempo
        }
        
        if(node == 0 ){
            caso = 1;
        }
        if(node > 0 && node <cantNodos-1){
            caso = 2;
        }
        if(node == cantNodos-1){
            caso = 3;
        }
        for(int i = 0; i< 1200; i++){ 
            MPI_Scatter(matris, cantEnvio*sizeof(Celda), MPI_BYTE, recibo, fil*cantEnvio*sizeof(Celda), MPI_BYTE, 0, MPI_COMM_WORLD);
            //MPI_Bcast(&matris, fil*col*sizeof(Celda), MPI_BYTE, 0, MPI_COMM_WORLD);
            asignoNada(super1, cantCelReal);
            asignoNada(super2, cantCelReal);
            asignoNada(infe1, cantCelReal);     
            asignoNada(infe1, cantCelReal);
            proxEstado (&recibo, &rec2, &super1, &super2, &infe1, &infe2 , cantCelReal, t, caso, cantEnvio);
            MPI_Gather(recibo, cantEnvio*sizeof(Celda), MPI_BYTE, matris, cantEnvio*sizeof(Celda), MPI_BYTE, 0, MPI_COMM_WORLD);//matris2 era antes
            
        }//for de las cantidad de semanas 
        
        
        MPI_Barrier(MPI_COMM_WORLD);
        fin = MPI_Wtime();
        if(node==0){
        	printf("Tiempo: %.3f segundos.\n",fin-inicio);
        }
    }//for de ciclos 
    free(recibo);
    free(matris);
    free(matris2);
    

    MPI_Finalize();



}
