#ifndef NIGHTWING_BUILD_H_
#define NIGHTWING_BUILD_H_
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Some nasty macro definitions are and will be defined here.
// Keep your children away from this file.

#define OBSERVABLE_COLLECTION_SIZE 1024

// Disallows nightwing to start on DISPLAY:=0 when compiled for debugging.
#define NIGHTWING_DISALLOW_DISPLAY_0 1

// Defines maximum title size in sizeof(char)
#define NIGHTWING_TITLE_MAX_SIZE 128

// TODO: setup logging level to work with DEBUG, WARNING, INFO macros
// Open pipe for stderr and stdout to file? Easier

// THIS SHOULD NEVER BE IN BUILD.H!

namespace nightwing {
namespace datetime {
static const char* TimeNow() {
  static time_t rawtime;
  static tm* timeinfo;
  static char buffer[30];

  time(&rawtime);
  timeinfo = localtime(&rawtime);
  strftime(buffer, 30, "%I:%M%p", timeinfo);
  return static_cast<const char*>(buffer);
}
}  // namespace datetime
}  // namespace nightwing

#define DEBUG(Args...)                                       \
  do {                                                       \
    fprintf(stderr, "[ DEBUG ]  ");                          \
    fprintf(stderr, "%s: ", nightwing::datetime::TimeNow()); \
    fprintf(stderr, ##Args);                                 \
    fprintf(stderr, "\n");                                   \
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

#endif  // NIGHTWING_BUILD_H_
