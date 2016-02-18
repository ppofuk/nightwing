#include "session.h"

namespace nightwing {

Session* Session::instance_ = 0;

Session* Session::Instance() {
  if (instance_ == 0) {
    instance_ = new Session();
    instance_->has_errors_ = false;
  }

  return instance_;
}

void Session::Release() {
  if (instance_) {
    if (!instance_->x_kill_exit_) {
      instance_->RemoveDecorators();
    }
    delete instance_->dummy_root_;
    delete instance_;
  }

  instance_ = 0;
}

void Session::SetupMouse() {
  xcb_grab_button(dpy_, 0, root_,
                  XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE,
                  XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC, root_, XCB_NONE,
                  1,  // left mouse button
                  MOUSEMODKEY);

  xcb_grab_button(dpy_, 0, root_,
                  XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE,
                  XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC, root_, XCB_NONE,
                  2,  // middle mouse button
                  MOUSEMODKEY);

  xcb_grab_button(dpy_, 0, root_,
                  XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE,
                  XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC, root_, XCB_NONE,
                  3,  // right mouse button
                  MOUSEMODKEY);
}

// Moved all thing from constructor to Init method.
// Member class instances of Session get theirs constructor
// called after Session constuctor.
Session::Session() : x_kill_exit_(false) {
  // IT'S NOT SAFE HERE! MOVE TO Init()!
}

void Session::Init() {
  dpy_ = xcb_connect(NULL, &screen_number);
  if (xcb_connection_has_error(dpy_)) {
    has_errors_ = true;
    return;
  }

  screen_iter = xcb_setup_roots_iterator(xcb_get_setup(dpy_));
  for (int i = 0; i < screen_number; ++i) {
    xcb_screen_next(&screen_iter);
  }
  screen_ = screen_iter.data;

  if (!screen_) {
    ERROR("Can't get current screen. Exiting");
    xcb_disconnect(dpy_);
    return;
  }
  root_ = screen_->root;

  DEBUG("Session size is %dx%d\n\t\tRoot window: %d", screen_->width_in_pixels,
        screen_->height_in_pixels, screen_->root);

  screen_rect_.set_width(screen_->width_in_pixels);
  screen_rect_.set_height(screen_->height_in_pixels);

  DEBUG("Session root data %ud", screen_->root);

  // Check for RANDR extension and configure
  // TODO:

  // Need to be changed imho.
  SetupKeys();

  SetupMouse();

  mask_ = XCB_CW_EVENT_MASK;
  values_[0] = NIGHTWING_ROOT_MASK;

  cookie_ = xcb_change_window_attributes_checked(dpy_, root_, mask_, values_);
  error_ = xcb_request_check(dpy_, cookie_);

  dummy_root_ = new Window(root_);
  dummy_root_->set_rect(screen_rect_);

  InitVisualType();
  window_handler_.set_dpy(dpy_);
  window_handler_.set_screen(screen_);
  drawing_handler_.set_dpy(dpy_);
  drawing_handler_.set_visual_type(visual_type_);

  SetupScreens();

  xcb_flush(dpy_);
}

void Session::InitVisualType() {
  visual_type_ = NULL;
  xcb_depth_iterator_t depth_iter;

  depth_iter = xcb_screen_allowed_depths_iterator(screen_);
  for (; depth_iter.rem; xcb_depth_next(&depth_iter)) {
    xcb_visualtype_iterator_t visual_iter;

    visual_iter = xcb_depth_visuals_iterator(depth_iter.data);
    for (; visual_iter.rem; xcb_visualtype_next(&visual_iter)) {
      if (screen_->root_visual == visual_iter.data->visual_id) {
        visual_type_ = visual_iter.data;
        break;
      }
    }
  }
}

/*
 * @brief Setup all shortcut keys
 */
void Session::SetupKeys() {
  // This all SetupX() thing aren't way to go.
  // You'll kill the modularity (always restart after key bind change!?)
  // TODO:
}

/*
 * @brief Walk through all existing windows and set them up
 */
void Session::SetupScreens() {
  size_t len;
  xcb_window_t* children;
  xcb_query_tree_reply_t* reply;

  reply = xcb_query_tree_reply(dpy_, xcb_query_tree(dpy_, root_), 0);
  if (NULL == reply) {
    ERROR(" SetupScreens(), reply is NULL");
    return;
  }

  len = xcb_query_tree_children_length(reply);
  children = xcb_query_tree_children(reply);
  DEBUG("Number of children windows: %d", len);

  for (size_t i = 0; i < len; ++i) RegisterWindow(children[i]);
}

// private destructor (no wai sherlock! =))
Session::~Session() {}

void Session::MainLoop() {
  // TODO: create a flag on end of while loop for usage in determining the
  // ability of removing decorators (if it reached out of the while loop then we
  // can assume that X was shut down which generates an error in |Release()|
  // because the resources are unenviable)
  while ((event_ = xcb_wait_for_event(get_dpy()))) {
    switch (event_->response_type & ~0x80) {
      case XCB_DESTROY_NOTIFY:
        DEBUG("DESTROY_NOTIFY event");
        OnDestroyNotify();
        break;

      case XCB_CONFIGURE_REQUEST:
        DEBUG("CONFIGURE_REQUEST event");
        OnConfigureRequest();
        break;

      case XCB_MAP_REQUEST:
        DEBUG("MAP_REQUEST event");
        OnMapRequest();
        break;

      case XCB_UNMAP_NOTIFY:
        DEBUG("UNMAP_NOTIFY event");
        OnUnmapNotify();
        break;

      case XCB_ENTER_NOTIFY:
        DEBUG("ENTER_NOTIFY event");
        OnEnterNotify();
        break;

      case XCB_FOCUS_IN:
        DEBUG("XCB_FOCUS_IN event");
        OnFocusIn();
        break;

      case XCB_MOTION_NOTIFY:
        // Please, don't DEBUG this out, it's annoying to read one
        // millions lines of motion notify robo-rap sentences.
        OnMotionNotify();
        break;

      case XCB_EXPOSE:
        DEBUG("XCB_EXPOSE event");
        OnExpose();
        break;

      default:
        // Ignoring unknown event type
        break;
    }
  }
  x_kill_exit_ = true;  // Event loop exit, we can assume we lost X connection.
}

void Session::OnMotionNotify() {
  xcb_motion_notify_event_t* event = (xcb_motion_notify_event_t*)event_;

  // TODO: Check if we enable give focus on mouse over.
  Focus(event->child);
}

void Session::OnFocusIn() {
  xcb_focus_in_event_t* event = (xcb_focus_in_event_t*)event_;
  Focus(event->event);
}

void Session::OnEnterNotify() {
  xcb_enter_notify_event_t* event = (xcb_enter_notify_event_t*)event_;
  DEBUG("OnEnterNotifiy received for {%d}", event->event);

  Focus(event->event);
}

void Session::Focus(xcb_window_t id) {
  DEBUG("Request focus for %d.", id);

  if (window_handler_.Exists(id)) {
    Window* window = window_handler_.Get(id);
    if (window->get_type() == kDecorator) {
      Window* target = static_cast<Decorator*>(window)->get_target();
      window_handler_.Raise(target);
      window_handler_.SetInputFocus(target);

      DEBUG("Focus approved!");
    } else {
      window_handler_.Raise(window);
      window_handler_.SetInputFocus(window);

      DEBUG("Focus approved!");
    }
  }
}

void Session::OnMapRequest() {
  xcb_map_request_event_t* event = (xcb_map_request_event_t*)event_;

  RegisterWindow(event->window);
}

void Session::OnConfigureRequest() {
  xcb_configure_request_event_t* event = (xcb_configure_request_event_t*)event_;

  RegisterWindow(event->window);
  if (window_handler_.Exists(event->window)) {
    Window* window = window_handler_.Get(event->window);
    DEBUG("ConfigureRequest window %d, rect: {%d, %d, %d, %d}",
          window->get_id(), window->get_rect().x(), window->get_rect().y(),
          window->get_rect().width(), window->get_rect().height());
    window_handler_.SendConfigureNotify(window);
  }
}

void Session::OnDestroyNotify() {
  xcb_destroy_notify_event_t* event = (xcb_destroy_notify_event_t*)event_;

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

void Session::OnUnmapNotify() {
  xcb_unmap_notify_event_t* event = (xcb_unmap_notify_event_t*)event_;
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

void Session::OnExpose() {
  xcb_expose_event_t* event = (xcb_expose_event_t*)event_;

  if (!window_handler_.Exists(event->window)) return;

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

void Session::RegisterWindow(xcb_window_t id) {
  if (!window_handler_.Exists(id)) {
    DEBUG("** Registering window %d.", id);

    Window* window = new Window(id);
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

void Session::CreateDecorator(Window* window) {
  // TODO: check for fails.
  xcb_window_t id = xcb_generate_id(dpy_);
  Decorator* decorator = new Decorator(id);

  DEBUG("New decorator on with %d on window %d", id, window->id_);

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
    window_handler_.Remove(decorator->id_);
  }
}

void Session::RemoveDecorators() {
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
