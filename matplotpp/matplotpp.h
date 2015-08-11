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
#       pragma comment (lib, "matplotpp.lib")
#	endif
#else
#	define MATPLOT_API
#endif

inline int glutCreateWindow(const int left, const int top, const int width, const int height, const char* c) {
    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowPosition( left, top );
    glutInitWindowSize( width, height );   
    return glutCreateWindow(c);
};

inline int glutCreateWindow(const int left, const int top, const int width, const int height) {
    char c[] = "Untitled";
    return glutCreateWindow(left, top, width, height, c);
};

namespace MatPlotPP {

using std::string;
using std::vector;
using std::valarray;
using std::deque;
using std::max;
using std::min;

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

typedef vector<double> dvec;
typedef vector<vector<double>> dmat;
typedef vector<vector<vector<double>>> dcube;

template <typename T>
inline vector<T> linspace(const T minf, const T maxf, const size_t n);

template <typename T>
inline valarray<T> valinspace(const T minf, const T maxf, const int n);

template <typename T>
vector<vector<T>> contourc(const vector<T>& x, const vector<T>& y, const vector<vector<T>>& Z, const vector<T>& v);

/// contour
template <typename T>
struct ContourPoint {
    T x, y;
    int xj, yi;
    int xy;
    int done;
};

class Figure {
public:
    Figure(int id_);
    void add_child(int i);

public:
    int id;
    //int Status;// 0:minimized, 1:default position, 2:maximized 
    int Position[4];//left top width height
    bool isVisible;
    vector<int> Children;
};

class Layer {
 public:
    Layer(int id_);
    void add_child(int i);

public:
    int id;
    bool isVisible;
    string layername;
    vector<int> Children;
};

template <typename T>
class Axes {
public:
	Axes(int id_);
    void reset();
    void config();
    int ID();
    int selected();
    void selected(int i);
    void add_child(int i);    
    vector<T> make_tick(const T minf, T maxf);

public:
    int id;
    T cta, phi;   // controled by mouse
    T cta0, phi0; // default value or specified by command line
    T XMouse, YMouse;
    int Mouse;

    T xmin, xmax, ymin, ymax, zmin, zmax;
    int num_child;

    int View;// 0:2D, 1:3D

    vector<vector<T>> ColorMap;// for colorbar

// Matlab variables //
public:
    // styles
    int Box;           // 0:Off, 1:On
    string GridLineStyle;
    T LineWidth;
    string TickDir;    // {in} | out
    // string TickDirMode;
    // TickLength
    bool isVisible; //0:Off, 1:On
    int XGrid, YGrid, ZGrid; // {0:Off}, 1:On

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
class Line {
public:
	Line(int id_);
    int id;
    int Errorbar;

    void reset();
    void color(const T r,T g,T b);

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
class Surface {
public:
	Surface(int id_);
	void get();

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
    size_t NContour;
};

//Note: 
// [m,n] = size(Z), length(x) = n length(y) = m, (x(j),y(i),Z(i,j))
template <typename T>
class Patch {
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
    vector<vector<int>> Faces;
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
class Text {  
 public:
    Text(int id_);
    int id;
    string String;
    T Position[3];
    int Parent;
    int type;//0:axis 1:figure
};

template <typename T>
class MatPlot {
public:
    MatPlot();
    ~MatPlot();
    void virtual DISPLAY() {};

    // GLUT Callback Functions ///
    void display();
    void reshape(int w, int h);
    void mouse(int button, int state, int x, int y );
    void motion(int x, int y );
    void passivemotion(int x, int y);
    void keyboard(unsigned char key, int x, int y);
    
    // Layer ///
    int layer();
    //int layer(const string& s);    
    int layer(const string& s, const bool isVisible);
    int frame(const string& s, const bool isVisible);// do not use

    // Axes ///
    int axes();
    int gca();
    int subplot(int m,int n,int p);
    
    int colorbar();

    void axis(const T xMin, T xMax, T yMin, T yMax);
    void axis(const T xMin, T xMax, T yMin, T yMax, T zMin, T zMax);

    void axis(const string& s);
    void axis(int s);
    
    void grid(const string& s);
    void grid(int s);

    void ticklabel(const string& s);
    void ticklabel(int s);

    void title(const string& s);
    void xlabel(const string& s);
    void ylabel(const string& s);
    void zlabel(const string& s);

    //void xlim(const T min,T max);
    //void xlim(const string& s);
    //void legend(const string& s,int N);
    //int plotyy
    void mouse_capture(T *xmouse, T *ymouse);   
    // set, General Object Handling ///
    void set(const string& v);
    void set(const T v);  
    void set(const string& p, const T v);
    void set(const string& p, const string& v);
    void set(const int h, const string& p, const string& v);
    void set(const int h, const string& p, const T v);      
    int gco();
    
    // Line ///
    int begin();//do not use
    void end();//do not use
    void vertex(const T x,  const T y);
    void vertex(const T x, const T y, const T z);

    int line();
    int line(const vector<T>& x, const vector<T>& y);
    int line(const vector<T>& x, const vector<T>& y, const vector<T>& z);
    //line(X,Y)
    //line(X,Y,Z)

    int plot(const vector<T>& y);
    int plot(const vector<T>& x, const vector<T>& y);    
    //int plot(const vector<vector<T>>& Y);
    //int plot(const vector<T>& x, const vector<vector<T>>& Y);
    //int plot(const vector<vector<T>>& X, const vector<vector<T>>& Y);
    int plot(const valarray<T>& x, const valarray<T>& y);
    
    int plot3(const vector<T>& x, const vector<T>& y, const vector<T>& z);
    //int plot3(const vector<T>& x, const vector<T>& y, const vector<T>& z);

    int semilogx(const vector<T>& x, const vector<T>& y);
    int semilogy(const vector<T>& x, const vector<T>& y);
    //int loglog(vector<T> y);
    int loglog(const vector<T>& x, const vector<T>& y);    
    
    //int polar(vector<T> theta, vector<T> rho);

    void vertex(const T x, const T y, const T ep, const T em);
    int errorbar(const vector<T>& x, const vector<T>& y, const vector<T>& e);
    int errorbar(const vector<T>& x, const vector<T>& y, const vector<T>& ep, const vector<T>& em);

    //int quiver(U,V);
    //int quiver(X,Y,U,V);
    
    //int scatter(X,Y,S,C)
    //int scatter(X,Y,S)
    //int scatter(X,Y)

    // Surface, Contour ///
    vector<vector<T>> peaks(const int n);
    //vector<vector<T>> peaks(int m, int n);
    //vector<vector<T>> peaks(int m, int n, string type);

    int surface();
    int surface(const vector<vector<T>>& Z);
    int surface(const vector<vector<T>>& Z, const vector<vector<T>>& C);
    int surface(const vector<vector<T>>& Z, const vector<vector<vector<T>>>& C); //!!   
    int surface(const vector<T>& x, const vector<T>& y, const vector<vector<T>>& Z);
    int surface(const vector<T>& x, const vector<T>& y, const vector<vector<T>>& Z, const vector<vector<T>>& C);
    int surface(const vector<T>& x, const vector<T>& y, const vector<vector<T>>& Z, const vector<vector<vector<T>>>& C);//!!
    int surface(const vector<vector<T>>& X, const vector<vector<T>>& Y, const vector<vector<T>>& Z);
    int surface(const vector<vector<T>>& X, const vector<vector<T>>& Y, const vector<vector<T>>& Z, const vector<vector<T>>& C);
    int surface(const vector<vector<T>>& X, const vector<vector<T>>& Y, const vector<vector<T>>& Z, const vector<vector<vector<T>>>& C);//!!
    
    int pcolor(const vector<vector<T>>& C);
    int pcolor(const vector<vector<vector<T>>>& C);
    int pcolor(const vector<T>& x, const vector<T>& y, const vector<vector<T>>& C);
    int pcolor(const vector<T>& x, const vector<T>& y, const vector<vector<vector<T>>>& C);
    int pcolor(const vector<vector<T>>& X, const vector<vector<T>>& Y, const vector<vector<T>>& C);
    int pcolor(const vector<vector<T>>& X, const vector<vector<T>>& Y, const vector<vector<vector<T>>>& C);

    int contour(const vector<vector<T>>& Z);
    int contour(const vector<vector<T>>& Z, const int n);
    int contour(const vector<vector<T>>& Z, const vector<T>& v);
    int contour(const vector<T>& x, const vector<T>& y, const vector<vector<T>>& Z);
    int contour(const vector<T>& x, const vector<T>& y, const vector<vector<T>>& Z, const int n);
    int contour(const vector<T>& x, const vector<T>& y, const vector<vector<T>>& Z, const vector<T>& v);
    //int contour(vector<vector<T>>& X, vector<vector<T>>& Y, vector<vector<T>>& Z);
    //int contour(vector<vector<T>>& X, vector<vector<T>>& Y, vector<vector<T>>& Z,int n);
    //int contour(vector<vector<T>>& X, vector<vector<T>>& Y, vector<vector<T>>& Z, vector<T> v);

    //int mesh(vector<vector<T>>& Z);
    //int mesh(vector<vector<T>>& Z, vector<vector<T>>& C);
    //int mesh(vector<vector<T>>& Z, vector<vector<vector<T>>>& C);    
    int mesh(const vector<T>& x, const vector<T>& y, const vector<vector<T>>& Z);
    //int mesh(const vector<T>& x, const vector<T>& y, vector<vector<T>>& Z, vector<vector<T>>& C);
    //int mesh(const vector<T>& x, const vector<T>& y, vector<vector<T>>& Z, vector<vector<vector<T>>>& C);    
    //int mesh(vector<vector<T>>& X, vector<vector<T>>& Y, vector<vector<T>>& Z);
    //int mesh(vector<vector<T>>& X, vector<vector<T>>& Y, vector<vector<T>>& Z, vector<vector<T>>& C);
    //int mesh(vector<vector<T>>& X, vector<vector<T>>& Y, vector<vector<T>>& Z, vector<vector<vector<T>>>& C);
    // meshc()
    // meshz()

    int surf(const vector<T>& x, const vector<T>& y, const vector<vector<T>>& Z);

    // Patch ///

    int patch();
    int patch(const vector<vector<T>>& X, const vector<vector<T>>& Y);
    int patch(const vector<vector<T>>& X, const vector<vector<T>>& Y, const vector<T>& C);
    int patch(const vector<vector<T>>& X, const vector<vector<T>>& Y, const vector<vector<T>>& C);    
    int patch(const vector<vector<T>>& X, const vector<vector<T>>& Y, const vector<vector<T>>& Z, const vector<T>& C);//!!    
    int patch(const vector<vector<T>>& X, const vector<vector<T>>& Y, const vector<vector<T>>& Z, const vector<vector<T>>& C);//!!
    //int patch(vector<vector<T>>& X, vector<vector<T>>& Y,vector<vector<vector<T>>>& C);
    //int patch(vector<vector<T>>& X, vector<vector<T>>& Y, vector<vector<T>>& Z,vector<vector<vector<T>>>& C);

    int bar(const vector<T>& y);
    int bar(const vector<T>& y, const T width);
    int bar(const vector<T>& x, const vector<T>& y);
    int bar(const vector<T>& x, const vector<T>& y, const T width);

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
    int text(const T x, const T y, const string& s);
    void set_font(const char font_[], const int size);
    void ptext(const T x, const T y, const string& s);
    void ptext3(const T x, const T y, T z, const string& s);
    void ptext3c(const T x, const T y, T z, const string& s);

    // Colors ///
    void color(const T r, const T g, const T b);
    vector<T> colormap(const string& c, const T t);
    void colormap(const string& c);
    void colormap(const vector<vector<T>>& c);

    void gray();
    void jet();
    void hsv();
    void hot();
    void cool();
    void spring();
    void summer();
    void autumn();
    void winter();

    vector<T> map2color(const T x, const T xmin, const T xmax);
    
    void Shading(const string& c);
    void shading(const string& c);
    vector<T> ColorSpec2RGB(const string& c);
    string rgb2colorspec(const vector<T>& rgb);

    void print();
private:  
    // coordinate transform  //
    // figure coordination
    T ctx2(const T x);
    T cty2(const T y);
    // axes coordination
    T ctx(const T x);
    T cty(const T y);
    T ct3x(const T x);
    T ct3y(const T y);
    T ct3z(const T z);
    
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
    void Layer_mouse(const int button, const int state, const int x, const int y );
    void Axes_mouse(const int button, const int state, const int x, const int y );
    void Axes_motion(const int x, const int y );

    void surface_config();
    void line_config();
    void patch_config();
    vector<vector<T>> Index2TrueColor(const vector<T>& IC);

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

// export functions and class.
template MATPLOT_API std::vector<double> MatPlotPP::linspace(const double, const double, const size_t);
template MATPLOT_API std::vector<float> MatPlotPP::linspace(const float, const float, const size_t);
template class MATPLOT_API MatPlotPP::MatPlot<double>;
template class MATPLOT_API MatPlotPP::MatPlot<float>;

#endif

