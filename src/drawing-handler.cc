#include "drawing-handler.h"

namespace nightwing {

DrawingHandler::DrawingHandler() : visual_type_(NULL) {}

DrawingSurface DrawingHandler::CreateSurface(Window* window) {
  DrawingSurface surface;
  surface.set_rect(window->get_rect());
  surface.Init(window->get_id(), visual_type_, dpy_);
  surface.Update();
  return surface;
}

void DrawingHandler::InitSurface(DrawingSurface& surface, Window* window) {
  if (surface.get_cr()) surface.Destroy();

  surface.set_rect(window->get_rect());
  surface.Init(window->get_id(), visual_type_, dpy_);
  surface.Update();
}

}  // namespace nightwing
