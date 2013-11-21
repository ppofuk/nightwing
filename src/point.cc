#include "point.h"

namespace nightwing {

Point::Point(int x, int y) : x_(x), y_(y) {
}

Point::Point(xcb_point_t& point) : x_(point.x), y_(point.y) {
}

Point& Point::operator+=(const Point &rhs) {
  set_x(this->x() + rhs.x());
  set_y(this->y() + rhs.y());
  return *this;
}

Point& Point::operator-=(const Point &rhs) {
  set_x(this->x() - rhs.x());
  set_y(this->y() - rhs.y());
  return *this;
}


float Point::Distance(Point first, Point second) {
  return sqrt(pow(second.x() - first.x(), 2.0) +
              pow(second.y() - first.y(), 2.0));
}

} // namespace nightwing
