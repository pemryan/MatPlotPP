//Copyright (c) 2011 Yuichi Katori All Rights Reserved
//Author: Yuichi Katori (yuichi.katori@gmail.com)

#include "matplotpp.h"

using MatPlotPP::MatPlot;

template <typename T>
class MP : public MatPlot<T> { 
public:
	void DISPLAY() {
		std::vector<T> x(100), y(100);    

		for(int i=0; i<100; ++i) {
			x[i] = 0.1*i;
			y[i] = sin(x[i]);
		}

		plot(x, y);
	}
};


MP<double> mp;
void display() { mp.display(); }
void reshape(int w, int h) { mp.reshape(w, h); }

int main(int argc, char* argv[]) {

    glutInit(&argc, argv);
    glutCreateWindow(100, 100, 400, 300, "example1");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();    

    return 0;
}
