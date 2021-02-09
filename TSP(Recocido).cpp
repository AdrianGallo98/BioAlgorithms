#include <cmath>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
 
#define tam 5
 
using namespace std;
int ciudades[tam][tam] = {
    0,30,20,0,50,
    30,0,15,150,100,
    20,15,0,200,0,
    0,150,200,0,50,
    50,100,0,50,0
};
 
void shuffle(int vect[]) {
    int inicio, fin;
    inicio = 1+rand()%(tam-2);
    fin = 1+rand()%(tam-2);
    swap(vect[inicio],vect[fin]);
}
 
 
void initVec(int vect [], int inicio){
    ///Inicializa vector
    int c = 0;
    for(int i = 1; i < tam; i++){
        if(c == inicio)
            c+=1;
        vect[i] = c;
        c+=1;
    }
    vect[0] = vect[tam] = inicio;
}
 
void printVec(int vect[]){
    ///imprime vector
    for(int i = 0; i < tam+1; i++)
        cout<<vect[i]+1;
    cout<<endl;
}
 
bool metropolis(int fx, int fx1, double T){
    /**
    * Calcula el valor de la distribución de boltzman
    * para obtener el resultado del paso de metropolis.
    */
    double boltzman;
    boltzman = exp(-1*(fx1-fx)/T);
    if(boltzman < rand()/RAND_MAX)
        return false;
    return true;
}
 
int calculaValor(int vect[]){
    int suma = 0;
    for(int i = 1; i <= tam; i++){
        if(!ciudades[vect[i-1]][vect[i]])
            suma+=1000;
        else
        suma+=ciudades[vect[i-1]][vect[i]];
    }
    return suma;
}
 
int main(){
    srand(time(NULL));
    //Valor de temperatura inicial
    double T = 3906250;
    //v_cong es el valor de congelacion, que marca
    //el final del algoritmo
    double v_cong = 0.05;
    //actual y probable son las permutaciones que evaluaremos
    //para obtener un resultado óptimo... actual guardará la
    //solucion actual que tomamos como mínimo hasta este punto
    int actual[tam+1],probable[tam+1];
    //v_actual y v_probable es la solucion evaluda
    int v_actual,v_probable;
    //Obtenemos una solución inicial
    initVec(probable,0);
    shuffle(probable);
    memcpy(actual,probable,sizeof(actual));
    v_actual = calculaValor(actual);
    //Iniciamos el ciclo
    do{
        //Obtenemos una nueva solucion aleatoria
        memcpy(probable,actual,sizeof(actual));
        shuffle(probable);
        //Obtenemos el costo de la solucion candidato
        v_probable = calculaValor(probable);
        if(v_actual > v_probable){
            memcpy(actual,probable,sizeof(actual));
            v_actual = v_probable;
            memcpy(probable,actual,sizeof(probable));
        }
        else if(metropolis(v_actual,v_probable,T)){
            memcpy(actual,probable,sizeof(actual));
            v_actual = v_probable;
            memcpy(probable,actual,sizeof(probable));
        }
        //cout<<T<<endl;
        //Disminuimos T
        T=T*0.99;
    }while(T>v_cong);
    cout<<"La solucion optima es: ";
    printVec(actual);
    cout<<"Con el siguiente costo en distancia: "<<v_actual;
    return 0;
}
