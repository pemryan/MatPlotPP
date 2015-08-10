// Copyright (c) 2011 Yuichi Katori (yuichi.katori@gmail.com) All Rights Reserved

#include "matplotpp.h"

using namespace MatPlotPP;

class MP :public MatPlot <float> { 
	void DISPLAY(){
		vector<float> x(100), y(100);    
		for(int i=0; i<100; ++i){
			x[i] = 0.1f * i;
			y[i] = sin(x[i]);
		}
		plot(x,y);
	}
} mp;

void display() { mp.display(); }
void reshape(int w, int h){ mp.reshape(w, h); }

void idle( void ) { glutPostRedisplay(); Sleep(1);}
void mouse(int button, int state, int x, int y ) { mp.mouse(button,state,x,y); }
void motion(int x, int y ) { mp.motion(x, y); }
void passive(int x, int y ) { mp.passivemotion(x,y); }
void keyboard(unsigned char key, int x, int y) { mp.keyboard(key, x, y); }

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutCreateWindow(100, 100, 400, 300);
    glutDisplayFunc( display );
    glutReshapeFunc( reshape );
    glutIdleFunc( idle );
    glutMotionFunc( motion );
    glutMouseFunc( mouse );
    glutPassiveMotionFunc(passive);    
    glutKeyboardFunc( keyboard );        
    glutMainLoop();    
    return 0;
}
