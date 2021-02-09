#include<iostream>
#include<windows.h>
#include<stdlib.h>
#include <cstring>
#include<fstream>
#include<math.h>
#include<time.h>

#define MAX 10000

using namespace std;

float matrix[100][100], act[101],sig[101];

void aleatoreo(float a[]){
	int i, j,r,band;
	for(i=0;i<100;i++){
		do{
			band=0;
			r=rand()%(100+1);
			for(j=0;j<100;j++){
				if(act[j]==r){
					band++;	
				}
			}	
		}while(band>0);
		act[i]=r;	
	}
	act[100]=act[0];
}
void vecino(float vect[]) {
    int inicio, fin;
    inicio = 1+rand()%(100-2);
    fin = 1+rand()%(100-2);
    swap(vect[inicio],vect[fin]);
}
 
int costo(float a[]){
	float sum;
	sum=0;
	int fi, se, i;
	for(i=0;i<99;i++){
		fi=a[i];
		se=a[i+1];
		sum=sum+matrix[fi][se];
	}
	return sum;
}

void imprime_vect(float a[], int n){
	int i;
	for(i=0;i<n;i++){
		cout<<a[i]<<" ->> ";
	}
}

void imprime_mat(float n){
	int i,j;
	for(i=0;i<100;i++){
		for(j=0;j<100;j++){
			cout<<matrix[i][j]<<"   ";
		}
		cout<<endl;
	}
}

bool proba(float c1, float c2, float temp){
	double boltzman;
    boltzman = exp((c1-c2)/temp);
    if(boltzman > rand()/RAND_MAX)
        return false;
    else 
		return true;
}

int main(){
	srand(time(NULL));
	
	FILE *flujo;
	char linea[MAX]; 
	char contenido[]="NODE_COORD_SECTION\n";
	int i, j, mat[100][3], dx, dy;
	
	float Ti, Tf, c, n, bolt;//Temperatura inicial, temperatura final, constante de enfriamiento, iteraciones, cte boltzmann
	float t;
	float cost, cost1;
////////////////////////////////////////////////////Pide Datos///////////////////////////////////////////////////////////////////////////////
	do{
		system("cls");
		cout<<"Dame la temperatura inicial(mayor a 0): ";
		cin>>Ti;	
	}while(Ti<=0);
	do{
		system("cls");
		cout<<"Dame la temperatura final(menor que la inicial): ";
		cin>>Tf;	
	}while(Tf>=Ti);
	do{
		system("cls");
		cout<<"Dame la constante de enfriamiento(0-1): ";
		cin>>c;	
	}while(c<=0 || c>=1);
	
	do{
		system("cls");
		cout<<"Dame el numero de iteraciones: ";
		cin>>n;	
	}while(n<=0);
	
	//////////////////////////////////////////// Lee archivo/////////////////////////////////////////////////////////////////////////
	for(i=0; i<100; i++){
		for(j=0; j<3; j++){
			mat[i][j]=0;
		}
	}
	for(i=0; i<100; i++){
		for(j=0; j<100; j++){
			matrix[i][j]=0;
		}
		act[i]=0;
		sig[i]=0;
	}
	flujo=fopen("kroD100.txt", "r+");
	if(flujo==NULL){
		cout<<"E R R O R\n\n";
	}	
	
	while(!feof(flujo)){
			
		fgets(linea,MAX, flujo);
		if(strcmp(linea, contenido)==0){
			for(i=0; i<100; i++){
				for(j=0; j<3; j++){
					fscanf(flujo, "%d", &mat[i][j]);
				}
			}
		}		
	}
	fclose(flujo);
	//////////////////////////////////////////////////////////////matriz de adyacencia////////////////////////////////////////////////////////
	for(i=0;i<99;i++){ // sacamos las distancias entre ciudades
		for(j=i+1;j<100;j++){
			dx=mat[i][1]-mat[j][1];
			dy=mat[i][2]-mat[j][2];
			matrix[i][j]=sqrt((dx*dx)+(dy*dy));//formula de pitagoras
			matrix[j][i]=matrix[i][j];
		}
	}
////////////////////////////////////////////////////// Recocido simulado /////////////////////////////////////////////////////////////////
	aleatoreo(act);//generamos solucion aleatorea
	cost=costo(act);//calculamos el costo de la solucion aleatorea

	t=Ti;
	bolt=1;
	
	while(t>=Tf){
		for(i=0;i<n;i++){
			memcpy(sig,act,sizeof(act));//hace una copia de la solucion actualm en siguiente
			vecino(sig);//modifica a siguiente para generar una solucion vecina 
			cost1=costo(sig);//calcula costo de solucion vecina 
			if(cost1<cost){
				memcpy(act,sig,sizeof(sig));
				cost=cost1;
			}else if(proba(cost,cost1,t)){
				memcpy(act,sig,sizeof(sig));
				cost=cost1;
			}
		}
		t=(c*t);
	}
	
	
	
		
	imprime_vect(act,101);
	cout<<"\ncon costo: "<<cost;
	cout<<endl<<endl;
	system("pause");
	return 0;	
}
