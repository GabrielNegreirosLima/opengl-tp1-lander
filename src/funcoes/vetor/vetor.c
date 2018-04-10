
#include"vetor.h"
#include <math.h>//incluo -lm na hora de compilar, o arquivo.c do cabeçario



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

