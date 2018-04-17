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
#include "./funcoes/vetor/vetor.h"                          //funcoes de vetores
#include "./funcoes/texturas/textura.h"                     //as imagens e texturas estao aqui, basta invocar
#include "./funcoes/escritaTela/escritaTela.h"              //funcoes para escrevermos na tela
#include "./funcoes/aleatorio/random.h"                     //funcao que nos retorna um inteiro aleatório

typedef struct nave {

	VETOR posicao;
	VETOR velocidade;
	VETOR aceleracao;
	VETOR motorAceleracao;


}NAVE;


//Assinaturas Das funcoes
void redimensiona(GLsizei w, GLsizei h);
void keySpecialUp (int key, int x, int y);                      //funcoes ao desapertar teclas especias
void keySpecial (int key, int x, int y);                        //funcoes ao apertar teclas especiais
void keyASCIIPressed (unsigned char key, int x, int y) ;        //funcoes ao desapertar teclas normais
void desenhaCena(void);
void corDaFigura(int cor);                                      //utilizada para cor do texto ou para alguma figura que posssa vir a ter

void desenhaObjeto(float angulo,float posicaoX, float posicaoY, float textura, float verticesPoligono[4][2] , float verticesTextura[4][2]);
//a funcao acima rececebe dois vetores com as posicoes de sua textura e de seu poligono

void atualizaNave(void);                                        //funcoes de aceleracao, posicao e velocidade
void PararNave(void);                                           //sera utilizada quando o jogador atingir a base.
void inicializaBaseNave(void);                                  //ainda nao utilizada, para definir as bases onde a nave ira pousar
void inicializaFogoCombustaoNave(void);
void inicializaNaveDesligada(void);                             //inicializa dados importantes da nave
void inicializa(void);                                          //Inicializa dados importantes e fixos
void inicializaFundoMundo(void);
void informacoesJogo(void);
void verificarAcoplamentoComBase(void);                          //verifica se encostou na base


#define naveIncrementoAngulo 9                                   // incrementoemGraus
#define moduloAceleracaoMotor 0.01                               //modulo de aceleracao do motor
#define extremoMundo 300                                         // glorto: -extremidades-
#define tempoSistema 25                                          //utilizado na funcao de timer
#define espacoFinal extremoMundo*2                               //utilizado na gravidade
#define velocidadeInicial (1/tempoSistemaMilisegundos)
#define velocidadeTerminal 0.3
#define grausParaRadianos(graus) ((graus * M_PI) / 180)

bool paused;
int pousado;
int contadorReset;
int EXPLODIR;                                                    // se a nave for explodir, tera 1
int motorNaveLigado;
int contadorSaida;
int listaNave;
int anguloNave;
float texturaNaveMotorDesligado, texturaNaveMotorLigado;
float texturaDesenhoFundoMundo;
float texturaDesenhoFogoCombustao;
float texturaBaseNave;
float texturaNaveExplodir;
float moduloVelocidadeNave;
float raioDaNave;
float raioDesenhoFundoMundo;
float raioBaseNave;
float vetorVerticesNave[4][2], vetorverticestexturaNaveMotorDesligado[4][2];            //  estes vetores tem 4 vertices com 2 coordenadas
float vetorVerticesFundoMundo[4][2], vetorverticesTexturaFundoMundo[4][2];
float vetorVerticesNaveMotorLigado[4][2], vetorVerticesTexturaNaveMotorLigado[4][2];
float vetorVerticesNaveExplodir[4][2], vetorVerticesTexturaNaveExplodir[4][2];
float vetorVerticesBaseNave[4][2], vetorVerticesTexturaBaseNave[4][2];
int pontuacaoNave;

int flagDesenhaNave;

NAVE nave;
VETOR GRAVIDADE;
VETOR DesenhoFundoMundo;
VETOR BaseNave;

void naveUltrapassaTela(void){


    if(nave.posicao.x > extremoMundo)                               //nave passou da janela
        nave.posicao.x = 0;                                         //aparece na tela esquerda

    else if(nave.posicao.x < 0)                                     //nave passou da janela
        nave.posicao.x = extremoMundo;                              //aparece na tela esquerda

    if((nave.posicao.y + raioDaNave)> extremoMundo){                //nave passou da janela
        nave.posicao.y = extremoMundo - raioDaNave;                 //aparece na tela esquerda
        nave.velocidade.y = 0;
        nave.aceleracao.y = 0;
    }

    if(nave.posicao.y < 0){                                         //nave passou da janela
        PararNave();                                                //aparece na tela esquerda
    }
}

void inicializaNaveDesligada(void){// Define as carateristicas da nave

	paused = false;
	contadorSaida = 0;
    pontuacaoNave = 0;
    flagDesenhaNave = 1;
    motorNaveLigado =0;
    nave.posicao.x = pegaIntRandomico(300);
	nave.posicao.y = pegaIntRandomico(300) + 30;                    // 30 é altura da base
	anguloNave = 90;
    inserirCoordenadasVetor(&nave.aceleracao,0,0);                  //Aceleracao.x = 0, aceleracao.y=0
    inserirCoordenadasVetor(&nave.velocidade,0,0);                  //Aceleracao.x = 0, aceleracao.y=0
	raioDaNave = 13;
    texturaNaveMotorDesligado = iniciarTextura("./funcoes/texturas/foguete.png" );
    
    //vertices do quadrado da nave
    //inicializaVetor( vetorverticestexturaNaveMotorDesligado,0,0.5,0,0,0.25,0.5,0.25,0);
    vetorverticestexturaNaveMotorDesligado[0][0]=0.5; vetorverticestexturaNaveMotorDesligado[0][1]=0.25;//vertices da textura
    vetorverticestexturaNaveMotorDesligado[1][0]=0.5; vetorverticestexturaNaveMotorDesligado[1][1]=0;
    vetorverticestexturaNaveMotorDesligado[2][0]=1;   vetorverticestexturaNaveMotorDesligado[2][1]=0.25;
    vetorverticestexturaNaveMotorDesligado[3][0]=1; vetorverticestexturaNaveMotorDesligado[3][1]=0;

     //vertices do textura alinhada ao quadrado da nave
    vetorVerticesNave[0][0]=-raioDaNave; vetorVerticesNave[0][1]=raioDaNave;//vertices do quadrado nave
    vetorVerticesNave[1][0]=-raioDaNave;  vetorVerticesNave[1][1]=-raioDaNave;
    vetorVerticesNave[2][0]= raioDaNave; vetorVerticesNave[2][1]=raioDaNave;
    vetorVerticesNave[3][0]=raioDaNave; vetorVerticesNave[3][1]=-raioDaNave;


}


void inicializaNaveDesligadaAcelerando(void){// Define as carateristicas da nave


    texturaNaveMotorLigado = iniciarTextura("./funcoes/texturas/foguete.png" );//textura
    vetorVerticesTexturaNaveMotorLigado[0][0]=0; vetorVerticesTexturaNaveMotorLigado[0][1]=0.25;//vertices da textura
    vetorVerticesTexturaNaveMotorLigado[1][0]=0; vetorVerticesTexturaNaveMotorLigado[1][1]=0;
    vetorVerticesTexturaNaveMotorLigado[2][0]=0.5; vetorVerticesTexturaNaveMotorLigado[2][1]=0.25;
    vetorVerticesTexturaNaveMotorLigado[3][0]=0.5; vetorVerticesTexturaNaveMotorLigado[3][1]=0;
     //vertices do textura alinhada ao quadrado da nave
    vetorVerticesNaveMotorLigado[0][0]=-raioDaNave; vetorVerticesNaveMotorLigado[0][1]=raioDaNave;//vertices do quadrado nave
    vetorVerticesNaveMotorLigado[1][0]=-raioDaNave;  vetorVerticesNaveMotorLigado[1][1]=-raioDaNave;
    vetorVerticesNaveMotorLigado[2][0]= raioDaNave; vetorVerticesNaveMotorLigado[2][1]=raioDaNave;
    vetorVerticesNaveMotorLigado[3][0]=raioDaNave; vetorVerticesNaveMotorLigado[3][1]=-raioDaNave;


}


void inicializaBaseNave(void){// Define as carateristicas da base onde a nave pousará


    BaseNave.x =extremoMundo/4;
    BaseNave.y=30;
    texturaBaseNave = iniciarTextura("./funcoes/texturas/plastico.jpg" );//textura
    raioBaseNave = raioDaNave;
    vetorVerticesTexturaNaveMotorLigado[0][0]=0; vetorVerticesTexturaBaseNave[0][1]=0.25;//vertices da textura
    vetorVerticesTexturaBaseNave[1][0]=0; vetorVerticesTexturaBaseNave[1][1]=0;
    vetorVerticesTexturaBaseNave[2][0]=0.5; vetorVerticesTexturaBaseNave[2][1]=0.25;
    vetorVerticesTexturaBaseNave[3][0]=0.5; vetorVerticesTexturaBaseNave[3][1]=0;
    
    vetorVerticesBaseNave[0][0]=-2*raioBaseNave; vetorVerticesBaseNave[0][1]=raioBaseNave;//vertices da base
    vetorVerticesBaseNave[1][0]=-2*raioBaseNave;  vetorVerticesBaseNave[1][1]=0;
    vetorVerticesBaseNave[2][0]= 2*raioBaseNave; vetorVerticesBaseNave[2][1]=raioBaseNave;
    vetorVerticesBaseNave[3][0]=2*raioBaseNave; vetorVerticesBaseNave[3][1]=0;

}

void inicializaNaveDesligadaExplodir(void){



    texturaNaveExplodir = iniciarTextura("./funcoes/texturas/explosao.png" );//textura
    vetorVerticesTexturaNaveExplodir[0][0]=0; vetorVerticesTexturaNaveExplodir[0][1]=1;//vertices da textura
    vetorVerticesTexturaNaveExplodir[1][0]=0; vetorVerticesTexturaNaveExplodir[1][1]=0.75;
    vetorVerticesTexturaNaveExplodir[2][0]=0.25; vetorVerticesTexturaNaveExplodir[2][1]=1;
    vetorVerticesTexturaNaveExplodir[3][0]=0.25; vetorVerticesTexturaNaveExplodir[3][1]=0.75;
     
    vetorVerticesNaveExplodir[0][0]=-raioDaNave; vetorVerticesNaveExplodir[0][1]=raioDaNave;//vertices do quadrado nave
    vetorVerticesNaveExplodir[1][0]=-raioDaNave; vetorVerticesNaveExplodir[1][1]=-raioDaNave;
    vetorVerticesNaveExplodir[2][0]= raioDaNave; vetorVerticesNaveExplodir[2][1]=raioDaNave;
    vetorVerticesNaveExplodir[3][0]=raioDaNave; vetorVerticesNaveExplodir[3][1]=-raioDaNave;


}



void inicializaFundoMundo(void){

    DesenhoFundoMundo.x = extremoMundo/2;
	DesenhoFundoMundo.y = extremoMundo/2;                                      //extremoMundo-(raioDaNave*1.5);
	raioDesenhoFundoMundo = 494.6;                                             //((larguraDaTela-10) * sqrt(2)/2);
    texturaDesenhoFundoMundo = iniciarTextura("./funcoes/texturas/mars.jpg" ); //textura
    //vertices do quadrado da nave
    vetorverticesTexturaFundoMundo[0][0]=0; vetorverticesTexturaFundoMundo[0][1]=1;//v0
    vetorverticesTexturaFundoMundo[1][0]=0; vetorverticesTexturaFundoMundo[1][1]=0;//v1
    vetorverticesTexturaFundoMundo[2][0]=1; vetorverticesTexturaFundoMundo[2][1]=1;//v2
    vetorverticesTexturaFundoMundo[3][0]=1; vetorverticesTexturaFundoMundo[3][1]=0;//v3
     //vertices do textura alinhada ao quadrado da nave
    vetorVerticesFundoMundo[0][0]=-raioDesenhoFundoMundo; vetorVerticesFundoMundo[0][1]=raioDesenhoFundoMundo;//vertices do quadrado nave
    vetorVerticesFundoMundo[1][0]=-raioDesenhoFundoMundo; vetorVerticesFundoMundo[1][1]=-raioDesenhoFundoMundo;
    vetorVerticesFundoMundo[2][0]= raioDesenhoFundoMundo; vetorVerticesFundoMundo[2][1]=raioDesenhoFundoMundo;
    vetorVerticesFundoMundo[3][0]=raioDesenhoFundoMundo;  vetorVerticesFundoMundo[3][1]=-raioDesenhoFundoMundo;
}



void desenhaObjeto(float angulo,float posicaoX, float posicaoY, float textura, float verticesPoligono[4][2] , float verticesTextura[4][2]){

     corDaFigura(8);
     glPushMatrix();
	    glLoadIdentity();
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textura); 
        glTranslatef(posicaoX, posicaoY, 0);
        glRotatef(angulo , 0, 0, 1);					
        glBegin(GL_TRIANGLE_STRIP);
	        glTexCoord2f(verticesTextura[0][0],verticesTextura[0][1]);//v1 textura
            glVertex2f(verticesPoligono[0][0],verticesPoligono[0][1]);//v1 objeto
	        glTexCoord2f(verticesTextura[1][0],verticesTextura[1][1]);//v2 textura
            glVertex2f(verticesPoligono[1][0],verticesPoligono[1][1]);//v2 objeto
	        glTexCoord2f(verticesTextura[2][0],verticesTextura[2][1]);//v3 textura
            glVertex2f(verticesPoligono[2][0],verticesPoligono[2][1]);//v3 objeto
	        glTexCoord2f(verticesTextura[3][0],verticesTextura[3][1]);//v4 textura
            glVertex2f(verticesPoligono[3][0],verticesPoligono[3][1]);//v4 objeto
      glEnd();
      glDisable(GL_TEXTURE_2D);
    glPopMatrix();					
  /* atencao nos vértices, sentido antihorário
     v0 ----v2
     |      |
     v1-----v3 */

}


void informacoesJogo(void){

    corDaFigura(8);//escreve em branco
	escreveNaCenaString("ALTITUDE: ",5,280,0);  
    escreveNaCenaNumero(nave.posicao.y,36,280,0);  
	escreveNaCenaString("HORIZONTAL SPEED: ",5,270,0);  
    escreveNaCenaNumero(nave.velocidade.x,60,270,0);  
	escreveNaCenaString("VERTICAL SPEED:",5,260,0);  
    escreveNaCenaNumero(nave.velocidade.y,52,260,0);  
	escreveNaCenaString("ANGLE Deg:",5,250,0);  
    escreveNaCenaNumero(anguloNave,40,250,0);  

	escreveNaCenaString("SCORE:",240,280,0);  
    escreveNaCenaNumero(pontuacaoNave,260,280,0); 

    if(contadorSaida==1 && !paused && contadorReset!=1){
        escreveNaCenaString("Deseja sair? Aperte: SIM(ESC) NÃO(c)",100, 280, 0);
        PararNave();
    }
    if(paused && contadorReset!=1   ){
        escreveNaCenaString("PAUSADO",140, 280, 0);
        PararNave();
    }
    if(contadorReset == 1){
        escreveNaCenaString("Deseja reiniciar? Aperte: SIM(r) NÃO(c)",100, 280, 0);
        PararNave();
    }
    if(pousado == 1){
        escreveNaCenaString("VOCE POUSOU!",100, 280, 0);
    }


}


void corDaFigura(int cor){
 //preto 0,verm 1,verde 2,azul 3,amarel 4,magent 5,ciano 6,cinza 7, branco 8

 float cores [9][3]={{0,0,0},{1,0,0},{0,1,0},{0,0,1},{1,1,0},{1,0,1},{0,1,1},{0.6,0.6,0.6},{1,1,1}};
 glColor3f(cores[cor][0], cores[cor][1],cores[cor][2]);
}


void PararNave(void){//quando pousar

     	nave.velocidade.x =0;
		nave.velocidade.y =0;
		nave.aceleracao.x =0;
		nave.aceleracao.y =0;

}

void verificarAcoplamentoComBase(void){

    if ( ((nave.posicao.y - raioDaNave) <= (BaseNave.y+(raioBaseNave/2))) && (nave.posicao.x <= (BaseNave.x+raioBaseNave) && nave.posicao.x >= (BaseNave.x-raioBaseNave))){// encontrou
        if(nave.velocidade.y >= -0.3 && (anguloNave%90)==0){
            pousado = 1;
            pontuacaoNave+=10;
            nave.posicao.x = pegaIntRandomico(300);
            nave.posicao.y = pegaIntRandomico(300) + 30;
            sleep(3);
        }
        else{
            pousado = 0;
            EXPLODIR = 1;
            PararNave();
        }
    }
}


void atualizaNave(void){ //desenho a nave, funcao deve ser chamada na callback desenha cena.
	 moduloVelocidadeNave = normaDoVetor(nave.velocidade);
     float moduloVelocidadeNaveX = sqrt(pow(nave.velocidade.x,2));
     float moduloVelocidadeNaveY = sqrt(pow(nave.velocidade.y,2));
     VETOR velocidadeNaveNormalizada;

    nave.posicao = somaVetores( nave.posicao, nave.velocidade);

	//VELOCIDADE TERMINAL
    if(moduloVelocidadeNave > velocidadeTerminal){                                                          //caso nave ultrapasse velocidade
        inserirCoordenadasVetor(&velocidadeNaveNormalizada, nave.velocidade.x, nave.velocidade.y);
        normalizarVetor(&velocidadeNaveNormalizada);                                                        //normaliza vetor velocidade
        vetorMultiplicaEscalar(&velocidadeNaveNormalizada,velocidadeTerminal-0.001);                        //multiplica o vetor normalizado pela velocidade Máxima
        nave.velocidade = velocidadeNaveNormalizada;                                                        //normalizada e multplicada pela velocidade terminal
    }

    //VELOCIDADE NORMAL
    else{
        nave.velocidade = somaVetores( nave.velocidade, nave.aceleracao);
        nave.aceleracao = somaVetores( nave.aceleracao, GRAVIDADE);//soma apenas em y (-)
    }
    verificarAcoplamentoComBase();
    naveUltrapassaTela();
}

void imprimirDados(char d,char c){

	    printf("\n***************VELOCIDADE terminal? %c %c***************************************",d,c);
	    printf("\n ANGULO:%d", anguloNave);
	    printf("\n Coseno:%.10f ", cos(grausParaRadianos( anguloNave)));
	    printf("\n Seno:%f", sin(grausParaRadianos( anguloNave)));
	    printf("\nnave.aceleracao.x:%f",nave.aceleracao.x);
	    printf("\nnave.aceleracao.y:%f, Gravidade:%f,%f",nave.aceleracao.y,GRAVIDADE.y,GRAVIDADE.x);
	    printf("\nnave.velocidade.x:%f",nave.velocidade.x);
	    printf("\nnave.velocidade.y:%f",nave.velocidade.y);
	    printf("\nnave.posicao.x:%f",nave.posicao.x);
	    printf("\nnave.posicao.y:%f",nave.posicao.y);


}

// FUNCOES DE TECLADO - COMECO
void keyASCIIPressed (unsigned char key, int x, int y){ // Teclas normais pressionadas

    switch(key)
    {
        // Tecla ESC
        case 27:
            contadorSaida++;
            if(contadorSaida==2)
                exit(0);
            break;

        case 'c':
            contadorReset--;
            contadorSaida--;
            break;
        case 'p':
            if(paused)
                paused = false;
            else
                paused = true;
            break;

        case 'r':
            contadorReset++;
            if(contadorReset == 2){
                pontuacaoNave = 0;
                nave.posicao.x = pegaIntRandomico(300);
                nave.posicao.x = pegaIntRandomico(300) + 30;            // altura da base
                EXPLODIR = 0;
                anguloNave = 90;
                contadorReset = 0;
            }
            break;
        default:
            break;
    }
	glutPostRedisplay();
}

void keySpecial (int key, int x, int y){ 	//MODIFICAR

    VETOR aceleracaoMotor;
    switch(key){

        case  GLUT_KEY_LEFT:	// esquerda
           anguloNave += naveIncrementoAngulo;
           break;

        case GLUT_KEY_RIGHT:	// direita
              anguloNave -= naveIncrementoAngulo;

	      break;
        case GLUT_KEY_UP:
             motorNaveLigado = 1;
            inserirCoordenadasVetor(&aceleracaoMotor, cos(grausParaRadianos( anguloNave)), sin(grausParaRadianos( anguloNave)));
            normalizarVetor(&aceleracaoMotor);//normaliza vetor velocidade
            vetorMultiplicaEscalar(&aceleracaoMotor,moduloAceleracaoMotor);//multiplica o vetor normalizado pela velocidade Máxima
            nave.aceleracao = somaVetores( nave.aceleracao, aceleracaoMotor);//somo a aceleracao da nave com a aceleracao do motor
          

        break;
        case GLUT_KEY_DOWN:
        default:
        break;
    }
    //glutPostRedisplay();

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
		    //nave.motorAceleracao.x = 0;
		   // nave.motorAceleracao.y = 0;
            motorNaveLigado = 0;
            break;
        case GLUT_KEY_DOWN:
        default:
        break;
    }
}


// FUNCOES DE TECLADO - FIM


void inicializa(void){                          // Inicia algumas variaveis de estado


    GRAVIDADE.y= -0.001;
    GRAVIDADE.x = 0;
    EXPLODIR = 0;
    glClearColor(1, 1, 1, 0);                   // Cor para limpar a tela com a co branca
    inicializaFundoMundo();
	inicializaNaveDesligada();
    inicializaNaveDesligadaAcelerando();
    inicializaNaveDesligadaExplodir();
    inicializaBaseNave();

}


void redimensiona(GLsizei w, GLsizei h){        // Funcao que sera passada como callback chamada quando o tamanho da janela se alterado
	if(h == 0) h = 1;                           // Evita a divisao por zero
   	glViewport(0, 0, w,h);                      // Especifica as dimencoes da Viewport

   	glMatrixMode(GL_PROJECTION);                // Inicializa o sistema de coordenadas
   	glLoadIdentity();

   	if (w <= h)                                 // Estabelece a janela de selecao (left, right, bottom, top)
		glOrtho(0,extremoMundo, 0, extremoMundo * h/w,-1,1);// Left, right, bottom, top, near, far
   	else
	    glOrtho(0, extremoMundo * w/h,0,extremoMundo,-1,1);
   	glMatrixMode(GL_MODELVIEW);
   	glLoadIdentity();
}

void atualizaCena(int valor){

	atualizaNave();
   	glutPostRedisplay();
   	glutTimerFunc(tempoSistema, atualizaCena, 0);					// Por que 25? 1000/25 = 40fps

}

void desenhaCena(void){


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Apaga o conteudo da janela

    desenhaObjeto(0,DesenhoFundoMundo.x, DesenhoFundoMundo.y,  texturaDesenhoFundoMundo,\
    vetorVerticesFundoMundo,vetorverticesTexturaFundoMundo);
    if(motorNaveLigado ==0 && EXPLODIR==0){
        desenhaObjeto(anguloNave,nave.posicao.x, nave.posicao.y,\
        texturaNaveMotorDesligado, vetorVerticesNave,vetorverticestexturaNaveMotorDesligado);// Desenha a nave.
    }else if(motorNaveLigado ==1 && EXPLODIR==0){
        desenhaObjeto(anguloNave,nave.posicao.x, nave.posicao.y,\
        texturaNaveMotorLigado, vetorVerticesNaveMotorLigado,vetorVerticesTexturaNaveMotorLigado);//nave com motor ligado
    }else{
        desenhaObjeto(anguloNave,nave.posicao.x, nave.posicao.y,\
        texturaNaveExplodir, vetorVerticesNave,vetorVerticesTexturaNaveExplodir);
    }

  desenhaObjeto(0,BaseNave.x, BaseNave.y,texturaBaseNave, vetorVerticesBaseNave,vetorVerticesTexturaBaseNave);//base da nave
  informacoesJogo();
  glutSwapBuffers();
}

int main(int argc, char **argv){

	glutInit(&argc, argv);// Acordando o GLUT
	glutInitContextVersion(1, 1);// Definindo a versao do OpenGL que vamos usar
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);	// Configuraao inicial da janela do GLUT
	glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGBA);
	glutInitWindowPosition(0, 0);// Informa local inicial para o canto esquerdo superior da janela.
    glutInitWindowSize(700, 700);

	glutCreateWindow("Arcade TP1");// Cria a janela
    inicializa();
	//Registra callbacks para alguns eventos
	glutDisplayFunc(desenhaCena);// O que queremos desenhar deve ser referenciado aqui
	glutReshapeFunc(redimensiona);// O que queremos aconteça quando a tela for redimensionada
	glutKeyboardFunc(keyASCIIPressed);
	glutSpecialFunc(keySpecial);
	glutSpecialUpFunc(keySpecialUp);

 	glutTimerFunc(0, atualizaCena, 0);//chama imediatamente
	glutMainLoop();
	return 0;

}
