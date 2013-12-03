#ifndef NIGHTWING_DRAWING_HANDLER_H_
#define NIGHTWING_DRAWING_HANDLER_H_

#include <xcb/xcb.h>
#include <stdio.h>
#include "build.h"
#include "decorator.h"
#include "type-helpers.h"
#include "drawing-surface.h"

namespace nightwing {

class DrawingHandler {
 public:
  DrawingHandler(); 
  
  // It's important to call this methods correctly before facilitating 
  // this class.
  void set_visual_type(xcb_visualtype_t* visual_type) {
    visual_type_ = visual_type; 
  }

 private:
  xcb_visualtype_t* visual_type_; 
  
  DISALLOW_COPY_AND_ASSIGN(DrawingHandler);
};

} // namespace nightwing
#endif // NIGHTWING_DRAWING_HANDLER_H_
