#ifndef test_calculate2dObjectCoordinatesBehindCam_cpp
#define test_calculate2dObjectCoordinatesBehindCam_cpp

#include <vector>
#include <string>
#include <cmath>

#include <glm/vec3.hpp>

#include <ezprint.cpp>

#include "../test_funks.cpp"
#include "../../src/ecs/systems/transformation_funktions.cpp"

namespace test_calculate2dObjectCoordinatesBehindCam {
  const double PI = 3.14159265;

void test_function(const glm::dvec3& rotated_pos, const Transform& transform,
                   const std::vector<double>& correct_result, std::string case_num) {
  std::vector<double> result = pce::transform::calculate2dCoordinatesOfObjectBehindCam(
                                                   rotated_pos, transform);
  pce::test::report_result<std::vector<double>>(result, correct_result, case_num);
  ezp::print_item(result[0]);
  ezp::print_item(result[1]);
}

void run_test_module() {
  pce::test::test_connection();

  auto a = glm::dvec3(1.0, 1.0, 100.0);
  Transform b = Transform{};
  std::vector<double> cr{500.0*cos(PI/4.0), 500.0*sin(PI/4.0)};
  std::string case_num = "1";
  test_function(a, b, cr, case_num);

}

}


#endif /* test_calculate2dObjectCoordinatesBehindCam_cpp */
