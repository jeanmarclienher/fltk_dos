//
// "$Id: Fl_get_system_colors.cxx,v 1.6.2.7.2.13 2002/08/09 03:17:30 easysw Exp $"
//
// System color support for the Fast Light Tool Kit (FLTK).
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
#include <FL/fl_draw.H>
#include <FL/x.H>
#include <FL/math.h>
#include <FL/fl_utf8.H>
#include "flstring.h"
#include <stdio.h>
#include <stdlib.h>
#include <FL/Fl_Pixmap.H>
#include <FL/Fl_Tiled_Image.H>
#include "tile.xpm"

#if defined(__APPLE__) && defined(__MWERKS__)
extern "C" int putenv(const char*);
#endif // __APPLE__ && __MWERKS__


void Fl::background(uchar r, uchar g, uchar b) {
  // replace the gray ramp so that FL_GRAY is this color
  if (!r) r = 1; else if (r==255) r = 254;
  double powr = log(r/255.0)/log((FL_GRAY-FL_GRAY_RAMP)/(FL_NUM_GRAY-1.0));
  if (!g) g = 1; else if (g==255) g = 254;
  double powg = log(g/255.0)/log((FL_GRAY-FL_GRAY_RAMP)/(FL_NUM_GRAY-1.0));
  if (!b) b = 1; else if (b==255) b = 254;
  double powb = log(b/255.0)/log((FL_GRAY-FL_GRAY_RAMP)/(FL_NUM_GRAY-1.0));
  for (int i = 0; i < FL_NUM_GRAY; i++) {
    double gray = i/(FL_NUM_GRAY-1.0);
    Fl::set_color(fl_gray_ramp(i),
		  uchar(pow(gray,powr)*255+.5),
		  uchar(pow(gray,powg)*255+.5),
		  uchar(pow(gray,powb)*255+.5));
  }
}

void Fl::foreground(uchar r, uchar g, uchar b) {
  Fl::set_color(FL_FOREGROUND_COLOR,r,g,b);
}

void Fl::background2(uchar r, uchar g, uchar b) {
  Fl::set_color(FL_BACKGROUND2_COLOR,r,g,b);
  Fl::set_color(FL_FOREGROUND_COLOR,
                get_color(fl_contrast(FL_FOREGROUND_COLOR,FL_BACKGROUND2_COLOR)));
}

// these are set by Fl::args() and override any system colors:
const char *fl_fg = NULL;
const char *fl_bg = NULL;
const char *fl_bg2 = NULL;

static void set_selection_color(uchar r, uchar g, uchar b) {
  Fl::set_color(FL_SELECTION_COLOR,r,g,b);
}

#if defined(WIN32) || defined(__APPLE__)

#  include <stdio.h>
// simulation of XParseColor:
int fl_parse_color(const char* p, uchar& r, uchar& g, uchar& b) {
  if (*p == '#') p++;
  int n = strlen(p);
  int m = n/3;
  const char *pattern = 0;
  switch(m) {
  case 1: pattern = "%1x%1x%1x"; break;
  case 2: pattern = "%2x%2x%2x"; break;
  case 3: pattern = "%3x%3x%3x"; break;
  case 4: pattern = "%4x%4x%4x"; break;
  default: return 0;
  }
  int R,G,B; if (sscanf(p,pattern,&R,&G,&B) != 3) return 0;
  switch(m) {
  case 1: R *= 0x11; G *= 0x11; B *= 0x11; break;
  case 3: R >>= 4; G >>= 4; B >>= 4; break;
  case 4: R >>= 8; G >>= 8; B >>= 8; break;
  }
  r = R; g = G; b = B;
  return 1;
}
#else
// Wrapper around XParseColor...
int fl_parse_color(const char* p, uchar& r, uchar& g, uchar& b) {
  XColor x;
  if (!fl_display) fl_open_display();
  if (XParseColor(fl_display, fl_colormap, p, &x)) {
    r = x.red>>8;
    g = x.green>>8;
    b = x.blue>>8;
    return 1;
  } else return 0;
}
#endif // WIN32 || __APPLE__

#if defined(WIN32)
static void
getsyscolor(int what, const char* arg, void (*func)(uchar,uchar,uchar))
{
  if (arg) {
    uchar r,g,b;
    if (!fl_parse_color(arg, r,g,b))
      Fl::error("Unknown color: %s", arg);
    else
      func(r,g,b);
  } else {
    DWORD x = GetSysColor(what);
    func(uchar(x&255), uchar(x>>8), uchar(x>>16));
  }
}

void Fl::get_system_colors() {
  getsyscolor(COLOR_WINDOW,	fl_bg2,Fl::background2);
  getsyscolor(COLOR_WINDOWTEXT,	fl_fg, Fl::foreground);
  getsyscolor(COLOR_BTNFACE,	fl_bg, Fl::background);
  getsyscolor(COLOR_HIGHLIGHT,	0,     set_selection_color);
}

#elif defined(__APPLE__)
// MacOS X currently supports two color schemes - Blue and Graphite.
// Since we aren't emulating the Aqua interface (even if Apple would
// let us), we use some defaults that are similar to both.  The
// Fl::scheme("plastic") color/box scheme provides a usable Aqua-like
// look-n-feel...
void Fl::get_system_colors()
{
  fl_open_display();

  foreground(0, 0, 0);
  background(0xe0, 0xe0, 0xe0);
  background2(0xf0, 0xf0, 0xf0);
  set_selection_color(0x80, 0x80, 0x80);
}
#else

// Read colors that KDE writes to the xrdb database.

// XGetDefault does not do the expected thing: it does not like
// periods in either word. Therefore it cannot match class.Text.background.
// However *.Text.background is matched by pretending the program is "Text".
// But this will also match *.background if there is no *.Text.background
// entry, requiring users to put in both (unless they want the text fields
// the same color as the windows).

static void
getsyscolor(const char *key1, const char* key2, const char *arg, const char *defarg, void (*func)(uchar,uchar,uchar))
{
  if (!arg) {
    arg = XGetDefault(fl_display, key1, key2);
    if (!arg) arg = defarg;
  }
  XColor x;
  if (!XParseColor(fl_display, fl_colormap, arg, &x))
    Fl::error("Unknown color: %s", arg);
  else
    func(x.red>>8, x.green>>8, x.blue>>8);
}

void Fl::get_system_colors()
{
  fl_open_display();
  const char* key1 = 0;
  if (Fl::first_window()) key1 = Fl::first_window()->xclass();
  if (!key1) key1 = "fltk";
  getsyscolor(key1,  "background",	fl_bg,	"#c0c0c0", Fl::background);
  getsyscolor(key1,  "foreground",	fl_fg,	"#000000", Fl::foreground);
  getsyscolor("Text","background",	fl_bg2,	"#ffffff", Fl::background2);
  getsyscolor(key1,  "selectBackground",0,	"#000080", set_selection_color);
}

#endif


//// Simple implementation of 2.0 Fl::scheme() interface...
#define D1 BORDER_WIDTH
#define D2 (BORDER_WIDTH+BORDER_WIDTH)

extern void	fl_up_box(int, int, int, int, Fl_Color);
extern void	fl_down_box(int, int, int, int, Fl_Color);
extern void	fl_thin_up_box(int, int, int, int, Fl_Color);
extern void	fl_thin_down_box(int, int, int, int, Fl_Color);
extern void	fl_round_up_box(int, int, int, int, Fl_Color);
extern void	fl_round_down_box(int, int, int, int, Fl_Color);

extern void	fl_up_frame(int, int, int, int, Fl_Color);
extern void	fl_down_frame(int, int, int, int, Fl_Color);
extern void	fl_thin_up_frame(int, int, int, int, Fl_Color);
extern void	fl_thin_down_frame(int, int, int, int, Fl_Color);

const char	*Fl::scheme_ = (const char *)0;
Fl_Image	*Fl::scheme_bg_ = (Fl_Image *)0;

static Fl_Pixmap	tile(tile_xpm);

int Fl::scheme(const char *s) {
  if (!s) {
    if ((s = fl_getenv("FLTK_SCHEME")) == NULL) {
#if !defined(WIN32) && !defined(__APPLE__)
      const char* key = 0;
      if (Fl::first_window()) key = Fl::first_window()->xclass();
      if (!key) key = "fltk";
      fl_open_display();
      s = XGetDefault(fl_display, key, "scheme");
#endif // !WIN32 && !__APPLE__
    }
  }

  if (s) {
    if (!strcasecmp(s, "none") || !strcasecmp(s, "base") || !*s) s = 0;
    else s = strdup(s);
  }
  if (scheme_) free((void*)scheme_);
  scheme_ = s;

  // Save the new scheme in the FLTK_SCHEME env var so that child processes
  // inherit it...
  static char e[1024];
  strcpy(e,"FLTK_SCHEME=");
  if (s) strlcat(e,s,sizeof(e));
  putenv(e);

  // Load the scheme...
  return reload_scheme();
}

int Fl::reload_scheme() {
  Fl_Window *win;

  get_system_colors();

  if (scheme_ && !strcasecmp(scheme_, "plastic")) {
    // Update the tile image to match the background color...
    uchar r, g, b;

    get_color(FL_GRAY, r, g, b);
    sprintf(tile_cmap[0], "O c #%02x%02x%02x", r, g, b);
    sprintf(tile_cmap[1], "o c #%02x%02x%02x", 0xe0 * (int)r / 0xf0,
            0xe0 * (int)g / 0xf0, 0xe0 * (int)b / 0xf0);
    sprintf(tile_cmap[2], ". c #%02x%02x%02x", 0xd8 * (int)r / 0xf0,
            0xd8 * (int)g / 0xf0, 0xd8 * (int)b / 0xf0);

    if (tile.id) {
      fl_delete_offscreen(tile.id);
      tile.id = 0;
    }

    if (tile.mask) {
      fl_delete_bitmask(tile.mask);
      tile.mask = 0;
    }

    if (!scheme_bg_) scheme_bg_ = new Fl_Tiled_Image(&tile, w(), h());

    // Load plastic buttons, etc...
    set_boxtype(FL_UP_FRAME,        FL_PLASTIC_UP_FRAME);
    set_boxtype(FL_DOWN_FRAME,      FL_PLASTIC_DOWN_FRAME);
    set_boxtype(FL_THIN_UP_FRAME,   FL_PLASTIC_UP_FRAME);
    set_boxtype(FL_THIN_DOWN_FRAME, FL_PLASTIC_DOWN_FRAME);

    set_boxtype(FL_UP_BOX,          FL_PLASTIC_UP_BOX);
    set_boxtype(FL_DOWN_BOX,        FL_PLASTIC_DOWN_BOX);
    set_boxtype(FL_THIN_UP_BOX,     FL_PLASTIC_UP_BOX);
    set_boxtype(FL_THIN_DOWN_BOX,   FL_PLASTIC_DOWN_BOX);
    set_boxtype(_FL_ROUND_UP_BOX,   FL_PLASTIC_UP_BOX);
    set_boxtype(_FL_ROUND_DOWN_BOX, FL_PLASTIC_UP_BOX);
  } else {
    // Use the standard FLTK look-n-feel...
    if (scheme_bg_) {
      delete scheme_bg_;
      scheme_bg_ = (Fl_Image *)0;
    }

    set_boxtype(FL_UP_FRAME,        fl_up_frame, D1, D1, D2, D2);
    set_boxtype(FL_DOWN_FRAME,      fl_down_frame, D1, D1, D2, D2);
    set_boxtype(FL_THIN_UP_FRAME,   fl_thin_up_frame, 1, 1, 2, 2);
    set_boxtype(FL_THIN_DOWN_FRAME, fl_thin_down_frame, 1, 1, 2, 2);

    set_boxtype(FL_UP_BOX,          fl_up_box, D1, D1, D2, D2);
    set_boxtype(FL_DOWN_BOX,        fl_down_box, D1, D1, D2, D2);
    set_boxtype(FL_THIN_UP_BOX,     fl_thin_up_box, 1, 1, 2, 2);
    set_boxtype(FL_THIN_DOWN_BOX,   fl_thin_down_box, 1, 1, 2, 2);
    set_boxtype(_FL_ROUND_UP_BOX,   fl_round_up_box, 3, 3, 6, 6);
    set_boxtype(_FL_ROUND_DOWN_BOX, fl_round_down_box, 3, 3, 6, 6);
  }

  // Set (or clear) the background tile for all windows...
  for (win = first_window(); win; win = next_window(win)) {
    win->labeltype(scheme_bg_ ? FL_NORMAL_LABEL : FL_NO_LABEL);
    win->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE | FL_ALIGN_CLIP);
    win->image(scheme_bg_);
    win->redraw();
  }

  return 1;
}


//
// End of "$Id: Fl_get_system_colors.cxx,v 1.6.2.7.2.13 2002/08/09 03:17:30 easysw Exp $".
//
