#include "event-handler.h"

namespace nightwing {

EventHandler::EventHandler() : is_init_(false) {
}

void EventHandler::Init(xcb_connection_t* dpy,
                        xcb_screen_t* screen,
                        xcb_visualtype_t* visual_type) {
  // TODO(ppofuk): Check if all of these are needed in EventHandler.
  set_dpy(dpy);
  set_screen(screen);
  set_visual_type(visual_type);

  window_handler_.set_dpy(dpy_);
  window_handler_.set_screen(screen_);
  drawing_handler_.set_dpy(dpy_);
  drawing_handler_.set_visual_type(visual_type_);

  screen_rect_.set_width(screen_->width_in_pixels);
  screen_rect_.set_height(screen_->height_in_pixels);

  dummy_root_ = new Window(screen->root);
  dummy_root_->set_rect(screen_rect_);

  is_init_ = true;
}

void EventHandler::Release() {
  if (is_init()) {
    delete dummy_root_;
    is_init_ = false;
  }
}

void EventHandler::ProcessEvent(xcb_generic_event_t* event) {
  switch (event->response_type & ~0x80) {
    case XCB_DESTROY_NOTIFY:
      DEBUG("DESTROY_NOTIFY event");
      OnDestroyNotify((xcb_destroy_notify_event_t*)event);
      break;

    case XCB_CONFIGURE_REQUEST:
      DEBUG("CONFIGURE_REQUEST event");
      OnConfigureRequest((xcb_configure_request_event_t*)event);
      break;

    case XCB_MAP_REQUEST:
      DEBUG("MAP_REQUEST event");
      OnMapRequest((xcb_map_request_event_t*)event);
      break;

    case XCB_UNMAP_NOTIFY:
      DEBUG("UNMAP_NOTIFY event");
      OnUnmapNotify((xcb_unmap_notify_event_t*)event);
      break;

    case XCB_ENTER_NOTIFY:
      DEBUG("ENTER_NOTIFY event");
      OnEnterNotify((xcb_enter_notify_event_t*)event);
      break;

    case XCB_FOCUS_IN:
      DEBUG("XCB_FOCUS_IN event");
      OnFocusIn((xcb_focus_in_event_t*)event);
      break;

    case XCB_MOTION_NOTIFY:
      // Please, don't DEBUG this out, it's annoying to read one
      // millions lines of motion notify robo-rap sentences.
      OnMotionNotify((xcb_motion_notify_event_t*)event);
      break;

    case XCB_EXPOSE:
      DEBUG("XCB_EXPOSE event");
      OnExpose((xcb_expose_event_t*)event);
      break;

    default:
      // Ignoring unknown event type
      break;
  }
}

void EventHandler::OnDestroyNotify(xcb_destroy_notify_event_t* event) {
  if (window_handler_.Exists(event->window)) {
    DEBUG("OnDestroyNotify window id %d", event->window);

    Window* window = window_handler_.Remove(event->window);
    Decorator* decorator = static_cast<Decorator*>(window->get_decorator());

    if (decorator) {
      DEBUG("Has decorator!");
      window_handler_.Remove(decorator->get_id());
      window_handler_.Destroy(decorator);
      delete decorator;
    }
    delete window;
  }
  //  window_handler_.SendExposeEventToAll();
}

void EventHandler::OnUnmapNotify(xcb_unmap_notify_event_t* event) {
  DEBUG("UnmapNotify on %d", event->window);

  if (window_handler_.Exists(event->window)) {
    DEBUG("Found window %d", event->window);

    Window* window = window_handler_.Get(event->window);
    if (window->get_decorator()) {
      DEBUG("%d has decorator, unmaping it.", event->window);
      window->get_decorator()->Unmap();
      window_handler_.ApplyVisiablity(window->get_decorator());
    }
  }
}

void EventHandler::OnConfigureRequest(xcb_configure_request_event_t* event) {
  RegisterWindow(event->window);
  if (window_handler_.Exists(event->window)) {
    Window* window = window_handler_.Get(event->window);
    DEBUG("ConfigureRequest window %d, rect: {%d, %d, %d, %d}",
          window->get_id(),
          window->get_rect().x(),
          window->get_rect().y(),
          window->get_rect().width(),
          window->get_rect().height());
    window_handler_.SendConfigureNotify(window);
  }
}

void EventHandler::OnMapRequest(xcb_map_request_event_t* event) {
  RegisterWindow(event->window);
}

void EventHandler::OnEnterNotify(xcb_enter_notify_event_t* event) {
  DEBUG("OnEnterNotifiy received for {%d}", event->event);
  Focus(event->event);
}

void EventHandler::OnMotionNotify(xcb_motion_notify_event_t* event) {
  // TODO(ppofuk): Check if we enable give focus on mouse over.
  Focus(event->child);
}

void EventHandler::OnExpose(xcb_expose_event_t* event) {
  if (!window_handler_.Exists(event->window))
    return;

  Window* window = window_handler_.Get(event->window);

  if (window->get_type() == kDecorator) {
    Decorator* decorator = static_cast<Decorator*>(window);
    decorator->OnExpose();
    window_handler_.SendExposeEvent(decorator->get_target());

  } else if (window->get_type() == kSpecial) {
    // Our special redraw need
  } else {
    window_handler_.SendExposeEvent(window);
  }
}

void EventHandler::OnFocusIn(xcb_focus_in_event_t* event) {
  Focus(event->event);
}

void EventHandler::Focus(xcb_window_t id) {
  DEBUG("Request focus for %d.", id);

  if (window_handler_.Exists(id)) {
    Window* window = window_handler_.Get(id);
    if (window->get_type() == kDecorator) {
      Window* target = static_cast<Decorator*>(window)->get_target();
      window_handler_.Raise(target);
      window_handler_.SetInputFocus(target);

      DEBUG("Focus approved on decorator!");
    } else {
      window_handler_.Raise(window);
      window_handler_.SetInputFocus(window);

      DEBUG("Focus approved on window");
    }
  }
}

void EventHandler::RegisterWindow(xcb_window_t id) {
  if (!window_handler_.Exists(id)) {
    DEBUG("** Registering window %d.", id);

    Window* window = new Window(id);
    window_handler_.UpdateWindowName(window);
    window_handler_.Add(window);

    // xcb_configure_window(dpy_, id, XCB_CW_EVENT_MASK, values_);

    window->set_rect(screen_rect_);
    window->set_border_width(0);

    CreateDecorator(window);  // CreateDecorator() will apply settings to window
                              // This will need to be changed.
    window_handler_.SetInputFocus(window);  // It's a new window
                                            // let's give it focus.
  } else {
    Window* window = window_handler_.Get(id);

    if (window->get_type() == kNormal) {
      if (window->get_decorator() == NULL) {
        CreateDecorator(window);
      } else {
        Decorator* decorator = static_cast<Decorator*>(window->get_decorator());
        decorator->Map();
        window->Map();
        decorator->ApplyRect();
        window_handler_.Apply(decorator);
        window_handler_.Apply(window);
      }
    }
  }
}

void EventHandler::CreateDecorator(Window* window) {
  // TODO: check for fails.
  xcb_window_t id = xcb_generate_id(dpy_);
  Decorator* decorator = new Decorator(id);

  DEBUG("New decorator on with %d on window %d", id, window->get_id());

  decorator->set_target(window);
  decorator->set_parent(dummy_root_);
  window->set_decorator(decorator);
  window->set_parent(decorator);

  decorator->ApplyRects(window->get_rect());
  window_handler_.CreateWindow(decorator);
  drawing_handler_.InitSurface(decorator->get_surface(), decorator);

  // Check if the cairo surface is created.
  if (decorator->get_surface().get_cr()) {
    window_handler_.Add(decorator);  // Hashmap the decorator
    window_handler_.ApplyProperties(window);

    window_handler_.Apply(decorator);
    window_handler_.Reparent(window, decorator->TargetTopLeft());  // Add
                                                                   // child

  } else {
    ERROR("Can't init cairo surface on decorator!\n");
    window_handler_.Destroy(decorator);
    window_handler_.Remove(decorator->get_id());
  }
}

void EventHandler::RemoveDecorators() {
  WindowHandler::WindowMapType& window_map = window_handler_.get_window_map();
  WindowHandler::WindowMapType::iterator it = window_map.begin();
  for (; it != window_map.end(); it++) {
    Window* window = (*it).second;
    if (window->get_type() == kDecorator) {
      // We need to reparent decorated windows to root
      Decorator* decorator = static_cast<Decorator*>(window);
      decorator->get_target()->set_parent(dummy_root_);
      window_handler_.Reparent(decorator->get_target(), Point(0, 0));

      // Destroy decorator
      window_handler_.Destroy(window);
      window_handler_.Remove(window->get_id());
      delete window;
    }
  }
}

}  // namespace nightwing
