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

namespace pce {

const double FOCUS_DISTANCE = 30.0; // distance at which actual length = screen pixels
auto VIEW_PLANE = pce::math_objs::Plane{.x=0.0, .y=0.0, .z=1.0, .c=0.0};

// starting camera info
const double start_xz_angle = 0.0;
const double start_y_angle = 0.0;
const auto start_pov = glm::dvec3(0.0, 0.0, 1.0);
// const auto start_dov = glm::dvec3(0.0, 0.0, -1.0);
const double start_pos_scalar = 3000000;


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
    camera_.xz_circle_radius = start_pos_scalar;
    camera_.focus_distance = FOCUS_DISTANCE;
    camera_.location_vec3 = start_pov * start_pos_scalar;
    pce::transform::updateCameraReverseRotationVersor(camera_);
  }

  void UpdateCamera() {
    pce::transform::updateCameraPosition(camera_, keyboard_);
  }

  void UpdateEntities() {
    UpdateCamera();
    for (auto const& entity : entities) {
      // ezp::print_labeled_item("transforming entity: ", entity);
      auto& transform = control.GetComponent<Transform>(entity);
      auto& rotated_location = control.GetComponent<RotatedLocation>(entity);
      auto const& rigid_body = control.GetComponent<ScaledRigidBody>(entity);
      auto const& position = control.GetComponent<ScaledPosition>(entity);

      rotated_location.rot_pos = pce::transform::performObjectReverseRotation(
                                                     camera_.rotation_versor,
                                                     position.center_point);

      transform.if_on_screen = pce::transform::checkIfObjectIsInFrontOfPOV(
                                                 position.center_point,
                                                 camera_.location_vec3);
      
      transform.render_radius = pce::transform::calculateObjectRenderRadius(
                                                    position.center_point,
                                                    rigid_body.radius,
                                                    camera_);

      pce::transform::calculate2dObjectCoordinates(rotated_location.rot_pos,
                                                   transform,
                                                   camera_.pov_scalar);

      // ezp::print_item("------------------");
      // ezp::print_labeled_item("if on screen: ", transform.if_on_screen);
      // ezp::print_item("location: ");
      // ezp::print_dvec3(position.center_point);
      // // ezp::print_item("updated rot_pos: ");
      // // ezp::print_dvec3(rotated_location.rot_pos);

      // ezp::print_labeled_item("2D x: ", transform.x);
      // ezp::print_labeled_item("2D y: ", transform.y);
      // ezp::print_labeled_item("render radius: ", transform.render_radius);
    }
    
  }

private:
  Camera camera_;
  VirtualKeyboard keyboard_;


};



}







#endif /* transform_system_cpp */
