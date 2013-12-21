#include "window.h"

namespace nightwing {

Window::Window(xcb_window_t id) : id_(id), 
                                  decorator_(NULL),
                                  parent_(NULL), 
                                  visiable_(true),
                                  type_(kNormal) {
  
}

} // namespace nightwing
