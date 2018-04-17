#include "vetor.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
//funcao para testar os arquivos. vetor.h e vetor.c

int main(int argc, char **argv){
  VETOR vetorA, vetorB;
  VETOR vetorSoma;
  float angulo;


	inserirCoordenadasVetor(&vetorA, 3,-4);//Devo passar o endereço para que seja modficado pelo ponteiro
	inserirCoordenadasVetor(&vetorB, -1,2);
	printf("VetorA(%.1f, %.1f)\n",vetorA.x, vetorA.y);
	printf("VetorB(%.1f, %.1f)\n",vetorB.x, vetorB.y);

	vetorSoma = somaVetores(vetorA, vetorB);
	angulo = anguloEntreDoisVetores(vetorA, vetorB);

    printf("Norma Vetor =  %.5f \n", normaDoVetor(vetorA));
	printf("VetorSoma (%.1f,%.1f)\n",vetorSoma.x, vetorSoma.y);
	printf("angulo entre VetorerAB =  %.5f \n", angulo);
    

     normalizarVetor(&vetorA);
    vetorMultiplicaEscalar(&vetorA,2);
    printf("VetorA Multiplicado Por Escalar x%f, %f  \n", vetorA.x,vetorA.y);
    vetorA = vetorB;
    printf("VetorA(%.1f, %.1f)\n",vetorA.x, vetorA.y);




  
}
