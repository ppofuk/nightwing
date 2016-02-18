#ifndef NIGHTWING_RECT_H_
#define NIGHTWING_RECT_H_

#include "point.h"

namespace nightwing {

class Rect {
 public:
  Rect(Point& origin, int width = 0, int height = 0);

  Rect(int x = 0, int y = 0, int width = 0, int height = 0);

  Rect(xcb_rectangle_t& rect);

  Rect(xcb_get_geometry_reply_t& rect);

  // Set function behaves same as giving parameters in constructor
  void Set(Point& origin, int width = 0, int height = 0);

  void Set(int x = 0, int y = 0, int width = 0, int height = 0);

  void Set(xcb_rectangle_t& rect);

  void Set(xcb_get_geometry_reply_t& rect);

  void Set(Rect& rect);

  // Return the end point of rect.
  Point Destination() const;

  uint32_t* ValueList();

  // Returns the origin point
  Point Origin() const { return origin_; }

  int x() const { return origin_.x(); }

  int y() const { return origin_.y(); }

  int width() const { return width_; }

  int height() const { return height_; }

  void set_origin(const Point& origin) { origin_ = origin; }

  void set_height(int height) { height_ = height; }

  void set_width(int width) { width_ = width; }

  void set_x(const int x) { origin_.set_x(x); }

  void set_y(const int y) { origin_.set_y(y); }

  operator uint32_t* () {
    return ValueList();
  }

  operator xcb_rectangle_t() const {
    xcb_rectangle_t ret = {
      static_cast<int16_t>(x()),
      static_cast<int16_t>(y()),
      static_cast<uint16_t>(width_),
      static_cast<uint16_t>(height_)
    };
    return ret;
  }

 private:
  Point origin_;
  int width_;
  int height_;
  uint32_t value_list_[4];
};

// The returning Rect has a width and height shrinked by rhs.
inline Rect operator-(const Rect& lhs, const int rhs) {
  Rect result(lhs);
  result.set_width(lhs.width() - rhs);
  result.set_height(lhs.height() - rhs);
  return result;
}

// The returning Rect has a width and height added by rhs.
inline Rect operator+(const Rect& lhs, const int rhs) {
  Rect result(lhs);
  result.set_width(lhs.width() + rhs);
  result.set_height(lhs.height() + rhs);
  return result;
}



} // namespace nightwing

#endif // #define NIGHTWING_RECT_H_
