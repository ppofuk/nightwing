#ifndef NIGHTWING_TYPE_HELPER_H_
#define NIGHTWING_TYPE_HELPER_H_

#include <memory>
#include <xcb/xcb.h>

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
// http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml?showone=Copy_Constructors#Copy_Constructors
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

#define DISALLOW_COPY(TypeName) TypeName(const TypeName&)

#define DISALLOW_ASSIGN(TypeName) void operator=(const TypeName&)

// Collection of event masks
#define NIGHTWING_PARENTED_MASK                                       \
  (XCB_EVENT_MASK_PROPERTY_CHANGE | XCB_EVENT_MASK_STRUCTURE_NOTIFY | \
   XCB_EVENT_MASK_FOCUS_CHANGE)

#define NIGHTWING_DECORATOR_MASK                                            \
  (XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE |            \
   XCB_EVENT_MASK_POINTER_MOTION | XCB_EVENT_MASK_EXPOSURE |                \
   XCB_EVENT_MASK_STRUCTURE_NOTIFY | XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT | \
   XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY | XCB_EVENT_MASK_ENTER_WINDOW)

#define NIGHTWING_ROOT_MASK                                             \
  (XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT | XCB_EVENT_MASK_BUTTON_PRESS | \
   XCB_EVENT_MASK_STRUCTURE_NOTIFY | XCB_EVENT_MASK_POINTER_MOTION |    \
   XCB_EVENT_MASK_PROPERTY_CHANGE | XCB_EVENT_MASK_ENTER_WINDOW)

#define MODKEY XCB_MOD_MASK_1

#define MOUSEMODKEY XCB_MOD_MASK_1

namespace nightwing {}
#endif  // NIGHTWING_TYPE_HELPER_H_
