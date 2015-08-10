/****************************************************************************
Copyright (c) 2011 Yuichi Katori All Rights Reserved
License: Gnu Public license (GPL) v3
Author: Yuichi Katori (yuichi.katori@gmail.com)
Project: MATPLOT++ (MATLAB-like plotting tool in C++).
Version: 0.3.13
****************************************************************************/
#ifndef MAT_PLOT_H_
#define MAT_PLOT_H_

#pragma warning( disable : 4251 )

#include <vector>
#include <deque>
#include <string>
#include <valarray>
#include <iostream>
#include <math.h>
#include <time.h>

#include <GL/glut.h>
#include "gl2ps.h"

#ifdef _MSC_VER
#	ifdef DLL_matplotpp
#		define MATPLOT_API __declspec(dllexport)
#		else
#		define MATPLOT_API __declspec(dllimport)
#	endif
#else
#	define MATPLOT_API
#endif

 inline int glutCreateWindow(int left, int top, int width, int height, const char* c) {
    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowPosition( left, top );
    glutInitWindowSize( width, height );   
    return glutCreateWindow(c);
};

 inline int glutCreateWindow(int left, int top, int width, int height) {
    char c[] = "untitled";
    return glutCreateWindow(left, top, width, height, c);
};

namespace MatPlotPP {

using namespace std;

#define PI 3.14159265358979323846264

const int tFigure = 1;
const int tAxes = 2;
const int tLine = 3;
const int tSurface = 4;
const int tText = 5;
const int tLayer = 6;
const int tPatch = 7;

typedef vector<float> fvec;
typedef vector<vector<float>> fmat;
typedef vector<vector<vector<float>>> fcube;

typedef vector<float> dvec;
typedef vector<vector<float>> dmat;
typedef vector<vector<vector<float>>> dcube;

template <typename T>
 inline vector<T> linspace(T minf, T maxf, int n) {
    vector<T> a;
    if(n<1) {
		n = 1;
	}
    a.resize(n);
    for(int i=0; i<n; ++i) {
		a[i] = minf+(maxf-minf)*i/(n-1);
	}
    return a;
};

template <typename T>
 inline valarray<T> valinspace(T minf, T maxf, int n) {
    valarray<T> a; 
    a.resize(n);
    for(int i=0; i<n; ++i) {
		a[i] =  minf + (maxf - minf)*i/(n-1);
	}
    return a;
};

class Figure {///
 public:
    Figure(int id_) {
		id = id_;
		//Status=1;
		//Position[0] = 100;
		//Position[1] = 100;
		//Position[2] = 800;
		//Position[3] = 800;
		Visible=1;
    };
    void add_child(int i);

public:
    int id;
    //int Status;// 0:minimized, 1:default position, 2:maximized 
    int Position[4];//left top width height
    int Visible;
    vector<int> Children;
};

class  Layer {///
 public:
    Layer(int id_);
    void add_child(int i);

public:
    int id;
    int Visible;
    string layername;
    vector<int> Children;
};

template <typename T>
class Axes {///
public:
	Axes(int id_);
    void reset();
    void config();
    int ID();
    int selected();
    void selected(int i);
    void add_child(int i);    
    vector<T> make_tick(T minf, T maxf);

public:
    int id;
    T cta,phi;  // controled by mouse
    T cta0,phi0;// default value or specified by command line
    // Mouse 
    T XMouse, YMouse;
    int Mouse;

    T xmin, xmax, ymin, ymax, zmin, zmax;
    int num_child;

    int View;// 0:2D, 1:3D

    vector<vector<T>> ColorMap;// for colorbar

    // Matlab variables //
    // styles
    int Box;//0:Off, 1:On
    string GridLineStyle;
    T LineWidth;
    string TickDir; // {in} | out
    //string TickDirMode;
    //TickLength
    int Visible;//0:Off, 1:On
    int XGrid,YGrid,ZGrid;// {0:Off}, 1:On

    // General Information 
    int Parent;
    vector<int> Children;
    int Selected;
    T Position[4];//left bottom width height
    T Viewport3d[4];//left bottom width height

    //Scale
    string XAxisLocation;
    string YAxisLocation;

    //string XDir,YDir,ZDir;

    T XLim[2], YLim[2], ZLim[2];//plot range
    int XLimMode, YLimMode, ZLimMode;//0:Auto 1:Manual
    int XScale, YScale, ZScale;// linear | log

    vector<T> XTick,YTick,ZTick;
    string XTickMode,YTickMode,ZTickMode;
    int TickLabel;// 0:Off, {1:On}
    //View
    T CameraPosition[3];
    T CameraTarget[3];
    T CameraUpVector[3];

    // Label
    string Title;
    string XLabel,YLabel,ZLabel;

    T CLim[2];
};

template <typename T>
class  Line {///
public:
	Line(int id_);
    int id;
    int Errorbar;

    void reset();
    void color(T r,T g,T b);

public:
    // Matlab oriented variables //
    vector<T> XData, YData, ZData;
    vector<T> YPData, YMData;
    //vector<vector<T>> XData,YData,ZData;
    //vector<vector<T>> EData,UData,LData;
    //vector<vector<T>> VData,WData;

    string Color;
    string LineStyle;// {-} | - - | : | -. | none
    T  LineWidth;
    string Marker;// {none}
    T  MarkerSize;
    string MarkerEdgeColor;
    string MarkerFaceColor;

    int Clipping;
    //string EraseMode;
    int SelectionHighlight;
    int Visible;

    // General Information 
    int Parent;
    int Selected;  
};

template <typename T>
class  Surface {///
public:
	Surface(int id_);

	void get() {
		cout <<"FaceColor: "<< FaceColor <<endl;
		cout <<"EdgeColor: "<< EdgeColor <<endl;
		cout <<"LineStyle: "<< LineStyle <<endl;
		cout <<"LineWidth: "<< LineWidth <<endl;
    }

public:
    int type;
    int id;
    string ColorMap;

    //vector<T> XData,YData;
    vector<vector<T>> XData, YData, ZData, CDataIndex;
    vector<T> V, UserData;
    vector<vector<vector<T>>> CData;
    
    string FaceColor;//ColorSpec    | none | {flat} 
    string EdgeColor;//ColorSpec{k} | none | flat
    
    string LineStyle;// {-} | - - | : | -. | none
    T  LineWidth;
    string Marker;// {none}
    T  MarkerSize;
    string MarkerEdgeColor;
    string MarkerFaceColor;

    int Parent;
    int NContour;
};

//Note: 
// [m,n] = size(Z), length(x) = n length(y) = m, (x(j),y(i),Z(i,j))

template <typename T>
class  Patch {///
 public:
    Patch(int id_) {	
		id=id_;
		type=0;
		LineWidth=1;
		FaceColor="r"; 
		EdgeColor="k";
		LineStyle="-";
    }

public:
    int id;
    int type;
    vector< vector<int> > Faces;
    vector<vector<T>> Vertices;
    vector<vector<T>> FaceVertexCData;
    vector<vector<T>> XData,YData,ZData;

    //vector<T> ICVec;
    //vector<vector<T>> ICMat;    
    //vector<vector<vector<T>>> CData;
    vector<vector<T>> CData;

    string EdgeColor,FaceColor;//{ColorSpec}|none|flat|interp 

    string LineStyle; // {-} | - - | : | -. | none
    T  LineWidth;
};
//Note: XData[iv][if]

template <typename T>
class  Text {///  
 public:
    Text(int id_);
    int id;
    string String;
    T Position[3];
    int Parent;
    int type;//0:axis 1:figure
};

/// contour
template <typename T>
struct  ContourPoint {
    T x, y;
    int xj, yi;
    int xy;
    int done;
};

template <typename T>
 vector<vector<T>> contourc(vector<T> x, vector<T> y, vector<vector<T>> Z, vector<T> v);

template <typename T>
class MatPlot {///
public:
    MatPlot();
    ~MatPlot();
    void virtual DISPLAY() {};

    // GLUT Callback Functions ///
    void display();
    void reshape(int w, int h);
    void mouse(int button, int state, int x, int y );
    void motion(int x, int y );
    void passivemotion(int x,int y);
    void keyboard(unsigned char key, int x, int y);
    
    // Layer ///
    int layer();
    //int layer(string s);    
    int layer(string s, int Visible);
    int frame(string s, int Visible);// do not use

    // Axes ///
    int axes();
    int gca();
    int subplot(int m,int n,int p);
    
    int colorbar();

    void axis(T xMin, T xMax, T yMin, T yMax);
    void axis(T xMin, T xMax, T yMin, T yMax, T zMin, T zMax);

    void axis(string s);
    void axis(int s);
    
    void grid(string s);
    void grid(int s);

    void ticklabel(string s);
    void ticklabel(int s);

    void title(string s);
    void xlabel(string s);
    void ylabel(string s);
    void zlabel(string s);

    //void xlim(T min,T max);
    //void xlim(string s);
    //void legend(string s,int N);
    //int plotyy
    void mouse_capture(T *xmouse, T *ymouse);   
    // set, General Object Handling ///
    void set(string v);
    void set(T v);  
    void set(string p,T v);
    void set(string p,string v);
    void set(int h,string p,string v);
    void set(int h,string p,T v);      
    int gco();
    
    // Line ///
    int begin();//do not use
    void end();//do not use
    void vertex(T x, T y);
    void vertex(T x, T y, T z);

    int line();
    int line(vector<T> x, vector<T> y);
    int line(vector<T> x, vector<T> y, vector<T> z);
    //line(X,Y)
    //line(X,Y,Z)

    int plot(vector<T> y);
    int plot(vector<T> x, vector<T> y);    
    //int plot(vector<vector<T>> Y);
    //int plot(vector<T> x, vector<vector<T>> Y);
    //int plot(vector<vector<T>> X, vector<vector<T>> Y);
    int plot(valarray<T> x,valarray<T> y);
    
    int plot3(vector<T> x, vector<T> y, vector<T> z);
    //int plot3(vector<T> X, vector<T> Y, vector<T> Z);

    int semilogx(vector<T> x, vector<T> y);
    int semilogy(vector<T> x, vector<T> y);
    //int loglog(vector<T> y);
    int loglog(vector<T> x, vector<T> y);    
    
    //int polar(vector<T> theta, vector<T> rho);

    void vertex(T x, T y, T ep, T em);
    int errorbar(vector<T> x, vector<T> y, vector<T> e);
    int errorbar(vector<T> x, vector<T> y, vector<T> ep, vector<T> em);

    //int quiver(U,V);
    //int quiver(X,Y,U,V);
    
    //int scatter(X,Y,S,C)
    //int scatter(X,Y,S)
    //int scatter(X,Y)

    // Surface, Contour ///
    vector<vector<T>> peaks(int n);
    //vector<vector<T>> peaks(int m,int n);
    //vector<vector<T>> peaks(int m,int n,string type);

    int surface();
    int surface(vector<vector<T>> Z);
    int surface(vector<vector<T>> Z, vector<vector<T>> C);
    int surface(vector<vector<T>> Z, vector<vector<vector<T>>> C); //!!   
    int surface(vector<T> x, vector<T> y, vector<vector<T>> Z);
    int surface(vector<T> x, vector<T> y, vector<vector<T>> Z, vector<vector<T>> C);
    int surface(vector<T> x, vector<T> y, vector<vector<T>> Z, vector<vector<vector<T>>> C);//!!
    int surface(vector<vector<T>> X, vector<vector<T>> Y, vector<vector<T>> Z);
    int surface(vector<vector<T>> X, vector<vector<T>> Y, vector<vector<T>> Z, vector<vector<T>> C);
    int surface(vector<vector<T>> X, vector<vector<T>> Y, vector<vector<T>> Z, vector<vector<vector<T>>> C);//!!
    
    int pcolor(vector<vector<T>> C);
    int pcolor(vector<vector<vector<T>>> C);
    int pcolor(vector<T> x, vector<T> y, vector<vector<T>> C);
    int pcolor(vector<T> x, vector<T> y, vector<vector<vector<T>>> C);
    int pcolor(vector<vector<T>> X, vector<vector<T>> Y, vector<vector<T>> C);
    int pcolor(vector<vector<T>> X, vector<vector<T>> Y, vector<vector<vector<T>>> C);

    int contour(vector<vector<T>> Z);
    int contour(vector<vector<T>> Z,int n);
    int contour(vector<vector<T>> Z, vector<T> v);
    int contour(vector<T> x, vector<T> y, vector<vector<T>> Z);
    int contour(vector<T> x, vector<T> y, vector<vector<T>> Z,int n);
    int contour(vector<T> x, vector<T> y, vector<vector<T>> Z, vector<T> v);
    //int contour(vector<vector<T>> X, vector<vector<T>> Y, vector<vector<T>> Z);
    //int contour(vector<vector<T>> X, vector<vector<T>> Y, vector<vector<T>> Z,int n);
    //int contour(vector<vector<T>> X, vector<vector<T>> Y, vector<vector<T>> Z, vector<T> v);

    //int mesh(vector<vector<T>> Z);
    //int mesh(vector<vector<T>> Z, vector<vector<T>> C);
    //int mesh(vector<vector<T>> Z, vector<vector<vector<T>>> C);    
    int mesh(vector<T> x, vector<T> y, vector<vector<T>> Z);
    //int mesh(vector<T> x, vector<T> y, vector<vector<T>> Z, vector<vector<T>> C);
    //int mesh(vector<T> x, vector<T> y, vector<vector<T>> Z, vector<vector<vector<T>>> C);    
    //int mesh(vector<vector<T>> X, vector<vector<T>> Y, vector<vector<T>> Z);
    //int mesh(vector<vector<T>> X, vector<vector<T>> Y, vector<vector<T>> Z, vector<vector<T>> C);
    //int mesh(vector<vector<T>> X, vector<vector<T>> Y, vector<vector<T>> Z, vector<vector<vector<T>>> C);
    // meshc()
    // meshz()

    int surf(vector<T> x, vector<T> y, vector<vector<T>> Z);

    // Patch ///

    int patch();
    int patch(vector<vector<T>> X, vector<vector<T>> Y);
    int patch(vector<vector<T>> X, vector<vector<T>> Y, vector<T> C);
    int patch(vector<vector<T>> X, vector<vector<T>> Y, vector<vector<T>> C);    
    int patch(vector<vector<T>> X, vector<vector<T>> Y, vector<vector<T>> Z, vector<T> C);//!!    
    int patch(vector<vector<T>> X, vector<vector<T>> Y, vector<vector<T>> Z, vector<vector<T>> C);//!!
    //int patch(vector<vector<T>> X, vector<vector<T>> Y,vector<vector<vector<T>>> C);
    //int patch(vector<vector<T>> X, vector<vector<T>> Y, vector<vector<T>> Z,vector<vector<vector<T>>> C);

    int bar(vector<T> y);
    int bar(vector<T> y,T width);
    int bar(vector<T> x, vector<T> y);
    int bar(vector<T> x, vector<T> y,T width);

    //int bar(Y)
    //int bar(Y,T width);
    //int bar(Y,string style);
    //int bar(Y,T width,string style);

    //int bar(x,Y)
    //int bar(x,Y,T width);
    //int bar(x,Y,string style);
    //int bar(x,Y,T width,string style);
      
    //int hist(y);
    //int hist(y,x);
    //int hist(y,nbins);

    //int pie(vector<T> x);
    //int pie(vector<T> x, vector<int> Explode);

    // Text ///
    //TODO: more fonts    
    int text();
    int text(T x,T y,string s);
    void set_font(char font_[],int size);
    void ptext(T x,T y,string s);
    void ptext3(T x,T y,T z,string s);
    void ptext3c(T x,T y,T z,string s);

    // Colors ///
    void color(T r,T g,T b);
    vector<T> colormap(string c,T t);
    void colormap(string c);
    void colormap(vector<vector<T> > c);

    void gray();
    void jet();
    void hsv();
    void hot();
    void cool();
    void spring();
    void summer();
    void autumn();
    void winter();

    vector<T> map2color(T x, T xmin, T xmax);
    
    void Shading(string c);
    void shading(string c);
    vector<T> ColorSpec2RGB(string c);
    string rgb2colorspec(vector<T> rgb);

    // print ///
    void print();


private:  
    // coordinate transform  //
    // figure coordination
    T ctx2(T x);
    T cty2(T y);
    // axes coordination
    T ctx(T x);
    T cty(T y);
    T ct3x(T x);
    T ct3y(T y);
    T ct3z(T z);
    
    int figure();

    // display //
    void display_figure();    
    void display_layer();
    void display_layer2();
    
    void display_axes();
    void display_axes_2d();
    void display_axes_3d();
    void display_axes_colorbar();
    
    void display_line();
    
    void display_surface();
    void display_surface_2d();
    void display_surface_3d();
    void display_pcolor();
    void display_contour();

    void display_patch();
    void display_patch_2d();
    void display_patch_3d();

    void display_bar();

    void display_text();

    // mouse //
    void Layer_mouse(int button, int state, int x, int y );
    void Axes_mouse(int button, int state, int x, int y );
    void Axes_motion(int x, int y );

    void surface_config();
    void line_config();
    void patch_config();
    vector<vector<T>> Index2TrueColor(vector<T> IC);

private:
    vector<vector<T>> cmap;//TODO move to class Figure
    
    int isInited;   // 0:initialization 1:configuration
    int init_level; // initialization level of objects
    int hObj;       // handle number of current object

    int time_layer_clicked, time_layer_clicked_last;

    // Events //
    int window_w, window_h;
    T xButtonDown, yButtonDown;  // last clicked mouse position
    T ctaButtonDown, phiButtonDown;
    int xPassive, yPassive;

    // pointer to current objects //
    Figure *cf;
    Layer *cfr;
    Axes<T> *ca;
    Line<T> *cl;
    Surface<T> *cs;
    Patch<T> *cp;
    Text<T> *ct;    

    // objects containers //
    vector<Figure> vFigure;
    vector<Layer> vLayer;
    vector<Axes<T>> vAxes; 
    vector<Line<T>> vLine;
    vector<Surface<T>> vSurface;
    vector<Patch<T>> vPatch;
    vector<Text<T>> vText;

    // objects counter //
    int iFigure;
    int iLayer;
    int iAxes;
    int iLine;
    int iSurface;
    int iPatch;
    int iText;

    // Selected object //
    int iAxesSelected;
};

} // end namespace matplotpp

#include "matplotpp_impl.h"

#endif

