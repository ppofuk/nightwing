#include "window.h"

namespace nightwing {

Window::Window(xcb_window_t id) : id_(id), visiable_(true),
                                  type_(kNormal) {
  
}

} // namespace nightwing
