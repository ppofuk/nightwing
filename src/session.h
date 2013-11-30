#ifndef NIGHTWING_SESSION_H_
#define NIGHTWING_SESSION_H_

#include <xcb/xcb.h>
#include <xcb/randr.h>
#include <xcb/xcb_keysyms.h>
#include <xcb/xcb_atom.h>
#include <xcb/xcb_icccm.h>
#include <xcb/xproto.h>
#include <xcb/xcb_util.h> 

#include <unistd.h>
#include <stdio.h>

#include "build.h"
#include "type-helpers.h"
#include "window-handler.h"

namespace nightwing {

// TODO: class description
class Session {
 public:
  void Init();

  // XCB configure stuff for user interaction
  void SetupMouse();
  void SetupKeys();
  void SetupScreens();

  // Create new single instance of Session.
  static Session* Instance();

  // Destroy Session instance.
  static void Release();

  // Callback on XCB_DESTROY_NOTIFY. 
  // DestroyNotify event is triggered when the calling window is
  // immediately destroyed. 
  void OnDestroyNotify();
 
  // Callback on XCB_UNMAP_NOTIFY. 
  // The UnmapNotify event is triggered when the notifying window 
  // is being hiden. This doesn't imply that the window must be 
  // destroyed. 
  void OnUnmapNotify(); 

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
  void OnConfigureRequest(); 
  
  // Callback on XCB_MAP_REQUEST. 
  // MapReqeust is triggered when a window appeares on screen, or 
  // is shown again (switched from hidden state). 
  void OnMapRequest();
  
  // Callback on XCB_ENTER_NOTIFY. 
  // EnterNotify is triggered when the mouse pointer gets moved onto
  // window. 
  void OnEnterNotify(); 
  
  // Callback on XCB_MOTION_NOTIFY.
  // MotionNotifiy is triggered on mouse movement without interaction 
  // with active window (for example moving mouse cursor on root). 
  void OnMotionNotify(); 


  // Main loop is essentially an xcb event loop. 
  void MainLoop();

  // Getters
  bool HasErrors() { return has_errors_; }
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
  
  Rect screen_rect_; 

  uint32_t mask_;
  uint32_t values_[2];

  xcb_void_cookie_t cookie_;
  xcb_generic_error_t* error_;  
  xcb_generic_event_t* event_; 
  
  WindowHandler window_handler_; 
};

} // namespace nightwing

#endif /* _SESSION_H_ */
