install:	
	sudo apt-get update
	sudo apt-get install freeglut3 freeglut3-dev binutils-gold g++ cmake libglew-dev mesa-common-dev libglew1.5-dev libglm-dev
	sudo apt-get install libsoil-dev



	
game:	main.c ./funcoes/vetor/vetor.c ./funcoes/aleatorio/random.c ./funcoes/escritaTela/escritaTela.c ./funcoes/texturas/textura.c 
	gcc -o main.exe main.c ./funcoes/vetor/vetor.c ./funcoes/aleatorio/random.c ./funcoes/escritaTela/escritaTela.c ./funcoes/texturas/textura.c -lSOIL -lglut -lGL -lGLEW -lGLU -lm

run:	game
	./main.exe
