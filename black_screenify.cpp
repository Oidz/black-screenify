/* Compile with:
 * g++ black_screenify.cpp -lX11
 */

#include <X11/Xlib.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <string>
#include <iomanip>
#include <unistd.h>
#include <tuple>

/* create the x11 window overlay  */
void build_window();

int main(const int argc, char* const* argv) {

  build_window();
  
  return 0;
}

void build_window(){

  Display *display;

  display = XOpenDisplay(NULL);
  if(display == NULL) {
    fprintf(stderr, "failed X server connection to :0");
    std::cout << "failed to open display\n";
    exit(1);
  }

  // getting basic window info:
  // get default screen of x server:
  int screen_number = DefaultScreen(display);

  // set attrs in the struct
  XSetWindowAttributes x_window_attributes;
  x_window_attributes.override_redirect = True;
  x_window_attributes.do_not_propagate_mask = KeyPressMask;

  x_window_attributes.background_pixel = BlackPixel(display, screen_number);
  
  // OR in the values that correspond to the struct attrs
  int value_bitmask = CWDontPropagate | CWOverrideRedirect |CWBackPixel;

  
  Window win = XCreateWindow(display, RootWindow(display, screen_number), 0, 0,
		      DisplayWidth(display, screen_number),
		      DisplayHeight(display, screen_number),
		      0, CopyFromParent, CopyFromParent, CopyFromParent,
		      value_bitmask, &x_window_attributes);
 
   XSelectInput(display, win, ExposureMask | KeyPressMask | PointerMotionMask
	       | ButtonMotionMask | ButtonPressMask);

  // display will not show up until this is called
  XMapWindow(display, win);

  // don't forget to flush remaining requests to the x server
  // this broke it last time
  XFlush(display);

  while(1) {}
  
  XCloseDisplay(display);
}
