//
// "$Id: fl_vertex.cxx,v 1.5.2.3.2.5 2002/01/01 15:11:32 easysw Exp $"
//
// Portable drawing routines for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2002 by Bill Spitzak and others.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

// Portable drawing code for drawing arbitrary shapes with
// simple 2D transformations.  See also fl_arc.cxx

#include <FL/fl_draw.H>
#include <FL/x.H>
#include <FL/math.h>
#include <stdlib.h>
#include <stdio.h>

struct matrix {double a, b, c, d, x, y;};

static matrix m = {1, 0, 0, 1, 0, 0};

static matrix stack[10];
static int sptr = 0;

void fl_push_matrix() {stack[sptr++] = m;}

void fl_pop_matrix() {m = stack[--sptr];}

void fl_mult_matrix(double a, double b, double c, double d, double x, double y) {
  matrix o;
  o.a = a*m.a + b*m.c;
  o.b = a*m.b + b*m.d;
  o.c = c*m.a + d*m.c;
  o.d = c*m.b + d*m.d;
  o.x = x*m.a + y*m.c + m.x;
  o.y = x*m.b + y*m.d + m.y;
  m = o;
}

void fl_scale(double x,double y) {fl_mult_matrix(x,0,0,y,0,0);}

void fl_scale(double x) {fl_mult_matrix(x,0,0,x,0,0);}

void Fl_Fltk::translate(double x,double y) {fl_mult_matrix(1,0,0,1,x,y);}

void fl_rotate(double d) {
  if (d) {
    double s, c;
    if (d == 0) {s = 0; c = 1;}
    else if (d == 90) {s = 1; c = 0;}
    else if (d == 180) {s = 0; c = -1;}
    else if (d == 270 || d == -90) {s = -1; c = 0;}
    else {s = sin(d*M_PI/180); c = cos(d*M_PI/180);}
    fl_mult_matrix(c,-s,s,c,0,0);
  }
}

static XPoint *p = (XPoint *)0;
// typedef what the x,y fields in a point are:
#ifdef WIN32
typedef int COORD_T;
#else
typedef short COORD_T;
#endif

static int p_size;
static int n;
static int what;
enum {LINE, LOOP, POLYGON, POINT_};

void Fl_Fltk::begin_points() {n = 0; what = POINT_;}

void Fl_Fltk::begin_line() {n = 0; what = LINE;}

void Fl_Fltk::begin_loop() {n = 0; what = LOOP;}

void Fl_Fltk::begin_polygon() {n = 0; what = POLYGON;}

double fl_transform_x(double x, double y) {
  return x*m.a + y*m.c + m.x;
}

double fl_transform_y(double x, double y) {
  return x*m.b + y*m.d + m.y;
}

double fl_transform_dx(double x, double y) {
  return x*m.a + y*m.c;
}

double fl_transform_dy(double x, double y) {
  return x*m.b + y*m.d;
}

static void fl_trans_vertex(COORD_T x, COORD_T y) {
  if (fl->type == FL_GDI_DEVICE) {
    x = (COORD_T)(x * fl->s + fl->L);
	y = (COORD_T)(y * fl->s + fl->T);
  }
  if (!n || x != p[n-1].x || y != p[n-1].y) {
    if (n >= p_size) {
      p_size = p ? 2*p_size : 16;
      p = (XPoint *)realloc((void*)p, p_size*sizeof(*p));
    }
    p[n].x = x;
    p[n].y = y;
    n++;
  }
}

void Fl_Fltk::transformed_vertex(double xf, double yf) {
	fl_trans_vertex(COORD_T(rint(xf)), COORD_T(rint(yf)));
}

void Fl_Fltk::vertex(double x,double y) {
  Fl_Fltk::transformed_vertex(x*m.a + y*m.c + m.x, x*m.b + y*m.d + m.y);
}

void Fl_Fltk::end_points() {
#ifdef WIN32
  for (int i=0; i<n; i++) SetPixel(fl_gc, p[i].x, p[i].y, fl_RGB());
#elif defined(__APPLE__)
  for (int i=0; i<n; i++) { MoveTo(p[i].x, p[i].y); Line(0, 0); } 
#else
  if (n>1) XDrawPoints(fl_display, fl_window, fl_gc, p, n, 0);
#endif
}

void Fl_Fltk::end_line() {
#ifdef WIN32
  if (n>1) {
    Polyline(fl_gc, p, n);
  }
#elif defined(__APPLE__)
  if (n<=1) return;
  MoveTo(p[0].x, p[0].y);
  for (int i=1; i<n; i++) LineTo(p[i].x, p[i].y);
#else
  if (n>1) XDrawLines(fl_display, fl_window, fl_gc, p, n, 0);
#endif
}

static void fixloop() {  // remove equal points from closed path
  while (n>2 && p[n-1].x == p[0].x && p[n-1].y == p[0].y) n--;
}

void Fl_Fltk::end_loop() {
  fixloop();
  if (n>2) {
	  int g = 0;
	  if (fl->type == FL_GDI_DEVICE) {
	    g = 1;
		fl->type = FL_FLTK_DEVICE;
	  }
	  fl_trans_vertex((COORD_T)p[0].x, (COORD_T)p[0].y);
	  if (g) fl->type = FL_GDI_DEVICE;
  }
  Fl_Fltk::end_line();
}

void Fl_Fltk::end_polygon() {
  fixloop();
#ifdef WIN32
  if (n>2) {
    SelectObject(fl_gc, fl_brush());
    Polygon(fl_gc, p, n);
  }
#elif defined(__APPLE__)
  if (n<=1) return;
  PolyHandle ph = OpenPoly();
  MoveTo(p[0].x, p[0].y);
  for (int i=1; i<n; i++) LineTo(p[i].x, p[i].y);
  ClosePoly();
  PaintPoly(ph);
  KillPoly(ph);
#else
  if (n>2) XFillPolygon(fl_display, fl_window, fl_gc, p, n, Convex, 0);
#endif
}

static int garp;
#ifdef WIN32
static int counts[20];
static int numcount;
#endif

void Fl_Fltk::begin_complex_polygon() {
  Fl_Fltk::begin_polygon();
  garp = 0;
#ifdef WIN32
  numcount = 0;
#endif
}

void Fl_Fltk::gap() {
  while (n>garp+2 && p[n-1].x == p[garp].x && p[n-1].y == p[garp].y) n--;
  if (n > garp+2) {
	int g = 0;
	if (fl->type == FL_GDI_DEVICE) {
	  g = 1;
      fl->type = FL_FLTK_DEVICE;
	}
    fl_trans_vertex((COORD_T)p[garp].x, (COORD_T)p[garp].y);
    if (g) fl->type = FL_GDI_DEVICE; 
#ifdef WIN32
    counts[numcount++] = n-garp;
#endif
    garp = n;
  } else {
    n = garp;
  }
}

void Fl_Fltk::end_complex_polygon() {
  Fl_Fltk::gap();
#ifdef WIN32
  if (n>2) {
    SelectObject(fl_gc, fl_brush());
    PolyPolygon(fl_gc, p, counts, numcount);
  }
#elif defined(__APPLE__)
  if (n<=1) return;
  PolyHandle ph = OpenPoly();
  MoveTo(p[0].x, p[0].y);
  for (int i=1; i<n; i++) LineTo(p[i].x, p[i].y);
  ClosePoly();
  PaintPoly(ph);
  KillPoly(ph);
#else
  if (n>2) XFillPolygon(fl_display, fl_window, fl_gc, p, n, 0, 0);
#endif
}

// shortcut the closed circles so they use XDrawArc:
// warning: these do not draw rotated ellipses correctly!
// See fl_arc.c for portable version.

void Fl_Fltk::circle(double x, double y,double r) {
  double xt = fl_transform_x(x,y);
  double yt = fl_transform_y(x,y);
  double rx = r * (m.c ? sqrt(m.a*m.a+m.c*m.c) : fabs(m.a));
  double ry = r * (m.b ? sqrt(m.b*m.b+m.d*m.d) : fabs(m.d));
  if (fl->type == FL_GDI_DEVICE) {
    xt = xt*fl->s+fl->L;
	yt = yt*fl->s+fl->T;
	rx = rx*fl->s;
	ry = ry*fl->s;
  }
  int llx = (int)rint(xt-rx);
  int w = (int)rint(xt+rx)-llx;
  int lly = (int)rint(yt-ry);
  int h = (int)rint(yt+ry)-lly;
#ifdef WIN32
  if (what==POLYGON) {
    SelectObject(fl_gc, fl_brush());
    Pie(fl_gc, llx, lly, llx+w, lly+h, 0,0, 0,0); 
  } else
    Arc(fl_gc, llx, lly, llx+w, lly+h, 0,0, 0,0); 
#elif defined(__APPLE__)
  Rect rt; rt.left=llx; rt.right=llx+w; rt.top=lly; rt.bottom=lly+h;
  (what == POLYGON ? PaintOval : FrameOval)(&rt);
#else
  (what == POLYGON ? XFillArc : XDrawArc)
    (fl_display, fl_window, fl_gc, llx, lly, w, h, 0, 360*64);
#endif
}

//
// End of "$Id: fl_vertex.cxx,v 1.5.2.3.2.5 2002/01/01 15:11:32 easysw Exp $".
//
