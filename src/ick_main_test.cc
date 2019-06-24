#include "src/application.h"

int main(int argc, char* argv[]) {

  IApplication app(true, false, false);
  app.main(argc, argv);

}