#ifndef NIGHTWING_DECORATOR_H_
#define NIGHTWING_DECORATOR_H_

#include "window.h"

namespace nightwing {

// Decorator is an special Window type. 
class Decorator : public Window {
 public:
  void OnExpose();

  void set_target(Window* target) { target_ = target; }
  
  Window* get_target() { return target_; }
  
  

 private:
  Window* target_;
  
};

} // namespace nightwing
#endif // NIGHTWING_DECORATOR_H_
