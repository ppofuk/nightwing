#ifndef NIGHTWING_DECORATOR_H_
#define NIGHTWING_DECORATOR_H_

#include "build.h"
#include "window.h"
#include "drawing-surface.h"

namespace nightwing {

// Decorator is an special Window type.
class Decorator : public Window {
 public:
  Decorator(xcb_window_t& id);
  virtual ~Decorator();

  void OnExpose();

  void set_target(Window* target) { target_ = target; }

  Window* get_target() { return target_; }

  DrawingSurface& get_surface() { return surface_; }

  // This method will apply position and size to decorator's |rect_|,
  // and to targets |rect_|, where target is a window being decorated.
  virtual void ApplyRects(Rect rect);

  // Same as ApplyRects without changing the decorators rect.
  virtual void ApplyRect();

  virtual Point TargetTopLeft();

 private:
  Window* target_;
  DrawingSurface surface_;
};

} // namespace nightwing
#endif // NIGHTWING_DECORATOR_H_
