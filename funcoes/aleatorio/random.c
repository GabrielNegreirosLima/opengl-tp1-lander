
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include "random.h"

void inicRandom(){
    long ultime;
    time(&ultime);
    srand((unsigned) ultime);
}


int pegaIntRandomico(int limite){


    int indice;

    inicRandom();
    indice = 10 + rand()%limite;
    return indice;


}
