#ifndef test_transformation_funktions_cpp
#define test_transformation_funktions_cpp

/*----------------------------------------------------------------|
--------------------- Module Description -------------------------|
to test transformation funktions
-----------------------------------------------------------------*/
#include <string>

#include <glm/vec3.hpp>

#include <ezprint.cpp>

#include "test_funks.cpp"
#include "../src/ecs/components/transform_component.cpp"
#include "../src/ecs/systems/transformation_funktions.cpp"


namespace pce{
namespace test{

void test_checkIfObjectIsInFrontOfPOV(const glm::dvec3& obj_pos, 
                                      const glm::dvec3& camera_pos,
                                      const double camera_pov_scalar,
                                      bool correct_result,
                                      std::string& case_num) {
                                      
  const bool result = pce::transform::checkIfObjectIsInFrontOfPOV(obj_pos,
                                                                  camera_pos,
                                                                  camera_pov_scalar);

  pce::test::report_result<bool>(result, correct_result, case_num);
}



void run_module_test() {
  ezp::print_item("Running Module Test: TransformationFunctions");
  ezp::print_item("testing function: checkIfObjectIsInFrontOfPOV");

  std::string case_num = "1";
  auto a = glm::dvec3(0.0, 0.0, 10.0);
  auto b = glm::dvec3(0.0, 0.0, 100.0);
  double c = 100.0;
  bool cr = true;
  test_checkIfObjectIsInFrontOfPOV(a, b, c, cr, case_num);

  case_num = "2";
  a = glm::dvec3(0.0, 0.0, 200.0);
  b = glm::dvec3(0.0, 0.0, 100.0);
  c = 100.0;
  cr = false;
  test_checkIfObjectIsInFrontOfPOV(a, b, c, cr, case_num);

  case_num = "3";
  a = glm::dvec3(0.0, 0.0, -100.0);
  b = glm::dvec3(0.0, 0.0, 100.0);
  c = 100.0;
  cr = true;
  test_checkIfObjectIsInFrontOfPOV(a, b, c, cr, case_num);

  case_num = "4";
  a = glm::dvec3(0.0, 0.0, 100.0);
  b = glm::dvec3(0.0, 0.0, 100.0);
  c = 100.0;
  cr = false;
  test_checkIfObjectIsInFrontOfPOV(a, b, c, cr, case_num);

  case_num = "5";
  a = glm::dvec3(0.0, 100.0, 0.0);
  b = glm::dvec3(0.0, 0.0, 100.0);
  c = 100.0;
  cr = true;
  test_checkIfObjectIsInFrontOfPOV(a, b, c, cr, case_num);

  case_num = "6";
  a = glm::dvec3(200.0, 0.0, -200.0);
  b = glm::dvec3(0.0, 0.0, 100.0);
  c = 100.0;
  cr = true;
  test_checkIfObjectIsInFrontOfPOV(a, b, c, cr, case_num);

  case_num = "7";
  a = glm::dvec3(-100.0, 0.0, 0.0);
  b = glm::dvec3(-90.0, 0.0, 0.0);
  c = 90.0;
  cr = false;
  test_checkIfObjectIsInFrontOfPOV(a, b, c, cr, case_num);

  case_num = "8";
  a = glm::dvec3(100.0, 0.0, 0.0);
  b = glm::dvec3(-90.0, 0.0, 0.0);
  c = 90.0;
  cr = true;
  test_checkIfObjectIsInFrontOfPOV(a, b, c, cr, case_num);

  case_num = "9";
  a = glm::dvec3(-10.0, 100.0, 10.0);
  b = glm::dvec3(90.0, 0.0, 0.0);
  c = 90.0;
  cr = true;
  test_checkIfObjectIsInFrontOfPOV(a, b, c, cr, case_num);
  
  case_num = "10";
  a = glm::dvec3(-10.0, 10.0, 10.0);
  b = glm::dvec3(0.0, 0.0, -50.0);
  c = 90.0;
  cr = true;
  test_checkIfObjectIsInFrontOfPOV(a, b, c, cr, case_num);
  
  ezp::print_item("-------------------------------");
}


}
}







#endif /* test_transformation_funktions_cpp */
