#ifndef rigid_body_component_cpp
#define rigid_body_component_cpp

#include "../../../include/glm/vec3.hpp"

struct RigidBody {
  double radius;
  glm::dvec3 location;
  double direction; // for now, either 1 or -1
};





#endif /* rigid_body_component_cpp */
