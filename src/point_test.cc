#include "point.h"
#include "test.h"

#include <assert.h>
#include <stdio.h>

namespace nightwing {
namespace test {

void point() {
  xcb_point_t xcb_point; 
  xcb_point.x = 15;
  xcb_point.y = 10;
  
  nightwing::Point point(xcb_point);
  NW_TEST(xcb_point.x == point.x());
  NW_TEST(xcb_point.y == point.y());
  
  nightwing::Point arth;
  arth = point + xcb_point; 
  NW_TEST(arth.x() == 30);
  NW_TEST(arth.y() == 20);

  point += point; 
  
  NW_TEST(point.x() == 30);
  NW_TEST(point.y() == 20);

  NW_TEST(point == arth) << "Testing the == operator.";
  NW_TEST(!(point != arth));
 
}

} // namespace test
} // namespace nightwing 
