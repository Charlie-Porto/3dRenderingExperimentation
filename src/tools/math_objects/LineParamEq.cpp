#ifndef LineParamEq_cpp
#define LineParamEq_cpp


/*----------------------------------------------------------------|
--------------------- Module Description -------------------------|
struct for a line in terms of a parametric equation
-----------------------------------------------------------------*/

namespace pce {
namespace math_objs {

struct LineParametricEquation {
  double x_coefficient;
  double y_coefficient;
  double z_coefficient;

  double x_constant;
  double y_constant;
  double z_constant;
};

}
}





#endif /* LineParamEq_cpp */
