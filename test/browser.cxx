//
// "$Id: browser.cxx,v 1.5.2.6.2.4 2002/01/01 15:11:32 easysw Exp $"
//
// Browser test program for the Fast Light Tool Kit (FLTK).
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

/*
This is a test of how the browser draws lines.
This is a second line.
This is a third.

That was a blank line above this.

@r@_Right justify
@c@_Center justify
@_Left justify

@bBold text
@iItalic text
@b@iBold Italic
@fFixed width
@f@bBold Fixed
@f@iItalic Fixed
@f@i@bBold Italic Fixed
@lLarge
@l@bLarge bold
@sSmall
@s@bSmall bold
@s@iSmall italic
@s@i@bSmall italic bold
@uunderscore
@C1RED
@C2Green
@C4Blue

	You should try different browser types:
	Fl_Browser
	Fl_Select_Browser
	Fl_Hold_Browser
	Fl_Multi_Browser
*/

#include <FL/Fl.H>
#include <FL/Fl_Select_Browser.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Int_Input.H>
#include <FL/fl_ask.H>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

Fl_Select_Browser *browser;
Fl_Button	*top,
		*bottom,
		*middle,
		*visible;
Fl_Int_Input	*field;

void b_cb(Fl_Widget* o, void*) {
  printf("callback, selection = %d, event_clicks = %d\n",
	 ((Fl_Browser*)o)->value(), Fl::event_clicks());
}

void show_cb(Fl_Widget *o, void *) {
  int line = atoi(field->value());

  if (!line) {
    fl_alert("Please enter a number in the text field\n"
             "before clicking on the buttons.");
    return;
  }

  if (o == top)
    browser->topline(line);
  else if (o == bottom)
    browser->bottomline(line);
  else if (o == middle)
    browser->middleline(line);
  else
    browser->make_visible(line);
}

int main(int argc, char **argv) {
  int i;
  if (!Fl::args(argc,argv,i)) Fl::fatal(Fl::help);
  const char* fname = (i < argc) ? argv[i] : "browser.cxx";
  Fl_Window window(400,400,fname);
  browser = new Fl_Select_Browser(0,0,400,350,0);
  browser->type(FL_MULTI_BROWSER);
  //browser->type(FL_HOLD_BROWSER);
  //browser->color(42);
  browser->callback(b_cb);
  // browser->scrollbar_right();
  //browser->has_scrollbar(Fl_Browser::BOTH_ALWAYS);
  if (!browser->load(fname)) {
#ifdef _MSC_VER
    // if 'browser' was started from the VisualC environment in Win32, 
    // the current directory is set to the environment itself, 
    // so we need to correct the browser file path
    int done = 1;
    if ( i == argc ) 
    {
      fname = "../test/browser.cxx";
      done = browser->load(fname);
    }
    if ( !done )
    {
      printf("Can't load %s, %s\n", fname, strerror(errno));
      exit(1);
    }
#else
    printf("Can't load %s, %s\n", fname, strerror(errno));
    exit(1);
#endif
  }
  browser->position(0);

  field = new Fl_Int_Input(50, 350, 350, 25, "Line #:");
  field->callback(show_cb);

  top = new Fl_Button(0, 375, 100, 25, "Top");
  top->callback(show_cb);

  bottom = new Fl_Button(100, 375, 100, 25, "Bottom");
  bottom->callback(show_cb);

  middle = new Fl_Button(200, 375, 100, 25, "Middle");
  middle->callback(show_cb);

  visible = new Fl_Button(300, 375, 100, 25, "Make Vis.");
  visible->callback(show_cb);

  window.resizable(browser);
  window.show(argc,argv);
  return Fl::run();
}

//
// End of "$Id: browser.cxx,v 1.5.2.6.2.4 2002/01/01 15:11:32 easysw Exp $".
//

