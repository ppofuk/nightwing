#ifndef NIGHTWING_DRAWING_SURFACE_H_
#define NIGHTWING_DRAWING_SURFACE_H_

#include <cairo/cairo-xcb.h>
#include <stdio.h>
#include "rect.h"
#include "build.h"
#include "type-helpers.h"

namespace nightwing {

class DrawingHandler; 

class DrawingSurface {
 public:
  friend class DrawingHandler; 
  DrawingSurface(); 
  
  // Must be called before usage. 
  // In most of the times, you don't need to call this manually. 
  // It's recommended to use |DrawingHandler::CreateSurface()|.
  void Init(xcb_drawable_t drawable, 
            xcb_visualtype_t* visual_type,
            xcb_connection_t* dpy);
  
  // Should be called when there is no more need for drawing surface. 
  void Destroy(); 

  // Should be called when |rect_| has been changed.
  void Update();
  
  Rect get_rect() { return rect_; }

  void set_rect(const Rect rect) { rect_ = rect; }
 
  cairo_t* get_cr() const { return cr_; }
  
  cairo_surface_t* get_surface() { return surface_; }

 private:
  Rect rect_; 
  cairo_surface_t* surface_; 
  cairo_t* cr_;
};

} // namespace nightwing
#endif // NIGHTWING_DRAWING_SURFACE_H_
