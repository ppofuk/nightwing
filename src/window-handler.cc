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

bool WindowHandler::Exists(xcb_window_t id) {
  return window_map_.find(id) != window_map_.end();
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
                       window->get_rect() - 
                       window->get_border_width() * 2);

  uint32_t border_width = window->get_border_width(); 
  xcb_configure_window(dpy_, 
                       window->id_, 
                       XCB_CONFIG_WINDOW_BORDER_WIDTH,
                       &border_width);
  
  xcb_flush(dpy_);
}

void WindowHandler::ApplyVisiablity(Window* window) {
  if (!dpy_) {
    // TODO: notification and log dpy_ fail!
    ERROR("dpy_ not set! Forgot to set_dpy()?");
    return; 
  }
  
  if (window->is_visiable()) {
    xcb_map_window(dpy_, window->id_);
  } else {
    xcb_unmap_window(dpy_, window->id_);
  }

  xcb_flush(dpy_);
}

void WindowHandler::Apply(Window* window) {
  ApplyProperties(window);
  ApplyVisiablity(window);
}

void WindowHandler::SendConfigureNotify(Window* window) {
  xcb_configure_notify_event_t response; 

  response.response_type = XCB_CONFIGURE_NOTIFY; 
  response.event = response.window = window->get_id(); 
  response.x = window->get_rect().x(); 
  response.y = window->get_rect().y(); 
  response.width = window->get_rect().width(); 
  response.height = window->get_rect().height(); 
  response.border_width = window->get_border_width();
  response.above_sibling = XCB_NONE; 
  response.override_redirect = 0; 
  
  xcb_send_event(dpy_,
                 0,
                 window->id_,
                 XCB_EVENT_MASK_STRUCTURE_NOTIFY,
                 (char *)((void *)&response)); // Run children ruuun!!
  xcb_flush(dpy_); 
}

} // namespace nightwing
