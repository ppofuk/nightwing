#ifndef NIGHTWING_TEST_H_
#define NIGHTWING_TEST_H_

// So how do you test a test? :)

#include "build.h"
#include <stdio.h>
#include <string.h>

// This is a wrapper macro that creates an instance of Test class and 
// outputs the passing message to stderr. 
// Use it to perform tests. 
//
// Simple example that tests if variable myint is of value 1:
//  NW_TEST(myint == 1);
// 
// NW_TEST will return a new instance of nightwing::Test class that
// has defined operator << to append your messages. 
// For example: 
//  NW_TEST(myint == 1) << "myint should always be 1!";
//
// NW_TEST will not send sigterm to process if the expression fails. 
// To do so you can test the expression passing with HasPassed() method.
// For example this will send sigterm to self if the test failed: 
//  if (NW_TEST(myint == 1).HasPassed() == false) 
//    kill(getpid(), SIGTERM);
//
// You can retrive the outputed string with method Info(). Note, that 
// this will not retrive user appended strings. 
#define NW_TEST(expr)                           \
  nightwing::Test::TestExpr((expr), #expr, __LINE__, __func__, __FILE__)

namespace nightwing {

// This is a helper class that should not be called without
// a wrapping macro. 
class Test {
 public:
  static Test TestExpr(bool expr,
                       const char* expr_str,
                       unsigned int line, 
                       const char* func, 
                       const char* file);
  
  Test& operator<<(const char* rhs);
  
  // Returns true if the tests expression has evaled true.
  bool HasPassed() const { return passed_; }

  // This will return the informative text of test evaluation 
  // that has been written to stderr. 
  // Note that this will not retrive the custom appended messages. 
  const char* Info() const { return info_; }
  
 private:
  bool passed_; 
  char info_[128]; 
};

} // namespace nightwing

#endif // #ifndef NIGHTWING_TEST_H_
