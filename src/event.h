#ifndef _EVENT_H_
#define _EVENT_H_

namespace nightwing {

enum Events {
    XCB_EXPOSE,  // xcb_expose_event_t
    XCB_BUTTON_PRESS,   // xcb_button_press_event_t
    XCB_BUTTON_RELEASE,  // xcb_button_release_event_t
    XCB_MOTION_NOTIFY,  // xcb_motion_notify_event_t
    XCB_ENTER_NOTIFY,  // xcb_enter_notify_event_t
    XCB_KEY_PRESS,  // xcb_key_press_event_t
    XCB_KEY_RELEASE,  // xcb_key_press_event_t
};

} // namespace nightwing

#endif /* _SCREEN_H_ */
