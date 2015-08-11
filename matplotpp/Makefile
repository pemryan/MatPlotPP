#Makefile
GLUT_INCLUDE = ../../include

matplotpp: matplotpp.cpp gl2ps.c
	g++ -std=c++11 -D_USE_MATH_DEFINES -I$(GLUT_INCLUDE) -g -O0 -c ./matplotpp.cpp ./gl2ps.c
	ar r matplotpp.a matplotpp.o gl2ps.o
	rm matplotpp.o gl2ps.o

clean:
	rm matplotpp.a
