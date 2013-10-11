#include "screen.h"
#include "observable.h"
#include "observer.h"

namespace nightwing {

Screen* Screen::instance_ = 0;

Screen* Screen::Instance() {
  if (instance_ == 0) {
    instance_ = new Screen();
    instance_->has_errors_ = false;
  }

  return instance_;
}

void Screen::Release() {
  if (instance_)
    delete instance_;

  instance_ = 0;
}

// It is safe to initialize things in constructor
// due to nature of singleton
Screen::Screen() {
  dpy_ = xcb_connect(NULL, NULL);
  if (xcb_connection_has_error(dpy_)) {
    has_errors_ = true;
    return;
  }

  screen_ = xcb_setup_roots_iterator(xcb_get_setup(dpy_)).data;
  root_ = screen_->root;
}

Screen::~Screen() {

}

void Screen::MainLoop() {
    // TODO: refactor to class definition
    xcb_button_press_event_t *bp;
    xcb_expose_event_t *expose;

    xcb_generic_event_t* event;

    FILE *log;
    // TODO: create observable


    while( event = xcb_wait_for_event(get_dpy()) ) {
        switch(event->response_type & ~0x80)
        {
            case XCB_EXPOSE:
                expose = (xcb_expose_event_t *)event;

                fprintf (stdout, "Window %ld exposed. Region to be redrawn at location (%d,%d), with dimension (%d,%d)\n",
                        expose->window, expose->x, expose->y, expose->width, expose->height );
                // TODO:
                break;
            case XCB_BUTTON_PRESS:
                bp = (xcb_button_press_event_t *)event;
                log = fopen("log.txt", "a");
                fprintf(log, "%ld\n", bp->event);
                fclose(log);

                break;
                // TODO: create window, NOTE: window class not finished
                // TODO: register window on observable
                // TODO: send
                break;
            case XCB_BUTTON_RELEASE:
                // TODO:
                break;
            case XCB_MOTION_NOTIFY:
                // TODO:
                break;
            case XCB_ENTER_NOTIFY:
                // TODO:
                break;
            case XCB_LEAVE_NOTIFY:
                // TODO:
                break;
            case XCB_KEY_PRESS:
                // TODO:
                break;
            case XCB_KEY_RELEASE:
                // TODO:
                break;
            case XCB_DESTROY_NOTIFY:
                // TODO:
                break;
            case XCB_CREATE_NOTIFY:
                // TODO:
                break;
            default:
                fprintf(stderr, "There was an error: %d\n",
                        event->response_type);
                break;
        }

        delete event; // AUDIT:
  }
}


} // namespace nightwing
