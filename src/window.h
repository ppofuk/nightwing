#ifndef NIGHTWING_WINDOW_H_
#define NIGHTWING_WINDOW_H_

#include <xcb/xcb.h>
#include "rect.h"

namespace nightwing {

class WindowHandler; 
class Session; 
// This a base class for meta informations and properties on Window.
// It's designed to get easy access and manipulation of properties. 
// Each window is identified by its |id_|. 
//
// Basic geometrical properties (x, y, width, height) are contained 
// in |rect_|. See rect.h for more informatio about this structure. 
//
// Applying properties on X can be done with WindowHandler class. 
// Ref to window-handler.h. 
class Window {
 public:
  friend class WindowHandler; 
  friend class Session; 

  Window(xcb_window_t id);
  
  Window* get_parent() { return parent_; }
  
  void set_parent(Window* parent) { parent_ = parent; }
  
  int get_border_width() const { return border_width_; }

  void set_border_width(int border_width) { 
    border_width_ = border_width; 
  }

  Rect get_rect() { return rect_; }

  void set_rect(const Rect rect) { rect_ = rect; }

  xcb_window_t get_id() const { return id_; }
  
  bool is_visiable() { return visiable_; }
  
  bool Map() { visiable_ = true; }

  bool Unmap() { visiable_ = false; }

  operator xcb_window_t() const {
    return id_; 
  }

 protected:
  xcb_window_t id_; 
  int border_width_; 
  Rect rect_; 

  Window* parent_; 
  bool visiable_; 

 private:
};

} // namespace nightwing
#endif // NIGHTWING_WINDOW_H_ 
