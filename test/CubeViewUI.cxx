// generated by Fast Light User Interface Designer (fluid) version 1.0100

#include "CubeViewUI.h"

inline void CubeViewUI::cb_vrot_i(Fl_Roller* o, void*) {
  cube->v_angle(((Fl_Roller *)o)->value());
cube->redraw();
}
void CubeViewUI::cb_vrot(Fl_Roller* o, void* v) {
  ((CubeViewUI*)(o->parent()->parent()->parent()->user_data()))->cb_vrot_i(o,v);
}

inline void CubeViewUI::cb_ypan_i(Fl_Slider* o, void*) {
  cube->pany(((Fl_Slider *)o)->value());
cube->redraw();
}
void CubeViewUI::cb_ypan(Fl_Slider* o, void* v) {
  ((CubeViewUI*)(o->parent()->parent()->parent()->user_data()))->cb_ypan_i(o,v);
}

inline void CubeViewUI::cb_xpan_i(Fl_Slider* o, void*) {
  cube->panx(((Fl_Slider *)o)->value());
cube->redraw();
}
void CubeViewUI::cb_xpan(Fl_Slider* o, void* v) {
  ((CubeViewUI*)(o->parent()->parent()->parent()->user_data()))->cb_xpan_i(o,v);
}

inline void CubeViewUI::cb_hrot_i(Fl_Roller* o, void*) {
  cube->h_angle(((Fl_Roller *)o)->value());
cube->redraw();
}
void CubeViewUI::cb_hrot(Fl_Roller* o, void* v) {
  ((CubeViewUI*)(o->parent()->parent()->parent()->user_data()))->cb_hrot_i(o,v);
}

inline void CubeViewUI::cb_zoom_i(Fl_Value_Slider* o, void*) {
  cube->size=((Fl_Value_Slider *)o)->value();
cube->redraw();
}
void CubeViewUI::cb_zoom(Fl_Value_Slider* o, void* v) {
  ((CubeViewUI*)(o->parent()->parent()->user_data()))->cb_zoom_i(o,v);
}

CubeViewUI::CubeViewUI() {
  Fl_Double_Window* w;
  { Fl_Double_Window* o = mainWindow = new Fl_Double_Window(415, 405, "CubeView");
    w = o;
    o->box(FL_UP_BOX);
    o->labelsize(12);
    o->user_data((void*)(this));
    { Fl_Group* o = new Fl_Group(5, 3, 374, 399);
      { Fl_Group* o = VChange = new Fl_Group(5, 100, 37, 192);
        { Fl_Roller* o = vrot = new Fl_Roller(5, 100, 17, 186, "V Rot");
          o->labeltype(FL_NO_LABEL);
          o->labelsize(12);
          o->minimum(-180);
          o->maximum(180);
          o->step(1);
          o->callback((Fl_Callback*)cb_vrot);
          o->align(FL_ALIGN_WRAP);
        }
        { Fl_Slider* o = ypan = new Fl_Slider(25, 100, 17, 186, "V Pan");
          o->type(4);
          o->selection_color(136);
          o->labeltype(FL_NO_LABEL);
          o->labelsize(12);
          o->minimum(-25);
          o->maximum(25);
          o->step(0.1);
          o->callback((Fl_Callback*)cb_ypan);
          o->align(FL_ALIGN_CENTER);
        }
        o->end();
      }
      { Fl_Group* o = HChange = new Fl_Group(120, 362, 190, 40);
        { Fl_Slider* o = xpan = new Fl_Slider(122, 364, 186, 17, "H Pan");
          o->type(5);
          o->selection_color(136);
          o->labeltype(FL_NO_LABEL);
          o->labelsize(12);
          o->minimum(25);
          o->maximum(-25);
          o->step(0.1);
          o->callback((Fl_Callback*)cb_xpan);
          o->align(FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
        }
        { Fl_Roller* o = hrot = new Fl_Roller(122, 383, 186, 17, "H Rotation");
          o->type(1);
          o->labeltype(FL_NO_LABEL);
          o->labelsize(12);
          o->minimum(-180);
          o->maximum(180);
          o->step(1);
          o->callback((Fl_Callback*)cb_hrot);
          o->align(FL_ALIGN_RIGHT);
        }
        o->end();
      }
      { Fl_Group* o = MainView = new Fl_Group(46, 27, 333, 333);
        { Fl_Box* o = cframe = new Fl_Box(46, 27, 333, 333);
          o->box(FL_DOWN_FRAME);
          o->color(4);
          o->selection_color(69);
        }
        { CubeView* o = cube = new CubeView(48, 29, 329, 329, "This is the cube_view");
          o->align(FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
        }
        o->end();
      }
      { Fl_Value_Slider* o = zoom = new Fl_Value_Slider(106, 3, 227, 19, "Zoom");
        o->type(5);
        o->selection_color(136);
        o->labelfont(1);
        o->labelsize(12);
        o->minimum(1);
        o->maximum(50);
        o->step(0.1);
        o->value(10);
        o->textfont(1);
        o->callback((Fl_Callback*)cb_zoom);
        o->align(FL_ALIGN_LEFT);
      }
      o->end();
    }
    o->end();
    o->resizable(o);
  }
}

void CubeViewUI::show(int argc, char **argv) {
  mainWindow->show(argc, argv);
}
