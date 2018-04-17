
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>//utiliza Radiano
#include <string.h>
#include <locale.h>
#include "textura.h"


float iniciarTextura(char *nomeTextura){	
	// FALTA: Implementar as texturas corretamente
	  char texto[100]; 
      float textura; 
       strcpy(texto,nomeTextura);
       textura = SOIL_load_OGL_texture(
        texto,
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
	);
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    if (textura == 0) {
        printf("Erro do SOIL: '%s'\n", SOIL_last_result());
    }
    return textura;
}
