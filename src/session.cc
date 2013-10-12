#include "session.h"
#include "observable.h"
#include "observer.h"

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
  if (instance_)
    delete instance_;

  instance_ = 0;
}


void Session::SetupMouse()
{
    xcb_grab_button(dpy_, 0, root_,
                    XCB_EVENT_MASK_BUTTON_PRESS
                    | XCB_EVENT_MASK_BUTTON_RELEASE,
                    XCB_GRAB_MODE_ASYNC,
                    XCB_GRAB_MODE_ASYNC, root_, XCB_NONE,
                    1, /* left mouse button */
                    MOUSEMODKEY);

    xcb_grab_button(dpy_, 0, root_,
                    XCB_EVENT_MASK_BUTTON_PRESS
                    | XCB_EVENT_MASK_BUTTON_RELEASE,
                    XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC,
                    root_, XCB_NONE,
                    2, /* middle mouse button */
                    MOUSEMODKEY);

    xcb_grab_button(dpy_, 0, root_,
                    XCB_EVENT_MASK_BUTTON_PRESS
                    | XCB_EVENT_MASK_BUTTON_RELEASE,
                    XCB_GRAB_MODE_ASYNC,
                    XCB_GRAB_MODE_ASYNC, root_, XCB_NONE,
                    3, /* right mouse button */
                    MOUSEMODKEY);
}


// It is safe to initialize things in constructor
// due to nature of singleton
Session::Session() {
  dpy_ = xcb_connect(NULL, &screen_number);
  if (xcb_connection_has_error(dpy_)) {
    has_errors_ = true;
    return;
  }

  screen_iter = xcb_setup_roots_iterator(xcb_get_setup(dpy_));
  for (int i = 0; i < screen_number; ++i)
  { xcb_screen_next(&screen_iter); }
  screen_ = screen_iter.data;

  if (!screen_)
  {
      ERROR("Can't get current screen. Exiting");
      xcb_disconnect(dpy_);
      return;
  }
  root_ = screen_->root;

  DEBUG("Session size is %dx%d\n\t\tRoot window: %d",
        screen_->width_in_pixels,
        screen_->height_in_pixels,
        screen_->root);

  DEBUG("Session root data %ud", screen_->root);


  /* TODO: Check for RANDR extension and configure */

  /* TODO: Loop over all clients and setup */

  /* TODO: Setup key bindings */

  /* TODO: Grab mouse buttons */
  SetupMouse();

  /* TODO: Subscribe to events */
  mask = XCB_CW_EVENT_MASK;

  values[0] = XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT
          | XCB_EVENT_MASK_STRUCTURE_NOTIFY
          | XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY;

  cookie = xcb_change_window_attributes_checked(
      dpy_, root_, mask, values);

  error = xcb_request_check(dpy_, cookie);

  xcb_flush(dpy_);
}

// private destructor
Session::~Session() {}

void Session::MainLoop() {
    // TODO: refactor to class definition
    xcb_map_request_event_t *map_request;
    xcb_button_press_event_t *bp;
    xcb_expose_event_t *expose;

    xcb_generic_event_t* event;


    // TODO: create observable

    while( event = xcb_wait_for_event(get_dpy()) ) {
        switch(event->response_type & ~0x80)
        {
            case XCB_MAP_REQUEST:
                DEBUG("XCB_MAP_REQUEST event triggered");
                map_request = (xcb_map_request_event_t*)event;

                // TODO: handle_new_window(map_request->window);
                break;

            case XCB_DESTROY_NOTIFY:
                DEBUG("XCB_DESTROY_NOTIFY event triggered");
                // TODO:
                break;

            case XCB_BUTTON_PRESS:
                DEBUG("XCB_BUTTON_PRESS event triggered");
                bp = (xcb_button_press_event_t *)event;

                // TODO: create window, !window class not finished
                // TODO: register window on observable
                // TODO: send
                break;

            case XCB_MOTION_NOTIFY:
                DEBUG("XCB_MOTION_NOTIFY event triggered");
                // TODO:
                break;

            case XCB_BUTTON_RELEASE:
                DEBUG("XCB_BUTTON_RELEASE event triggered");
                // TODO:
                break;

            case XCB_KEY_PRESS:
                DEBUG("XCB_KEY_PRESS event triggered");
                // TODO:
                break;

            case XCB_KEY_RELEASE:
                DEBUG("XCB_KEY_RELEASE event triggered");
                // TODO:
                break;

            case XCB_ENTER_NOTIFY:
                DEBUG("XCB_ENTER_NOTIFY event triggered");
                // TODO:
                break;

            case XCB_CONFIGURE_NOTIFY:
                DEBUG("XCB_CONFIGURE_NOTIFY event triggered");
                // TODO:
                break;

            case XCB_CONFIGURE_REQUEST:
                DEBUG("XCB_CONFIGURE_REQUEST event triggered");
                // TODO:
                break;

            case XCB_CLIENT_MESSAGE:
                DEBUG("XCB_CLIENT_MESSAGE event triggered");
                // TODO:
                break;

            case XCB_CIRCULATE_REQUEST:
                DEBUG("XCB_CIRCULATE_REQUEST event triggered");
                // TODO:
                break;

            case XCB_MAPPING_NOTIFY:
                DEBUG("XCB_MAPPING_NOTIFY event triggered");
                // TODO:
                break;

            case XCB_UNMAP_NOTIFY:
                DEBUG("XCB_UNMAP_NOTIFY event triggered");
                // TODO:
                break;


            case XCB_LEAVE_NOTIFY:
                DEBUG("XCB_LEAVE_NOTIFY event triggered");
                // TODO:
                break;



            case XCB_CREATE_NOTIFY:
                DEBUG("XCB_CREATE_NOTIFY event triggered");
                // TODO:
                break;

            default:
                ERROR("Unknown event type catched: error type: %d",
                      event->response_type);
                break;
        }

        delete event; // AUDIT:
  }
}


} // namespace nightwing
