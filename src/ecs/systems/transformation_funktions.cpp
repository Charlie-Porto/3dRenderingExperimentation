#ifndef transformation_funktions_cpp
#define transformation_funktions_cpp

#include <cmath>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
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
auto VIEW_PLANE = pce::math_objs::Plane{.x=0.0, .y=0.0, .z=1.0, .c=0.0};

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

bool checkIfObjectIsInFrontOfPOV(const glm::dvec3& obj_pos, const glm::dvec3& camera_pos,
                                                            const double camera_pov_scalar) {
  const double distance_obj_to_POV = vfunc::calculateDistanceBetweenPosition3Vectors(camera_pos, obj_pos);
  const double distance_obj_to_origin = vfunc::calculateMagnitude(obj_pos);

  if (distance_obj_to_origin < distance_obj_to_POV) {
    return true;
  } else {
    if (distance_obj_to_origin < camera_pov_scalar) {
      return true;
    }
  }
  return false;
}

// NOTE: SIGNS (+-) ARE VERY IMPORTANT IN THIS FUNCTION
void updateCameraReverseRotationVersor(Camera& camera) {
  const glm::dquat vert_rot_versor = cam_func::getCameraVerticalRotationVersor(-camera.y_angle);
  const glm::dquat horiz_rot_versor = cam_func::getCameraHorizontalRotationVersor(camera.xz_angle);
  camera.rotation_versor = horiz_rot_versor * vert_rot_versor;
}


glm::dvec3 performObjectReverseRotation(const glm::dquat& rotation_versor,
                                        const glm::dvec3& location) {
  return qfunc::rotateVector3byQuaternion(location, rotation_versor);
}


double calculateObjectRenderRadius(const glm::dvec3& location, const double& radius, 
                                   const Camera& cam) {
  const double obj_distance_from_pov = vfunc::calculateDistanceBetweenPosition3Vectors(location, cam.location_vec3);
  const double render_radius = radius * (cam.focus_distance/obj_distance_from_pov);
  return render_radius;
}


void calculate2dCoordinatesOfObjectBehindCam(const glm::dvec3& rotated_pos, 
                                             Transform& transform) {
  const double tx = rotated_pos.x; 
  const double ty = rotated_pos.y; 

  const double sign_x = double(abs(tx))/tx;
  const double sign_y = double(abs(ty))/ty;

  const auto point_in_xy_plane = glm::dvec2(tx, ty);
  const auto point_xy_distance_from_cam = vfunc::calculateMagnitude(point_in_xy_plane);

  const double angle_of_point = acos(tx/point_xy_distance_from_cam);
  const double screen_view_radius = 1000.0;

  double new_x = screen_view_radius * cos(angle_of_point);
  double new_y = screen_view_radius * sin(angle_of_point) * sign_y;
  transform.x = new_x;
  transform.y = new_y;
}


void calculate2dObjectCoordinates(const glm::dvec3& rotated_pos, 
                                  Transform& transform, 
                                  double pov_scalar) {
  const auto camera_pos = glm::dvec3(0.0, 0.0, 1.0) * pov_scalar;

    pce::math_objs::LineParametricEquation object_wire = vfunc::getLineThrough3dVectors(
                                                            camera_pos, rotated_pos);
    VIEW_PLANE.z = pov_scalar-1.0;
    glm::dvec3 point_wire_intersects_viewplane = vfunc::getPointAtWhichLineIntersectsPlane(
                                                        object_wire, VIEW_PLANE);
    

  if (transform.if_on_screen == true) {
    transform.x = point_wire_intersects_viewplane.x * 2.0;
    transform.y = point_wire_intersects_viewplane.y * 2.0;
  } else {
    calculate2dCoordinatesOfObjectBehindCam(rotated_pos, transform);
    // transform.x = -point_wire_intersects_viewplane.x;
    // transform.y = -point_wire_intersects_viewplane.y;
  }
}


JoystickReport pollVirtualKeyboard(VirtualKeyboard& keyboard) {
  return keyboard.check_buttons();
}


void updateCameraXZAngle(Camera& camera, const double& direction) {
  if (direction != 0.0) {
    camera.xz_angle += global_const::hop_angle * direction;
  }

  const double new_camera_xpos = camera.xz_circle_radius * sin(PI * camera.xz_angle/180.0);
  const double new_camera_zpos = camera.xz_circle_radius * cos(PI * camera.xz_angle/180.0);

  camera.location_vec3.x = new_camera_xpos;
  camera.location_vec3.z = new_camera_zpos;

  ezp::print_dvec3(camera.location_vec3);
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

void updateCameraPositionScalar(Camera& camera, double direction) {
  camera.pov_scalar += direction * global_const::movement_speed;
  updateCameraYAngle(camera, 0.0);
}

void updateCameraPosition(Camera& camera, VirtualKeyboard& keyboard) {
  JoystickReport joystick_report = pollVirtualKeyboard(keyboard);
  if (joystick_report.R_pressed == true) { 
    // ezp::print_item("JOYSTICK: RIGHT");
    updateCameraXZAngle(camera, 1.0);
  }
  if (joystick_report.L_pressed == true) {
    // ezp::print_item("JOYSTICK: LEFT");
    updateCameraXZAngle(camera, -1.0);
  }
  if (joystick_report.Up_pressed == true) {
    // ezp::print_item("JOYSTICK: UP");
    updateCameraYAngle(camera, 1.0);
  }
  if (joystick_report.Down_pressed == true) {
    // ezp::print_item("JOYSTICK: DOWN");
    updateCameraYAngle(camera, -1.0);
  }
  if (joystick_report.W_pressed == true) {
    // ezp::print_item("JOYSTICK: W");
    updateCameraPositionScalar(camera, -1.0);
  }
  if (joystick_report.S_pressed == true) {
    // ezp::print_item("JOYSTICK: S");
    updateCameraPositionScalar(camera, 1.0);
  }
}




}
}





#endif /* transformation_funktions_cpp */
