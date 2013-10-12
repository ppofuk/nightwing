#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <xcb/xcb.h>
#include "observer.h"

namespace nightwing {

class Window : Observer {
  public:
    void Notify(Observable* observable, void* arg);
    void Notify(Observable* observable, xcb_button_press_event_t* arg);
    // TODO: rest signatures

  private:
    xcb_window_t *root;
    xcb_window_t *child;
    xcb_window_t *event;
};

} // namespace nightwing
#endif /* _WINDOW_H_ */
