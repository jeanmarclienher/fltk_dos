//
// "$Id: output.cxx,v 1.4.2.3.2.2 2002/08/20 16:15:29 easysw Exp $"
//
// Output test program for the Fast Light Tool Kit (FLTK).
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

#include <FL/Fl.H>
#include <FL/Fl_Value_Input.H> // necessary for bug in mingw32?
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Hor_Value_Slider.H>
#include <FL/Fl_Toggle_Button.H>
#include <FL/Fl_Input.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Multiline_Output.H>

Fl_Output *text;
Fl_Multiline_Output *text2;
Fl_Input *input;
Fl_Value_Slider *fonts;
Fl_Value_Slider *sizes;
Fl_Window *window;

void font_cb(Fl_Widget *,void *) {
  text->textfont(int(fonts->value()));
  text->redraw();
  text2->textfont(int(fonts->value()));
  text2->redraw();
}

void size_cb(Fl_Widget *,void *) {
  text->textsize(int(sizes->value()));
  text->redraw();
  text2->textsize(int(sizes->value()));
  text2->redraw();
}

void input_cb(Fl_Widget *,void *) {
  text->value(input->value());
  text2->value(input->value());
}

int main(int argc, char **argv) {
  window = new Fl_Window(400,400);

  input = new Fl_Input(50,0,350,25);
  input->static_value("The quick brown fox\njumped over\nthe lazy dog.");
  input->when(FL_WHEN_CHANGED);
  input->callback(input_cb);

  sizes = new Fl_Hor_Value_Slider(50,25,350,25,"Size");
  sizes->align(FL_ALIGN_LEFT);
  sizes->bounds(1,64);
  sizes->step(1);
  sizes->value(14);
  sizes->callback(size_cb);

  fonts = new Fl_Hor_Value_Slider(50,50,350,25,"Font");
  fonts->align(FL_ALIGN_LEFT);
  fonts->bounds(0,15);
  fonts->step(1);
  fonts->value(0);
  fonts->callback(font_cb);

  text2 = new Fl_Multiline_Output(100,150,200,100,"Fl_Multiline_Output");
  text2->value(input->value());
  text2->align(FL_ALIGN_BOTTOM);
  text2->tooltip("This is an Fl_Multiline_Output widget.");
  window->resizable(text2);

  text = new Fl_Output(100,280,200,30,"Fl_Output");
  text->value(input->value());
  text->align(FL_ALIGN_BOTTOM);
  text->tooltip("This is an Fl_Output widget.");

  window->forms_end();
  window->show(argc,argv);
  return Fl::run();
}

//
// End of "$Id: output.cxx,v 1.4.2.3.2.2 2002/08/20 16:15:29 easysw Exp $".
//
