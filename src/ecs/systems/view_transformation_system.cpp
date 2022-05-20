#ifndef view_transformation_system_cpp
#define view_transformation_system_cpp


/*----------------------------------------------------------------|
--------------------- Module Description -------------------------|
system class that determines an entity's 2D screen coordinates
based on that entity's 3D location and size.
-----------------------------------------------------------------*/

#include <iostream>
#include <cmath>

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

#include <quaternion_funktions.cpp>
#include <virtual_keyboard.cpp>
#include <ezprint.cpp>
#include <camera.cpp>

#include "../../constants/static_variables.cpp"
#include "../System.cpp"
#include "../../tools/math_objects/Plane.cpp"
#include "../../tools/math_objects/LineParamEq.cpp"
#include "../../tools/math_functions/vector_functions.cpp"

extern ControlPanel control;

const double PI = 3.14159265;
const double FOCUS_DISTANCE = 30.0; // distance at which actual length = screen pixels
const glm::dvec3 Y_AXIS_UNIT_VECTOR = glm::dvec3(0.0, 1.0, 0.0);

// starting parameters
const glm::dvec3 start_pov = glm::dvec3(0.0, 0.0, 100.0);




class ViewTransformationSystem : public ISystem {
public:
  ViewTransformationSystem() {
    ezp::print_item("creating View Transformation System");
  }

  void Init() {
    camera_.point_of_view = start_pov;
  }

  void CalculateViewPlane() {
    view_plane_.x = camera_.point_of_view.x;
    view_plane_.y = camera_.point_of_view.y;
    view_plane_.z = camera_.point_of_view.z;
    view_plane_.c = 0.0;
  }

  LineParamEq CalculateObjectWireLine(glm::dvec3 object_position) {
    LineParamEq wire = vfunc::getLineThrough3dVectors(camera_.point_of_view, object_position);
    return wire;
  }

  glm::dvec3 CalculatePointAtWhichWireIntersectsViewPlane(glm::dvec3 object_position) {
    LineParamEq wire = CalculateWire(object_position);
    glm::dvec3 point_wire_intersects_viewplane = vfunc::getPointAtWhichLineIntersectsPlane(wire, view_plane_);
    return point_wire_intersects_viewplane;
  }

  double CalculateBallRenderRadius(glm::dvec3 location, double radius) {
    const double ball_distance_from_pov = vfunc::calculateDistanceBetweenPosition3Vectors(location, camera_.point_of_view);
    const double render_radius = radius * ((camera_.focus_distance/ball_distance_from_pov) * camera_.zoom_scalar);
    return render_radius;
  }

  bool CheckIfObjectIsInFrontOfPOV(glm::dvec3 object_position) {
    const double distance_ball_to_POV = vfunc::calculateDistanceBetweenPosition3Vectors(camera_.point_of_view, object_position);
    const double distance_ball_to_origin = vfunc::calculateMagnitude(object_position);

    if (distance_ball_to_origin < distance_ball_to_POV) {
      return true;
    } else {
      const double distance_POV_to_origin = vfunc::calculateMagnitude(camera_.point_of_view);
      if (distance_ball_to_origin < distance_POV_to_origin) {
        return true;
      }
    }
    return false;
  }

  void PollVirtualKeyboard() {
    Joystick_ buttons_pressed = virtual_keyboard_.check_buttons();  
    if (buttons_pressed.R_pressed == true) { 
      // print_item("JOYSTICK: RIGHT");
      // UpdateXZPointOfViewPosition(1);
    }
    if (buttons_pressed.L_pressed == true) {
      // print_item("JOYSTICK: LEFT");
      // UpdateXZPointOfViewPosition(-1);
    }
    if (buttons_pressed.Up_pressed == true) {
      // print_item("JOYSTICK: UP");
      // UpdateYPointOfViewPosition(1);
    }
    if (buttons_pressed.Down_pressed == true) {
      // print_item("JOYSTICK: DOWN");
      // UpdateYPointOfViewPosition(-1);
    }
  }
  

private:
  Camera camera_;
  Plane view_plane_;

  VirtualKeyboard virtual_keyboard_;

};




#endif /* view_transformation_system_cpp */
