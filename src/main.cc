#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string>
#include <assert.h>
#include "session.h"
#include "build.h"
using namespace nightwing;

void AtExit() { Session::Release(); }

void Exec(Session* session, const char* filename) { system(filename); }

// Parse options
void Options(Session* session, int argc, char* argv[]) {
  const char opt_str[] = "e:";
  option options[] = {{"exec", required_argument, 0, 'e'}, {0, 0, 0, 0}};

  int option_i = 0;
  int c;
  while ((c = getopt_long(argc, argv, opt_str, options, &option_i)) != -1) {
    switch (c) {
      case 'e':
        // options --exec
        Exec(session, optarg);
        break;

      case '?':
        // getopt_long printed the message
        break;

      default:
        exit(1);
    }
  }
}

#ifdef NIGHTWING_DEBUG
void _Debug() {
  std::string display_env = getenv("DISPLAY");
#if NIGHTWING_DISALLOW_DISPLAY_0 == 1
  if (display_env == ":0") setenv("DISPLAY", ":1", 1);
#endif
}
#else   // NIGHTWING_DEBUG not defined
void _Debug() {}
#endif  // NIGHTWING_DEBUG

int main(int argc, char* argv[]) {
  atexit(AtExit);  // I ate your camel!

  Session* session = Session::Instance();
  Options(session, argc, argv);

  _Debug();

  session->Init();
  if (session->HasErrors()) {
    fprintf(stderr, "Connection to X failed! Bye, bye.\n");
    exit(1);
  }

  session->MainLoop();
  exit(0);
}
