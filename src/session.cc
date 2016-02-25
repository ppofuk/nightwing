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
      instance_->event_handler_.RemoveDecorators();
    }
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


  InitVisualType();

  event_handler_.Init(dpy_, screen_, visual_type_);

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
  DEBUG("Number of children windows: %d", static_cast<int>(len));

  for (size_t i = 0; i < len; ++i) event_handler_.RegisterWindow(children[i]);
}

// private destructor (no wai sherlock! =))
Session::~Session() {}

void Session::MainLoop() {
  // TODO: create a flag on end of while loop for usage in determining the
  // ability of removing decorators (if it reached out of the while loop then we
  // can assume that X was shut down which generates an error in |Release()|
  // because the resources are unenviable)
  while ((event_ = xcb_wait_for_event(get_dpy()))) {
    event_handler_.ProcessEvent(event_);
  }
  x_kill_exit_ = true;  // Event loop exit, we can assume we lost X connection.
}


}  // namespace nightwing
