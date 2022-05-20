#ifndef transform_system_cpp
#define transform_system_cpp


#include "transformation_funktions.cpp"
#include "../System.cpp"
#include "../../constants/static_variables.cpp"
#include "../../tools/math_objects/Plane.cpp"
#include "../../tools/math_objects/LineParamEq.cpp"
#include "../../tools/math_functions/vector_functions.cpp"


#include <glm/vec3.hpp>
#include <ezprint.cpp>
#include <camera.cpp>
#include <virtual_keyboard.cpp>


extern ControlPanel control;
const double FOCUS_DISTANCE = 30.0; // distance at which actual length = screen pixels

namespace pce {

// starting camera info
const double start_xz_angle = 0.0;
const double start_y_angle = 0.0;
const auto start_pov = glm::dvec3(0.0, 0.0, 1.0);
// const auto start_dov = glm::dvec3(0.0, 0.0, -1.0);
const double start_pos_scalar = 100.0;


class TransformSystem : public ISystem {
public:
  TransformSystem() {
    ezp::print_item("creating Transform system");
  }


  void Init() {
    ezp::print_item("initializing Transform system");
    camera_.pov_unitvec3 = start_pov;
    camera_.pov_scalar = start_pos_scalar;
    camera_.y_angle = start_y_angle;
    camera_.xz_angle = start_xz_angle;
  }


  void UpdateCamera() {
    // update the camera. to relocate this responsibility eventually; should not be in this class
    // - camera position (angles): poll keyboard
    pce::transform::updateCameraPosition(camera_, keyboard_);

  }

  void UpdateEntities() {
    // updates 2D coordinates and render radius for each object
    for (auto const& entity : entities) {
      auto& transform = control.GetComponent<Transform>(entity);
      auto const& rigid_body = control.GetComponent<RigidBody>(entity);

    }
    
  }

private:
  Camera camera_;
  VirtualKeyboard keyboard_;


};



}







#endif /* transform_system_cpp */
