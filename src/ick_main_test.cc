#include "src/application.h"

#include <unistd.h>

int main(int argc, char* argv[]) {

  IApplication app(true, false, true);
  app.main(argc, argv);
  while(true) {
    usleep(1000000);
  }
}