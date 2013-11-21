#ifndef NIGHTWING_RECT_H_
#define NIGHTWING_RECT_H_

#include "point.h"

namespace nightwing {

class Rect {
 public:
  Rect(Point& origin, int width = 0, int height = 0);

  Rect(int x = 0, int y = 0, int width = 0, int height = 0);

  Rect(xcb_rectangle_t& rect);

  // Returns the origin point
  Point& origin() const { return origin_; }

  int width() const { return width_; }

  int height() const { return height_; }
  
  void set_origin(const Point& origin) { origin_ = origin; }
  
  void set_height(int height) { height = height_; }
  
  void set_width(int width) { width = width_; }

 private:
  Point origin_; 
  int width_; 
  int height_; 
};

} // namespace nightwing 

#endif // #define NIGHTWING_RECT_H_
