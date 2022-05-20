#ifndef transformation_system_cpp
#define transformation_system_cpp


/*----------------------------------------------------------------|
--------------------- Module Description -------------------------|
system class that determines an entity's 2D screen coordinates
based on that entity's 3D location and size.
-----------------------------------------------------------------*/

#include <iostream>
#include <cmath>

#include "../System.cpp"
#include "../../constants/static_variables.cpp"

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

#include <quaternion_funktions.cpp>
#include <virtual_keyboard.cpp>
#include <ezprint.cpp>
#include "../../tools/math_objects/Plane.cpp"
#include "../../tools/math_objects/LineParamEq.cpp"
#include "../../tools/math_functions/vector_functions.cpp"

#ifndef PI
#define PI 3.14159265
#endif
using Scalar = double;
using Angle = double;

const double FOCUS_DISTANCE = 30.0; // distance at which actual length = screen pixels
const glm::dvec3 Y_AXIS_UNIT_VECTOR = glm::dvec3(0.0, 1.0, 0.0);
extern ControlPanel control;

class TransformationSystem : public ISystem { 
public:
  TransformationSystem() {
    std::cout<<"creating TestTransformation System"<< '\n';
    point_of_view = glm::dvec3(0, 0, 100);
    previous_time = 0;
  }
  TransformationSystem(glm::dvec3 pov) {
    std::cout<<"creating TestTransformation System"<< '\n';
    point_of_view = pov;
    previous_time = 0;
  }

  void Init() {
    joystick = VirtualKeyboard();
    xz_view_angle_ = 0.0;
    y_view_angle_ = 0.0;
    pov_magnitude = vfunc::calculateMagnitude(point_of_view);
    pov_vertical_rotation_versor_ = glm::dquat(1, 0, 0, 0);
    conj_pov_vertical_rotation_versor_ = glm::conjugate(pov_vertical_rotation_versor_);
    xz_circle_radius_ = cos(y_view_angle_ / 180.0 * PI) * pov_magnitude;
    print_labeled_item("circle radius: ", xz_circle_radius_);
    CalculateLatitudeAndLongitude();
    CalculateViewPlane();
  }

  void CalculateLatitudeAndLongitude() {
    pov_magnitude = vfunc::calculateMagnitude(point_of_view);
    latitude = tan(((width_angle_of_view/2.0) * PI) / 180.0) * pov_magnitude;
    longitude = tan(((height_angle_of_view/2.0) * PI) / 180.0) * pov_magnitude; 
  }

  void CalculateViewPlane() {
    view_plane.x = point_of_view.x;
    view_plane.y = point_of_view.y;
    view_plane.z = point_of_view.z;
    view_plane.c = 0.0;
  }

  void CalculateWire(glm::dvec3 ball_position) {
    wire = vfunc::getLineThrough3dVectors(point_of_view, ball_position);
  }

  void CalculatePointAtWhichWireIntersectsViewPlane() {
    point_wire_intersects_viewplane = vfunc::getPointAtWhichLineIntersectsPlane(wire, view_plane);
  }

  int CalculateSignOfDouble(double num) {
    if (num < 0) {
      return -1;
    } else {
      return 1;
    }
  }


  // TODO: update this function
  void Calculate2dObjectCoordinates() {
    const glm::dvec3 normed_point = CalculateNormalizedIntersectionPoint(point_wire_intersects_viewplane);

    // const int point_x_sign = CalculateSignOfDouble(point_wire_intersects_viewplane.x);
    // const int point_z_sign = CalculateSignOfDouble(point_wire_intersects_viewplane.z);
    // const int point_y_sign = CalculateSignOfDouble(point_wire_intersects_viewplane.y);

    const int point_x_sign = CalculateSignOfDouble(normed_point.x);
    ezp::print_labeled_item("point_x_sign", point_x_sign);
    const int point_z_sign = CalculateSignOfDouble(normed_point.z);
    const int point_y_sign = CalculateSignOfDouble(normed_point.y);

    const int POV_z_sign = CalculateSignOfDouble(point_of_view.z);
    ezp::print_labeled_item("pov_z_sign", POV_z_sign);
    const int POV_x_sign = CalculateSignOfDouble(point_of_view.x);

    const int screen_x_sign = point_x_sign * POV_z_sign;
    const int screen_z_sign = point_z_sign * POV_x_sign;
    const int screen_y_sign = point_y_sign;

    // const double render_x = sqrt(pow(normed_point.x, 2.0) + 
                                //  pow(normed_point.z, 2.0)) *
                                //  double(screen_x_sign);
                                         
    const double render_x = sqrt(pow(point_wire_intersects_viewplane.x, 2.0) + 
                                 pow(point_wire_intersects_viewplane.z, 2.0)) * 
                                 double(screen_x_sign) / 
                                 cos(y_view_angle_);

    // const double render_y = sqrt(pow(point_wire_intersects_viewplane.y, 2.0) + 
                                //  pow(render_x, 2.0)) * 
                                //  double(screen_y_sign);

    const double render_y = point_wire_intersects_viewplane.y;

    ball_render_x = render_x; 
    ball_render_y = render_y; 
  }

  glm::dvec3 CalculateNormalizedIntersectionPoint(glm::dvec3 location) {
    glm::dquat location_quat;
    location_quat.w = 0.0;
    location_quat.x = location.x;
    location_quat.y = location.y;
    location_quat.z = location.z;
    // ezp::print_item("========== not-yet-normalized: ===========");
    // ezp::print_dquat(location_quat);
    

    glm::dquat normed_location_quat = conj_pov_vertical_rotation_versor_ * location_quat * pov_vertical_rotation_versor_;
    glm::dvec3 normed_location_dvec3 = glm::dvec3(normed_location_quat.x, normed_location_quat.y, normed_location_quat.z);
    // ezp::print_item("========== intersection normalized: ===========");
    // ezp::print_dquat(normed_location_quat);
    // ezp::print_item("-------------");
    return normed_location_dvec3;
  }


  double CalculateBallRenderRadius(glm::dvec3 location, double radius) {
    const double ball_distance_from_pov = vfunc::calculateDistanceBetweenPosition3Vectors(location, point_of_view);
    const double render_radius = radius * ((FOCUS_DISTANCE/ball_distance_from_pov) * zoom_scalar);
    const double ball_render_radius = render_radius; 
    return ball_render_radius;
  }

  bool CheckIfObjectIsInFrontOfPOV(glm::dvec3 ball_position) {
    const double distance_ball_to_POV = vfunc::calculateDistanceBetweenPosition3Vectors(point_of_view, ball_position);
    const double distance_ball_to_origin = vfunc::calculateMagnitude(ball_position);

    if (distance_ball_to_origin < distance_ball_to_POV) {
      return true;
    } else {
      const double distance_POV_to_origin = vfunc::calculateMagnitude(point_of_view);
      if (distance_ball_to_origin < distance_POV_to_origin) {
        return true;
      }
    }
    return false;
  }
    
  void UpdateXZPointOfViewPosition(int incremental_angle_sign) {
    print_item("MOVING POSITION");
    xz_view_angle_ += global_const::hop_angle * double(incremental_angle_sign);
    const std::vector<double> new_z_and_x = vfunc::movePointAroundXZCircleByAngle(
                                                xz_view_angle_,
                                                pov_magnitude);

    const double current_y = point_of_view.y;
    point_of_view = glm::dvec3(new_z_and_x[1], current_y, new_z_and_x[0]);

    // print_labeled_item("pov x:", point_of_view.x);
    // print_labeled_item("pov y:", point_of_view.y);
    // print_labeled_item("pov z:", point_of_view.z);

    CalculateViewPlane();
  }

  void UpdateYPointOfViewPosition(int incremental_angle_sign) {
    y_view_angle_ += global_const::hop_angle * double(incremental_angle_sign);
    const double new_y_coordinate = vfunc::movePointAroundYCircleByAngle(y_view_angle_,
                                                                         pov_magnitude);

    print_item("MOVING POSITION");
    print_labeled_item("y_view_angle: ", y_view_angle_);
    // MUST: update xz_circle_radius based on new y_view_angle
    xz_circle_radius_ = cos(y_view_angle_ / 180.0 * PI) * pov_magnitude;
    print_labeled_item("xz_circle_radius: ", xz_circle_radius_);
    // THEN MUST: update x and y points based on current position
    // NOTE: xz angle did not change; only xz radius.
    const std::vector<double> new_z_and_x = vfunc::movePointAroundXZCircleByAngle(
                                                xz_view_angle_,
                                                xz_circle_radius_);
    point_of_view = glm::dvec3(new_z_and_x[1], new_y_coordinate, new_z_and_x[0]);

    // print_labeled_item("pov x:", point_of_view.x);
    // print_labeled_item("pov y:", point_of_view.y);
    // print_labeled_item("pov z:", point_of_view.z);

    CalculateVerticalRotationQuaternion();
    CalculateViewPlane();
  }


  void PollVirtualKeyboard() {
    Joystick_ buttons_pressed = joystick.check_buttons();  
    int incremental_angle_sign = 1;
    if (buttons_pressed.R_pressed == true) { 
      // print_item("JOYSTICK: RIGHT");
      UpdateXZPointOfViewPosition(1);
    }
    if (buttons_pressed.L_pressed == true) {
      // print_item("JOYSTICK: LEFT");
      UpdateXZPointOfViewPosition(-1);
    }
    if (buttons_pressed.Up_pressed == true) {
      // print_item("JOYSTICK: UP");
      UpdateYPointOfViewPosition(1);
    }
    if (buttons_pressed.Down_pressed == true) {
      // print_item("JOYSTICK: DOWN");
      UpdateYPointOfViewPosition(-1);
    }
  }

  void CalculateVerticalRotationQuaternion() {
    ezp::print_item("CALCULATING NEW ROTATION QUATERNION");
    glm::dvec3 y_axis = Y_AXIS_UNIT_VECTOR;
    glm::dvec3 rotation_axis = glm::cross(y_axis, point_of_view);
    rotation_axis = glm::normalize(rotation_axis);
    ezp::print_item("rotation axis: ");
    ezp::print_dvec3(rotation_axis);
    pov_vertical_rotation_versor_ = qfunc::convertAngleAxisToQuaternion(-y_view_angle_, rotation_axis);
    pov_vertical_rotation_versor_ = qfunc::zeroOutQuaternionRealComponent(pov_vertical_rotation_versor_);
    conj_pov_vertical_rotation_versor_ = glm::conjugate(pov_vertical_rotation_versor_);
    ezp::print_item("vertical rotation versor: ");
    ezp::print_dquat(pov_vertical_rotation_versor_);
  }



  void UpdateEntities() {
    PollVirtualKeyboard();
    for (auto const& entity : entities) {

      auto& transform = control.GetComponent<Transform>(entity);
      auto const& rigid_body = control.GetComponent<RigidBody>(entity);
      const bool if_object_in_render_zone = CheckIfObjectIsInFrontOfPOV(rigid_body.location);
      transform.if_on_screen = if_object_in_render_zone;

      if (transform.if_on_screen) {

        CalculateWire(rigid_body.location);
        CalculatePointAtWhichWireIntersectsViewPlane();
        Calculate2dObjectCoordinates();

        double render_radius = CalculateBallRenderRadius(rigid_body.location, rigid_body.radius);

        transform.x = ball_render_x;
        transform.y = ball_render_y;
        transform.render_radius = int(render_radius);
      }
    }
  }


private:
  glm::dvec3 point_of_view;
  double pov_magnitude; // distance from origin
  glm::dquat pov_vertical_rotation_versor_;
  glm::dquat conj_pov_vertical_rotation_versor_;
  double xz_circle_radius_;
  glm::dvec3 view_direction; // unused currently; always pointing directly at origin
  Angle xz_view_angle_;
  Angle y_view_angle_;
  Plane view_plane;

  LineParamEq wire;
  glm::dvec3 point_wire_intersects_viewplane;
  double latitude;
  double longitude;
  glm::dvec3 view_pane_left_point;
  glm::dvec3 view_pane_right_point;

  Angle width_angle_of_view = global_const::width_angle_of_view;
  Angle height_angle_of_view = global_const::height_angle_of_view;

  double ball_render_y;
  double ball_render_x;

  Scalar zoom_scalar = 1;
  VirtualKeyboard joystick;
  double previous_time;
};





#endif /* transformation_system_cpp */
