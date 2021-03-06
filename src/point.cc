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

void Point::Set(int x, int y) {
  x_ = x; 
  y_ = y; 
}

void Point::Set(xcb_point_t& point) {
  x_ = point.x; 
  y_ = point.y; 
}

void Point::Set(Point& point) {
  x_ = point.x(); 
  y_ = point.y(); 
}

uint32_t* Point::ValueList() {
  value_list_[0] = x_; 
  value_list_[1] = y_; 
  return value_list_; 
}

} // namespace nightwing
