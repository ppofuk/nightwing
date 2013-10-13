#ifndef _SESSION_H_
#define _SESSION_H_

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

namespace nightwing {

// TODO: class description
class Session {
  public:
    // XCB configure stuff for user interaction
    void SetupMouse();
    void SetupKeys();
    void SetupScreens();

    // Create new single instance of Session.
    static Session* Instance();

    // Destroy Session instance.
    static void Release();

    // Main loop. Check for events and etc.
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

    uint32_t mask;
    uint32_t values[2];

    xcb_void_cookie_t cookie;
    xcb_generic_error_t *error;

    char *focus_col;
    char *unfocus_col;
    char *fixed_col;

};

} // namespace nightwing

#endif /* _SESSION_H_ */
