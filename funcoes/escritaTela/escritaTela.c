#include <GL/glew.h>
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <locale.h>
#include <string.h>
#include "escritaTela.h"

void escreveTexto(void * font, char *s, float x, float y, float z);
char *conversaoIntToChar(float num,char *string);
void escreveNaCenaNumero(float s,int x, int y,int z);
void escreveNaCenaString(char *s,int x, int y,int z);


void escreveTexto(void * font, char *s, float x, float y, float z){

    int i;
    glRasterPos3f(x, y, z);

    for (i=0; i < strlen(s); i++)
       glutBitmapCharacter(font, s[i]);
}
//..................................................................................................
 char *conversaoIntToChar(float num,char *string){//num alterada para float


  char texto[10];
  sprintf(texto,"%f",num);
  strcpy(string,texto); //destino, origem
  
}
//....................................................................................................
void escreveNaCenaNumero(float s,int x, int y,int z){//num alterada para float

   char texto[10];
   conversaoIntToChar(s,texto);//conversao de inteiro para string
   escreveTexto(GLUT_BITMAP_HELVETICA_12, texto, x, y, z);
   //escreveTexto(GLUT_BITMAP_TIMES_ROMAN_10, "fox jumps", 20, 20, 0);

}

//..................................................................................................
void escreveNaCenaString(char *s,int x, int y,int z)
{
   
    escreveTexto(GLUT_BITMAP_HELVETICA_12, s, x, y, z);
   //escreveTexto(GLUT_BITMAP_TIMES_ROMAN_10, "fox jumps", 20, 20, 0);

}
//....................................................................................................



