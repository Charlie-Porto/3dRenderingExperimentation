#ifndef test_cpp
#define test_cpp

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h> 

#include <ezprint.cpp>

#include "test_transformation_funktions.cpp"


int main() {
  pce::test::run_module_test();
  return 0;
}


#endif /* test_cpp */
