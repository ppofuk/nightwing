#include "test.h"

namespace nightwing {
Test Test::TestExpr(bool expr, 
                    const char *expr_str, 
                    unsigned int line, 
                    const char *func, 
                    const char *file) {
  // TODO: sprintf every const char* and line from args to 
  // Test instance.
  Test ret;
  ret.passed_ = expr; 
  sprintf(ret.info_, "%s: (%s) in %s: %s:%d\n",
          expr ? "[PASS]" : "[FAIL]", expr_str,
          file, func, line);        
  fprintf(stderr, ret.info_);  
  return ret;
}

Test& Test::operator<<(const char *rhs) {
  fprintf(stderr, "%s... %s\n", 
          passed_ ? "[PASS]" : "[FAIL]",
          rhs);
  return *this; 
}
  
} // namespace nightwing
