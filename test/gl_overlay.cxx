//
// "$Id: gl_overlay.cxx,v 1.4.2.3.2.1 2002/01/01 15:11:33 easysw Exp $"
//
// OpenGL overlay test program for the Fast Light Tool Kit (FLTK).
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

#include <config.h>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Hor_Slider.H>
#include <FL/Fl_Toggle_Button.H>
#include <FL/math.h>

#if !HAVE_GL
#include <FL/Fl_Box.H>
class shape_window : public Fl_Box {
public:	
  int sides;
  shape_window(int x,int y,int w,int h,const char *l=0)
    :Fl_Box(FL_DOWN_BOX,x,y,w,h,l){
      label("This demo does\nnot work without GL");
  }
};
#else
#include <FL/gl.h>
#include <FL/Fl_Gl_Window.H>

class shape_window : public Fl_Gl_Window {
  void draw();
  void draw_overlay();
public:
  int sides;
  int overlay_sides;
  shape_window(int x,int y,int w,int h,const char *l=0);
};

shape_window::shape_window(int x,int y,int w,int h,const char *l) :
Fl_Gl_Window(x,y,w,h,l) {
  sides = overlay_sides = 3;
}

void shape_window::draw() {
// the valid() property may be used to avoid reinitializing your
// GL transformation for each redraw:
  if (!valid()) {
    valid(1);
    glLoadIdentity();
    glViewport(0,0,w(),h());
  }
// draw an amazing but slow graphic:
  glClear(GL_COLOR_BUFFER_BIT);
  //  for (int j=1; j<=1000; j++) {
    glBegin(GL_POLYGON);
    for (int i=0; i<sides; i++) {
      double ang = i*2*M_PI/sides;
      glColor3f(float(i)/sides,float(i)/sides,float(i)/sides);
      glVertex3f(cos(ang),sin(ang),0);
    }
    glEnd();
  // }
}

void shape_window::draw_overlay() {
// the valid() property may be used to avoid reinitializing your
// GL transformation for each redraw:
  if (!valid()) {
    valid(1);
    glLoadIdentity();
    glViewport(0,0,w(),h());
  }
// draw an amazing graphic:
  gl_color(FL_RED);
  glBegin(GL_LINE_LOOP);
  for (int i=0; i<overlay_sides; i++) {
    double ang = i*2*M_PI/overlay_sides;
    glVertex3f(cos(ang),sin(ang),0);
  }
  glEnd();
}
#endif

// when you change the data, as in this callback, you must call redraw():
void sides_cb(Fl_Widget *o, void *p) {
  shape_window *sw = (shape_window *)p;
  sw->sides = int(((Fl_Slider *)o)->value());
  sw->redraw();
}

#if HAVE_GL
void overlay_sides_cb(Fl_Widget *o, void *p) {
  shape_window *sw = (shape_window *)p;
  sw->overlay_sides = int(((Fl_Slider *)o)->value());
  sw->redraw_overlay();
}
#endif
#include <stdio.h>
int main(int argc, char **argv) {

  Fl_Window window(300, 370);

  shape_window sw(10, 75, window.w()-20, window.h()-90);
//sw.mode(FL_RGB);
  window.resizable(&sw);

  Fl_Hor_Slider slider(60, 5, window.w()-70, 30, "Sides:");
  slider.align(FL_ALIGN_LEFT);
  slider.callback(sides_cb,&sw);
  slider.value(sw.sides);
  slider.step(1);
  slider.bounds(3,40);

  Fl_Hor_Slider oslider(60, 40, window.w()-70, 30, "Overlay:");
  oslider.align(FL_ALIGN_LEFT);
#if HAVE_GL
  oslider.callback(overlay_sides_cb,&sw);
  oslider.value(sw.overlay_sides);
#endif
  oslider.step(1);
  oslider.bounds(3,40);

  window.end();
  window.show(argc,argv);
#if HAVE_GL
  printf("Can do overlay = %d\n", sw.can_do_overlay());
  sw.show();
  sw.redraw_overlay();
#else
  sw.show();
#endif

  return Fl::run();
}

//
// End of "$Id: gl_overlay.cxx,v 1.4.2.3.2.1 2002/01/01 15:11:33 easysw Exp $".
//
