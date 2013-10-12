#include <stdio.h>
#include <stdlib.h>
#include "session.h"

using namespace nightwing;

void AtExit() {
  Session::Release();
}

int main() {
  atexit(AtExit); // I ate your camel!

  Session* session = Session::Instance();
  if (session->HasErrors()) {
    fprintf(stderr, "Connection to X failed! Bye, bye.");
    exit(1);
  }

  session->MainLoop();

  exit(0);
}
