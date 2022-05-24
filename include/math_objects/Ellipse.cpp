#ifndef Ellipse_cpp
#define Ellipse_cpp

namespace pce {
namespace math_objs {


struct Ellipse{
  double p; // semi-latus rectum
  double e; // eccentricity
  double r_max; // semi-minor axis
  double r_min; // perihelion
};


}}


#endif /* Ellipse_cpp */
