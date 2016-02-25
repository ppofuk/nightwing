#ifndef NIGHTWING_SESSION_H_
#define NIGHTWING_SESSION_H_

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
#include "event-handler.h"

namespace nightwing {

// TODO: class description
class Session {
 public:
  void Init();

  // XCB configure stuff for user interaction
  void SetupMouse();
  void SetupKeys();
  void SetupScreens();

  void InitVisualType();

  // Create new single instance of Session.
  static Session* Instance();

  // Destroy Session instance.
  static void Release();

  // Main loop is essentially an xcb event loop.
  void MainLoop();

  // Getters
  bool has_errors() { return has_errors_; }
  xcb_connection_t* get_dpy() { return dpy_; }

 private:
  Session();
  ~Session();

 protected:
  static Session* instance_;

  int screen_number;
  bool has_errors_;
  xcb_connection_t* dpy_;
  xcb_screen_iterator_t screen_iter;
  xcb_screen_t* screen_;
  xcb_drawable_t root_;
  xcb_visualtype_t* visual_type_;

  uint32_t mask_;
  uint32_t values_[2];

  xcb_void_cookie_t cookie_;
  xcb_generic_error_t* error_;
  xcb_generic_event_t* event_;

  EventHandler event_handler_;

  bool x_kill_exit_;
};

}  // namespace nightwing

#endif // ifndef NIGHTWING_SESSION_H_
