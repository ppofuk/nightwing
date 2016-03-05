#include "window-handler.h"

namespace nightwing {

WindowHandler::WindowHandler() : dpy_(NULL), screen_(NULL) {
}

void WindowHandler::Add(Window* window) {
  window_map_.insert(
      std::pair<xcb_window_t, Window*>(window->get_id(), window));
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

void WindowHandler::ApplyProperties(Window* window) {
  if (!dpy_) {
    // TODO: notification and log dpy_ fail!
    ERROR("dpy_ not set! Forgot to set_dpy()?");
    return;
  }

  xcb_configure_window(dpy_,
                       window->get_id(),
                       XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y |
                           XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT,
                       window->get_rect() - window->get_border_width() * 2);

  uint32_t border_width = window->get_border_width();
  xcb_configure_window(
      dpy_, window->id_, XCB_CONFIG_WINDOW_BORDER_WIDTH, &border_width);

  DEBUG("Applying properties to %d: {%d, %d, %d, %d}, %d",
        window->id_,
        window->rect_.x(),
        window->rect_.y(),
        window->rect_.width(),
        window->rect_.height(),
        border_width);

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
    DEBUG("ApplyVisiablity: mapping %d.", window->id_);
  } else {
    DEBUG("ApplyVisiablity: unmapping %d.", window->id_);
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

  DEBUG("Sending ConfigureNotify to %d.", window->id_);
  xcb_send_event(dpy_,
                 0,
                 window->id_,
                 XCB_EVENT_MASK_STRUCTURE_NOTIFY,
                 (char*)((void*)&response));  // Run children ruuun!!
  xcb_flush(dpy_);
}

void WindowHandler::SendExposeEvent(Window* window) {
  xcb_expose_event_t event;
  event.response_type = XCB_EXPOSE;  // Pretty sure it's not needed
  event.x = window->rect_.x();
  event.y = window->rect_.y();
  event.width = window->rect_.width();
  event.height = window->rect_.height();
  event.window = window->id_;

  DEBUG("Sending ExposeEvent to %d", window->id_);

  xcb_send_event(
      dpy_, 0, window->id_, XCB_EVENT_MASK_EXPOSURE, (char*)((void*)&event));
  xcb_flush(dpy_);
}

void WindowHandler::SendExposeEventToAll() {
  std::tr1::unordered_map<xcb_window_t, Window*>::iterator it =
      window_map_.begin();

  for (; it != window_map_.end(); it++) {
    Window* window = (*it).second;
    SendExposeEvent((*it).second);
    if (window->get_type() == kNormal) {
      Raise(window);
    }
  }
}

void WindowHandler::CreateWindow(Window* window) {
  if (screen_ == NULL) {
    ERROR("screen_ not set! Forgot set_screen()?");
    return;
  }

  uint32_t value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
  uint32_t values[2] = {
      screen_->black_pixel,
      XCB_EVENT_MASK_STRUCTURE_NOTIFY | XCB_EVENT_MASK_EXPOSURE};
  if (window->type_ == kDecorator) {
    values[1] = NIGHTWING_DECORATOR_MASK;
  }

  DEBUG("Creating new window id: %d", window->id_);

  xcb_create_window(dpy_,
                    XCB_COPY_FROM_PARENT,
                    window->id_,
                    window->parent_->id_,
                    window->rect_.x(),
                    window->rect_.y(),
                    window->rect_.width(),
                    window->rect_.height(),
                    window->border_width_,
                    XCB_WINDOW_CLASS_INPUT_OUTPUT,
                    screen_->root_visual,
                    value_mask,
                    values);
  xcb_flush(dpy_);
}

void WindowHandler::Raise(Window* window) {
  const uint32_t values[] = {XCB_STACK_MODE_ABOVE};
  if (window->get_decorator()) {
    xcb_configure_window(
        dpy_, window->decorator_->id_, XCB_CONFIG_WINDOW_STACK_MODE, values);
  } else {
    xcb_configure_window(
        dpy_, window->id_, XCB_CONFIG_WINDOW_STACK_MODE, values);
  }
  xcb_flush(dpy_);
}

void WindowHandler::SetInputFocus(Window* window) {
  xcb_set_input_focus(
      dpy_, XCB_INPUT_FOCUS_POINTER_ROOT, window->id_, XCB_TIME_CURRENT_TIME);
  xcb_flush(dpy_);
}

void WindowHandler::Destroy(Window* window) {
  DEBUG("Destroying %d.", window->id_);

  xcb_destroy_window(dpy_, window->id_);
  xcb_flush(dpy_);
}

void WindowHandler::Reparent(Window* window, Point top_left) {
  DEBUG("Reparenting (child: %d) with (parent: %d)",
        window->id_,
        window->parent_->id_);

  uint32_t values[] = {XCB_NONE};
  xcb_change_window_attributes(dpy_, window->id_, XCB_CW_EVENT_MASK, values);

  xcb_reparent_window(
      dpy_, window->id_, window->parent_->id_, top_left.x(), top_left.y());

  values[0] = NIGHTWING_PARENTED_MASK & ~XCB_EVENT_MASK_ENTER_WINDOW;
  xcb_change_window_attributes(dpy_, window->id_, XCB_CW_EVENT_MASK, values);
  xcb_flush(dpy_);

  xcb_change_save_set(dpy_, XCB_SET_MODE_INSERT, window->id_);
}

void WindowHandler::UpdateWindowName(Window* window) {
  xcb_get_property_cookie_t cookie;
  xcb_get_property_reply_t* reply;

  cookie = xcb_get_property(
      dpy_, 0, window->get_id(), XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 0, 0);
  reply = xcb_get_property_reply(dpy_, cookie, NULL);

  if (reply) {
    DEBUG("UpdateWindowName to %s", (char*)xcb_get_property_value(reply));
    if (xcb_get_property_value_length(reply)) {
      window->set_title(static_cast<char*>(xcb_get_property_value(reply)));
    } else {
      window->set_title("\0");
    }

    free(reply);
  } else {
    window->set_title("\0");
  }
}

}  // namespace nightwing
