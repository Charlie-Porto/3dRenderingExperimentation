#ifndef transformation_funktions_cpp
#define transformation_funktions_cpp

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

#include <ezprint.cpp>
#include <virtual_keyboard.cpp>
#include <camera.cpp>

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

glm::dvec3 calculatePovCoordinates(const glm::dvec3& pos_unitvec3, const double& pos_scalar) {
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


JoystickReport pollVirtualKeyboard(VirtualKeyboard& keyboard) {
  return keyboard.check_buttons();
}

void updateCameraYAngle(Camera& camera, double direction) {
}
void updateCameraXZAngle(Camera& camera, double direction) {
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
