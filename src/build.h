#ifndef NIGHTWING_BUILD_H_
#define NIGHTWING_BUILD_H_

// Some nasty macro definitions are and will be defined here.
// Keep your children away from this file.

#define OBSERVABLE_COLLECTION_SIZE 1024

// Disallows nightwing to start on DISPLAY:=0 when compiled for debugging.
#define NIGHTWING_DISALLOW_DISPLAY_0 1

// TODO: setup logging level to work with DEBUG, WARNING, INFO macros
// Open pipe for stderr and stdout to file? Easier

// THIS SHOULD NEVER BE IN BUILD.H!!!!!!!
// + it's unusable as it is. (test it)

#define DEBUG(Args...)              \
  do {                              \
    fprintf(stderr, "[ DEBUG ]: "); \
    fprintf(stderr, ##Args);        \
    fprintf(stderr, "\n");          \
  } while (0)

#define ERROR(Args...)              \
  do {                              \
    fprintf(stderr, "[ ERROR ]: "); \
    fprintf(stderr, ##Args);        \
    fprintf(stderr, "\n");          \
  } while (0)

#define INFO(Args...)              \
  do {                             \
    fprintf(stderr, "[ INFO ]: "); \
    fprintf(stderr, ##Args);       \
    fprintf(stderr, "\n");         \
  } while (0)

#define WARNING(Args...)              \
  do {                                \
    fprintf(stderr, "[ WARNING ]: "); \
    fprintf(stderr, ##Args);          \
    fprintf(stderr, "\n");            \
  } while (0)

// TODO: remove thoes from here (they are not build information)
#define MODKEY XCB_MOD_MASK_1

#define MOUSEMODKEY XCB_MOD_MASK_1

#define TERMINAL "urxvt"

#endif  // NIGHTWING_BUILD_H_
