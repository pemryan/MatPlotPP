# MatPlotPP
This repository is a fork of the [matplotpp](https://code.google.com/p/matplotpp/) library by **Yuichi Katori**. 

![logo](http://wiki.matplotpp.googlecode.com/git/logo.png)

# Welcome
**MATPLOT++** is a Matlab like simple plotting framework in C++. In cases where you need to make plots of data generated in C/C++, `matplotpp` enables you to generate 2D/3D plots, histograms, scatterplots, etc, just by adding a few lines of code, without any external plotting tool such as MATLAB or Gnuplot.

MATPLOT++ features are the following:
* MATLAB like command set.
* Cross platform compatibility on Linux, MacOS, and Windows. `matplotpp` is based on **OpenGL/GLUT** which is a simple cross-platform windowing **API**.
* High quality vector output: PS, EPS, PDF, and SVG.

You can generate 2D/3D plots, histograms, scatterplots, etc, by adding just few lines of code in C++ as following. 

![logo](http://wiki.matplotpp.googlecode.com/git/intro.png)

# Minimal Example
Following screenshot and code are a minimal example that provide just a sinusoidal curve. 
![logo](http://wiki.matplotpp.googlecode.com/git/ex_minimal.png)

```C++
#include "matplotpp.h"

using namespace MatPlotPP;
class MP : public MatPlot<double> { 
void DISPLAY() {
    vector<double> x(100), y(100);    
    for(int i=0; i<100; ++i){
        x[i] = 0.1*i;
        y[i] = sin(x[i]);
    }
    plot(x, y);
}
};

MP mp;
void display() { mp.display(); }
void reshape(int w, int h) { mp.reshape(w, h); }

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutCreateWindow(100, 100, 400, 300);
    glutDisplayFunc( display );
    glutReshapeFunc( reshape );
    glutMainLoop();    
    return 0;
}
```
The core of this code is on DISPLAY function (from line 4 to 11), which is a member function of class MP that is publicly derived from the MatPlot class. In this function, plot() conducts on plotting the sinusoidal function. In the main() function, functions prefixed by glut are about initialization of graphic environment. MATPLOT++ users are not required to know about OpenGL or GLUT. Just remember these functions are necessary for initialization of graphic environment. 

# Compilation
git clone https://github.com/pemryan/MatPlotPP.git

MSVC project files come with code.

