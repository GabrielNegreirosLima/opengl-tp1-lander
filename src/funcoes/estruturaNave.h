
/*-----------------------------------------------------------------------------------------------------------*/
typedef struct vetor2d {
	GLfloat x;
	GLfloat y; 
}VETOR2D;
/*-----------------------------------------------------------------------------------------------------------*/
typedef struct nave {

	TIPO_VETOR2D posicao;		//estrutura do tipo vetor (x,y)
	GLfloat anguloDirecao ;		//angulo de giro inicial graus
	GLint numeroLadosCirculo; 	//numero de lados para se obter o circulo
	GLfloat angleGrauCirculo;	//angulograu para para se obter o circulo
  GLfloat angleRadCirculo;	//anguloradia lados para se obter o circulo
	GLfloat incrementoAngulo;	//inclento ao se usar teclas
	GLfloat raio;				//raio do circulo no qual é inscrito o triangulo
	GLfloat aceleracaoDianteira;//aceleracao da nave
  GLint aceleracaoTraseira;	//nao utilizado     
	GLint vidas;				//nao utilizado  
	GLuint pontuacao;	     	//nao utilizado  
	GLint identificadorTextura;	//nao utilizado 
  GLint tiroDisparado;		//enumera o tiro corrente do vetor de tiros atualiada em calculoDistanciaTiro()

}NAVE;  	// criacao do objeto nave
/*-----------------------------------------------------------------------------------------------------------*/
