/****************************************************************************
Copyright (c) 2011 Yuichi Katori All Rights Reserved
License: Gnu Public license (GPL) v3
Author: Yuichi Katori (yuichi.katori@gmail.com)
Project: MatPlot++ (MATLAB-like plotting tool in C++).
Version:0.3.13
****************************************************************************/
#include "MatPlotpp.h"

namespace MatPlotPP {

void Figure::add_child(int i) {
	Children.push_back(i);
}

template <typename T>
Axes<T>::Axes(int id_) {
	id = id_;
	Selected=0;
	Position[0] = 0.13f;
	Position[1] = 0.11f;
	Position[2] = 0.775f;
	Position[3] = 0.815f;
    
	Viewport3d[0] = 0.0f;
	Viewport3d[1] = 0.0f;
	Viewport3d[2] = 1.0f;
	Viewport3d[3] = 1.0f;
	
	Mouse = 1;
	View = 0;
	Visible = 1;
	Box = 1;
	Children.clear();
	
	cta0 = 30; 
	phi0 = 30;
	cta = cta0; 
	phi = cta0;
	
	CameraPosition[0] = 1; CameraPosition[1] = 1; CameraPosition[2] = 1;
	CameraTarget[0] = 0.;  CameraTarget[1] = 0; CameraTarget[2] = 0;
	CameraUpVector[0] = 0; CameraUpVector[1] = 0; CameraUpVector[2] = 1;
	
	LineWidth = 1;
	
	GridLineStyle = ":";

	XGrid = 0;
	YGrid = 0;
	ZGrid = 0;
	
	XLim[0] = 0;    XLim[1] = 10;
	YLim[0] = 0;    YLim[1] = 10;
	ZLim[0] = 0;    ZLim[1] = 10;
	
	XLimMode = 0; YLimMode = 0; ZLimMode = 0;
	XAxisLocation = "bottom"; //top | bottom
	YAxisLocation = "left";   // left | right
	XScale = 0;               // {0:linear} | 1:log
	YScale = 0;
	ZScale = 0;
    
	TickLabel = 1;
	TickDir = "in";
	
	xmin = T(1e19);    xmax = T(-1e19);
	ymin = T(1e19);    ymax = T(-1e19);
	zmin = T(1e19);    zmax = T(-1e19);
	
	CLim[0] = 0;
	CLim[1] = 0;

	num_child = 0;
	// MakeTick();
	// Parent = i_figure;

	XMouse = 0;
	YMouse = 0;
};

/// Axes
template <typename T>
void Axes<T>::reset() {
    num_child = 0;
    xmin = 1e19f; xmax = -1e19f;
    ymin = 1e19f; ymax = -1e19f;
    zmin = 1e19f; zmax = -1e19f;	
}

template <typename T>
void Axes<T>::config() {
    T extent = 0.f, extent_linear = 0.03f;
    if((XLimMode == 0) && (xmax>xmin)) {
		if(XScale == 0) {
			extent =extent_linear;
		}
		if(XScale == 1) {
			extent = 0.f;
		}
		XLim[0] = xmin-extent*(xmax-xmin); 
		XLim[1] = xmax+extent*(xmax-xmin);
    }

    if((YLimMode == 0)&&(ymax>ymin)) {
		if(YScale == 0) {
			extent = extent_linear;
		}
		if(YScale == 1) {
			extent = 0;
		}
		YLim[0] = ymin-extent*(ymax-ymin); 
		YLim[1] = ymax+extent*(ymax-ymin);
    }

    if((ZLimMode == 0) && (zmax>zmin)) {
		ZLim[0] = zmin-extent*(zmax-zmin); 
		ZLim[1] = zmax+extent*(zmax-zmin);
    }	
    //printf("Z: %d,%f,%f\n",ZLimMode,ZLim[0],ZLim[1]);
    //if(num_child) {Visible = 1;}else{Visible = 0;}
    
    XTick = make_tick(XLim[0],XLim[1]);
    YTick = make_tick(YLim[0], YLim[1]);
    ZTick = make_tick(ZLim[0],ZLim[1]);
}

template <typename T>
int Axes<T>::ID() {return id;}

template <typename T>
int Axes<T>::selected() {return Selected;}

template <typename T>
void Axes<T>::selected(int i) {Selected = i;}

template <typename T>
void Axes<T>::add_child(int i) {Children.push_back(i);}
    
template <typename T>
vector<T> Axes<T>::make_tick(T minf, T maxf) {
	int i,j;
	T dg;
	T x, y;
	int z;
	x = fabs(maxf-minf);
	z = (int)log10(x);
	y = x/pow(T(10), T(z));
	dg = 1*pow(T(10), T(z));
	if(y<5) {dg = T(0.5)*pow(T(10), T(z));}
	if(y<2) {dg = T(0.2)*pow(T(10), T(z));}

	T min0;
	min0 = minf- fmod(minf,dg); j = 0;

	vector<T> tick;
	tick.clear();
	if(maxf>minf) { i =-2; while(maxf>=min0+dg*i) { if(minf<=min0+dg*i) { tick.push_back(min0+dg*i); j++; } i+= 1;} }
	if(maxf<minf) { i =-2; while(maxf<=min0-dg*i) { if(minf>=min0-dg*i) { tick.push_back(min0-dg*i); j++; } i+= 1;} }
	return tick;
}

/// Line
template <typename T>
Line<T>::Line(int id_) {	
	id=id_;

	Color="b";
	LineStyle="-";
	LineWidth=0.5;

	Marker="none";
	MarkerSize=6;
	MarkerEdgeColor="k";
	MarkerFaceColor="w";

	Errorbar=0;
	//PlotStyle=0;
} 

template <typename T>
void Line<T>::reset() {
    XData.clear();
    YData.clear();
    ZData.clear();
    YPData.clear();
    YMData.clear();
}

template <typename T>
void Line<T>::color(T r,T g,T b) {
    //Color[0] = r;
    //Color[1] = g;
    //Color[2] = b;
}

/// Surface
template <typename T>
Surface<T>::Surface(int id_) {
	id=id_;

	ColorMap="Gray";
	//Shading="faceted";
	FaceColor="flat"; 
	EdgeColor="b";
	LineStyle="-";
	LineWidth=0.5;
	NContour=10;
	V.clear();
}

/// Text
template <typename T>
Text<T>::Text(int id_) 
	: id(id_), type(0) {
}

/// Layer
Layer::Layer(int id_) 
	: id(id_) { 
    Children.clear();	
}

/// Patch
void Layer::add_child(int i) {Children.push_back(i);}

//// MatPlot //
template <typename T>
MatPlot<T>::MatPlot() {
    isInited = 0;
    init_level = 0;

    iAxesSelected =-1;

    colormap("Jet");

    xPassive = 100;
    yPassive = 0;
}

template <typename T>
MatPlot<T>::~MatPlot() {
    vFigure.clear();
    vAxes.clear();
    vLine.clear();	
}

//void MatPlot<T>::virtual DISPLAY() {};

template <typename T>
void MatPlot<T>::display() {

    int is_done = 0;
    int mode_next =-1;
    while(!is_done) {
		if(isInited == 0) {// Allocation of objects
			iFigure = 0;
			iLayer  = 0; 	
			iAxes   = 0;
			iLine   = 0;
			iSurface = 0;
			iPatch  = 0;
			iText   = 0;	    
    
			if(init_level == 0) {vFigure.clear();}
			if(init_level<= 1) {vLayer.clear();}
			if(init_level<=2) {vAxes.clear();}
			if(init_level<=3) {vLine.clear();}
			if(init_level<=3) {vSurface.clear();}
			if(init_level<=3) {vText.clear();}
			if(init_level<=3) {vPatch.clear();}
	    
			//default objects
			figure();
			layer();
			axes();

			DISPLAY();

			mode_next = 1;
		}

		if(isInited == 1) {// Configuration of objects
			iFigure = 0;
			iLayer  = 0; 	
			iAxes   = 0;
			iLine   = 0;
			iSurface = 0;
			iPatch  = 0;
			iText   = 0;

			//default objects
			figure();
			layer();
			axes();

			DISPLAY();
	
			mode_next =2;

			// checking number of objects. if it is inconsistent, then objects are reallocated.
			if(vFigure.size() != iFigure ) { mode_next = 0; }
			if(vAxes.size()   != iAxes   ) { mode_next = 0; }
			if(vLine.size()   != iLine   ) { mode_next = 0; }
			if(vSurface.size()!= iSurface) { mode_next = 0; }
			if(vText.size()   != iText   ) { mode_next = 0; }
			if(vLayer.size()  != iLayer  ) { mode_next = 0; }

			for(int i = 0; i<vAxes.size(); ++i) { vAxes[i].config(); }
		}

		if(isInited ==2) {// display
			display_figure();
			mode_next = 1;
			is_done = 1;
		}

		isInited = mode_next;
    }	
}

template <typename T>
void MatPlot<T>::color(T r,T g,T b) {
    if(isInited) {
	//cl->Color[0] = r;
	//cl->Color[1] = g;
	//cl->Color[2] = b;
    }
}
// coordinate transform  ///

// figure coordination
template <typename T>
T MatPlot<T>::ctx2(T x) {
    T t;

    if(ca->XScale == 0) {//linear
		return ca->Position[0] + ca->Position[2] * ( (x-ca->XLim[0])/(ca->XLim[1]-ca->XLim[0]) );
    }

    if(ca->XScale == 1) {//log
		t = ( log10(x) - log10(ca->XLim[0]) )/( log10(ca->XLim[1])-log10(ca->XLim[0]) );
		if(x<= 0) {
			t = -1;
		}
		return ca->Position[0] + ca->Position[2] * t;
    }
    
	return 0.f;
}

template <typename T>
T MatPlot<T>::cty2(T y) { 
    if(ca->YScale == 0) {//linear
		return ca->Position[1] + ca->Position[3]*( (y-ca->YLim[0])/(ca->YLim[1]-ca->YLim[0]) );
    }

    if(ca->YScale == 1) {//log
		return ca->Position[1] + ca->Position[3]*( log10(y) - log10(ca->YLim[0]) )/( log10(ca->YLim[1])-log10(ca->YLim[0]) );
    }

	return 0.f;
}

// axes coordination
template <typename T>
T MatPlot<T>::ctx(T x) {
    if(ca->XScale == 0) {//linear
		return (x-ca->XLim[0])/(ca->XLim[1]-ca->XLim[0]);
    }

    if(ca->XScale == 1) {//log
		return ( log10(x) - log10(ca->XLim[0]) )/( log10(ca->XLim[1])-log10(ca->XLim[0]) );
    }

	return 0.f;
}

template <typename T>
T MatPlot<T>::cty(T y) {
    if(ca->YScale == 0) {//linear
		return (y-ca->YLim[0])/(ca->YLim[1]-ca->YLim[0]);
    }

    if(ca->YScale == 1) {//log
		return ( log10(y) - log10(ca->YLim[0]) )/( log10(ca->YLim[1])-log10(ca->YLim[0]) );
    }

	return 0.f;
}

template <typename T>
T MatPlot<T>::ct3x(T x) {	
    return -1 + 2*(x-ca->XLim[0]) / (ca->XLim[1]-ca->XLim[0]);
}

template <typename T>
T MatPlot<T>::ct3y(T y) {	
    return -1+2*(y-ca->YLim[0])/(ca->YLim[1]-ca->YLim[0]);
}

template <typename T>
T MatPlot<T>::ct3z(T z) {	
    return -1+2*(z-ca->ZLim[0])/(ca->ZLim[1]-ca->ZLim[0]);
}

/// set(v)
template <typename T>
void MatPlot<T>::set(string v) {
    int h = gco();
    int tObj = h%100;
    int iObj = h/100;

    //if( (tObj == tLine) && (iObj<vLine.size()) ) {
    if( v=="k" ) { set(h,"COLOR","k"); }
    if( v=="r" ) { set(h,"COLOR","r"); }
    if( v=="b" ) { set(h,"COLOR","b"); }
    if( v=="g" ) { set(h,"COLOR","g"); }
    if( v=="c" ) { set(h,"COLOR","c"); }
    if( v=="m" ) { set(h,"COLOR","m"); }
    if( v=="y" ) { set(h,"COLOR","y"); }
    if( v=="w" ) { set(h,"COLOR","w"); }

    if( v=="dr" ) { set(h,"COLOR","dr"); }
    if( v=="db" ) { set(h,"COLOR","db"); }
    if( v=="dg" ) { set(h,"COLOR","dg"); }
    if( v=="dc" ) { set(h,"COLOR","dc"); }
    if( v=="dm" ) { set(h,"COLOR","dm"); }
    if( v=="dy" ) { set(h,"COLOR","dy"); }

    if( v=="lr" ) { set(h,"COLOR","lr"); }
    if( v=="lb" ) { set(h,"COLOR","lb"); }
    if( v=="lg" ) { set(h,"COLOR","lg"); }
    if( v=="lc" ) { set(h,"COLOR","lc"); }
    if( v=="lm" ) { set(h,"COLOR","lm"); }
    if( v=="ly" ) { set(h,"COLOR","ly"); }

    if( v=="ur" ) { set(h,"COLOR","ur"); }
    if( v=="ub" ) { set(h,"COLOR","ub"); }
    if( v=="ug" ) { set(h,"COLOR","ug"); }
    if( v=="uy" ) { set(h,"COLOR","uy"); }
    if( v=="uc" ) { set(h,"COLOR","uc"); }
    if( v=="up" ) { set(h,"COLOR","up"); }
    if( v=="uo" ) { set(h,"COLOR","uo"); }
    if( v=="um" ) { set(h,"COLOR","um"); }
    if( v=="ubr" ) {set(h,"COLOR","ubr"); }

    if( v=="-"  ) { set(h,"LineStyle","-");   set(h,"Marker","none"); }
    if( v=="- -") { set(h,"LineStyle","- -"); set(h,"Marker","none"); }
    if( v==":"  ) { set(h,"LineStyle",":");   set(h,"Marker","none"); }
    if( v=="-." ) { set(h,"LineStyle","-.");  set(h,"Marker","none"); }
    
    if( v=="." ) { set(h,"Marker","."); set(h,"LineStyle","none"); }
    if( v=="+" ) { set(h,"Marker","+"); set(h,"LineStyle","none"); }
    if( v=="x" ) { set(h,"Marker","x"); set(h,"LineStyle","none"); }
    if( v=="d" ) { set(h,"Marker","d"); set(h,"LineStyle","none"); }
    if( v=="^" ) { set(h,"Marker","^"); set(h,"LineStyle","none"); }
    if( v=="v" ) { set(h,"Marker","v"); set(h,"LineStyle","none"); }
    if( v=="o" ) { set(h,"Marker","o"); set(h,"LineStyle","none"); }
    if( v=="*" ) { set(h,"Marker","*"); set(h,"LineStyle","none"); }
    if( v=="s" ) { set(h,"Marker","s"); set(h,"LineStyle","none"); }
    if( v==">" ) { set(h,"Marker",">"); set(h,"LineStyle","none"); }
    if( v=="<" ) { set(h,"Marker","<"); set(h,"LineStyle","none"); }
    if( v=="p" ) { set(h,"Marker","p"); set(h,"LineStyle","none"); }
    if( v=="h" ) { set(h,"Marker","h"); set(h,"LineStyle","none"); } 
}

template <typename T>
void MatPlot<T>::set(T v) {
    int h = gco();
    int tObj =h%100;
    int iObj =h/100;
    if( (tObj == tLine) && (iObj<vLine.size()) ) {
		vLine[iObj].LineWidth = v;
		vLine[iObj].MarkerSize = v;	
    }
}

/// set(p,v),set(h,p,v)
template <typename T>
void MatPlot<T>::set(string p, string v) {
    int h =gco();
    set(h,p,v);
}

template <typename T>
void MatPlot<T>::set(string p, T v) {
    int h =gco();
    set(h,p,v);
}

template <typename T>
void MatPlot<T>::set(int h, string p, string v) {
    int tObj =h%100;
    int iObj =h/100;
    // Axes //
    if( (tObj == tAxes) && (iObj<vAxes.size()) ) {
		if(p=="TickDir") { vAxes[iObj].TickDir = v; }
		//if(p=="") { vLine[iObj].=v; }	
    }
    // Line //
    if( (tObj == tLine) && (iObj<vLine.size()) ) {
		if(p=="COLOR") { vLine[iObj].Color = v; vLine[iObj].MarkerEdgeColor = v; }
		if(p=="Color") { vLine[iObj].Color = v; }
		if(p=="Marker") { vLine[iObj].Marker = v; }
		if(p=="LineStyle") { vLine[iObj].LineStyle =v; }
		if(p=="MarkerEdgeColor") { vLine[iObj].MarkerEdgeColor = v; }
		if(p=="MarkerFaceColor") { vLine[iObj].MarkerFaceColor = v; }
		//if(p=="") { vLine[iObj].=v; }	
    }
    // Surface //
    if( (tObj == tSurface) && (iObj<vSurface.size()) ) {
		if(p=="COLOR") { vSurface[iObj].EdgeColor = v; }
		if(p=="LineStyle") { vSurface[iObj].LineStyle =v; }
		if(p=="EdgeColor") { vSurface[iObj].EdgeColor = v; }
		if(p=="FaceColor") { vSurface[iObj].FaceColor = v; }
		//if(p=="") { vLine[iObj].=v; }	
    }    
    // Patch //
    if( (tObj == tPatch) && (iObj<vPatch.size()) ) {
		//if(p=="") { vPatch[iObj].=v; }	
		if(p=="COLOR") { vPatch[iObj].EdgeColor = v; }
		if(p=="LineStyle") { vPatch[iObj].LineStyle =v; }
		if(p=="EdgeColor") { vPatch[iObj].EdgeColor = v; }
		if(p=="FaceColor") { vPatch[iObj].FaceColor = v; }
    }
}

template <typename T>
void MatPlot<T>::set(int h,string p,T v) {
    int tObj = h%100;
    int iObj = h/100;
    // Line //
    if( (tObj == tLine) && (iObj<vLine.size()) ) {
		if(p=="LineWidth") { vLine[iObj].LineWidth =v; }	
		if(p=="MarkerSize") { vLine[iObj].MarkerSize =v; }	
    }
    // Surface //
    if( (tObj == tSurface) && (iObj<vSurface.size()) ) {
		if(p=="LineWidth") { vSurface[iObj].LineWidth =v; }
		//if(p=="") { vLine[iObj].=v; }	
    }
    // Patch //
    if( (tObj == tPatch) && (iObj<vPatch.size()) ) {
		if(p=="LineWidth") { vPatch[iObj].LineWidth =v; }
		//if(p=="") { vLine[iObj].=v; }	
    }
    
}

template <typename T>
int MatPlot<T>::gco() {
    return hObj;
}

//void MatPlot<T>::set_line_width(T linewidth) {
//    cl->LineWidth =linewidth;
//}

// Events ///
template <typename T>
void MatPlot<T>::reshape(int w, int h) {
    window_w=w;
    window_h =h;
    if(isInited ==2) {
		glViewport(0,0,w,h);
    }
    //cout <<"window size: "<< w <<" "<<h<<endl;
}

template <typename T>
void MatPlot<T>::mouse(int button, int state, int x, int y ) {
    Layer_mouse(button,state,x, y);
    Axes_mouse(button,state,x, y);	
    display();
}

template <typename T>
void MatPlot<T>::motion(int x, int y ) {
    Axes_motion(x, y);
    //glutPostRedisplay(); 
}

template <typename T>
void MatPlot<T>::passivemotion(int x,int y) {
    xPassive = x;
    yPassive = y;
    //cout <<"Passive: "<<x<<" "<<y<<endl;
}

template <typename T>
void MatPlot<T>::keyboard(unsigned char key, int x, int y) {
    switch(key) {
    case 'q':
		exit(0);
		break;
    case 'p':
		print();
		break;
    }
}

// print ///
template <typename T>
void MatPlot<T>::print() {
    FILE *fp;
    int state = GL2PS_OVERFLOW, buffsize = 0;
    
    fp = fopen("out.eps", "wb");
    printf("Writing 'out.eps'... ");
    while(state == GL2PS_OVERFLOW) {
		buffsize += 2024*2024;
		gl2psBeginPage("test", "gl2ps", NULL, GL2PS_EPS, GL2PS_SIMPLE_SORT, 
				       GL2PS_USE_CURRENT_VIEWPORT, 
				       GL_RGBA, 0, NULL, 0, 0, 0, buffsize, fp, "out.eps");
		display();
		state = gl2psEndPage();
    }
    fclose(fp);
    printf("Done!\n");
}

// Figure ///
template <typename T>
int MatPlot<T>::figure() {
    int h = iFigure*100 + tFigure; 
	hObj = h;

    if(isInited == 0) {	    
		vFigure.push_back(Figure(h));
    }

    if(isInited == 1) {
	
    }

    if(iFigure<vFigure.size()) {cf = &vFigure[iFigure];}
    iFigure++;
    return h;
}

template <typename T>
void MatPlot<T>::display_figure() {
    int tObj;// type of child object
    int iObj;// index of child object
    if(cf->Visible) {
		glEnable(GL_DEPTH_TEST);
		//glDepthFunc(GL_NEVER);

		glClearColor(1, 1, 1, 0.);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		glViewport(0,0, (int)(window_w),(int)(window_h));
		glLoadIdentity();
		//gluOrtho2D( 0.0, 1.0, 0.0, 1.0 );
		glOrtho(0, 1, 0, 1, -1, 3);
	
		for(int i = 0; i<cf->Children.size(); ++i) {
			tObj = cf->Children[i]%100;
			iObj = cf->Children[i]/100;
			//printf("Obj t:%3d i:%3d \n",tObj,iObj);
	    
			if(tObj == tLayer) {
				cfr = &vLayer[iObj];
				if(cfr->Visible) { display_layer();}
			}
		}
		/*
		  for(int i1= 0; i1<cf->Children.size(); ++i1) {
		  tObj1= cf->Children[i1]%100;
			iObj1= cf->Children[i1]/100;		
			if(tObj1== tLayer) {
			cfr = &vLayer[iObj1];
			if(cfr->Visible) { display_layer();}
		
			for(int i2= 0; i2<cfr->Children.size(); ++i2) {
			tObj2= cfr->Children[i2]%100;
			iObj2= cfr->Children[i2]/100;
			if(tObj2== tAxes) {
			ca = &vAxes[iObj2];
			if(ca->Children.size()) {display_axes();}
			}
			}
			}
			}
		*/
	    	
		//Visible(1);

		display_layer2();
	
		glFlush();
		glViewport(0, 0, window_w, window_h);
	
    }
    glutSwapBuffers();
}
    
// Layer ///
template <typename T>
int MatPlot<T>::layer() {
    int h = iLayer*100 + tLayer;
	hObj =h;
    if(isInited == 0) {	    
		vLayer.push_back(Layer(h));
		cf->add_child(h);
    }

    if(isInited == 1) {
	
    }

    if(iLayer<vLayer.size()) {cfr = &vLayer[iLayer];}
    iLayer++;

    axes();//default axes

    return h;
}

template <typename T>
void MatPlot<T>::display_layer() {
    int tObj;// type  of child object
    int iObj;// index of child object	
    for(int i = 0; i < cfr->Children.size(); ++i) {
		tObj = cfr->Children[i] % 100;
		iObj = cfr->Children[i] / 100;
		if(tObj == tAxes) {
			ca = &vAxes[iObj];
			display_axes();
		}
    }//i
}

template <typename T>
void MatPlot<T>::display_layer2() {
    int l,t,w,h,r;
    string s;
    l = 1;
    t = 1;
    w = 20;//button_width;
    h = 20;//button_height;
    r = 3;

    if(xPassive<25) {
		glViewport(0,0, (int)(window_w),(int)(window_h));
		glLoadIdentity();
		gluOrtho2D( 0.0, (int)(window_w), (int)(window_h),0 );
	
		glDisable(GL_LINE_STIPPLE); 
		gl2psDisable(GL2PS_LINE_STIPPLE);

		glLineWidth(2);
		glColor3d(0,0,1);
	
		for(int j = 0; j<vLayer.size(); ++j) {
			//glBegin(GL_LINE_STRIP);
			//glVertex2d(l   ,h*j );
			//glVertex2d(l   ,h*j+h );
			//glVertex2d(l+w ,h*j+h );
			//glVertex2d(l+w ,h*j );
			//glEnd();
			if(vLayer[j].Visible == 1) {// [v]    
				glBegin(GL_LINE_STRIP);
				glVertex2d(l+r   ,h*j+r );
				glVertex2d(l+r   ,h*j+h-r );
				glVertex2d(l+w-r ,h*j+h-r );
				glVertex2d(l+w-r ,h*j+r );
				glVertex2d(l+r   ,h*j+r );
				glEnd();
		
				glBegin(GL_LINE_STRIP);
				glVertex2d(l+9 ,h*j+5 );
				glVertex2d(l+8 ,h*j+15 );
				glVertex2d(l+15  ,h*j+7 );
				glEnd();
				//ptext(5,h*j+h-3,"[v]");
			}

			if(vLayer[j].Visible == 0) {// []
				glBegin(GL_LINE_STRIP);
				glVertex2d(l+r   ,h*j+r );
				glVertex2d(l+r   ,h*j+h-r );
				glVertex2d(l+w-r ,h*j+h-r );
				glVertex2d(l+w-r ,h*j+r );
				glVertex2d(l+r   ,h*j+r );
				glEnd();
		
				//ptext(5,h*j+h-3,"[  ]");
			}
			//ptext(22,h*j+h-3,vLayer[j].layername);
			glColor3f(0, 0,1);
			glRasterPos2f(22.f, h*j+h-6.f);
			s = vLayer[j].layername;
			gl2psText(s.c_str(), "Arial", 12);

			for(int i = 0; i<s.size(); ++i) {
				glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, s[i] );
			}
		}
    }
}

/// events (mouse)
template <typename T>
void MatPlot<T>::Layer_mouse(int button, int state, int x, int y ) {
    int l ,t, w, h;
    int is;
    T dt;
    l = 1;
    t = 1;
    w = 20;//button_width;
    h =20;//button_height;
    //cout <<"button:"<< button<<" state:"<<state<<" x:"<<x<<" y:"<<y<<endl;
	if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ) {// Left Click
		for(int j = 0; j<vLayer.size(); ++j) {
			if((l<x)&&(x<w)&&(h*j<y)&&(y<h*j+h)) {	
				//visibility
				if(vLayer[j].Visible == 1) {is= 0;}
				if(vLayer[j].Visible == 0) {is= 1;}
				vLayer[j].Visible = is;
		
				//T click to focus layer
				time_layer_clicked = clock();
				dt =(T)(time_layer_clicked - time_layer_clicked_last)/CLOCKS_PER_SEC;
				if(dt < 0.05 ) {
					for(int k= 0;k<vLayer.size(); ++k) { vLayer[k].Visible = 0; }
					vLayer[j].Visible = 1;
					//cout <<"!!"<<endl;
				}
				//cout <<"click: "<<time_layer_clicked<<" "<<CLOCKS_PER_SEC<<" "<<dt<<endl;
				time_layer_clicked_last = time_layer_clicked;

				//cout<<"pushed:"<<j<<" Visible:"<<vLayer[j].Visible<<" "<<is<<endl;
				for(int i = 0; i<vAxes.size(); ++i) {
					if(vAxes[i].Parent =vLayer[j].id) {vAxes[i].Visible = is;}
				}
			}
		}
    }	
}

template <typename T>
int MatPlot<T>::layer(string s,int Visible) {
    int h =layer();
    if(isInited == 0) {
	cfr->Visible = Visible;
	cfr->layername = s;
    }
    return h;
}

template <typename T>
int MatPlot<T>::frame(string s,int Visible) {
    int h =layer();
    if(isInited == 0) {
		cfr->Visible =Visible;
		cfr->layername =s;
    }
    return h;
}

// Axes ///
template <typename T>
int MatPlot<T>::gca() {
    return ca->id;	
}

template <typename T>
int MatPlot<T>::axes() {
    int h = iAxes*100 + tAxes; 
	hObj =h;
    //int h =handle(iAxes,tAxes);
    
    if(isInited == 0) {
		cfr->add_child(h);
		vAxes.push_back(Axes<T>(h));
	}

    if(isInited == 1) {
	
    }

    if(iAxes<vAxes.size()) {ca = &vAxes[iAxes];}
    ca->Visible = cfr->Visible;
    iAxes++;
    return h;
}

template <typename T>
void MatPlot<T>::display_axes() {	
	if(ca->Children.size()) {
	    if(ca->View== 0) {//2D
			display_axes_2d();
	    }
	    if(ca->View== 1) {//2D
			display_axes_3d();
	    }	
	}

	if(ca->View==2) {//colorbar
	    display_axes_colorbar();
	}

	// childlen //
	int tObj;// type  of child object
	int iObj;// index of child object
	for(int i = 0; i<ca->Children.size(); ++i) {
	    tObj = ca->Children[i]%100;
	    iObj = ca->Children[i]/100;

	    if(tObj == tLine) {
			cl = &vLine[iObj];
			display_line();
	    }

	    if(tObj == tSurface) {
			cs= &vSurface[iObj];
			display_surface();
	    }

	    if(tObj == tText) {
			ct = &vText[iObj];
			display_text();
	    }

	    if(tObj == tPatch) {
			cp= &vPatch[iObj];
			display_patch();
	    }
	}//i
}
/// display

template <typename T>
void MatPlot<T>::display_axes_2d() {
	
	
	char ctmp[255] = {'\0'};
    T l, b, w, h;//left, bottom, width, height
    T r = 0.01f;

    l = ca->Position[0];
    b = ca->Position[1];
    w = ca->Position[2];
    h = ca->Position[3];
    
    // Viewport Figure (VpF) for drawing axes
    glViewport(0,0, (int)(window_w),(int)(window_h));
    glLoadIdentity();
    gluOrtho2D(0.0, 1.0, 0.0, 1.0);
    
    glDisable(GL_LINE_STIPPLE); 
    gl2psDisable(GL2PS_LINE_STIPPLE);
    
    T x_axis_location = 1, y_axis_location = 1;

    if(ca->XAxisLocation == "top"  ) {
		x_axis_location = 1;
	} 
	else {
		x_axis_location = -1;
	}

    if(ca->YAxisLocation == "right") {
		y_axis_location = 1;
	} 
	else {
		y_axis_location = -1;
	}
 
    int char_w = 6, char_h = 12;
    T offset = 0.01f;
    size_t num_char = 4;
   
    int gridlinestyle;

    int tickdir = 1;//1:in, -1:out
    
    if(ca->Box) {
		// box //  
		glLineWidth(GLfloat(ca->LineWidth));
		gl2psLineWidth(GLfloat(ca->LineWidth));

		if(ca->Selected) {
			glLineWidth(GLfloat(2* ca->LineWidth));
			gl2psLineWidth(GLfloat(ca->LineWidth));
		}

		glColor3f(0.f, 0.f, 0.f);
		glBegin(GL_LINE_LOOP);	    
		glVertex2d(l, b);
		glVertex2d(l+w, b);
		glVertex2d(l+w, b+h);
		glVertex2d(l, b+h);
		glEnd();

		// mouse capture //
		if(ca->Selected) {
			sprintf(ctmp, "Mouse:(%f,%f)", ca->XMouse, ca->YMouse);
			ptext(l, b+h+r, ctmp);
		}

		// Grid //
		gridlinestyle =3;
		if(ca->GridLineStyle =="-"  ) {gridlinestyle = 1;}
		if(ca->GridLineStyle =="- -") {gridlinestyle = 2;}
		if(ca->GridLineStyle ==":"  ) {gridlinestyle = 3;}
		if(ca->GridLineStyle =="-." ) {gridlinestyle = 4;}

		if(ca->XGrid) {
			glLineWidth(GLfloat(ca->LineWidth));
			gl2psLineWidth(GLfloat(ca->LineWidth));
			for(int i = 0; i<ca->XTick.size(); ++i) {
				//cout <<"grid "<<gridlinestyle<<" "<<ca->XTick[i]<<endl;

				if(gridlinestyle == 1) {// -
					glDisable(GL_LINE_STIPPLE); 
					gl2psDisable(GL2PS_LINE_STIPPLE);
				}

				if(gridlinestyle == 2) {//- -
					glEnable(GL_LINE_STIPPLE);
					glLineStipple(1, 0xF0F0);
					gl2psEnable(GL2PS_LINE_STIPPLE);
				}

				if(gridlinestyle == 3) {//:		    
					glEnable(GL_LINE_STIPPLE);
					glLineStipple(1, 0xCCCC);
					gl2psEnable(GL2PS_LINE_STIPPLE);
				}

				if(gridlinestyle == 4) {//-.
					glEnable(GL_LINE_STIPPLE);	   
					glLineStipple(1, 0x087F);
					gl2psEnable(GL2PS_LINE_STIPPLE);
				}

				glBegin(GL_LINE_STRIP);
				glVertex2d(GLdouble(ctx2(ca->XTick[i])), GLdouble(b) );
				glVertex2d(GLdouble(ctx2(ca->XTick[i])), GLdouble(b+h) );//!! TODO
				glEnd();		
			}
		}

		if(ca->YGrid) {	    
			for(int i = 0; i<ca->YTick.size(); ++i) {
				if(gridlinestyle == 1) {// -
					glDisable(GL_LINE_STIPPLE); 
					gl2psDisable(GL2PS_LINE_STIPPLE);
				}

				if(gridlinestyle == 2) {//- -
					glEnable(GL_LINE_STIPPLE);
					glLineStipple(1, 0xF0F0);
					gl2psEnable(GL2PS_LINE_STIPPLE);
				}
				if(gridlinestyle == 3) {//:
					glEnable(GL_LINE_STIPPLE);
					glLineStipple(1, 0xCCCC);
					gl2psEnable(GL2PS_LINE_STIPPLE);
				}
				if(gridlinestyle == 4) {//-.
					glEnable(GL_LINE_STIPPLE);	   
					glLineStipple(1, 0x087F);
					gl2psEnable(GL2PS_LINE_STIPPLE);
				}
				glBegin(GL_LINE_STRIP);
				glVertex2d(GLdouble(l), GLdouble(cty2(ca->YTick[i])) );
				glVertex2d(GLdouble(l+w), GLdouble(cty2(ca->YTick[i])) );
				glEnd();
			}
		}

		// Ticks // 
		if(ca->TickDir == "in") {tickdir = 1;}
		if(ca->TickDir == "out") {tickdir = -1;}
	
		glDisable(GL_LINE_STIPPLE); 
		gl2psDisable(GL2PS_LINE_STIPPLE);
		//TODO precise adjustment of tick location
		// x tick 
		for(int i = 0; i<ca->XTick.size(); ++i) {
			glBegin(GL_LINE_STRIP);
			glVertex2d(GLdouble(ctx2(ca->XTick[i])), GLdouble(b) );
			glVertex2d(GLdouble(ctx2(ca->XTick[i])), GLdouble(b + tickdir*0.01) );//b-0.02*h
			glEnd();
		}

		// x tick label
		if(ca->TickLabel) {
			for(int i = 0; i<ca->XTick.size(); ++i) {
				sprintf(ctmp, "%4.1f", ca->XTick[i]);
				//ptext( ctx2(ca->XTick[i])-0.02, b-0.025,ctmp );//b-0.05*h
				ptext( ctx2(ca->XTick[i])-(T)num_char*char_w/window_w/2.f, b-offset-1.f*char_h/window_h, ctmp );//b-0.05*h
			}
		}

		// y tick 
		for(int i = 0; i<ca->YTick.size(); ++i) {
			glBegin(GL_LINE_STRIP);
			glVertex2d(GLdouble(l), GLdouble(cty2(ca->YTick[i])) );
			glVertex2d(GLdouble(l+tickdir*0.01), GLdouble(cty2(ca->YTick[i])) );		
			glEnd();
		}

		// y tick label
		if(ca->TickLabel) {
			for(int i = 0; i<ca->YTick.size(); ++i) {
				sprintf(ctmp,"%4.1f",ca->YTick[i]);
				//ptext( l-0.05,cty2(ca->YTick[i])-0.0,ctmp );
				ptext( T(l- T(num_char)*char_w/window_w-offset), T(cty2(ca->YTick[i])-0.5*char_h/window_h), ctmp );
			}
		}
    }//Box
        
    //Title
    num_char = ca->Title.length();    
    ptext(l+w/2.0f-(T)num_char*char_w/window_w/2.0f, T(b+h+offset), ca->Title );

    //XLabel
    num_char = ca->XLabel.length();    
    ptext(l+w/2.0f-(T)num_char*char_w/window_w/2.0f, T(b-offset-2.0f*char_h/window_h), ca->XLabel );

    //YLabel
    num_char = ca->YLabel.length();   
    ptext(l, b+h+offset, ca->YLabel );

    // Viewport Axes (VpA) for drawing lines and surfaces
    glViewport((int)(ca->Position[0]*window_w ),
	           (int)(ca->Position[1]*window_h ),
	           (int)(ca->Position[2]*window_w ),
	           (int)(ca->Position[3]*window_h ));
    glLoadIdentity();
    gluOrtho2D( 0.0, 1.0, 0.0, 1.0 );
}

template <typename T>
void MatPlot<T>::display_axes_3d() {
	
    char ctmp[255];
    T l,b,w,h;//left,bottom,width,height
    T r = 0.01f;
    
    l = ca->Position[0];
    b = ca->Position[1];
    w = ca->Position[2];
    h = ca->Position[3];
    
    // Viewport Axes 
    /*
    glViewport((int)(ca->Position[0]*window_w ),
	       (int)(ca->Position[1]*window_h ),
	       (int)(ca->Position[2]*window_w ),
	       (int)(ca->Position[3]*window_h ));
    */

    glViewport((int)(ca->Viewport3d[0]*window_w ),
	       (int)(ca->Viewport3d[1]*window_h ),
	       (int)(ca->Viewport3d[2]*window_w ),
	       (int)(ca->Viewport3d[3]*window_h ));
    
    glLoadIdentity();
    //glOrtho(-1.7, 1.7, -1.7, 1.7, -1.5, 3);
    glOrtho(-1.8, 1.8, -1.8, 1.8, -1.5, 3);
    
    gluLookAt(cos(ca->cta*PI/180)*cos(ca->phi*PI/180), 
	          sin(ca->cta*PI/180)*cos(ca->phi*PI/180), 
	          sin(ca->phi*PI/180),
	          //gluLookAt(CameraPosition[0],CameraPosition[1],CameraPosition[2],
	          ca->CameraTarget[0],  ca->CameraTarget[1],  ca->CameraTarget[2],
	          ca->CameraUpVector[0],ca->CameraUpVector[1],ca->CameraUpVector[2]);
    
    /*
    // x, y,z axes for test
    glColor3f(1,0,0);
    glBegin(GL_LINE_STRIP);
    glVertex3d(0,0,0); glVertex3d(1,0,0);
    glEnd();
    
    glColor3f(0,1,0);
    glBegin(GL_LINE_STRIP);
    glVertex3d(0,0,0); glVertex3d(0,1,0);
    glEnd();
    
    glColor3f(0,0,1);
    glBegin(GL_LINE_STRIP);
    glVertex3d(0,0,0); glVertex3d(0,0,1);
    glEnd();
    
    glColor3f(0,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex3d(-1,-1,0); glVertex3d(1,-1,0); glVertex3d(1,1,0); glVertex3d(-1,1,0);
    glEnd();
    */
   
    int char_w = 6, char_h = 12;
    T offset = T(0.01);
    int num_char = 4;
   
    if(ca->Box) {
		// tick 
		T cta0;
		T r1 = T(1.05); //tick width
		T r2 = T(1.2);
		T r3 = T(1.4);
		int signx, signy;
		cta0 = ca->cta; 
		cta0 = fmod(ca->cta, 360);
		if((  0<= cta0)&&(cta0< 90)) {signx = 1; signy= 1;}
		if(( 90<= cta0)&&(cta0<190)) {signx =-1; signy= 1;}
		if((180<= cta0)&&(cta0<270)) {signx =-1; signy=-1;}
		if((270<= cta0)&&(cta0<360)) {signx = 1; signy=-1;}

		glColor3f(0.f, 0.f, 0.f);

		// axes //
		// x
		glBegin(GL_LINE_STRIP);
		glVertex3d(GLdouble(-1), GLdouble(signy), GLdouble(-1)); 
		glVertex3d(GLdouble(1), GLdouble(signy), GLdouble(-1));
		glEnd();
		// y
		glBegin(GL_LINE_STRIP);
		glVertex3d(GLdouble(signx), -1, -1); 
		glVertex3d(GLdouble(signx), 1, -1);
		glEnd();
		// z
		glBegin(GL_LINE_STRIP);
		glVertex3d(GLdouble(signy), -signx, -1); 
		glVertex3d(GLdouble(signy), -signx, 1);
		glEnd();

		// Tick //
		//x
		for(int i = 0; i<ca->XTick.size(); ++i) {
			glBegin(GL_LINE_STRIP);
			glVertex3d(GLdouble(ct3x(ca->XTick[i])), signy, -1 );
			glVertex3d(GLdouble(ct3x(ca->XTick[i])), signy*r1, -1 );
			glEnd();
		}
		// y
		for(int i = 0; i<ca->YTick.size(); ++i) {
			glBegin(GL_LINE_STRIP);
			glVertex3d( signx   , ct3y(ca->YTick[i]), -1 );
			glVertex3d( signx*r1, ct3y(ca->YTick[i]), -1 );
			glEnd();
		}
		// z
		for(int i = 0; i<ca->ZTick.size(); ++i) {
			glBegin(GL_LINE_STRIP);
			glVertex3d( signy , -signx , ct3z(ca->ZTick[i]) );
			glVertex3d( signy*r1, -signx, ct3z(ca->ZTick[i]) );
			glEnd();
		}
		// Tick Label //
		if(ca->TickLabel) {
			//x
			for(int i = 0; i<ca->XTick.size(); ++i) {
				sprintf(ctmp, "%4.1f", ca->XTick[i]);
				ptext3c(T(ct3x(ca->XTick[i])), T(signy*r2), T(-1), ctmp );
			}
			// y	
			for(int i = 0; i<ca->YTick.size(); ++i) {
				sprintf(ctmp, "%4.1f", ca->YTick[i]);
				ptext3c(T(signx*r2), T(ct3y(ca->YTick[i])), T(-1), ctmp );
			}
			// z
			for(int i = 0; i<ca->ZTick.size(); ++i) {
				sprintf(ctmp, "%4.1f", ca->ZTick[i]);
				ptext3c(T(signy*r2), T(-signx), T(ct3z(ca->ZTick[i])), ctmp );
			}
		}
		// xyz Label //
		ptext3c(0, signy*r3, -1, "x");
		ptext3c(signx*r3, 0, -1, "y");
		ptext3c(T(signy*r3), T(-signx), T(0), "z");

    }//box
}

/// colorbar
template <typename T>
void MatPlot<T>::display_axes_colorbar() {
    char ctmp[100];
	T l, b, w, h;//left,bottom,width,height
	T r = T(0.01);

	l = ca->Position[0];
	b = ca->Position[1];
	w = ca->Position[2];
	h = ca->Position[3];
	    
	// Viewport Figure (VpF) for drawing axes
	glViewport(0,0, (int)(window_w), (int)(window_h));
	glLoadIdentity();
	gluOrtho2D( 0.0, 1.0, 0.0, 1.0 );
	
	glDisable(GL_LINE_STIPPLE); 
	gl2psDisable(GL2PS_LINE_STIPPLE);
	
	if(ca->Box) {
	    // box 	    
	    glLineWidth(GLfloat(ca->LineWidth));
	    gl2psLineWidth(GLfloat(ca->LineWidth));
	    glColor3f(GLfloat(0), GLfloat(0), GLfloat(0));
	    glBegin(GL_LINE_LOOP);	    
	    glVertex2d(GLdouble(l), GLdouble(b));
	    glVertex2d(GLdouble(l+w), GLdouble(b));
	    glVertex2d(GLdouble(l+w), GLdouble(b+h));
	    glVertex2d(GLdouble(l), GLdouble(b+h));
	    glEnd();	    
	    
	    // z tick 
	    for(int i = 0; i<ca->ZTick.size(); ++i) {
		glBegin(GL_LINE_STRIP);
		glVertex2d(GLdouble(l+w), GLdouble(cty2(ca->ZTick[i])) );
		glVertex2d(GLdouble(l+w+0.01), GLdouble(cty2(ca->ZTick[i])) );		
		glEnd();
	    }
	    // z tick number
	    for(int i = 0; i<ca->ZTick.size(); ++i) {
		sprintf(ctmp, "%4.1f",ca->ZTick[i]);
		ptext(T(l+w+0.01), T(cty2(ca->ZTick[i])), ctmp );
	    }
	}//Box

	vector<T> rgb;
	size_t n = cmap.size();
	for(int i = 0; i<n; ++i) {
	    rgb = ca->ColorMap[i];
	    glColor3f(GLfloat(rgb[0]), GLfloat(rgb[1]), GLfloat(rgb[2]));
	    
	    glBegin(GL_QUADS); 
	    glVertex2d(GLdouble(l), GLdouble(b+h*i/n));
	    glVertex2d(GLdouble(l+w), GLdouble(b+h*i/n));
	    glVertex2d(GLdouble(l+w), GLdouble(b+h*(i+1)/n));
	    glVertex2d(GLdouble(l), GLdouble(b+h*(i+1)/n));
	    glEnd();	    
	}
}

/// events (mouse, motion)
template <typename T>
void MatPlot<T>::Axes_mouse(int button, int state, int x, int y ) {

	T X, Y;
	T rx, ry, mx, my; //mouse
	X = (T)x / window_w;
	Y = (T)(window_h-y) / window_h;
	T l, b, w, h;

	if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ) {// Left Click
	    xButtonDown = T(x);
	    yButtonDown = T(y);    
	    // mouse capture axes //
	    if(iAxesSelected>= 0) {

		ca = &vAxes[iAxesSelected];
		if(ca->Visible) {
		    l = ca->Position[0];
		    b = ca->Position[1];
		    w = ca->Position[2];
		    h = ca->Position[3];
		    
		    if( (l<= x)&&(X<=l+w)&&(b<= y)&&(Y<=b+h)&&(ca->Mouse == 1) ) {
				rx = (X-l)/w;
				ry = (Y-b)/h;
				mx = rx*(ca->XLim[1]-ca->XLim[0]) + ca->XLim[0];
				my = ry*(ca->YLim[1]-ca->YLim[0]) + ca->YLim[0];
				ca->XMouse = mx;
				ca->YMouse = my;
		    }
		}		
	    }
	    
	    // axes select //
	    iAxesSelected =-1;
	    
	    for(int i = 0; i<vAxes.size(); ++i) {
			ca = &vAxes[i];
			ca->Selected = 0;
			if(ca->Visible) {
				l = ca->Position[0];
				b = ca->Position[1];
				w = ca->Position[2];
				h = ca->Position[3];
		    
				if( (l<= x)&&(X<=l+w)&&(b<= y)&&(Y<=b+h) ) {
				iAxesSelected = i;
				ca->Selected = 1;
				if(ca->View == 1) {//3D			
					ctaButtonDown = ca->cta;
					phiButtonDown = ca->phi;
				}
				//cout << "axes "<< i << " is selected "<< vAxes[i].selected()  << endl;
				//cout <<"(cta,phi) = "<< ctaButtonDown <<" "<< phiButtonDown << endl;		
				}
			}
	    }
	}// left click
}

template <typename T>
void MatPlot<T>::Axes_motion(int x, int y ) {
	T phi,cta;
	for(int i = 1; i<vAxes.size(); ++i) {
	    ca = &vAxes[i];
	    if((ca->Selected)&&(ca->View== 1)) {
			cta = ctaButtonDown - (T) (x - xButtonDown)*1;
			phi = phiButtonDown + (T) (y - yButtonDown)*1;
			if(phi>= 90) {phi = 90;}
			if(phi<=-90) {phi =-90;}
			if(cta>360) {cta+=-360;}
			if(cta<  0) {cta+= 360;}
		
			ca->phi = phi;
			ca->cta = cta;
			//cout <<"( phi,cta ) = ( "<< vAxes[i].phi <<","<< vAxes[i].cta <<" )"<<endl;
	    }
	}

	//T phi,cta;
	//phi = phiButtonDown +(T) (y - yButtonDown)*0.01;
	//cta = ctaButtonDown -(T) (x - xButtonDown)*0.01;
	//cout <<"( dx,dy ) = ( "<< x-xButtonDown <<","<<y-yButtonDown <<" )"<<endl;
	//cout <<"( phi,cta ) = ( "<< phi <<","<< cta <<" )"<<endl;
	
	//glutPostRedisplay(); 
}


/// subplot
template <typename T>
int MatPlot<T>::subplot(int m,int n,int p) {	
    int h = axes();
    int ix, iy;
    ix = (p-1)%n;
    iy=(m-1) - (p-1)/n;
    ca->Position[0] = (ix+0.13)/n;
    ca->Position[1] = (iy+0.11)/m;
    ca->Position[2] = 0.775/n;
    ca->Position[3] = 0.815/m;

    ca->Viewport3d[0] = 1.0*ix/n;
    ca->Viewport3d[1] = 1.0*iy/m;
    ca->Viewport3d[2] = 1.0/n;
    ca->Viewport3d[3] = 1.0/m;

    return h;
}

/// colorbar
template <typename T>
int MatPlot<T>::colorbar() {
    T l, b, w, h;
    l = ca->Position[0];
    b = ca->Position[1];
    w = ca->Position[2];
    h = ca->Position[3];
    T zmin,zmax;
    zmin = ca->ZLim[0];
    zmax = ca->ZLim[1];
    
    // TODO use in 3D
    
    int hh = axes();
    ca->ColorMap = cmap;
    ca->View = 2;
    ca->Position[0] = l+w+w*0.01;
    ca->Position[1] = b;
    ca->Position[2] = w*0.05;
    ca->Position[3] = h;
    ca->ZLim[0] = zmin;
    ca->ZLim[1] = zmax;
    ca->YLim[0] = zmin;
    ca->YLim[1] = zmax;
    return hh;
}

/// axis
template <typename T>
void MatPlot<T>::axis(T xMin,T xMax,T yMin,T yMax) {	
    if(xMin!= xMax) {
		ca->XLim[0] = xMin;
		ca->XLim[1] = xMax;
		ca->XLimMode = 1;
    }

    if(yMin!= yMax) {
		ca->YLim[0] = yMin;
		ca->YLim[1] = yMax;		
		ca->YLimMode = 1;
    }
    ca->View= 0;//2D
}

template <typename T>
void MatPlot<T>::axis(T xMin,T xMax,T yMin,T yMax,T zMin,T zMax) {
    ca->XLim[0] = xMin; ca->XLim[1] = xMax;
    ca->YLim[0] = yMin; ca->YLim[1] = yMax;
    ca->ZLim[0] = zMin; ca->ZLim[1] = zMax;
    ca->XLimMode = 1;
    ca->YLimMode = 1;
    ca->ZLimMode = 1;
    ca->View = 1;//3D
}

template <typename T>
void MatPlot<T>::axis(string s) {
    if( s=="on"  ) {ca->Box = 1;}
    if( s=="off" ) {ca->Box = 0;}    
}

template <typename T>
void MatPlot<T>::axis(int s) {
    if(s) {ca->Box = 1;}
    else{ ca->Box = 0;}    
}

template <typename T>
void MatPlot<T>::grid(string s) {
    if( s=="on" ) {ca->XGrid = 1; ca->YGrid = 1; ca->ZGrid = 1;}
    if( s=="off") {ca->XGrid = 0; ca->YGrid = 0; ca->ZGrid = 0;}    
}

template <typename T>
void MatPlot<T>::grid(int s) {
    if(s) {ca->XGrid = 1; ca->YGrid = 1; ca->ZGrid = 1;}
    else{ca->XGrid = 0; ca->YGrid = 0; ca->ZGrid = 0;}    
}

template <typename T>
void MatPlot<T>::ticklabel(int s) {
    if(s) {ca->TickLabel = 1;}
    else{ ca->TickLabel = 0;}    
}

template <typename T>
void MatPlot<T>::title(string s) {
    ca->Title = s;
}

template <typename T>
void MatPlot<T>::xlabel(string s) {
    ca->XLabel = s;
}

template <typename T>
void MatPlot<T>::ylabel(string s) {
    ca->YLabel = s;
}

template <typename T>
void MatPlot<T>::mouse_capture(T *xmouse,T *ymouse) {
    ca->Mouse = 1;
    //ca->XMouse = xmouse;
    //ca->YMouse = ymouse;
}

/// Fmax Fmin
template <typename T>
T Fmax(vector<T> x) {
    T maxf =-1e19;
    for(int i = 0; i<x.size(); ++i) {
	if(x[i]>maxf) {maxf = x[i];}
    }
    return maxf;
}

template <typename T>
T Fmin(vector<T> x) {
    T minf = 1e19;
    for(int i = 0; i<x.size(); ++i) {
		if(x[i]<minf) {minf = x[i];}
    }
    return minf;
}

template <typename T>
T Fmax(vector<vector<T>> x) {
    T maxf =-1e19;
    for(int i = 0; i<x.size(); ++i) {
		for(int j = 0; j<x[i].size(); ++j) {
			if(x[i][j] > maxf) {maxf = x[i][j];}
		}
    }
    return maxf;
}

template <typename T>
T Fmin(vector<vector<T>> x) {
    T minf = 1e19;
    for(int i = 0; i<x.size(); ++i) {
		for(int j = 0; j<x[i].size(); ++j) {
			if(x[i][j] < minf) {minf = x[i][j];}
		}
    }
    return minf;
}

/// line
template <typename T>
int MatPlot<T>::line() {
    int h = iLine*100 + tLine; 
	hObj = h;

    if(isInited == 0) {	    
		ca->add_child(h);	    	    
		vLine.push_back(Line<T>(h));
		//al.Parent =gca();
    }

    if(isInited == 1) {
		if(iLine<vLine.size()) { vLine[iLine].reset(); }
    }

    if(iLine<vLine.size()) { cl = &vLine[iLine]; }
    iLine++;    
    return h;
}

template <typename T>
void MatPlot<T>::line_config() {
    size_t n;
    T t; 
    n = cl->XData.size();
    if(ca->XScale == 0) {//linear
		for(int i = 0; i<n; ++i) {
			t = cl->XData[i];	    
			if(ca->xmin>t) {ca->xmin = t;}
			if(ca->xmax<t) {ca->xmax = t;}
		}
    }

    if(ca->XScale == 1) {//log
		for(int i = 0; i<n; ++i) {
			t = cl->XData[i];	    
			if((ca->xmin>t)&&(t>0)) {ca->xmin = t;}
			if(ca->xmax<t) {ca->xmax = t;}
		}
    }

    T y;
    n = cl->YData.size();
    for(int i = 0; i<n; ++i) {
		y = cl->YData[i];	    
		if(ca->ymin>y) { ca->ymin = y; }
		if(ca->ymax<y) { ca->ymax = y; }
    }

    T z;
    n = cl->ZData.size();
    for(int i = 0; i<n; ++i) {
		z = cl->ZData[i];  
		if(ca->zmin>z) {ca->zmin = z;}
		if(ca->zmax<z) {ca->zmax = z;}
    }
}

template <typename T>
int MatPlot<T>::line(vector<T> x,vector<T> y) {
    int h =line();
    if(cfr->Visible) {
		cl->XData = x;
		cl->YData = y;
		line_config();
    }
    return h;
}

template <typename T>
int MatPlot<T>::line(vector<T> x,vector<T> y,vector<T> z) {
    int h =line();
    if(cfr->Visible) {
		cl->XData = x;
		cl->YData = y; 
		cl->ZData = z;
		line_config();
    }
    return h;
}

/// vertex
template <typename T>
int MatPlot<T>::begin() { return line(); }

template <typename T>
void MatPlot<T>::end() {}

template <typename T>
void  MatPlot<T>::vertex(T x,T y) {
    if(cfr->Visible) {
		if(ca->xmin>x) {ca->xmin = x;}
		if(ca->xmax<x) {ca->xmax = x;}
		if(ca->ymin>y) {ca->ymin = y;}
		if(ca->ymax<y) {ca->ymax = y;}	    
		cl->XData.push_back(x);
		cl->YData.push_back(y);
    }
}

/// plot, semilogx, semilogy, loglog
template <typename T>
int MatPlot<T>::plot(vector<T> y) {
    int n = y.size();
    vector<T> x;
    x.resize(n);    
    for(int i = 0; i<n; ++i) {x[i] = 1.0*i/(n-1);}
    return line(x, y);
}

template <typename T>
int MatPlot<T>::plot(vector<T> x,vector<T> y) {	
    return line(x, y);
}

template <typename T>
int MatPlot<T>::plot(valarray<T> x,valarray<T> y) {	
    vector<T> xx, yy;
    for(int i = 0; i<x.size(); ++i) {xx.push_back(x[i]);}
    for(int i = 0; i<y.size(); ++i) {yy.push_back(y[i]);}    
    return line(xx, yy);
}

template <typename T>
int MatPlot<T>::semilogx(vector<T> x,vector<T> y) {
    ca->XScale = 1;
    int h =line();
    if(cfr->Visible) {
		cl->XData = x;
		cl->YData = y; 
		line_config();
    }
    return h;
}

template <typename T>
int MatPlot<T>::semilogy(vector<T> x,vector<T> y) {
    ca->YScale = 1;    
    int h =line();
    if(cfr->Visible) {
		cl->XData = x;
		cl->YData = y; 
		line_config();
    }
    return h;
}


template <typename T>
int MatPlot<T>::loglog(vector<T> x,vector<T> y) {
    ca->XScale = 1;
    ca->YScale = 1;
    int h =line();

    if(cfr->Visible) {
		cl->XData = x;
		cl->YData = y; 
		line_config();
    }
    return h;
}

/// errorbar
template <typename T>
void MatPlot<T>::vertex(T x,T y,T ep,T em) {//for errorbar
	if(ca->xmin>x) {ca->xmin = x;}
	if(ca->xmax<x) {ca->xmax = x;}
	if(ca->ymin>y+ep) {ca->ymin = y+ep;}
	if(ca->ymax<y-em) {ca->ymax = y-em;}
	cl->XData.push_back(x);
	cl->YData.push_back(y);
	cl->YPData.push_back(ep);
	cl->YMData.push_back(em);	
}

template <typename T>
int MatPlot<T>::errorbar(vector<T> x,vector<T> y,vector<T> e) {	
	begin();	
	for(int i = 0; i<x.size(); ++i) { vertex(x[i], y[i], e[i], e[i]); }
	end();
	cl->Errorbar = 1;
	return 0;
}

template <typename T>
int MatPlot<T>::errorbar(vector<T> x,vector<T> y,vector<T> ep,vector<T> em) {
	begin();
	for(int i = 0; i<x.size(); ++i) { vertex(x[i], y[i], ep[i], em[i]); }
	end();
	cl->Errorbar = 1;
	return 0;
}

/// 3D line
template <typename T>
void MatPlot<T>::vertex(T x,T y,T z) {
    if(cfr->Visible) {
		if(ca->xmin>x) {ca->xmin = x;}
		if(ca->xmax<x) {ca->xmax = x;}
		if(ca->ymin>y) {ca->ymin = y;}
		if(ca->ymax<y) {ca->ymax = y;}
		if(ca->zmin>z) {ca->zmin = z;}
		if(ca->zmax<z) {ca->zmax = z;}
		cl->XData.push_back(x);
		cl->YData.push_back(y);
		cl->ZData.push_back(z);
    }
    //if(cl->LineStyle) {//Line
    //glVertex3d(ct3x(x),ct3y(y),ct3z(z));
    //}
} 

template <typename T>   
int MatPlot<T>::plot3(vector<T> x,vector<T> y,vector<T> z) {
    ca->View= 1;
    begin();
    for(int i = 0; i<x.size(); ++i) { vertex(x[i], y[i],z[i]); }
    end();
    return 0;
}

/// display_line
template <typename T>
void MatPlot<T>::display_line() {
    
    T xx, yy;// transformed coordination 
    T r;//marker size
    T rx, ry;
    vector<T> rgb = ColorSpec2RGB(cl->Color);
    glColor3f(GLfloat(rgb[0]), GLfloat(rgb[1]), GLfloat(rgb[2]));
    
    glLineWidth(GLfloat(cl->LineWidth));
    glPointSize(GLfloat(cl->LineWidth));
    gl2psLineWidth(GLfloat(cl->LineWidth));
    gl2psPointSize(GLfloat(cl->LineWidth));
    // 2D //
		if(ca->View== 0) {
		if(cl->LineStyle !="none" ) {// Line //
	    
			if(cl->LineStyle =="-") {
				glDisable(GL_LINE_STIPPLE); 
				gl2psDisable(GL2PS_LINE_STIPPLE);
			}
			if(cl->LineStyle =="- -") {
				glEnable(GL_LINE_STIPPLE);
				glLineStipple(1, 0xF0F0);
				gl2psEnable(GL2PS_LINE_STIPPLE);
			}
			if(cl->LineStyle ==":") {
				glEnable(GL_LINE_STIPPLE);
				glLineStipple(1, 0xCCCC);
				gl2psEnable(GL2PS_LINE_STIPPLE);
			}
			if(cl->LineStyle =="-.") {
				glEnable(GL_LINE_STIPPLE);	   
				glLineStipple(1, 0x087F);
				gl2psEnable(GL2PS_LINE_STIPPLE);
			}
			if(cl->XData.size()) {
				glBegin(GL_LINE_STRIP);
				for(int i = 0; i<cl->XData.size(); ++i) {
					//printf("i:%d %f %f\n",i,xx, yy);
					xx = ctx(cl->XData[i]);
					yy= cty(cl->YData[i]);
					glVertex2d(xx, yy);
				}
				glEnd();
			}
		}
	 
		if(cl->Marker != "none") {// Marker //

			r = cl->MarkerSize/500.0;
			rx = cl->MarkerSize/window_w;
			ry= cl->MarkerSize/window_h;


			glDisable(GL_LINE_STIPPLE); 
			gl2psDisable(GL2PS_LINE_STIPPLE);

			for(int i = 0; i<cl->XData.size(); ++i) {
			xx = ctx(cl->XData[i]);
			yy = cty(cl->YData[i]);		
		
			if(cl->Marker ==".") {//.
				glPointSize(GLfloat(cl->LineWidth));
				glBegin(GL_POINTS);
				glVertex2d(GLdouble(xx), GLdouble(yy));
				glEnd();
			}		
			if(cl->Marker =="+") {//+
				glBegin(GL_LINE_STRIP); 
				glVertex2d(GLdouble(xx-rx), GLdouble(yy));
				glVertex2d(GLdouble(xx+rx), GLdouble(yy));
				glEnd();
				glBegin(GL_LINE_STRIP); 
				glVertex2d(GLdouble(xx), GLdouble(yy-ry));
				glVertex2d(GLdouble(xx), GLdouble(yy+ry));
				glEnd();
			}
			if(cl->Marker =="x") {//x
				glBegin(GL_LINE_STRIP);
				glVertex2d(GLdouble(xx-rx), yy-ry);
				glVertex2d(GLdouble(xx+rx), yy+ry);
				glEnd();
				glBegin(GL_LINE_STRIP);
				glVertex2d(GLdouble(xx+rx), yy-ry);
				glVertex2d(GLdouble(xx-rx), yy+ry);
				glEnd();
			}
			if(cl->Marker =="d") {//d diamond
				glBegin(GL_LINE_LOOP);
				glVertex2d(GLdouble(xx),   yy+ry);
				glVertex2d(GLdouble(xx+rx), yy);
				glVertex2d(GLdouble(xx),   yy-ry);
				glVertex2d(GLdouble(xx-rx), yy);
				glEnd();
			}
			if(cl->Marker =="^") {//^
				glBegin(GL_LINE_LOOP);
				glVertex2d(GLdouble(xx),   yy+ry);
				glVertex2d(GLdouble(xx+rx), yy-ry);
				glVertex2d(GLdouble(xx-rx), yy-ry);
				glEnd();
			}
			if(cl->Marker =="v") {//v
				glBegin(GL_LINE_LOOP);
				glVertex2d(GLdouble(xx),   yy-ry);
				glVertex2d(GLdouble(xx+rx), yy+ry);
				glVertex2d(GLdouble(xx-rx), yy+ry);
				glEnd();
			}
			if(cl->Marker =="o") {//o
				glBegin(GL_LINE_LOOP);
				for(int i = 0; i<20; i++) {
				glVertex2d(GLdouble(xx+rx*cos(2*PI*(T)i/(T)(20))),
					   GLdouble(yy+ry*sin(2*PI*(T)i/(T)(20))));
				}
				glEnd();
			}
			if(cl->Marker =="*") {//*
				glBegin(GL_LINE_STRIP); 
				glVertex2d(GLdouble(xx-rx), yy);
				glVertex2d(GLdouble(xx+rx), yy);
				glEnd();
				glBegin(GL_LINE_STRIP); 
				glVertex2d(GLdouble(xx), yy-ry);
				glVertex2d(GLdouble(xx), yy+ry);
				glEnd();
				glBegin(GL_LINE_STRIP); 
				glVertex2d(GLdouble(xx-rx), yy-ry);
				glVertex2d(GLdouble(xx+rx), yy+ry);
				glEnd();
				glBegin(GL_LINE_STRIP); 
				glVertex2d(GLdouble(xx+rx), yy-ry);
				glVertex2d(GLdouble(xx-rx), yy+ry);
				glEnd();

			}
			if(cl->Marker =="s") {//s :squire
				glBegin(GL_LINE_LOOP);
				glVertex2d(GLdouble(xx-rx), yy-ry);
				glVertex2d(GLdouble(xx-rx), yy+ry);
				glVertex2d(GLdouble(xx+rx), yy+ry);
				glVertex2d(GLdouble(xx+rx), yy-ry);
				glEnd();
			}
			//TODO use switch
			}//i
		}// Marker

	
		if(cl->Errorbar) {// Errorbar //
			T xx, yy, yyp, yym;// transformed coordination 
	
			glDisable(GL_LINE_STIPPLE); 
			gl2psDisable(GL2PS_LINE_STIPPLE);
			//r = cl->MarkerSize/500;

			for(int i = 0; i<cl->XData.size(); ++i) {
				xx = ctx(cl->XData[i]);
				yy = cty(cl->YData[i]);
				yyp = cty(cl->YData[i] + cl->YPData[i]);
				yym = cty(cl->YData[i] - cl->YMData[i]);

				glBegin(GL_LINE_STRIP);
				glVertex2d(GLdouble(xx), GLdouble(yyp));
				glVertex2d(GLdouble(xx), GLdouble(yym));
				glEnd();
				glBegin(GL_LINE_STRIP);
				glVertex2d(GLdouble(xx-rx), GLdouble(yy));
				glVertex2d(GLdouble(xx+rx), GLdouble(yy));
				glEnd();
				glBegin(GL_LINE_STRIP);
				glVertex2d(GLdouble(xx-rx), GLdouble(yyp));
				glVertex2d(GLdouble(xx+rx), GLdouble(yyp));
				glEnd();
				glBegin(GL_LINE_STRIP);
				glVertex2d(GLdouble(xx-rx), GLdouble(yym));
				glVertex2d(GLdouble(xx+rx), GLdouble(yym));
				glEnd();
			}//i
		}//Errorbar
		//TODO:selection of error bar type 
	}//2D

	// 3D //
	if(ca->View== 1) {
		glBegin(GL_LINE_STRIP);
		for(int i = 0; i<cl->XData.size(); ++i) {
			glVertex3d(GLdouble(ct3x(cl->XData[i])),
					GLdouble(ct3y(cl->YData[i])),
					GLdouble(ct3z(cl->ZData[i])));
		}
		glEnd();
	}
}

// Surface ///
template <typename T>
int MatPlot<T>::surface() {
    int h = iSurface*100 + tSurface; hObj =h;

    if(isInited == 0) {
		ca->add_child(h);	   
		vSurface.push_back(Surface(h));
		//as.Parent =gca();
    }
    if(isInited == 1) {
	
    }
    if(iSurface<vSurface.size()) {cs= &vSurface[iSurface];}
    iSurface++;
    return h;    
}

template <typename T>
void MatPlot<T>::surface_config() {

    // check data size    
    int nzi,nzj;
    nzi = cs->ZData.size();
    if(nzi) {nzj = cs->ZData[0].size(); }
    
    int nci,ncj;
    nci = cs->CDataIndex.size();
    if(nci) {ncj = cs->CDataIndex[0].size();}

    // generate x and y data
    int nx = 0,ny= 0;
    if(nzi) {ny=nzi; nx =nzj;}
    if(nci) {ny=nci; nx =ncj;}
    
    //printf("%s %s:%d: %d %d %d %d \n", __func__, __FILE__, __LINE__,nzi,nci,nx,ny);
    if(cs->XData.size()== 0) {
		cs->XData.resize(1); 
		cs->XData[0] = linspace(1.0,(T)nx,nx);
    }
    if(cs->YData.size()== 0) {
		cs->YData.resize(1); 
		cs->YData[0] = linspace(1.0,(T)ny,ny);
    }
    
    // config data range
    ca->xmax = max(Fmax(cs->XData),ca->xmax);
    ca->xmin = min(Fmin(cs->XData),ca->xmin);
    ca->ymax = max(Fmax(cs->YData),ca->ymax);
    ca->ymin = min(Fmin(cs->YData),ca->ymin);
    ca->zmax = max(Fmax(cs->ZData),ca->zmax );
    ca->zmin = min(Fmin(cs->ZData),ca->zmin );

    // set CLim 
    //note: first called surface effects CLim
    if(ca->CLim[0] == ca->CLim[1]) {
		ca->CLim[0] = Fmin(cs->CDataIndex);
		ca->CLim[1] = Fmax(cs->CDataIndex);
    }

    // CData !!!
    if( (cs->CData.size()== 0) && (cs->CDataIndex.size()) ) {
	vector<T> rgb;
	//vector<vector<vector<T>>> cdata(ny,nx);
	vector<vector<vector<T>>> cdata(ny);
	
	for(int i = 0; i<ny; ++i) {
	    cdata[i].resize(nx);
	    for(int j = 0; j<nx; ++j) {
			rgb = map2color(cs->CDataIndex[i][j], ca->CLim[0], ca->CLim[1]);
			cdata[i][j] = rgb;
	    }
	}
	cs->CData = cdata;
    }

    // contour plot
    if(cs->V.size()== 0) {
		if(cs->NContour<1) {
			cs->NContour = 10;		    
		}
		cs->V = linspace(Fmin(cs->ZData),Fmax(cs->ZData),cs->NContour);	
    }
}

/// create surface
template <typename T>
int MatPlot<T>::surface(vector<vector<T>> Z) {    
    int h =surface();
    ca->View= 1;
    cs->type = 1;
    cs->ZData =Z;
    cs->CDataIndex =Z;
    cs->CData.clear();
    surface_config();
    return h;
}

template <typename T>
int MatPlot<T>::surface(vector<vector<T>> Z, vector<vector<T>> C) {    
    int h =surface();
    ca->View = 1;
    cs->type = 1;
    cs->ZData = Z;
    cs->CDataIndex = C;
    cs->CData.clear();
    surface_config();
    return h;
}

template <typename T>
int MatPlot<T>::surface(vector<vector<T>> Z, vector<vector<vector<T>>> C) {    
    int h = surface();
    ca->View= 1;
    cs->type = 1;
    cs->ZData =Z;
    cs->CDataIndex.clear();
    cs->CData = C;
    surface_config();
    return h;
}

template <typename T>
int MatPlot<T>::surface(vector<T> x, vector<T> y, vector<vector<T>> Z) {    
    int h =surface();
    ca->View= 1;
    cs->type = 1;
    cs->XData.resize(1); cs->XData[0] = x;
    cs->YData.resize(1); cs->YData[0] = y;
    cs->ZData = Z;
    cs->CDataIndex = Z;
    cs->CData.clear();
    surface_config();
    return h;
}

template <typename T>
int MatPlot<T>::surface(vector<T> x,vector<T> y,vector<vector<T>> Z,vector<vector<T>> C) {    
    int h =surface();
    ca->View= 1;
    cs->type = 1;
    cs->XData.resize(1); cs->XData[0] = x;
    cs->YData.resize(1); cs->YData[0] = y;
    cs->ZData = Z;
    cs->CDataIndex = C;
    cs->CData.clear();
    surface_config();
    return h;
}

template <typename T>
int MatPlot<T>::surface(vector<T> x,vector<T> y,vector<vector<T>> Z,vector<vector<vector<T>>> C) {    
    int h =surface();
    ca->View= 1;
    cs->type = 1;
    cs->XData.resize(1); cs->XData[0] = x;
    cs->YData.resize(1); cs->YData[0] = y;
    cs->ZData = Z;
    cs->CDataIndex.clear();
    cs->CData = C;
    surface_config();
    return h;
}

template <typename T>
int MatPlot<T>::surface(vector<vector<T>> X,vector<vector<T>> Y,vector<vector<T>> Z) {
    int h =surface();
    ca->View= 1;
    cs->type = 1;
    cs->XData = X;
    cs->YData = Y;
    cs->ZData = Z;
    cs->CDataIndex = Z;
    cs->CData.clear();
    surface_config();
    return h;
}

template <typename T>
int MatPlot<T>::surface(vector<vector<T>> X,vector<vector<T>> Y,vector<vector<T>> Z,vector<vector<T>> C) {
    int h =surface();
    ca->View= 1;
    cs->type = 1;
    cs->XData = X;
    cs->YData = Y;
    cs->ZData = Z;
    cs->CDataIndex = C;
    cs->CData.clear();

    surface_config();
    return h;
}

template <typename T>
int MatPlot<T>::surface(vector<vector<T>> X,vector<vector<T>> Y,vector<vector<T>> Z,vector<vector<vector<T>>> C) {
    int h =surface();
    ca->View= 1;
    cs->type = 1;
    cs->XData = X;
    cs->YData = Y
		;
    cs->ZData = Z;
    cs->CDataIndex.clear();
    cs->CData = C;
    
    surface_config();
    return h;
}

/// surf
template <typename T>
int MatPlot<T>::surf(vector<T> x, vector<T> y, vector<vector<T>> Z) {
    int h =surface();
    ca->View= 1;
    cs->type = 1;
    cs->XData.resize(1); cs->XData[0] = x;
    cs->YData.resize(1); cs->YData[0] = y;
    cs->ZData =Z;
    cs->CDataIndex =Z;
    cs->CData.clear();    
    cs->EdgeColor = "k";
    cs->FaceColor = "flat";
    surface_config();
    return h;
}

/// create pcolor
template <typename T>
int MatPlot<T>::pcolor(vector<vector<T>> C) {
    
    int h; h =surface();

    cs->type = 0;
    cs->XData.clear();
    cs->YData.clear();
    cs->ZData.clear();
    cs->CDataIndex = C;
    cs->CData.clear();

    surface_config();

    return h;
}

template <typename T>
int MatPlot<T>::pcolor(vector<vector<vector<T>>> C) {
    int h =surface();
    cs->type = 0;
    cs->XData.clear();
    cs->YData.clear();
    cs->ZData.clear();
    cs->CDataIndex.clear();
    cs->CData = C;    
    surface_config();
    return h;
}

template <typename T>
int MatPlot<T>::pcolor(vector<T> x, vector<T> y, vector<vector<T>> C) {
    int h =surface();
    cs->type = 0;
    cs->XData.resize(1); cs->XData[0] = x;
    cs->YData.resize(1); cs->YData[0] = y;
    cs->ZData.clear();
    cs->CDataIndex = C;
    cs->CData.clear();
    surface_config();
    return h;
}

template <typename T>
int MatPlot<T>::pcolor(vector<T> x, vector<T> y, vector<vector<vector<T>>> C) {
    int h =surface();
    cs->type = 0;
    cs->XData.resize(1); cs->XData[0] = x;
    cs->YData.resize(1); cs->YData[0] = y;
    cs->ZData.clear();
    cs->CDataIndex.clear();
    cs->CData = C;
    surface_config();
    return h;
}

template <typename T>
int MatPlot<T>::pcolor(vector<vector<T>> X,vector<vector<T>> Y,vector<vector<T>> C) {
    int h =surface();
    cs->type = 0; 
    cs->XData = X;
    cs->YData = Y;	
    cs->ZData.clear();
    cs->CDataIndex = C;
    cs->CData.clear();    
    surface_config();
    return h;
}

template <typename T>
int MatPlot<T>::pcolor(vector<vector<T>> X,vector<vector<T>> Y,vector<vector<vector<T>>> C) {
    int h =surface();
    cs->type = 0; 
    cs->XData = X;
    cs->YData = Y;	
    cs->ZData.clear();
    cs->CDataIndex.clear();
    cs->CData = C;    
    surface_config();
    return h;
}

/// mesh
template <typename T>
int MatPlot<T>::mesh(vector<T> x, vector<T> y, vector<vector<T>> Z) {
    int h =surface();
    ca->View= 1;
    cs->type = 1;
    cs->XData.resize(1); cs->XData[0] = x;
    cs->YData.resize(1); cs->YData[0] = y;
    cs->ZData =Z;
    cs->CDataIndex =Z;
    cs->CData.clear();    
    cs->EdgeColor = "k";
    cs->FaceColor = "w";
    surface_config();
    return h;
}

/// contour
template <typename T>
int MatPlot<T>::contour(vector<vector<T>> Z) {
    int h =surface();
    cs->type =3;
    cs->XData.clear();
    cs->YData.clear();
    cs->ZData =Z;
    cs->NContour = 0;
    cs->V.clear();      

    surface_config();
    return h;
}

template <typename T>
int MatPlot<T>::contour(vector<vector<T>> Z,int n) {
    int h =surface();
    cs->type =3;
    cs->XData.clear();
    cs->YData.clear();
    cs->ZData =Z;
    cs->NContour = n; 
    cs->V.clear();
   
    surface_config();
    return h;
}

template <typename T>
int MatPlot<T>::contour(vector<vector<T>> Z, vector<T> v) {
    int h =surface();
    cs->type =3;
    cs->XData.clear();
    cs->YData.clear();
    cs->ZData =Z;
    cs->NContour = v.size();
    cs->V=v;
    
    surface_config();
    return h;
}

template <typename T>
int MatPlot<T>::contour(vector<T> x,vector<T> y,vector<vector<T>> Z) {
    int h =surface();
    cs->type =3;
    cs->XData.resize(1); cs->XData[0] = x;
    cs->YData.resize(1); cs->YData[0] = y;	
    cs->ZData =Z;
    cs->NContour = 0;
    cs->V.clear();      

    surface_config();
    return h;
}

template <typename T>
int MatPlot<T>::contour(vector<T> x,vector<T> y,vector<vector<T>> Z,int n) {
    int h =surface();
    cs->type =3;
    cs->XData.resize(1); cs->XData[0] = x;
    cs->YData.resize(1); cs->YData[0] = y;	
    cs->ZData =Z;
    cs->NContour = n; 
    cs->V.clear();
   
    surface_config();
    return h;
}

template <typename T>
int MatPlot<T>::contour(vector<T> x, vector<T> y, vector<vector<T>> Z, vector<T> v) {
    int h =surface();
    cs->type =3;
    cs->XData.resize(1); cs->XData[0] = x;
    cs->YData.resize(1); cs->YData[0] = y;	
    cs->ZData =Z;
    cs->NContour = v.size();
    cs->V=v;
    
    surface_config();
    return h;
}

/// display
template <typename T> 
void MatPlot<T>::display_surface() {   
    //if(cs->type == 0) { display_pcolor(); }
    if(cs->type == 0) { display_surface_2d(); }
    if(cs->type == 1) { display_surface_3d(); }
    if(cs->type ==2) { display_surface_3d(); }
    if(cs->type ==3) { display_contour(); }
    
}

template <typename T>
void MatPlot<T>::display_surface_2d() {
    size_t nxi, nxj, nyi, nyj, nzi, nzj;
    vector<T> rgb;
    nzi = cs->ZData.size(); if(nzi) {nzj = cs->ZData[0].size();}    
    nxi = cs->XData.size(); if(nxi) {nxj = cs->XData[0].size();}
    nyi = cs->YData.size(); if(nyi) {nyj = cs->YData[0].size();}

    //printf("%s %s:%d  (%d %d) (%d %d) (%d %d) \n", __func__, __FILE__, __LINE__,nxi,nxj,nyi,nyj,nzi,nzj);

    // (Z) // do not use
		if(nxi == 0) {
		printf("%s:%d\n", __FILE__, __LINE__);
		// Edge
		if(cs->EdgeColor != "none") {	    
			glLineWidth(GLfloat(cs->LineWidth));
			rgb = ColorSpec2RGB(cs->EdgeColor);
			glColor3d(GLfloat(rgb[0]), GLfloat(rgb[1]), GLfloat(rgb[2]));
	    
			for(int i = 0; i<nzi; ++i) {		
				glBegin(GL_LINE_STRIP); //TODO add more style		    
				for(int j = 0; j<nzj; ++j) { 
					glVertex2d(ctx(ca->XLim[0]+(ca->XLim[1]-ca->XLim[0])*(T)(j)/(nzj-1)),
						   cty(ca->YLim[0]+(ca->YLim[1]-ca->YLim[0])*(T)(i)/(nzi-1)));
				}
				glEnd();
			}
			/*
			for(int j = 0; j<nzj; ++j) { 	    		
				glBegin(GL_LINE_STRIP); 		    
				for(int i = 0; i<nzi; ++i) {
					glVertex2d(ctx(ca->XLim[0]+(ca->XLim[1]-ca->XLim[0])*(T)(j)/(nzj-1)),
						   cty(ca->YLim[0]+(ca->YLim[1]-ca->YLim[0])*(T)(i)/(nzi-1)));
				}
				glEnd();
			}
			*/
		}
		// Face 
		if(cs->FaceColor != "none") {
			for(int i = 0; i<nzi-1; ++i) {
				for(int j = 0; j<nzj-1; ++j) {
		    
					rgb = ColorSpec2RGB(cs->FaceColor);		
					if(cs->FaceColor =="flat") { rgb = cs->CData[i][j]; }
					glColor3f(GLfloat(rgb[0]), GLfloat(rgb[1]), GLfloat(rgb[2]));

					glBegin(GL_QUADS);
					glVertex2d(GLfloat(ctx(ca->XLim[0]+(ca->XLim[1]-ca->XLim[0])*(T)(j  )/(T)(nzj-1))),
						   GLfloat(cty(ca->YLim[0]+(ca->YLim[1]-ca->YLim[0])*(T)(i  )/(T)(nzi-1))));
					glVertex2d(GLfloat(ctx(ca->XLim[0]+(ca->XLim[1]-ca->XLim[0])*(T)(j+1)/(T)(nzj-1))),
						   GLfloat(cty(ca->YLim[0]+(ca->YLim[1]-ca->YLim[0])*(T)(i  )/(T)(nzi-1))));
					glVertex2d(GLfloat(ctx(ca->XLim[0]+(ca->XLim[1]-ca->XLim[0])*(T)(j+1)/(T)(nzj-1))),
						   GLfloat(cty(ca->YLim[0]+(ca->YLim[1]-ca->YLim[0])*(T)(i+1)/(T)(nzi-1))));
					glVertex2d(GLfloat(ctx(ca->XLim[0]+(ca->XLim[1]-ca->XLim[0])*(T)(j  )/(T)(nzj-1))),
						   GLfloat(cty(ca->YLim[0]+(ca->YLim[1]-ca->YLim[0])*(T)(i+1)/(T)(nzi-1))));
					glEnd();
				}		
			}
		}//Face
    }

    // (x, y,Z) //
		if(nxi == 1) {
		//printf("%s %s:%d  %d %d \n", __func__, __FILE__, __LINE__,nxj,nyj);
		// Edge
		if(cs->EdgeColor != "none") {	    
	    
			glLineWidth(GLfloat(cs->LineWidth));
			rgb = ColorSpec2RGB(cs->EdgeColor);
			glColor3d(rgb[0],rgb[1],rgb[2]);
	    
			for(int i = 0; i<nyj; ++i) {	
				glBegin(GL_LINE_STRIP); //TODO add more style		    
				//for(int j = 0; j<nxj; ++j) {
					//glVertex2d(ctx(cs->XData[0][j]),cty(cs->YData[0][i]));
				//}
				glVertex2d(GLdouble(ctx(cs->XData[0][0])), GLdouble(cty(cs->YData[0][i])) );
				glVertex2d(GLdouble(ctx(cs->XData[0][nxj-1])), GLdouble(cty(cs->YData[0][i])) );
		
				glEnd();
			}

			for(int j = 0; j<nxj; ++j) { 	    		
				glBegin(GL_LINE_STRIP); 		    
				//for(int i = 0; i<nxj; ++i) {
				//glVertex2d(ctx(cs->XData[0][j]), cty(cs->YData[0][i]));
				//}
				glVertex2d(GLdouble(ctx(cs->XData[0][j])), GLdouble(cty(cs->YData[0][0])));
				glVertex2d(GLdouble(ctx(cs->XData[0][j])), GLdouble(cty(cs->YData[0][nyj-1])));
				glEnd();
			}
		}  
		// Face
		if(cs->FaceColor != "none") {
			for(int i = 0; i<nyj-1; ++i) {	    	     	    
				for(int j = 0; j<nxj-1; ++j) { 
					// color 
					rgb = ColorSpec2RGB(cs->FaceColor);
					if(cs->FaceColor =="flat") { rgb = cs->CData[i][j]; }
					glColor3f(GLfloat(rgb[0]), GLfloat(rgb[1]), GLfloat(rgb[2]));

					glBegin(GL_QUADS);
					glVertex2d(GLdouble(ctx(cs->XData[0][j])),
						   GLdouble(cty(cs->YData[0][i])) ); 
					glVertex2d(GLdouble(ctx(cs->XData[0][j])),
						   GLdouble(cty(cs->YData[0][i+1])) ); 
					glVertex2d(GLdouble(ctx(cs->XData[0][j+1])),
						   GLdouble(cty(cs->YData[0][i+1])) );
					glVertex2d(GLdouble(ctx(cs->XData[0][j+1])),
						   GLdouble(cty(cs->YData[0][i])) );		
					glEnd();
				}
			}
		}
    }//nxi == 1
    
    // (X, y,C) //
    if(nxi>1) {
		// Edge
		//printf("%s %s:%d\n", __func__, __FILE__, __LINE__);
		if(cs->EdgeColor != "none") {
			glLineWidth(GLfloat(cs->LineWidth));
			rgb = ColorSpec2RGB(cs->EdgeColor);
			glColor3d(GLdouble(rgb[0]), GLdouble(rgb[1]), GLdouble(rgb[2]));
	    
			for(int i = 0; i<nxi; ++i) {		
				glBegin(GL_LINE_STRIP); //TODO add more style		    
				for(int j = 0; j<nxj; ++j) { 
					glVertex2d(GLdouble(ctx(cs->XData[i][j])),
						   GLdouble(cty(cs->YData[i][j])));
				}
				glEnd();
			}
			for(int j = 0; j<nxi; ++j) { 	    		
				glBegin(GL_LINE_STRIP); 		    
				for(int i = 0; i<nxj; ++i) {
					glVertex2d(GLdouble(ctx(cs->XData[i][j])),
						   GLdouble(cty(cs->YData[i][j])));
				}
				glEnd();
			}
		}  
		// Face
		if(cs->FaceColor != "none") {
			for(int i = 0; i<nxi-1; ++i) {	    	     	    
			for(int j = 0; j<nxj-1; ++j) { 
				// color 
				rgb = ColorSpec2RGB(cs->FaceColor);		
				if(cs->FaceColor =="flat") {rgb = cs->CData[i][j]; }
				glColor3f(GLfloat(rgb[0]), GLfloat(rgb[1]), GLfloat(rgb[2]));
		    
				glBegin(GL_QUADS);
				glVertex2d(GLdouble(ctx(cs->XData[i  ][j])),
					   GLdouble(cty(cs->YData[i  ][j])) ); 
				glVertex2d(GLdouble(ctx(cs->XData[i  ][j+1])),
					   GLdouble(cty(cs->YData[i  ][j+1])) ); 
				glVertex2d(GLdouble(ctx(cs->XData[i+1][j+1])),
					   GLdouble(cty(cs->YData[i+1][j+1])) );
				glVertex2d(GLdouble(ctx(cs->XData[i+1][j])),
					   GLdouble(cty(cs->YData[i+1][j])) );		
				glEnd();
			}
			}
		}
    }
}

/// display 3d
template <typename T>
void MatPlot<T>::display_surface_3d() {
    vector<T> rgb;
    size_t ny = cs->ZData.size();
    size_t nx = cs->ZData[0].size();
    
		if(cs->XData.size()== 1) {
		//(x, y,Z);
		//Face
		if(cs->FaceColor != "none") {
			for(int i = 0; i<ny-1; ++i) {	    	     	    
				for(int j = 0; j<nx-1; ++j) { 
					rgb = ColorSpec2RGB(cs->FaceColor);		
					glColor3d(rgb[0], rgb[1], rgb[2]);
					if(cs->FaceColor =="flat") {
						rgb = cs->CData[i][j];
						glColor3f(GLfloat(rgb[0]), GLfloat(rgb[1]), GLfloat(rgb[2]));
					}
					glBegin(GL_TRIANGLES);
					glVertex3d(GLdouble(ct3x(cs->XData[0][j])),
						   GLdouble(ct3y(cs->YData[0][i])),
						   GLdouble(ct3z(cs->ZData[i][j])) ); 
					glVertex3d(GLdouble(ct3x(cs->XData[0][j])),
						   GLdouble(ct3y(cs->YData[0][i+1])),
						   GLdouble(ct3z(cs->ZData[i+1][j])) ); 
					glVertex3d(GLdouble(ct3x(cs->XData[0][j+1])),
						   GLdouble(ct3y(cs->YData[0][i+1])),
						   GLdouble(ct3z(cs->ZData[i+1][j+1])) ); 
					glEnd();
					glBegin(GL_TRIANGLES);
					glVertex3d(GLdouble(ct3x(cs->XData[0][j])),
						   GLdouble(ct3y(cs->YData[0][i])),
						   GLdouble(ct3z(cs->ZData[i][j])) ); 
					glVertex3d(GLdouble(ct3x(cs->XData[0][j+1])),
						   GLdouble(ct3y(cs->YData[0][i])),
						   GLdouble(ct3z(cs->ZData[i][j+1])) ); 
					glVertex3d(GLdouble(ct3x(cs->XData[0][j+1])),
						   GLdouble(ct3y(cs->YData[0][i+1])),
						   GLdouble(ct3z(cs->ZData[i+1][j+1])) ); 
					glEnd();
				}
			}
		}
		// Edge
		if(cs->EdgeColor != "none") {
			glLineWidth(GLfloat(cs->LineWidth));
			rgb = ColorSpec2RGB(cs->EdgeColor);
			glColor3d(GLdouble(rgb[0]), GLdouble(rgb[1]), GLdouble(rgb[2]));
	    
			for(int i = 0; i<ny; ++i) {		
				glBegin(GL_LINE_STRIP); 		    
				for(int j = 0; j<nx; ++j) { 
					glVertex3d(GLdouble(ct3x(cs->XData[0][j])),
						   GLdouble(ct3y(cs->YData[0][i])),
						   GLdouble(ct3z(cs->ZData[i][j])) ); 
				}

				glEnd();
			}

			for(int j = 0; j<nx; ++j) { 	    		
				glBegin(GL_LINE_STRIP); 		    
				for(int i = 0; i<ny; ++i) {
					glVertex3d(GLdouble(ct3x(cs->XData[0][j])),
						   GLdouble(ct3y(cs->YData[0][i])),
						   GLdouble(ct3z(cs->ZData[i][j])) ); 
				}
				glEnd();
			}
		}    
    }// (x, y,Z)
    // (X, y,Z) //
    if(cs->XData.size()>1) {

		//Face
		if(cs->FaceColor != "none") {
			for(int i = 0; i<ny-1; ++i) {	    	     	    
				for(int j = 0; j<nx-1; ++j) { 
					rgb = ColorSpec2RGB(cs->FaceColor);		
					glColor3d(rgb[0], rgb[1], rgb[2]);
					if(cs->FaceColor =="flat") {
						rgb = cs->CData[i][j];
						glColor3f(GLfloat(rgb[0]), GLfloat(rgb[1]), GLfloat(rgb[2]));
					}

					glBegin(GL_TRIANGLES);
					glVertex3d(GLdouble(ct3x(cs->XData[i][j])),
						   GLdouble(ct3y(cs->YData[i][j])),
						   GLdouble(ct3z(cs->ZData[i][j])) ); 
					glVertex3d(GLdouble(ct3x(cs->XData[i][j+1])),
						   GLdouble(ct3y(cs->YData[i][j+1])),
						   GLdouble(ct3z(cs->ZData[i][j+1])) ); 
					glVertex3d(GLdouble(ct3x(cs->XData[i+1][j+1])),
						   GLdouble(ct3y(cs->YData[i+1][j+1])),
						   GLdouble(ct3z(cs->ZData[i+1][j+1])) ); 
					glEnd();

					glBegin(GL_TRIANGLES);
					glVertex3d(GLdouble(ct3x(cs->XData[i][j])),
						   GLdouble(ct3y(cs->YData[i][j])),
						   GLdouble(ct3z(cs->ZData[i][j])) ); 
					glVertex3d(GLdouble(ct3x(cs->XData[i+1][j])),
						   GLdouble(ct3y(cs->YData[i+1][j])),
						   GLdouble(ct3z(cs->ZData[i+1][j])) ); 
					glVertex3d(GLdouble(ct3x(cs->XData[i+1][j+1])),
						   GLdouble(ct3y(cs->YData[i+1][j+1])),
						   GLdouble(ct3z(cs->ZData[i+1][j+1])) ); 
					glEnd();
				}
			}
		}
		// Edge
		if(cs->EdgeColor != "none") {
			glLineWidth(GLfloat(cs->LineWidth));
			rgb = ColorSpec2RGB(cs->EdgeColor);
			glColor3d(GLfloat(rgb[0]), GLfloat(rgb[1]), rgb[2]);
	
			for(int i = 0; i<ny; ++i) {		
				glBegin(GL_LINE_STRIP); 		    
				for(int j = 0; j<nx; ++j) { 
					glVertex3d(GLdouble(ct3x(cs->XData[i][j])),
						   GLdouble(ct3y(cs->YData[i][j])),
						   GLdouble(ct3z(cs->ZData[i][j])) ); 
				}
				glEnd();
			}
			for(int j = 0; j<nx; ++j) { 	    		
				glBegin(GL_LINE_STRIP); 		    
				for(int i = 0; i<ny; ++i) {
					glVertex3d(GLdouble(ct3x(cs->XData[i][j])),
						   GLdouble(ct3y(cs->YData[i][j])),
						   GLdouble(ct3z(cs->ZData[i][j])) ); 
				}
				glEnd();
			}
		}    
    }//(X, y,Z)
}

/// dispaly contour
template <typename T>
vector<vector<T>> contourc(vector<T> x, vector<T> y, vector<vector<T>> Z, vector<T> v) {
    //Z(i,j), x(j), y(i)
    T x0, y0, z0;    
    size_t ny = Z.size();
    size_t nx = Z[0].size();
    vector<vector<T>> C;
    ContourPoint<T> c;
    vector<ContourPoint<T>> vc;
    deque<ContourPoint<T>> ac;
    C.resize(2);
    //z0= 0.1;
    //v.clear();v.push_back(0.2);v.push_back(0.1);
    int is_print = 0;

	for(int iv = 0; iv<v.size(); ++iv) {
		z0 = v[iv];

		// find contour points
		vc.clear();
		for(int i = 0; i<ny; ++i) {
			for(int j = 0; j<nx; ++j) {
				if( (j<nx-1)&&( (Z[i][j+1]-z0)*(Z[i][j]-z0)<0 ) ) {
					x0 = x[j]+(x[j+1]-x[j])*(z0-Z[i][j])/( Z[i  ][j+1]-Z[i][j] );
					c.xj =j;c.yi = i;c.xy= 0;c.done = 0;
					c.x = x0;c.y= y[i];
					vc.push_back(c);		
				}
				if( (i<ny-1)&&( (Z[i+1][j]-z0)*(Z[i][j]-z0)<0 ) ) {
					y0 = y[i]+(y[i+1]-y[i])*(z0-Z[i][j])/( Z[i+1][j  ]-Z[i][j] );
					c.xj =j;c.yi = i;c.xy= 1;c.done = 0;
					c.x = x[j];c.y= y0;
					vc.push_back(c);
				}
			}
		} 
		if(is_print) {
			printf("vc.size %d\n",vc.size());
			for(int k= 0;k<vc.size(); ++k) {
				printf("vc: %2d : %2d %2d :%f %f\n",k,vc[k].xj,vc[k].yi,vc[k].x,vc[k].y);
			}
		}
		// sort contour points    
		int is, is_connect = 0;	
		int m, k, kk;
		int isInited, mode_next;

		k = 0;
		isInited = 0;
		while( isInited<5 ) {

			if(isInited == 0) {// set initial contour point
			ac.clear();
			is = 0; m = 0;		
			while( !is && (m<vc.size()) ) {
				if(!vc[m].done) { is = 1; kk = m; }
				m++;
			}
			if(is) {		    
				vc[kk].done =2; 
				c=vc[kk];
				ac.push_back(vc[kk]); 
				mode_next = 1;
			}else{
				mode_next =5;
			}
		
			}
			if( (isInited == 1)||(isInited ==3) ) {//search next contour point
				is= 0;
				m = 0;
				while( !is && (m<vc.size()) ) {
					is= 0;
					if( (!vc[m].done) || ((vc[m].done ==2)&&(ac.size()>2)) ) {		
					if( (c.xy== 0) && (vc[m].xy== 0) && (vc[m].xj == c.xj  ) && (vc[m].yi == c.yi-1) ) { is= 1; }
					if( (c.xy== 0) && (vc[m].xy== 0) && (vc[m].xj == c.xj  ) && (vc[m].yi == c.yi+1) ) { is=2; }
					if( (c.xy== 0) && (vc[m].xy== 1) && (vc[m].xj == c.xj  ) && (vc[m].yi == c.yi  ) ) { is=3; }
					if( (c.xy== 0) && (vc[m].xy== 1) && (vc[m].xj == c.xj+1) && (vc[m].yi == c.yi  ) ) { is=4; }
					if( (c.xy== 0) && (vc[m].xy== 1) && (vc[m].xj == c.xj  ) && (vc[m].yi == c.yi-1) ) { is=5; }
					if( (c.xy== 0) && (vc[m].xy== 1) && (vc[m].xj == c.xj+1) && (vc[m].yi == c.yi-1) ) { is=6; }
					if( (c.xy== 1) && (vc[m].xy== 1) && (vc[m].xj == c.xj+1) && (vc[m].yi == c.yi  ) ) { is=7; }
					if( (c.xy== 1) && (vc[m].xy== 1) && (vc[m].xj == c.xj-1) && (vc[m].yi == c.yi  ) ) { is=8; }
					if( (c.xy== 1) && (vc[m].xy== 0) && (vc[m].xj == c.xj  ) && (vc[m].yi == c.yi  ) ) { is=9; }
					if( (c.xy== 1) && (vc[m].xy== 0) && (vc[m].xj == c.xj  ) && (vc[m].yi == c.yi+1) ) { is= 10; }
					if( (c.xy== 1) && (vc[m].xy== 0) && (vc[m].xj == c.xj-1) && (vc[m].yi == c.yi  ) ) { is= 11; }
					if( (c.xy== 1) && (vc[m].xy== 0) && (vc[m].xj == c.xj-1) && (vc[m].yi == c.yi+1) ) { is= 12; }
					}
					if(is) {kk=m;}
					m++;
				}
				if(is) {		    
					vc[kk].done = 1; 
					c=vc[kk];
				}
				if(isInited == 1) {
					if(is) {		    
						ac.push_back(vc[kk]); 
						mode_next = 1;
					}
					else {
						mode_next =2;
					}
				}
				if(isInited ==3) {
					if(is) {		    
						ac.push_front(vc[kk]); 
						mode_next =3;
					}
					else {
						mode_next =4;
					}
				}
			}

			if(isInited ==2) {// set first terminal
				c=ac[0];
				mode_next =3;
			}

			if(isInited ==4) {// move the accumlated points 
				if(ac.size()) {
					C[0].push_back(z0);
					C[1].push_back(T(ac.size()));
					for(int i = 0; i<ac.size(); ++i) {
						C[0].push_back( ac[i].x );
						C[1].push_back( ac[i].y );
					}
				}
				mode_next = 0;
			}

			isInited =mode_next;
		}
	
    }//iv

    // print
    if(is_print) {
		for(int i = 0; i<C[0].size(); ++i) {
			printf("C: %3d %f %f\n",i,C[0][i],C[1][i]);
		}
		cout <<"done"<<endl;
    }
    return C;
}

template <typename T>
void MatPlot<T>::display_contour() {
    //vector<T> rgb;
    //int ny= cs->ZData.size();
    //int nx = cs->ZData[0].size();
    vector< vector< T > > C;
    T xx, yy;

    C = contourc(cs->XData[0],cs->YData[0],cs->ZData,cs->V);

    glDisable(GL_LINE_STIPPLE);
    gl2psDisable(GL2PS_LINE_STIPPLE);

    //glLineWidth(cl->LineWidth);
    glLineWidth(2);
    gl2psLineWidth(2);

    //rgb = cs->CData[i][j];
    //glColor3f(rgb[0],rgb[1],rgb[2]);
    glColor3f(0,0,0);

    //TODO: adjust line color and properties
    int k = 0, nk;
    for(int i = 0; i<C[0].size(); ++i) {
		if( k == 0) {
			nk=(int)C[1][i];
			glBegin(GL_LINE_STRIP);
		}
		else {
			if(k<=nk) {
				xx = ctx( C[0][i] );
				yy= cty( C[1][i] );
				glVertex2d(xx, yy);
			}
		}
		k++;
		if(k>nk) {
			k= 0;
			glEnd();
		}
    }   
}

template <typename T>
vector<vector<T>> MatPlot<T>::peaks(int n) {

    T x1 = 1, y1 = 0;
    T x2 = -1, y2 = 1;
    T x3 = -0.5, y3 =-1;
    T sr1, sr2, sr3;
    T sigma = 0.4;
    T a1 = 1, a2 = 0.5, a3 = 0.3;
    T x, y;
    //vector< vector< T > > Z(n,n);
    vector<vector<T>> Z(n,vector<T>(n));
    for(int i = 0; i<n; ++i) {
		for(int j = 0; j<n; ++j) {
			x =-2.f+4.f*j/(n-1);
			y=-2.f+4.f*i/(n-1);
			sr1=(x-x1)*(x-x1)+(y-y1)*(y-y1);
			sr2=(x-x2)*(x-x2)+(y-y2)*(y-y2);
			sr3=(x-x3)*(x-x3)+(y-y3)*(y-y3);
			Z[i][j] = a1/(1+sr1/sigma) +a2/(1+sr2/sigma) +a3/(1+sr3/sigma) ;
		}
    }
    return Z;
}


// Patch ///
template <typename T>
int MatPlot<T>::patch() {
    int h = iPatch*100 + tPatch; hObj =h;

    if(isInited == 0) {	    
		ca->add_child(h);	   
		vPatch.push_back(Patch(h));
		//as.Parent =gca();
    }
    if(isInited == 1) {
	
    }
    if(iPatch<vPatch.size()) {cp= &vPatch[iPatch];}
    iPatch++;    
    return h;
}

template <typename T>
void MatPlot<T>::patch_config() {
    ca->xmax =max( Fmax(cp->XData), ca->xmax);
    ca->xmin = min( Fmin(cp->XData), ca->xmin);
    ca->ymax =max( Fmax(cp->YData), ca->ymax);
    ca->ymin = min( Fmin(cp->YData), ca->ymin);
    ca->zmax =max( Fmax(cp->ZData), ca->zmax );
    ca->zmin = min( Fmin(cp->ZData), ca->zmin );
}

template <typename T>
vector<vector<T>> MatPlot<T>::Index2TrueColor(vector<T> IC) {
    if(ca->CLim[0] == ca->CLim[1]) {
		ca->CLim[0] = min( Fmin(IC), Fmin(IC) );
		ca->CLim[1] = max( Fmax(IC), Fmax(IC) );	
    }
    vector<T> rgb;
    vector<vector<T>> tc;	
    for(int j = 0; j<IC.size(); ++j) {
		rgb = map2color(IC[j],ca->CLim[0],ca->CLim[1]);
		tc.push_back(rgb);
    }
    return tc;    
}

/// patch
template <typename T>
int MatPlot<T>::patch(vector<vector<T>> X, vector<vector<T>> Y) {
    // Single color
    int h =patch();
    cp->type = 0;

    cp->XData = X;
    cp->YData = Y;
    cp->ZData.clear();
    cp->CData.clear();

    patch_config();
    return h;
}

template <typename T>
int MatPlot<T>::patch(vector<vector<T>> X, vector<vector<T>> Y, vector<T> C) {
    // One color per face with index color 
    int h =patch();
    cp->type = 0;
    cp->XData = X;
    cp->YData = Y;
    cp->ZData.clear();
    cp->CData = Index2TrueColor(C);

    patch_config();
    return h;
}

/*
template <typename T>
int MatPlot<T>::patch(vector<vector<T>> X, vector<vector<T>> Y, vector<vector<T>> C) {
    // One color per face with true color
    int h =patch();
    cp->type = 0;
    cp->XData = X;
    cp->YData = Y;
    cp->ZData.clear();
    cp->CData = C;

    patch_config();
    return h;
}
*/

template <typename T>
int MatPlot<T>::patch(vector<vector<T>> X, vector<vector<T>> Y, vector<vector<T>> Z) {
    // Single color
    int h =patch(); 
    ca->View= 1;
    cp->type = 1;
    cp->XData = X;
    cp->YData = Y;
    cp->ZData = Z;
    cp->CData.clear();

    patch_config();
    return h;
}

template <typename T>
int MatPlot<T>::patch(vector<vector<T>> X, vector<vector<T>> Y, vector<vector<T>> Z, vector<T> C) {
    // One color per face    
    int h = patch();
    ca->View = 1;
    cp->type = 1;

    cp->XData = X;
    cp->YData = Y;
    cp->ZData = Z;
    cp->CData = Index2TrueColor(C);

    patch_config();
    return h;
}

template <typename T>
int MatPlot<T>::patch(vector<vector<T>> X, vector<vector<T>> Y, vector<vector<T>> Z, vector<vector<T>> C) {
    // One color per face    
    int h = patch();    
    ca->View = 1;
    cp->type = 1;

    cp->XData = X;
    cp->YData = Y;
    cp->ZData = Z;
    cp->CData = C;

    patch_config();
    return h;
}
/// bar

template <typename T>
int MatPlot<T>::bar(vector<T> y) { 
    vector<T> x;
    x.resize(y.size());
    for(int i = 0; i<y.size(); ++i) {
	x[i] = 1.0*(1+i);
    }
    return bar(x, y,0.8);
}

template <typename T>
int MatPlot<T>::bar(vector<T> y,T width) { 
    vector<T> x;
    x.resize(y.size());
    for(int i = 0; i<y.size(); ++i) {
	x[i] = 1.f*(1+i);
    }
    return bar(x, y,width);
}

template <typename T>
int MatPlot<T>::bar(vector<T> x,vector<T> y) { 
    return bar(x, y,0.8);
}

template <typename T>
int MatPlot<T>::bar(vector<T> x,vector<T> y,T width) {    
    int h =patch();
    cp->type = 0;
    cp->XData.clear();
    cp->YData.clear();
    cp->ZData.clear();
    //cp->CData.clear();
    
    T wx = width*(Fmax(x)-Fmin(x))/x.size();

    vector<T> X(4), Y(4);
    for(int i = 0; i<x.size(); ++i) {
	X[0] = x[i]-wx/2.0; Y[0] = 0;
	X[1] = x[i]+wx/2.0; Y[1] = 0;
	X[2] = x[i]+wx/2.0; Y[2] = y[i];
	X[3] = x[i]-wx/2.0; Y[3] = y[i];
	cp->XData.push_back(X);
	cp->YData.push_back(Y);
    }
    patch_config();
    return h;	    
}

/// display
template <typename T>
void MatPlot<T>::display_patch() {
    if(cp->type == 0) { display_patch_2d(); }
    if(cp->type == 1) { display_patch_3d(); }
}

template <typename T>
void MatPlot<T>::display_patch_2d() {
    // FaceVertex & CData //
    
    vector<T> v(3);
    vector<int> f(3);
    T x, y;
    for(int i = 0; i<cp->Faces.size(); ++i) {
		f = cp->Faces[i];
		glBegin(GL_TRIANGLES);	
		x = ctx(cp->Vertices[f[0]][0]);
		y = cty(cp->Vertices[f[0]][1]);
		glVertex2d(x, y);
		x = ctx(cp->Vertices[f[1]][0]);
		y = cty(cp->Vertices[f[1]][1]);
		glVertex2d(x, y);
		x = ctx(cp->Vertices[f[2]][0]);
		y = cty(cp->Vertices[f[2]][1]);
		glVertex2d(x, y);	
		glEnd();
    }
    

    // XYZ Data //
    size_t nf, nv;//number of faces and vertex
    nf = cp->XData.size();
    vector<T> rgb;

    for(size_t i = 0; i<nf; ++i) {
		nv = cp->XData[i].size();

		// Edge
		if(cp->EdgeColor!="none") {
	    
			glLineWidth(GLfloat(cp->LineWidth));
			gl2psLineWidth(GLfloat(cp->LineWidth));
	    
			rgb = ColorSpec2RGB(cp->EdgeColor);
			glColor3f(GLfloat(rgb[0]), GLfloat(rgb[1]), GLfloat(rgb[2])); 	    
		
			glBegin(GL_LINE_LOOP);
			for(size_t iv = 0; iv<nv; ++iv) {
				glVertex2d( ctx(cp->XData[i][iv]),
						cty(cp->YData[i][iv]) );
			}
			glEnd();
		
		}
		// Face
		if(cp->FaceColor!="none") {
	    
			rgb = ColorSpec2RGB(cp->FaceColor);
			glColor3f(GLfloat(rgb[0]), GLfloat(rgb[1]), GLfloat(rgb[2])); 	    
	    
			if(cp->CData.size()) {
			rgb = cp->CData[i];
			glColor3d(GLdouble(rgb[0]), GLdouble(rgb[1]), GLdouble(rgb[2]));
			}
	    
			glBegin(GL_POLYGON);
			for(size_t iv= 0; iv<nv; ++iv) {
				glVertex2d(GLdouble(ctx(cp->XData[i][iv])),
						GLdouble(cty(cp->YData[i][iv])) );
			}
			glEnd();
		
		}
    }
}

template <typename T>
void MatPlot<T>::display_patch_3d() {
    // FaceVertex & CData //
    // XYZ Data //
    size_t nf, nv;//number of faces and vertex
    nf = cp->XData.size();
    vector<T> rgb;

    for(int i = 0; i<nf; ++i) {
		nv = cp->XData[i].size();
		// Edge
		if(cp->EdgeColor!="none") {   
			glLineWidth(GLfloat(cp->LineWidth));
			gl2psLineWidth(GLfloat(cp->LineWidth));
	    
			rgb = ColorSpec2RGB(cp->EdgeColor);
			glColor3f(GLfloat(rgb[0]), GLfloat(rgb[1]), GLfloat(rgb[2])); 	    
		
			glBegin(GL_LINE_LOOP);
			for(int iv= 0; iv<nv; ++iv) {
			glVertex3d(GLdouble(ct3x(cp->XData[i][iv])),
					GLdouble(ct3y(cp->YData[i][iv])), 
					GLdouble(ct3z(cp->ZData[i][iv])) );
			}
			glEnd();
		
		}
		// Face
		if(cp->FaceColor != "none") {
	    
			rgb = ColorSpec2RGB(cp->FaceColor);
			glColor3f(GLfloat(rgb[0]), GLfloat(rgb[1]), GLfloat(rgb[2])); 	    
	    
			if(cp->CData.size()) {
				rgb = cp->CData[i];
				glColor3d(GLdouble(rgb[0]), GLdouble(rgb[1]), GLdouble(rgb[2]));
			}
	    
			glBegin(GL_POLYGON);
			for(int iv= 0; iv<nv; ++iv) {
				glVertex3d(GLdouble(ct3x(cp->XData[i][iv])), GLdouble(ct3y(cp->YData[i][iv])),  GLdouble(ct3z(cp->ZData[i][iv])));
			}
			glEnd();
		}
    }
}

// Text ///
//TODO more fonts
/// text 
template <typename T>
int MatPlot<T>::text() {
    int h = iText*100 + tText; 
	hObj =h;
  
    if(isInited == 0) {
		vText.push_back(Text(h));
		ca->add_child(h);
		//at.Parent =gca();
    }

    if(isInited == 1) {
    }

    if(iText<vText.size()) {ct = &vText[iText];}
    iText++;    
    return h;
}

template <typename T>
void MatPlot<T>::display_text() {
    glColor3f(0,0,0);
    glRasterPos2f(GLfloat(ctx(ct->Position[0])), GLfloat(cty(ct->Position[1])) );
    gl2psText(ct->String.c_str(), "Arial", 12);		
    for(int i = 0; i<(int)ct->String.size(); ++i ) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, ct->String[i] );
    }
}

template <typename T>
int MatPlot<T>::text(T x, T y, string s) {
    // text on current axes	
    int h = text();    
    ct->Position[0] = x;
    ct->Position[1] = y;
    ct->String = s;
    return h;
}

template <typename T>
void MatPlot<T>::set_font(char font_[],int size) {
    // font = font_;
    // font_size = size;
}

/// ptext
template <typename T>
void MatPlot<T>::ptext(T x,T y,string s) {
    // Viewport Figure 
    glViewport(0, 0, (int)(window_w), (int)(window_h));
    glLoadIdentity();
    gluOrtho2D( 0.0, 1.0, 0.0, 1.0 );
    
    glColor3f(GLfloat(0), GLfloat(0), GLfloat(0));
    glRasterPos2f(GLfloat(x), GLfloat(y));
    gl2psText(s.c_str(), "Arial", 12);
    //gl2psText(test, "Times-Roman", 24);
    
    for(int i = 0; i<(int)s.size(); i++ ) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, s[i] );				
    }
}    

template <typename T>
void MatPlot<T>::ptext3(T x,T y,T z,string s) {
    glColor3f(0,0,0);
    glRasterPos3d(x, y,z);
    gl2psText(s.c_str(), "Arial", 12);    
    for(size_t i = 0; i<s.size(); i++ ) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, s[i] );				
    }
}  

template <typename T>
void MatPlot<T>::ptext3c(T x, T y, T z, string s) {
    int char_w = 6, char_h = 12;
    size_t num_char = s.length();
    glColor3f(0,0,0);
    glRasterPos3d(x, y, z);
    glBitmap(0, 0, 0, 0, -char_w*num_char/2.f, -char_h/2.f, NULL);
    gl2psText(s.c_str(), "Arial", 12);
    
    for(size_t i = 0; i<s.size(); i++ ) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, s[i] );				
    }
} 


/// Color ///
template <typename T>
void MatPlot<T>::Shading(string c) {
    shading(c);
}


template <typename T>
void MatPlot<T>::shading(string c) {

    int tObj = hObj%100;
    int iObj = hObj/100;
    if( (tObj == tSurface) && (iObj<vSurface.size()) ) {
		if(c=="faceted") { 
			vSurface[iObj].EdgeColor = "k"; 
		}
		if(c=="flat") { 
			vSurface[iObj].EdgeColor = "none";
		}
    }
}

/// colormap
template <typename T> 
vector<T> MatPlot<T>::colormap(string c,T t) {
    
    vector<T> rgb(3);
    if(t>1) {t = 1;}
    if(t<0) {t = 0;}
    
    if( c=="Gray" ) {
		rgb[0] = t;
		rgb[1] = t;
		rgb[2] = t;
		return rgb;
    }

    if( c=="HSV" ) {
		t *= 6.f;
		if(0.f<= t && t<= 1.f) {rgb[0] = 1;        rgb[1] = t;        rgb[2] = 0;}
		if(1.f<= t && t<=2.f) {rgb[0] = 1-(t-1);  rgb[1] = 1;        rgb[2] = 0;}
		if(2.f<= t && t<=3.f) {rgb[0] = 0;        rgb[1] = 1;        rgb[2] = t-2;}
		if(3.f<= t && t<=4.f) {rgb[0] = 0;        rgb[1] = 1-(t-3);  rgb[2] = 1;}
		if(4.f<= t && t<=5.f) {rgb[0] = t-4;      rgb[1] = 0;        rgb[2] = 1;}
		if(5.f<= t && t<=6.f) {rgb[0] = 1;        rgb[1] = 0;        rgb[2] = 1-(t-5);}
	
		return rgb;	
    }

    if( c=="Jet" ) {
		t *= 8.f;
		if(0.f<= t && t<= 1.f) {rgb[0] = 0.f;              rgb[1] = 0.f;              rgb[2] = 0.5f+0.5f*t;}
		if(1.f<= t && t<=3.f)  {rgb[0] = 0.f;              rgb[1] = 0.5f*(t-1);       rgb[2] = 1.f;}
		if(3.f<= t && t<=5.f)  {rgb[0] = 0.5f*(t-3.f);     rgb[1] = 1.f;              rgb[2] = 1.f-0.5f*(t-3.f);}
		if(5.f<= t && t<=7.f)  {rgb[0] = 1.f;              rgb[1] = 1.f-0.5f*(t-5.f); rgb[2] = 0.f;}
		if(7.f<= t && t<=8.f)  {rgb[0] = 1.f-0.5f*(t-7.f); rgb[1] = 0.f;              rgb[2] = 0.f;}
		return rgb;	
    }

    if( c=="Hot" ) {
		t*=3;
		if(0.f<= t && t<= 1.f) {rgb[0] = t;   rgb[1] = 0.f;   rgb[2] = 0.f;}
		if(1.f<= t && t<=2.f)  {rgb[0] = 1.f; rgb[1] = t-1.f; rgb[2] = 0.f;}
		if(2.f<= t && t<=3.f)  {rgb[0] = 1.f; rgb[1] = 1.f;   rgb[2] = t-2.f;}
		return rgb;
    }
    
    if( c=="Cool" ) {
		rgb[0] = t;
		rgb[1] = 1-t;
		rgb[2] = 1;
		return rgb;
    }

    if( c=="Spring" ) {// Magenta - Yellow
		rgb[0] = 1;
		rgb[1] = t;
		rgb[2] = 1-t;
		return rgb;
    }
    if( c=="Summer" ) {// Green Yellow
		rgb[0] = t;
		rgb[1] = 1;
		rgb[2] = 0;
		return rgb;
    }
    if( c=="Autumn" ) {
		rgb[0] = 1;
		rgb[1] = t;
		rgb[2] = 0;
		return rgb;
    }
    if( c=="Winter" ) {
		rgb[0] = 0;
		rgb[1] = t;
		rgb[2] = 1-t;
		return rgb;
    }
    if( c=="Bone" ) {
		rgb[0] = t;
		//rgb[1] = t; if(t<0.8) {rgb[1] = t;}
		rgb[2] = t;
		return rgb;
    }

	rgb[0] = 0.f;
	rgb[1] = 0.f;
	rgb[2] = 0.f;
	return rgb;
}

template <typename T>
void MatPlot<T>::colormap(string c) {

    int n;
    vector<T> rgb(3);

    cmap.clear();
    n = 64;

    for(int i = 0; i<n; ++i) {
		rgb = colormap(c,(T)i/(n-1));		
		cmap.push_back(rgb);
    }

    if(isInited == 1) {
		cs->ColorMap= c;
    }    
}

template <typename T>
void MatPlot<T>::colormap(vector<vector<T> > c) {
    cmap = c;
}

template <typename T>
void MatPlot<T>::gray() {colormap("Gray");};

template <typename T>
void MatPlot<T>::jet() { colormap("Jet"); }

template <typename T>
void MatPlot<T>::hsv() { colormap("HSV"); }

template <typename T>
void MatPlot<T>::hot() { colormap("Hot"); }

template <typename T>
void MatPlot<T>::cool() {colormap("Cool");}

template <typename T>  
void MatPlot<T>::spring() {colormap("Spring");}

template <typename T>   
void MatPlot<T>::summer() {colormap("Summer");}

template <typename T>     
void MatPlot<T>::autumn() {colormap("Autumn");}

template <typename T>
void MatPlot<T>::winter() {colormap("Winter");}     

template <typename T>
vector<T> MatPlot<T>::map2color(T x, T xmin, T xmax) {
    size_t n = cmap.size();
    T normx;
    vector<T> rgb(3);
    
    normx = (x-xmin)/(xmax-xmin);
    if(x>xmax) {normx = 1.f;}
    if(x<xmin) {normx = 0.f;}
    rgb = cmap[(int)(normx*(n-1))];
    //cout << "c: "<<(int)(normx*n) <<endl;
    //cout << "rgb: "<<rgb[0]<<" "<<endl;
    return rgb;
}

template <typename T>
vector<T> MatPlot<T>::ColorSpec2RGB(string c) {
    T r, g, b;
    //line
    if( c=="k" ) {r = 0.f; g = 0.f; b = 0.f;}// black
    if( c=="r" ) {r = 1.f; g = 0.f; b = 0.f;}// red
    if( c=="b" ) {r = 0.f; g = 0.f; b = 1.f;}// blue
    if( c=="g" ) {r = 0.f; g = 1.f; b = 0.f;}// green	    
    if( c=="c" ) {r = 0.f; g = 1.f; b = 1.f;}// cyan
    if( c=="m" ) {r = 1.f; g = 0.f; b = 1.f;}// magenta
    if( c=="y" ) {r = 1.f; g = 1.f; b = 0.f;}// yellow
    if( c=="w" ) {r = 1.f; g = 1.f; b = 1.f;}// white

    //dark color
    T h = 0.6f;
    if( c=="dr" ) {r = h;   g = 0.f; b = 0.f;}// red
    if( c=="db" ) {r = 0.f; g = 0.f; b = h;}// blue
    if( c=="dg" ) {r = 0.f; g = h;   b = 0.f;}// green	    
    if( c=="dc" ) {r = 0.f; g = h;   b = h;}// cyan
    if( c=="dm" ) {r = h;   g = 0.f; b = h;}// magenta
    if( c=="dy" ) {r = h;   g = h;   b = 0.f;}// yellow
    
    //light color
    h = 0.5;
    if( c=="lr" ) {r = 1.f; g = h;   b = h;}// red
    if( c=="lb" ) {r = h;   g = h;   b = 1.f;}// blue
    if( c=="lg" ) {r = h;   g = 1.f; b = h;}// green	    
    if( c=="lc" ) {r = h;   g = 1.f; b = 1.f;}// cyan
    if( c=="lm" ) {r = 1.f; g = h;   b = 1.f;}// magenta
    if( c=="ly" ) {r = 1.f; g = 1.f; b = h;}// yellow
    
    //universal color
    if( c=="ur" ) {r = 1.f;   g = 0.2f;  b = 0.f;  }//red
    if( c=="ub" ) {r = 0.f;   g = 0.25f; b = 1.f;  }//blue
    if( c=="ug" ) {r = 0.2f;  g = 0.6f;  b = 0.4f;}//green
    if( c=="uy" ) {r = 1.f;   g = 1.f;   b = 1.f;  }//yellow
    if( c=="uc" ) {r = 0.4f;  g = 0.8f;  b = 1.f;  }//sky blue
    if( c=="up" ) {r = 1.f;   g = 0.6f;  b = 0.6f;}//pink
    if( c=="uo" ) {r = 1.f;   g = 0.6f;  b = 0.f;  }//orange
    if( c=="um" ) {r = 0.6f;  g = 0.f;   b = 0.4f;}//perple
    if( c=="ubr") {r = 0.4f;  g = 0.2f;  b = 0.f;  }//brown

	if( c=="flat") {r = 0.4f;  g = 0.2f;  b = 0.f;  }//brown

    // char line[1000], *tp;
    char d[] = " ,:\t\n[]";//delimiter
    /*
    if(c.size()) {
	if( (c[0] =='[') && (c[c.size()-1] ==']') ) {
	    sprintf(line,c.c_str());
	    tp=strtok(line,d); if(tp) {r = atof(tp);}
	    tp=strtok(NULL,d); if(tp) {g = atof(tp);}
	    tp=strtok(NULL,d); if(tp) {b = atof(tp);}
	}
    }
    */
    //cout <<"c,r,g,b: "<< c <<" "<<r<<" "<<g <<" "<<b<<endl;
    vector<T> out(3);
    out[0] = r;
    out[1] = g;
    out[2] = b;

    return out;
       
}

template <typename T>
string MatPlot<T>::rgb2colorspec(vector<T> rgb) {
    char c[100];
    sprintf(c,"[%f %f %f]",rgb[0],rgb[1],rgb[2]);
    string s = c;
    return s;
}

//template<> class Axes<float>;
//template<> class MatPlot<float>;

//template<> class Axes<double>;
//template<> class MatPlot<double>;

} // end Namespace MatPlotPP

