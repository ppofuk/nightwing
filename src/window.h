#ifndef NIGHTWING_WINDOW_H_
#define NIGHTWING_WINDOW_H_

#include <xcb/xcb.h>
#include "rect.h"

namespace nightwing {

// This a base class for meta informations and properties on Window.
// It's designed to get easy access and manipulation of properties. 
// Each window is identified by its |id_|. 
//
// 
class Window {
 public:
  Window(xcb_window_t id);
  
  Window* Parent() { return parent_; }
  
  void set_parent(Window* parent) { parent_ = parent; }
  
  // Getters and setters
  int get_border_width() const { return border_width_; }

  void set_border_width(int border_width) { 
    border_width_ = border_width; 
  }

  // Returns the current geometrical properties
  Rect get_rect() const { return rect_; }

  void set_rect(const Rect rect) { rect_ = rect; }

  xcb_window_t get_id() const { return id_; }

  operator xcb_window_t() const {
    return id_; 
  }

 protected:
  xcb_window_t id_; 
  int border_width_; 
  Rect rect_; 

  Window* parent_; 
  
 private:
};

} // namespace nightwing
#endif // NIGHTWING_WINDOW_H_ 
