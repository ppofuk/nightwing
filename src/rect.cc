#include "rect.h"

namespace nightwing {

Rect::Rect(Point& origin, int width, int height) : origin_(origin),
                                                   width_(width),
                                                   height_(height) {
}

Rect::Rect(int x, int y, int width, int height) : origin_(Point(x, y)),
                                                  width_(width),
                                                  height_(height) {
}

Rect::Rect(xcb_rectangle_t& rect) : origin_(Point(rect.x, rect.y)),
                                    width_(rect.width),
                                    height_(rect.height) {
}

Point Rect::Destination() const {
  return Point(x() + width_, y() + height_);
}


} // namespace nightwing
