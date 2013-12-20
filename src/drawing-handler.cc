#include "drawing-handler.h"

namespace nightwing {

DrawingHandler::DrawingHandler() : visual_type_(NULL) {
  
}

DrawingSurface DrawingHandler::CreateSurface(Window *window) {
  DrawingSurface surface; 
  surface.Init(window->get_id(), visual_type_, dpy_); 
  surface.set_rect(window->get_rect());
  surface.Update(); 
  return surface; 
}

void DrawingHandler::InitSurface(DrawingSurface* surface, 
                                 Window* window) {
  if (surface->get_cr()) 
    surface->Destroy(); 
  
  surface->Init(window->get_id(), visual_type_, dpy_); 
  surface->set_rect(window->get_rect());
  surface->Update(); 
}



} // namespace nightwing
