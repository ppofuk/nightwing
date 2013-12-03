#include "drawing-surface.h"

namespace nightwing {

DrawingSurface::DrawingSurface() : surface_(NULL), cr_(NULL) {
  
}

void DrawingSurface::Init(xcb_drawable_t drawable, 
                          xcb_visualtype_t* visual_type,
                          xcb_connection_t* dpy) {
  if (rect_.width() == rect_.height()) {
    ERROR("rect_ no set. Forgot to set_rect()?"); 
    return; 
  }
  
  surface_ = cairo_xcb_surface_create(dpy, 
                                      drawable,
                                      visual_type,
                                      rect_.width(),
                                      rect_.height());
  if (surface_) {
    cr_ = cairo_create(surface_); 
  }
}

void DrawingSurface::Destroy() {
  if (surface_) {
    cairo_destroy(cr_); 
    cairo_surface_destroy(surface_); 
  }
}

void DrawingSurface::Update() {
  if (cr_)
    cairo_xcb_surface_set_size(surface_, rect_.width(), 
                               rect_.height()); 
}

} // namespace nightwing
