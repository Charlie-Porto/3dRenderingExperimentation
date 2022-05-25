#ifndef orbit_funktions_cpp
#define orbit_funktions_cpp


/*----------------------------------------------------------------|
--------------------- Module Description -------------------------|
functions to assist the orbit system
-----------------------------------------------------------------*/
#include <cmath>

#include <boost/multiprecision/cpp_int.hpp>
#include <glm/vec3.hpp>

#include <ezprint.cpp>

#include "../../tools/math_functions/vector_functions.cpp"

#include "../../constants/space_variables.cpp"

using huge_int = boost::multiprecision::cpp_int;

namespace pce {
namespace orbit {

// reason for this odd code: cannot store a 30-digit number in an integer lvalue
const huge_int front_half_mass_of_the_sun = 19090000000000000;
const huge_int mass_multiplier = 100000000000000000;
const huge_int mass_of_the_sun = front_half_mass_of_the_sun * mass_multiplier;

// G is inverted so that we can divide by integer instead of multiply by double
const huge_int inverted_gravitational_constant = 66743000000;

double calculateGravitationalForceFromTheSun(const glm::dvec3& object_position, 
                                             const double& object_mass) {
  ezp::print_labeled_item("mass of the sun: ", mass_of_the_sun);
  const huge_int sun_grav_times_grav_const = mass_of_the_sun / inverted_gravitational_constant;
  const huge_int distance = 7000646400; // meters
  const huge_int sun_times_grav_over_distance = sun_grav_times_grav_const / distance;
  const huge_int mercury_mass = 3285000000 * mass_multiplier;
  ezp::print_labeled_item("mass of mercury: ", mercury_mass);
  const huge_int sun_gravity = mercury_mass * sun_grav_times_grav_const / distance;
  ezp::print_labeled_item("sun gravity: ", sun_gravity);
  return 1.0;
}


}
}


#endif /* orbit_funktions_cpp */
