
#include"vetor.h"
#include <math.h>//incluo -lm na hora de compilar, o arquivo.c do cabeçario

VETOR somaVetores(VETOR vetorA, VETOR vetorB);
float anguloEntreDoisVetores(VETOR vetorA, VETOR vetorB);
void inserirCoordenadasVetor(VETOR *vetor, float valorX, float valorY);
void inserirCoordenadasVetor(VETOR *vetor, float valorX, float valorY);
float normaDoVetor(VETOR vetor);
void normalizarVetor(VETOR *vetor);
void vetorMultiplicaEscalar(VETOR *vetor,float escalar);

/*-------------------------------------------------------------------------------------*/
VETOR somaVetores(VETOR vetorA, VETOR vetorB){
	
  VETOR VetorResultante;
  VetorResultante.x = vetorA.x + vetorB.x;
  VetorResultante.y = vetorA.y + vetorB.y;  
  return VetorResultante;
}
/*--------------------------------------------------------------------------------------*/
float anguloEntreDoisVetores(VETOR vetorA, VETOR vetorB){
	
 	float angulo, cosenoAngulo, normaVetorA, normaVetorB, produtoEscalarVetores;
    float potencia1, potencia2 , raizQuadrada;
  
      
	produtoEscalarVetores =  (vetorA.x * vetorB.x)+(vetorA.y * vetorB.y); //numerador

  
	normaVetorA = sqrt((pow((vetorA.x),2)) + (pow((vetorA.y),2)));
	normaVetorB = sqrt(pow(vetorB.x,2)+pow(vetorB.y,2));
	cosenoAngulo = produtoEscalarVetores / (normaVetorA * normaVetorB);
	angulo = acos(cosenoAngulo);
	return angulo;

}
/*--------------------------------------------------------------------------------------*/
 
 void inserirCoordenadasVetor(VETOR *vetor, float valorX, float valorY){
	
  (*vetor).x = valorX;
  (*vetor).y = valorY;
  
}
/*--------------------------------------------------------------------------------------*/
 
 float normaDoVetor(VETOR vetor){

	float normaVetor= sqrt((pow((vetor.x),2)) + (pow((vetor.y),2)));
    return normaVetor;
  
}
/*--------------------------------------------------------------------------------------*/
 
void normalizarVetor(VETOR *vetor){

    VETOR vetorAux;
	float x,y;
    float normaVetor = normaDoVetor(vetorAux);

    inserirCoordenadasVetor(&vetorAux, vetor->x,vetor->y);//insere coordenadads de vetor


    x =  (vetor->x)/normaVetor;    
    y = (vetor->y)/normaVetor;

    inserirCoordenadasVetor(vetor,x,y);//altera o local da memoria do vetor no programa main.*/
  
}
/*--------------------------------------------------------------------------------------*/
 
void vetorMultiplicaEscalar(VETOR *vetor,float escalar){

    VETOR vetorAux;
	float x,y;
  
    x = (vetor->x)*escalar;    
    y = (vetor->y)*escalar;

    inserirCoordenadasVetor(vetor,x,y);//altera o local da memoria do vetor no programa main.
    //nao precisa colocar & pois já contem o endereco da memoria que quero modificar
  
}

