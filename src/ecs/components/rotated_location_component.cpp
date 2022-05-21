#ifndef rotated_location_component_cpp
#define rotated_location_component_cpp

#include <glm/vec3.hpp>

/*----------------------------------------------------------------|
--------------------- Module Description -------------------------|
holds data describing an objects position after rotating it by the 
Camera's reverse rotation versor
-----------------------------------------------------------------*/

struct RotatedLocation {
  glm::dvec3 rot_pos;
};





#endif /* rotated_location_component_cpp */
