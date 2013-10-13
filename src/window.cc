#include "window.h"

namespace nightwing {

Window::Window(int16_t x, int16_t y, uint16_t width,
               uint16_t height, uint16_t border_width)
        : x(x), y(y),
          width(width), height(height),
          border_width(border_width)
{

}
