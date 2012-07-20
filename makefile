2DNavierStokes:	src/2DNavierStokes.o src/initialisation.o src/computeAdvection.o src/output.o
	gcc -lfftw3 src/2DNavierStokes.o src/initialisation.o src/computeAdvection.o src/output.o -o 2DNavierStokes

src/2DNavierStokes.o: src/2DNavierStokes.c src/2DNavierStokes.h src/initialisation.h
	gcc -c src/2DNavierStokes.c -o src/2DNavierStokes.o

src/initialisation.o: src/initialisation.c src/initialisation.h src/2DNavierStokes.h src/problem.h
	gcc -c src/initialisation.c -o src/initialisation.o

src/computeAdvection.o: src/computeAdvection.c src/computeAdvection.h src/2DNavierStokes.h
	gcc -c src/computeAdvection.c -o src/computeAdvection.o

src/output.o:	src/output.c src/output.h src/2DNavierStokes.h
	gcc -c -Wall src/output.c -o src/output.o
