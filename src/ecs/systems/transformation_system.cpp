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

#include "../../../include/glm/vec3.hpp"
#include "../../tools/math_objects/Plane.cpp"
#include "../../tools/math_objects/LineParamEq.cpp"
#include "../../tools/math_functions/vector_functions.cpp"
#include "../../tools/other/virtual_keyboard.cpp"

#ifndef PI
#define PI 3.14159265
#endif
using Scalar = double;
using Angle = double;

const double FOCUS_DISTANCE = 30.0; // distance at which actual length = screen pixels
const double SPACE_CURVE_DISTANCE = 100.0;


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
    view_angle = 0.0;
    pov_magnitude = vfunc::calculateMagnitude(point_of_view);
    CalculateLatitudeAndLongitude();
    CalculateViewPlane();
  }

  void CalculateLatitudeAndLongitude() {
    pov_magnitude = vfunc::calculateMagnitude(point_of_view);
    latitude = tan(((width_angle_of_view/2.0) * PI) / 180.0) * pov_magnitude;
    longitude = tan(((height_angle_of_view/2.0) * PI) / 180.0) * pov_magnitude; 
    // print_labeled_item("latitude: ", latitude);
    // print_labeled_item("longitude: ", longitude);
  }

  void CalculateEndPointsOfViewPane() {
    // pick up here; brain is not in great shape on 5/3/2022 (poor sleep)

  }

  void CalculateViewPlane() {
    view_plane.x = point_of_view.x;
    view_plane.y = point_of_view.y;
    view_plane.z = point_of_view.z;
    view_plane.c = 0.0;

    // print_item("calculating view plane");
    // std::cout<< "x: " <<view_plane.x<<'\n';
    // std::cout<< "y: " <<view_plane.y<<'\n';
    // std::cout<< "z: " <<view_plane.z<<'\n';
    // std::cout<< "c: " <<view_plane.c<<'\n';
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

  int CalculateSignOf2dXGiven3dIntersectionPoint(glm::dvec3 intersection_point) {
    return 0;
  }



  void Calculate2dObjectCoordinates() {
    const int point_x_sign = CalculateSignOfDouble(point_wire_intersects_viewplane.x);
    const int point_z_sign = CalculateSignOfDouble(point_wire_intersects_viewplane.z);
    const int point_y_sign = CalculateSignOfDouble(point_wire_intersects_viewplane.y);
    const int POV_z_sign = CalculateSignOfDouble(point_of_view.z);

    const int screen_x_sign = point_x_sign * POV_z_sign;
    const int screen_y_sign = point_y_sign;

    // const double render_x = sqrt(pow(point_wire_intersects_viewplane.vect[0], 2.0) + 
                                //  pow(point_wire_intersects_viewplane.vect[2], 2.0));
                                         
    const double render_x = sqrt(pow(point_wire_intersects_viewplane.x, 2.0) + 
                                 pow(point_wire_intersects_viewplane.z, 2.0)) * 
                                 double(screen_x_sign);

    // const double render_y = sqrt(pow(point_wire_intersects_viewplane.vect[1], 2.0) + 
                                //  pow(point_wire_intersects_viewplane.vect[2], 2.0)) * 
                                //  double(screen_y_sign);

    const double render_y = point_wire_intersects_viewplane.y;

    ball_render_x = render_x; 
    ball_render_y = render_y; 
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
    
  void UpdatePointOfViewPosition(int incremental_angle_sign) {
    view_angle += global_const::hop_angle * double(incremental_angle_sign);
    const std::vector<double> new_z_and_x = vfunc::movePointAroundCircleByAngle(
                                                view_angle,
                                                pov_magnitude);

    const double current_y = point_of_view.y;
    point_of_view = glm::dvec3(new_z_and_x[1], current_y, new_z_and_x[0]);

    CalculateViewPlane();
  }

  void PollVirtualKeyboard() {
    std::vector<bool> buttons_pressed = joystick.check_buttons();  
    int incremental_angle_sign = 1;
    if (buttons_pressed[0] == true) { 
      print_item("JOYSTICK: RIGHT");
      UpdatePointOfViewPosition(1);
    }
    if (buttons_pressed[1] == true) {
      print_item("JOYSTICK: LEFT");
      UpdatePointOfViewPosition(-1);
    }
    if (buttons_pressed[2] == true) {
      // print_item("JOYSTICK: UP");
    }
    if (buttons_pressed[3] == true) {
      // print_item("JOYSTICK: DOWN");
    }
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
  glm::dvec3 view_direction; // unused currently; always pointing directly at origin
  Angle view_angle;
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
