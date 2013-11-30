#include "window.h"

namespace nightwing {

Window::Window(xcb_window_t id) : id_(id), visiable_(true) {
  
}

} // namespace nightwing
