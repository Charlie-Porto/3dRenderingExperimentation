#ifndef transformation_funktions_cpp
#define transformation_funktions_cpp

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

#include <ezprint.cpp>
#include <virtual_keyboard.cpp>
#include <camera.cpp>
#include <camera_functions.cpp>

#include "../../constants/static_variables.cpp"
#include "../../tools/math_objects/Plane.cpp"
#include "../../tools/math_objects/LineParamEq.cpp"
#include "../../tools/math_functions/vector_functions.cpp"

/*----------------------------------------------------------------|
--------------------- Module Description -------------------------|
free functions to assist the TransformSystem class
-----------------------------------------------------------------*/

namespace pce {
namespace transform {

const double PI = 3.14159265;

glm::dvec3 calculateCameraCoordinates(const glm::dvec3& pos_unitvec3, const double& pos_scalar) {
  return pos_scalar * pos_unitvec3;
}

pce::math_objs::LineParametricEquation calculateWire(glm::dvec3 object_rotated_pos,
                                                        glm::dvec3 camera_pos) {
  return vfunc::getLineThrough3dVectors(object_rotated_pos, camera_pos);
}

glm::dvec3 calculatePointWireIntersectsViewplane(pce::math_objs::LineParametricEquation wire,
                                                 pce::math_objs::Plane viewplane) {
  return vfunc::getPointAtWhichLineIntersectsPlane(wire, viewplane);

}

bool checkIfObjectIsInFrontOfPOV(const glm::dvec3& obj_pos, const glm::dvec3& camera_pos) {
  const double distance_obj_to_POV = vfunc::calculateDistanceBetweenPosition3Vectors(camera_pos, obj_pos);
  const double distance_obj_to_origin = vfunc::calculateMagnitude(obj_pos);

  if (distance_obj_to_origin < distance_obj_to_POV) {
    return true;
  } else {
    const double distance_POV_to_origin = vfunc::calculateMagnitude(camera_pos);
    if (distance_obj_to_origin < distance_POV_to_origin) {
      return true;
    }
  }
  return false;
}


void updateCameraReverseRotationVersor(Camera& camera) {
  const glm::dquat vert_rot_versor = cam_func::getCameraVerticalRotationVersor(-camera.y_angle);
  const glm::dquat horiz_rot_versor = cam_func::getCameraHorizontalRotationVersor(-camera.xz_angle);
  camera.rotation_versor = horizontal_rotation_versor * vertical_rotation_versor;
}


JoystickReport pollVirtualKeyboard(VirtualKeyboard& keyboard) {
  return keyboard.check_buttons();
}

void updateCameraXZAngle(Camera& camera, const double& direction) {
  if (direction != 0) {
    camera.xz_angle += global_const::hop_angle * direction;
  }

  const double new_camera_xpos = camera.xz_circle_radius * sin(PI * camera.xz_angle/180.0);
  const double new_camera_zpos = camera.xz_circle_radius * cos(PI * camera.xz_angle/180.0);

  camera.location_vec3.x = new_camera_xpos;
  camera.location_vec3.z = new_camera_zpos;

  // ezp::print_item("new camera position");
  // ezp::print_labeled_item("cam x: ", camera.location_vec3.x);
  // ezp::print_labeled_item("cam y: ", camera.location_vec3.y);
  // ezp::print_labeled_item("cam z: ", camera.location_vec3.x);

  updateCameraReverseRotationVersor(camera);
}

void updateCameraYAngle(Camera& camera, const double& direction) {
  camera.y_angle += global_const::hop_angle * direction;
  const double new_camera_ypos = camera.pov_scalar * sin(PI * camera.y_angle/180.0);
  const double new_camera_xz_radius = camera.pov_scalar * cos(PI * camera.y_angle/180.0);

  camera.location_vec3.y = new_camera_ypos;
  camera.xz_circle_radius = new_camera_xz_radius;
  updateCameraXZAngle(camera, 0.0);
}

void updateCameraPosition(Camera& camera, VirtualKeyboard& keyboard) {
  JoystickReport joystick_report = pollVirtualKeyboard(keyboard);
  if (joystick_report.R_pressed == true) { 
    ezp::print_item("JOYSTICK: RIGHT");
    updateCameraXZAngle(camera, 1.0);
  }
  if (joystick_report.L_pressed == true) {
    ezp::print_item("JOYSTICK: LEFT");
    updateCameraXZAngle(camera, -1.0);
  }
  if (joystick_report.Up_pressed == true) {
    ezp::print_item("JOYSTICK: UP");
    updateCameraYAngle(camera, 1.0);
  }
  if (joystick_report.Down_pressed == true) {
    ezp::print_item("JOYSTICK: DOWN");
    updateCameraYAngle(camera, -1.0);
  }
}




}
}





#endif /* transformation_funktions_cpp */
