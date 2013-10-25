#include "point.h"

namespace nightwing {

Point::Point(int x, int y) : x_(x), y_(y) {
}

Point::Point(xcb_point_t& point) : x_(point.x), y_(point.y) {  
}

Point& Point::operator+=(const Point &rhs) {
  set_x(rhs.x() + rhs.x());
  set_y(rhs.y() + rhs.y());
  return *this; 
}


} // namespace nightwing
