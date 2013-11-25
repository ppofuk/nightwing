#ifndef NIGHTWING_TYPE_HELPER_H_
#define NIGHTWING_TYPE_HELPER_H_

#include <memory>


// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
// http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml?showone=Copy_Constructors#Copy_Constructors
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

#define DISALLOW_COPY(TypeName)                 \
  TypeName(const TypeName&)

#define DISALLOW_ASSIGN(TypeName)               \
  void operator=(const TypeName&)

namespace nightwing {

}
#endif // NIGHTWING_TYPE_HELPER_H_
