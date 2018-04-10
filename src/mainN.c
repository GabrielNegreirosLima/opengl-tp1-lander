#include<stdbool.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>//utiliza Radiano
#include <string.h>
#include <locale.h>


//Bibliotecas proprias
#include "./funcoes/vetor/vetor.h" //Estrutura Vetor - Funcoes: inserirCoordenadasVetor(),somaVetores(),anguloEntreDoisVetores();


typedef struct nave {

	VETOR posicao;
	VETOR velocidade;		       
	VETOR aceleracao;	
	VETOR motorAceleracao;	        

}NAVE;  	

//Assinaturas Das funcoes
void redimensiona(GLsizei w, GLsizei h);
void inicializa(void);
void keySpecialUp (int key, int x, int y);
void keySpecial (int key, int x, int y);
void keyASCIIUp(unsigned char key, int x, int y);
void keyASCIIPressed (unsigned char key, int x, int y) ;
void desenhaCena(void);
void criaListaNave(void);
void iniciarTextura(void);
void inicializaNave(void);
void desenhaNave(void);
void atualizaNave(void);

double motory,motorx;

#define naveIncrementoAngulo 9// inclementoemGraus
#define raioDaNave 15
//#define VELOCIDADEMAXIMA -20/40
#define moduloAceleracaoMotor 0.001
#define extremoMundo 150 // glorto: -extremidades-
#define tempoSistema 25 
#define tempoSistemaMilisegundos 0.025
#define espacoFinal extremoMundo*2
#define tempoQueda 400
#define velocidadeInicial (1/tempoSistemaMilisegundos)
#define coeficienteArrastoD 0.001 //Para calculo da velocidade terminal
#define GRAVIDADE 0 // ((((espacoFinal + (velocidadeInicial * tempoQueda))* pow(tempoSistemaMilisegundos,2))/pow(tempoQueda,2)))
#define velocidadeTerminal 0.025
#define radianoParaGraus(radianos) (radianos * (180.0 / M_PI))
#define grausParaRadianos(graus) ((graus * M_PI) / 180.0)

float anguloNave;													// Inclinacao da nave 
int listaNave;													// Armazena display list number
int texturaNave;	
float larguraDaTela;          									// Obter tamanho da janela
float alturaDaTela;
int flagControlaVelocidadeMaxima;

float contadorTempo;
NAVE nave;

// FUNCOES DA NAVE - COMECO
void iniciarTextura(void){
	
	// FALTA: Implementar as texturas corretamente
	
    glClearColor (1, 1, 1, 0);

    texturaNave = SOIL_load_OGL_texture(
        "nave.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
	);
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    if (texturaNave == 0) {
        printf("Erro do SOIL: '%s'\n", SOIL_last_result());
    }

}


void inicializaNave(void){								// Define as carateristicas da nave

	/* FALTA: alterar para inicializar 
	* a nave em lugar aleatorio
	*/
    
	larguraDaTela = glutGet(GLUT_WINDOW_WIDTH); 		// Obter tamanho da janela
	alturaDaTela =  glutGet(GLUT_WINDOW_HEIGHT);  	 
	nave.posicao.x =0; 
	nave.posicao.y= extremoMundo-(raioDaNave*1.5);
	anguloNave = 90;//radianos 
	nave.aceleracao.x=0;
    nave.aceleracao.y =0;
   
}

void criaListaNave(void){// Invocado em inicializa()

    //listaNave= glGenLists(1);// GLuint glGenLists(GLsizei range);Specifies the number of contiguous empty display lists to be generated.


    // Começa a usar a textura que criamos
    
		// glNewList(listaNave,GL_COMPILE);   
      

		//glEndList();

 }
void desenhaNave(void){//desenho a nave, funcao deve ser chamada na callback desenha cena.
    
    
  printf("\n ANGULO:%f, coseno:%f seno:%f nave.motorAceleracao.x:%f nave.motorAceleracao.y:%f",anguloNave, cos(grausParaRadianos( anguloNave)), sin(grausParaRadianos( anguloNave)),nave.motorAceleracao.x,nave.motorAceleracao.y);
    glPushMatrix();										// Duplica a matriz atual, colocando no topo da pilha
	    glLoadIdentity();								// Limpar a matriz
        glEnable(GL_TEXTURE_2D);//habilita textura
        glBindTexture(GL_TEXTURE_2D, texturaNave); //chamatextura
	    glTranslatef(nave.posicao.x, nave.posicao.y, 0);// Translada o sistema de coordenadas para (x,y)     
	    glRotatef(anguloNave  , 0, 0, 1);					// Rotaciona o sistema de coordenadas no próprio lugar
        glBegin(GL_QUADS);	
            glTexCoord2f(0,0); glVertex2f(-raioDaNave,-raioDaNave);					
            glTexCoord2f(1,0);glVertex2f(raioDaNave,-raioDaNave);//		
            glTexCoord2f(1,1); glVertex2f( raioDaNave, raioDaNave );			
            glTexCoord2f(0,1); glVertex2f(-raioDaNave, raioDaNave); //Desenhei no sentio antihorario a frente do poligono			
            glEnd();						// Chamada da calllist da nave
        glDisable(GL_TEXTURE_2D);//desabilitarTextura
    glPopMatrix();								        // Remove a matriz atual do topo da pilha	

}


void PararNave(void){//desenho a nave, funcao deve ser chamada na callback desenha cena.

     	nave.velocidade.x =0;
		nave.velocidade.y =0;
		nave.aceleracao.x =0;
		nave.aceleracao.y =0;
	

}
void atualizaNave(void){ //desenho a nave, funcao deve ser chamada na callback desenha cena.

    
    
   float moduloAceleracaoNaveX = sqrt(pow(nave.aceleracao.x,2));
   float moduloAceleracaoNaveY = sqrt(pow(nave.aceleracao.y,2));   

        if (moduloAceleracaoNaveX >= velocidadeTerminal){//Verifico na aceleracao, pois a velocidade receberá o valor desta no futuro.
			nave.posicao.x += nave.velocidade.x;
			nave.posicao.y += nave.velocidade.y;
			nave.velocidade.x += nave.aceleracao.x;
			nave.velocidade.y += nave.aceleracao.y;
			nave.aceleracao.x +=nave.aceleracao.x + nave.motorAceleracao.x;//constante, nao varia
			nave.aceleracao.y +=(-GRAVIDADE + nave.motorAceleracao.y);
            printf("\n\n VELOCIDADE TERMINAL XXXX %f",velocidadeTerminal);//exit(0);
            
		}
        else if (moduloAceleracaoNaveY >= velocidadeTerminal){
			nave.posicao.x += nave.velocidade.x;
			nave.posicao.y += nave.velocidade.y;
			nave.velocidade.x += nave.aceleracao.x;
			nave.velocidade.y += nave.aceleracao.y;
			nave.aceleracao.x += nave.aceleracao.x + nave.motorAceleracao.x;
			nave.aceleracao.y = (-GRAVIDADE + nave.motorAceleracao.y);//sem variacao
            printf("\n\n VELOCIDADE TERMINAL YYYY %f",velocidadeTerminal);//exit(0);
		}
       else{           
			nave.posicao.x += nave.velocidade.x;
			nave.posicao.y += nave.velocidade.y;
			nave.velocidade.x += nave.aceleracao.x;
			nave.velocidade.y += nave.aceleracao.y;
			nave.aceleracao.x +=  nave.aceleracao.x + nave.motorAceleracao.x;
			nave.aceleracao.y +=(-GRAVIDADE + nave.motorAceleracao.y);
        }
        //printf("\n\n nave motorX %f motorY %f Velocide:%f,%f Posicao:%f,%f",nave.aceleracao.x,nave.aceleracao.y,nave.velocidade.x,nave.velocidade.y,nave.posicao.x,nave.posicao.y);
}

// FUNCOES DE TECLADO - COMECO
void keyASCIIPressed (unsigned char key, int x, int y){ // Teclas normais pressionadas

    switch(key)
    {
        // Tecla ESC
        case 27:
            exit(0);
            break;
        case 32:
        {   

            break;
        }
        case '=':
           
            break;
        case '-':
        case '_':
         
            break;
        default:
            break;
    }
	glutPostRedisplay();
}

void keyASCIIUp (unsigned char key, int x, int y){ 		// Teclas normais despressionadas


							
}

void keySpecial (int key, int x, int y){ 	//MODIFICAR

    // printf("\n gravidade %f",GRAVIDADE);
   
    switch(key)
    {
        // TECLAS
        case  GLUT_KEY_LEFT:	// esquerda
           anguloNave += naveIncrementoAngulo; 
            break;
        case GLUT_KEY_RIGHT:	// direita
              anguloNave -= naveIncrementoAngulo; 			
	      break;
        case GLUT_KEY_UP:
          
	      nave.motorAceleracao.x = moduloAceleracaoMotor * cos(grausParaRadianos( anguloNave));
          nave.motorAceleracao.y = moduloAceleracaoMotor * sin(grausParaRadianos( anguloNave));
          break;
        case GLUT_KEY_DOWN:
        default:
        break;
    }
    glutPostRedisplay();

}
void keySpecialUp (int key, int x, int y){		
   
    switch(key)
    {
        // TECLAS
        case  GLUT_KEY_LEFT:	// esquerda           
            break;
        case GLUT_KEY_RIGHT:	// direita              			
	      break;
        case GLUT_KEY_UP:          
            nave.motorAceleracao.x = 0;
            nave.motorAceleracao.y =0;


          break;
        case GLUT_KEY_DOWN:
        default:
        break;
    }
}


// FUNCOES DE TECLADO - FIM


void inicializa(void){ 									// Inicia algumas variaveis de estado
   
    
    contadorTempo=0;
    glClearColor(1, 1, 1, 0);							// Cor para limpar a tela com a co branca
	inicializaNave();
	iniciarTextura();
   // glFrontFace(GL_CW);// GL_CW(sentido horário) ou GL_CCW(sentido anti-horário).
}

void redimensiona(GLsizei w, GLsizei h){   				// Funcao que sera passada como callback chamada quando o tamanho da janela se alterado 
   
   
	if(h == 0) h = 1;           						// Evita a divisao por zero	
   	glViewport(0, 0, w,h);								// Especifica as dimencoes da Viewport
   	
   	glMatrixMode(GL_PROJECTION);						// Inicializa o sistema de coordenadas
   	glLoadIdentity();
   	
   	if (w <= h)            								// Estabelece a janela de selecao (left, right, bottom, top)
		glOrtho(-extremoMundo,extremoMundo, -extremoMundo, extremoMundo * h/w,-1,1);			// Left, right, bottom, top, near, far	 
   	else 
		glOrtho(-extremoMundo, extremoMundo * w/h, -extremoMundo, extremoMundo,-1,1);	
   	glMatrixMode(GL_MODELVIEW);
   	glLoadIdentity();		
}

void atualizaCena(int valor){

    
	atualizaNave();
   	glutPostRedisplay();
   	glutTimerFunc(tempoSistema, atualizaCena, 0);					// Por que 25? 1000/25 = 40fps
    contadorTempo ++;
    
}

void desenhaCena(void)
{   
     
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Apaga o conteudo da janela
 	desenhaNave();									// Desenha a nave.	
	glutSwapBuffers();									// Diz ao OpenGL para colocar o que desenhamos na tela

} 


int main(int argc, char **argv){	
	glutInit(&argc, argv);								// Acordando o GLUT		
	glutInitContextVersion(1, 1);						// Definindo a versao do OpenGL que vamos usar
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);	// Configuraao inicial da janela do GLUT
	glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGBA);
	
	glutInitWindowPosition(0, 0);		 				// Informa local inicial para o canto esquerdo superior da janela.
        glutInitWindowSize(700, 700);	
	glutCreateWindow("Arcade TP1");		 				// Cria a janela
	
	//Registra callbacks para alguns eventos
	glutDisplayFunc(desenhaCena);              	  		// O que queremos desenhar deve ser referenciado aqui 
	glutReshapeFunc(redimensiona);		          		// O que queremos aconteça quando a tela for redimensionada 
	glutKeyboardFunc(keyASCIIPressed);	          
	glutKeyboardUpFunc(keyASCIIUp); 	          
	glutSpecialFunc(keySpecial); 			 
	glutSpecialUpFunc(keySpecialUp);
	//criaListaNave();				
  	inicializa();        			
 	glutTimerFunc(0, atualizaCena, 0);                  //chama imediatamente
	glutMainLoop();		     	
	return 0;

}  
