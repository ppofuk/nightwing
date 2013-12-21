#ifndef NIGHTWING_WINDOW_HANDLER_H_
#define NIGHTWING_WINDOW_HANDLER_H_

#include "build.h"
#include "type-helpers.h"
#include "window.h"
#include "decorator.h"

#include <xcb/xcb.h>
#include <stdio.h>
// TOOD: some compilers don't support this type of hash map
//       so we need somekind of abstraction.
#include <tr1/unordered_map>

namespace nightwing {

// WindowHandler class servers as a hash map of all Windows, we can
// access coresponding Window instances based on xcb_window_t id. 
// It's also servers as an bridge between window model abstraction, 
// and xcb correspondent execution. 
class WindowHandler {
 public:
  WindowHandler();
  
  // Return |Window| with specified |id|
  // Returns NULL if the requested Window does not exist. 
  // Every XCB window is uniquely identified with it's window id, 
  // which is of type xcb_window_t. 
  // Having all windows mapped to their correspondent abstraction in 
  // form of \Window| class instance, allows easier manipulation and 
  // usage. 
  Window* Get(xcb_window_t id);
  
  // Add a window to window map. 
  void Add(Window* window); 

  // Removes a Window with specified |id| from the map.
  // Pointer to removed window is given. If the window is on heap, 
  // and you won't be needing it you can do something like:
  //   delete window_handler.Remove(event->window);
  Window* Remove(xcb_window_t id);
  
  // Returns true if a window with id is in window map.
  bool Exists(xcb_window_t id);

  // Apply Window properties. 
  // For now, the properties that take effect are all properties of 
  // |Window::rect_| (accessable with |Window::get_rect()|; see rect.h
  // for more deteiles) and |Window::border_size_|. 
  void ApplyProperties(Window* window);
  
  // Apply Window visiablity to screen. Window is visiable if 
  // |Window::is_visiable()| method yields true. 
  // The visiablity can be controlled with methods |Window::Show()|, 
  // and |Window::Hide()|
  void ApplyVisiablity(Window* window); 

  // Calls all Apply... methods. It's just for estetic. 
  void Apply(Window* window); 

  // This will send and xcb event that reposnds to configure request,
  // which tells application that it has configured it self correctly.
  // This should be used in conjuction with configure request event
  // handling. 
  void SendConfigureNotify(Window* window);
  
  // Create a new window based on meta-infomrations provided in 
  // |window|
  void CreateWindow(Window* window); 
  
  // Raise the |window| on top. 
  void Raise(Window* window); 

  // Destroy's the |window|. 
  void Destroy(Window* window); 

  xcb_connection_t* get_dpy() { return dpy_; }
  
  // It's important to call this methods correctly before facilitating 
  // this class.
  void set_dpy(xcb_connection_t* dpy) { dpy_ = dpy; }

  void set_screen(xcb_screen_t* screen) { screen_ = screen; }

 private:
  xcb_connection_t* dpy_;
  xcb_screen_t* screen_; 
  std::tr1::unordered_map<xcb_window_t, Window*> window_map_; 
  
  DISALLOW_COPY_AND_ASSIGN(WindowHandler);
};

} // namespace nightwing
#endif // NIGHTWING_WINDOW_HANDLER_H_
