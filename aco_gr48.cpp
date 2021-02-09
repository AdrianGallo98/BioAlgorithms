#include<iostream>
#include<windows.h>
#include<stdlib.h>
#include <cstring> 
#include<fstream>
#include<math.h>
#include<time.h>

#define MAX 10000
#define CIU 48
#define NH CIU/2

using namespace std;

	float matrix[CIU][CIU];
	int tabu[NH][CIU+2], bestsol[CIU+2], bestsola[CIU+2];
	float visi[CIU][CIU],fero[CIU][CIU];
	int permitidos[CIU];
	double proba[CIU];
	float alpha,beta, rho,Q;
	double sum;
//-------------------------------------------------------------------------------------------------------------------------------------
initmatrix(){ //inicializa la matriz de adyacencias en 0's
	int i, j;
	for(i=0; i<CIU; i++){
		for(j=0; j<CIU; j++){
			matrix[i][j]=0;
		}
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void initfero(float tao){//inicializa la matriz de feromonas en tao
	int i, j;
	for(i=0; i<CIU; i++){
		for(j=0; j<CIU; j++){
			if(i==j)
				fero[i][j]=0;
			else
				fero[i][j]=tao;
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------
void inittabu(){//inicializa la matriz del recorrido de cada hormiga en 0
	int i, j;
	for(i=0; i<NH; i++){
		for(j=0; j<CIU+2; j++){
			tabu[i][j]=-1;
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------
initvisi(){//inicializa la matriz de visivilidad con 1/matrix
	int i, j;
	for(i=0; i<CIU; i++){
		for(j=0; j<CIU; j++){
			if(matrix[i][j]!=0)
				visi[i][j]=1.0/matrix[i][j];
			else 
				visi[i][j]=0;
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------
void imprimemat(float a[][CIU]){///imprime la matriz que se requiera 
	int i, j;
	for(i=0; i<CIU; i++){
		for(j=0; j<CIU; j++){	
			cout<<a[i][j]<<" ";		
		}
		cout<<endl;
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------
void initpermitidos(int fil){//obtiene los nodos siguientes permitidos
	int i, j, iter;
	int band;
	iter=0;
	
	for(i=0; i<CIU; i++){
			permitidos[i]=-1;
			proba[i]=-1;
	}
	
	for(i=0;i<CIU;i++){
		band=0;
		for(j=0;j<CIU;j++){
			if(tabu[fil][j]==i){
				band=1;
				
				//cout<<i<<" ";
			}
		}
		if(band==0){
			permitidos[iter]=i;
			iter++;
		}
	}
	/*cout<<"\n\n";
	for(i=0;i<100;i++){
		cout<<permitidos[i]<<" ";
	}*/
}

//-------------------------------------------------------------------------------------------------------------------------------------
int nextNode(int fil, int prevNode){//con las probabilidades dara el siguente nodo que visitara la hormiga
	int next,i, j;
	double r, suma;
	initpermitidos(fil);
	suma=0;
	
	r=((double) rand() / (RAND_MAX));
	
	for(i=0;i<CIU;i++){
		if(permitidos[i]!=(-1)){//la matriz de permitidos esta inicializada en -1
			suma+=(  (pow(fero[prevNode][permitidos[i]],alpha))*(pow(visi[prevNode][permitidos[i]],beta))  );	
		}
	}
	//cout<<"\nsuma: "<<suma;
	proba[0]=( (pow(fero[prevNode][permitidos[0]],alpha)) * (pow(visi[prevNode][permitidos[0]],beta)) )/suma;
	for(i=0;i<CIU-1;i++){
		proba[i+1]= proba[i]+( (pow(fero[prevNode][permitidos[i]],alpha)) * (pow(visi[prevNode][permitidos[i]],beta)) )/suma;
		//cout<<"\n"<<proba[i];
	}
	if(r<proba[0]){
		next=permitidos[0];
		return next;
	}
	for(i=0;i<CIU;i++){
		//if(proba[i+1]!=-1){
			if(r>=proba[i] && r<proba[i+1] && proba[i+1]!=-1){
				next=permitidos[i+1];
				return next;
			}	
	//	}else if(r<proba)
		
	}
	//next=11;
	
}
//---------------------------------------------------------------------------------------------------------------------------------------
void refreshfero(){//actualiza la feromona 
	int i, j;
	int fi, se;
	float taoact;
	for(i=0;i<NH;i++){
		for(j=0;j<CIU;j++){
			fi=tabu[i][j];//el primer nodo de la arista por actualizar 
			se=tabu[i][j+1];///segundo nodo de la arista por acrualizar 
			taoact=fero[i][j]+(Q/tabu[i][CIU+1]);//funcion que actualiza la feromona
			fero[i][j]=taoact;
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------
void checkCost(){
	int  i, j;
	int fi, se;
	for(i=0;i<NH;i++){
		sum=0;
		for(j=0;j<CIU;j++){
			fi=tabu[i][j];
			se=tabu[i][j+1];
			sum=sum+matrix[fi][se];	
		}
		tabu[i][49]=sum;
	}
}
//-----------------------------------------------------------------------------------------------------------------------------
void evaporafero(){
	int i,j;
	float aux;
	for(i=0;i<CIU;i++){
		for(j=0;j<CIU;j++){
			aux=(fero[i][j])*(1-rho);
			fero[i][j]=aux;
		}
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------
int main(){
	srand(time(NULL));
	
	FILE *flujo;
	char linea[MAX]; 
	char contenido[]="EDGE_WEIGHT_SECTION\n";
	int i, j, mat[1176], dx, dy;
	float tao, ro, Q, alfa, beta;
	int band;
	
	initmatrix();
		
	flujo=fopen("gr48.txt", "r+");
	if(flujo==NULL){
		cout<<"E R R O R\n\n";
		system("pause");
		exit(-1);
	}	
////////////////////////////////////////////////////////Asigna los valores de los parametros///////////////////////////////////////////////
	int iter;
	cout<<"Dame el valor de Tao(feromona incicial): ";
	cin>>tao;
	cout<<"\nDame el valor de Q(constante para la actualizacion de feromonas): ";
	cin>>Q;
	cout<<"\n\nDame el valor de Alfa(improtancia de feromona): ";
	cin>>alpha;
	cout<<"\n\nDame el valor de Beta(improtancia de visibilidad): ";
	cin>>beta;
	cout<<"\n\nDame el valor de Rho(factor de evaporacion de feromonas): ";
	cin>>rho;
	cout<<"\n\nDame el numero de iteraciones: ";
	cin>>iter;
	
//////////////////////////////////////////////////Lee el archivo///////////////////////////////////////////////////////////////////////////	
	while(!feof(flujo)){
		fgets(linea,MAX, flujo);
		if(strcmp(linea, contenido)==0){
			for(i=0; i<1176; i++){
				//for(j=0; j<10; j++){
					fscanf(flujo, "%d", &mat[i]);
				//}
			}
		}		
	}
	fclose(flujo);

//////////////////////////////////////////////////////////////matriz de adyacencia////////////////////////////////////////////////////////
	int p=0;
	for(i=0;i<CIU;i++){
		for(j=0;j<CIU;j++){
			if(i>=j){
				matrix[i][j]=mat[p];
				matrix[j][i]=matrix[i][j];
				p++;
			}
		}
	}
	
	initvisi();
	/*cout<<"Visibilidad \n\n";
	imprimemat(visi);*/
	initfero(tao);
//////////////////////////////////////////////////////////////////ACO//////////////////////////////////////////////////////////////////////
	/*alpha=1;
	beta=5;
	Q=2;
	rho=.6;*/
	int n=0;
	do{
		inittabu();
	
		for(i=0;i<NH;i++){//Ponemos las hormigas en un nodo de origen aleatoreo(pueden ser diferentes)
			tabu[i][0]=rand()%(CIU);//origen aleatoreo 
			tabu[i][CIU]=tabu[i][0];//fin igual al inicio
		}
		for(i=0;i<NH;i++){//Hasta llenar tabu, el numero de hormigas
			for(j=1;j<CIU;j++){//Da la sigueiente ciudad por recorrer
				tabu[i][j]=nextNode(i,tabu[i][j-1]);//se manda el numero de hormiga y el nodo actual, nextnode da el siguiente nodo
			}
		}
		checkCost();
		refreshfero();
		evaporafero();
		n++;
	}while(n<iter);//(n>1000);//Hasta el numero de iteraciones 
	
	cout<<"\n\n\n";
	for(i=0; i<NH; i++){
		cout<<i+1<<" -> ";
		for(j=0; j<CIU+2; j++){	
			cout<<tabu[i][j]<<" ";		
		}
		cout<<endl;
	}

	float max;
	
	max=tabu[0][49];
	
	for(i=0;i<NH;i++){
		if(tabu[i][49]<max){
			max=tabu[i][49];
		}
	}
	
	cout<<"\n\n\nla distancia minima es: "<<max; 

	cout<<endl<<endl;
	system("pause");
	return 0;	
}
