#include "decorator.h"

namespace nightwing {

Decorator::Decorator(xcb_window_t& id) : Window(id) {
  type_ = kDecorator; 
}

Decorator::~Decorator() {
  surface_.Destroy(); 
}

void Decorator::OnExpose() {
  if (visiable_) {
    surface_.Update(); 
    DEBUG("Expose()"); 

    cairo_t* cr = surface_.get_cr(); 

    cairo_translate(cr, 0, 7);
    cairo_set_source_rgb(cr, 0.6, 1.0, 0);
    cairo_paint(cr);
  }
}

void Decorator::ApplyRects(Rect rect) {
  // This need's to be updated based on what we draw. 
  rect_ = rect; 
  Rect target_rect = Rect(rect_.x() + 4, 
                          rect_.y() + 4,
                          rect_.width() - 8, 
                          rect_.height() - 8);

  target_->set_rect(target_rect);
}

void Decorator::ApplyRect() {
  Rect target_rect = Rect(rect_.x() + 4, 
                          rect_.y() + 4,
                          rect_.width() - 8, 
                          rect_.height() - 8);

  target_->set_rect(target_rect);
}


} // namespace nightwing 
