#ifndef NIGHTWING_EVENT_H_
#define NIGHTWING_EVENT_H_

#include <xcb/xcb.h>
#include <xcb/randr.h>
#include <xcb/xcb_keysyms.h>
#include <xcb/xproto.h>

#include <unistd.h>
#include <stdio.h>

#include "build.h"
#include "type-helpers.h"
#include "window-handler.h"
#include "drawing-handler.h"

namespace nightwing {

class EventHandler {
 public:
  EventHandler();

  // Initialize EventHandler. Usually it should be called in |Session::Init|.
  void Init(xcb_connection_t* dpy,
            xcb_screen_t* screen,
            xcb_visualtype_t* visual_type);

  void Release();

  bool is_init() { return is_init_; }

  void set_dpy(xcb_connection_t* dpy) { dpy_ = dpy; }
  void set_screen(xcb_screen_t* screen) { screen_ = screen; }
  void set_visual_type(xcb_visualtype_t* visual_type) {
    visual_type_ = visual_type;
  }

  WindowHandler* get_window_handler() { return &window_handler_; }
  DrawingHandler* get_drawing_handler() { return &drawing_handler_; }

  // Call appropriate method to handle the given |event|.
  // Event handle methods are named with On prefix.
  void ProcessEvent(xcb_generic_event_t* event);

  // Callback on XCB_DESTROY_NOTIFY.
  // DestroyNotify event is triggered when the calling window is
  // immediately destroyed.
  void OnDestroyNotify(xcb_destroy_notify_event_t* event);

  // Callback on XCB_UNMAP_NOTIFY.
  // The UnmapNotify event is triggered when the notifying window
  // is being hiden. This doesn't imply that the window must be
  // destroyed.
  void OnUnmapNotify(xcb_unmap_notify_event_t* event);

  // Callback on XCB_CONFIGURE_REQUEST.
  // ConfigureRequest is triggered when windows wants to resize on
  // its own. We must always respond with
  // window_handler_.SendConfigureNotify, or the window may repost
  // the event and such be delayed (which is annoying and laggy).
  //
  // Note that some applications (like xterm) send double
  // configure request and not a single map request, so this callback
  // function is the main register for new windows.
  //
  // Every window must at least once call ConfigureRequest.
  void OnConfigureRequest(xcb_configure_request_event_t* event);

  // Callback on XCB_MAP_REQUEST.
  // MapReqeust is triggered when a window appears on screen, or
  // is shown again (switched from hidden state).
  void OnMapRequest(xcb_map_request_event_t* event);

  // Callback on XCB_ENTER_NOTIFY.
  // EnterNotify is triggered when the mouse pointer gets moved onto
  // window.
  void OnEnterNotify(xcb_enter_notify_event_t* event);

  // Callback on XCB_MOTION_NOTIFY.
  // MotionNotifiy is triggered on mouse movement without interaction
  // with active window (for example moving mouse cursor on root).
  void OnMotionNotify(xcb_motion_notify_event_t* event);

  // Callback on XCB_EXPOSE.
  // Call all our windows that need redraw.
  void OnExpose(xcb_expose_event_t* event);

  // Callback on XCB_FOCUS_IN.
  // Give input focus to focused window.
  void OnFocusIn(xcb_focus_in_event_t* event);

  void OnPropertyNotify(xcb_property_notify_event_t* event);

  // Raise and give focus to specified window with |id|.
  // If |retry| is set to true, and the window with |id| is not found in map,
  // then the |RegisterWindow(|id|)| and |Focus(|id|, false) will be called.
  // In other words, the method will try to register an unmaped window and call
  // it self once.
  void Focus(xcb_window_t id, bool retry = true);

  void RegisterWindow(xcb_window_t id);

  void CreateDecorator(Window* window);

  void RemoveDecorators();

 private:
  bool is_init_;

  Window* dummy_root_;
  xcb_connection_t* dpy_;
  xcb_screen_t* screen_;
  xcb_visualtype_t* visual_type_;

  Rect screen_rect_;

  WindowHandler window_handler_;
  DrawingHandler drawing_handler_;
};

}  // namespace nightwing

#endif  // ifndef NIGHTWING_EVENT_H_
