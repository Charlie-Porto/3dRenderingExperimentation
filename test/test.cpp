#ifndef test_cpp
#define test_cpp

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h> 

#include <ezprint.cpp>

#include "test_transformation_funktions.cpp"
#include "transform_functions/test_calculate2dObjectCoordinatesBehindCam.cpp"


int main() {
  // pce::test::run_module_test(); // for original testing functions file

  test_calculate2dObjectCoordinatesBehindCam::run_test_module();

  return 0;
}


#endif /* test_cpp */
