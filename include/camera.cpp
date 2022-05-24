#ifndef camera_cpp
#define camera_cpp

#include <glm/vec3.hpp>
#include <glm/ext/quaternion_double.hpp> 

struct Camera {
  glm::dvec3 location_vec3;
  glm::dvec3 pov_unitvec3;
  glm::dvec3 dov_unitvec3;
	double pov_scalar;
  double xz_angle;
	double xz_circle_radius;
  double y_angle;
  double focus_distance;
  glm::dquat rotation_versor;
};





#endif /* camera_cpp */
