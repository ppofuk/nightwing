#include "window-handler.h"

namespace nightwing {

WindowHandler::WindowHandler() : dpy_(NULL) {

}

void WindowHandler::Add(Window *window) {
  window_map_.insert(std::pair<xcb_window_t, Window*>(window->get_id(),
                                                      window));
}

Window* WindowHandler::Remove(xcb_window_t id) {
  Window* window = window_map_[id];
  window_map_.erase(id);
  return window; 
}

Window* WindowHandler::Get(xcb_window_t id) {
  return window_map_[id];
}

void WindowHandler::ApplyProperties(Window *window) {
  if (!dpy_) {
    // TODO: notification and log dpy_ fail!
    ERROR("dpy_ not set! Forgot to set_dpy()?");
    return; 
  }
  xcb_configure_window(dpy_, 
                       window->get_id(), 
                       XCB_CONFIG_WINDOW_X | 
                       XCB_CONFIG_WINDOW_Y |
                       XCB_CONFIG_WINDOW_WIDTH |
                       XCB_CONFIG_WINDOW_HEIGHT, 
                       window->get_rect());
  
  xcb_configure_window(dpy_, 
                       window->get_id(), 
                       XCB_CONFIG_WINDOW_BORDER_WIDTH,
                       window->get_border_width());
}

} // namespace nightwing
