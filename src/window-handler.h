#ifndef NIGHTWING_WINDOW_HANDLER_H_
#define NIGHTWING_WINDOW_HANDLER_H_

#include "build.h"
#include "type-helpers.h"
#include "window.h"
#include <xcb/xcb.h>
#include <stdio.h>
// TOOD: some compilers don't support this type of hash map
//       so we need somekind of abstraction or macroing.
#include <tr1/unordered_map>

namespace nightwing {

// WindowHandler class servers as a hash map of all Windows, we can
// access coresponding Window instances based on xcb_window_t id. 
class WindowHandler {
 public:
  friend class nightwing::Window; 
  
  WindowHandler();
  
  // Return |Window| with specified |id|
  // Returns NULL if the requested Window does not exist. 
  Window* Get(xcb_window_t id);
  
  // Add a window to window map. 
  void Add(Window* window); 

  // Removes a Window with specified |id| from the map.
  // Pointer to removed window is given. If the window is on heap, 
  // and you won't be needing it you can do something like:
  //   delete window_handler.Remove(some_id);
  Window* Remove(xcb_window_t id);

  // Apply Window properties 
  void ApplyProperties(Window* window);
  
  xcb_connection_t* get_dpy() { return dpy_; }
  
  void set_dpy(xcb_connection_t* dpy) { dpy_ = dpy; }

 private:
  xcb_connection_t* dpy_;
  std::tr1::unordered_map<xcb_window_t, Window*> window_map_; 
  
  DISALLOW_COPY_AND_ASSIGN(WindowHandler);
};

} // namespace nightwing
#endif // NIGHTWING_WINDOW_HANDLER_H_
