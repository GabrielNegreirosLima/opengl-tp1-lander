
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <locale.h>
#include "escritaTela.h"

#define xMundo 100
#define yMundo 100

int nLados=4;

//....................................................................................................
// Inicia algumas variáveis de estado
void inicializa(void)
{ 
    // cor para limpar a tela
    glClearColor(1, 1, 1, 0);      // branco
  

}

//....................................................................................................
void corDaFigura(int cor){

 float cores [10][3]={{0,0,0},{1,0,0},{0,1,0},{0,0,1},{1,1,0},{1,0,1},{0,1,1},{0.6,0.6,0.6},{0.3,0.6,0.2},{1,1,1}};
		 //preto 0,verm 1,verde 2,azul 3,amarel 4,magent 5,ciano 6,cinza 7, branco 8

 glColor3f(cores[cor][0], cores[cor][1],cores[cor][2]);

}

void desenharPoligono(int x, int y, int nLados,int raio, int cor,int preenchimento)
{

  
 float angulo  =  ((360/(nLados))*M_PI)/180;//convertido para radianos
 float anguloInicial =M_PI/2 ;
 if (nLados%2 == 0)
	anguloInicial = M_PI/4;

 if(preenchimento == 0){
	 glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
 }
 else{
 	 glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);	
 }
    
 corDaFigura(cor);

 //printf("%d",w);
 //glPointSize(20);
 glBegin(GL_TRIANGLE_FAN);    
 	for(int i=0; i < (nLados); i++){
		glVertex3f(x+(raio*cos(anguloInicial)), y+(raio*sin(anguloInicial)),0);   
		anguloInicial += angulo;

 }
 glEnd();

 
}

//.....................................................................................................
void desenhaCena(void)//callback de desenho (display)
{
    // Limpa a tela (com a cor definida por glClearColor(r,g,b)) para que
    // possamos desenhar
  
    char texto[10]; 
    int raio = xMundo/(14);//7 divisoes. Metade disso

    int cor = 0;
    int preenchimento = 1;
   
    
    

    inicializa();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//apaga o conteudo da janela

    // Começa a desenhar um polígono com os vértices especificados    
    
   
    escreveNaCenaString("Numero de Lados:",10,95,0);  //(int s,int cor,int x, int y,int z)
    escreveNaCenaNumero(nLados,40,95,0);  //(int s,int cor,int x, int y,int z)
    escreveNaCenaString("Aperte + ou - para alterar a forma dos Objetos",5,5,0);  //(int s,int cor,int x, int y,int z)

    for(int i=3*raio;i<100;i=i+(4*raio)){
	for(int j=3*raio;j<100;j=j+(4*raio)){
       		 desenharPoligono(i,j,nLados,raio,cor,preenchimento); 
                 cor ++;    
	}
	
    }

    // Diz ao OpenGL para colocar o que desenhamos na tela
    glutSwapBuffers();
}

//.....................................................................................................

// Função callback chamada quando o tamanho da janela é alterado 
void redimensiona(GLsizei w, GLsizei h)
{

    
   // Evita a divisao por zero
   if(h == 0) h = 1;
           
   // Especifica as dimensões da Viewport
   glViewport(0, 0, w,h);
  

   // Inicializa o sistema de coordenadas
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   // Estabelece a janela de seleção (left, right, bottom, top)
   if (w <= h) 
           
	glOrtho(0,xMundo, 0, yMundo*h/w,-1,1);
	
//left, right, bottom, top, near, far
	 
   else 

	glOrtho(0, xMundo*w/h, 0, yMundo,-1,1);

			
}
//.....................................................................................................
// Callback de evento de teclado
void teclado(unsigned char key, int x, int y)
{
   switch(key)
   {
      // Tecla ESC
      case 27:
         exit(0);
         break;
     case '+':
	nLados++;
     break;
     case '-':
        if(nLados>3)
		nLados--;
     break;
      default:
         break;
   }
	glutPostRedisplay();
}
//.....................................................................................................
  void atualizaCena(int valor)
  {
    // altera a cor do quadrado
   
    // atualiza a tela (desenha() será invocada novamente)
    glutPostRedisplay();
    // registra a callback novamente
    // por quê 25? 1000/25 = 40fps
    glutTimerFunc(25, atualizaCena, 0);
  }
//.................................................................................................
// Rotina principal
int main(int argc, char **argv)
{
    // Acordando o GLUT
    glutInit(&argc, argv);

    // Definindo a versão do OpenGL que vamos usar
    glutInitContextVersion(1, 1);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    // Configuração inicial da janela do GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);//glut init display mode
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);

    // Abre a janela
    glutCreateWindow("Quadrados Coloridos");

    // Registra callbacks para alguns eventos
    glutDisplayFunc(desenhaCena);
    glutReshapeFunc(redimensiona);
    glutKeyboardFunc(teclado);
    glutTimerFunc(0, atualizaCena, 0);
    
    inicializa();
    // Entra em loop e nunca sai
    glutMainLoop();
    return 0;
}

      
    
