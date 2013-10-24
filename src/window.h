#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <set>
#include <xcb/xcb.h>
#include "observer.h"

namespace nightwing {

class GraphicsContext;

class Window {
  public:
    friend class GraphicsContext;

    Window(int16_t x, int16_t y, uint16_t width,
           uint16_t height, uint16_t border_width);
    ~Window();

    void Show();
    void SetBackgroundColor(uint32_t back_pixel);
    void SetEventMask(uint32_t event_mask);

  private:
    int16_t x,y, width, height, border_width;

    std::set<xcb_cw_t> properties;

    xcb_connection_t* con;
    xcb_window_t window;
    xcb_screen_t* screen;

    uint32_t mask;

    uint32_t backPixmap;
    uint32_t backPixel;
    uint32_t borderPixmap;
    uint32_t borderPixel;
    uint32_t bitGravity;
    uint32_t winGravity;
    uint32_t backingStore;
    uint32_t backingPixel;
    uint32_t overrideRedirect;
    uint32_t saveUnder;
    uint32_t eventMask;
    uint32_t dontPropagate;
    uint32_t colormap;
    uint32_t cursor;

    uint32_t* genValues();

};

} // namespace nightwing
#endif /* _WINDOW_H_ */
