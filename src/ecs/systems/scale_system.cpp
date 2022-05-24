#ifndef scale_system_cpp
#define scale_system_cpp


/*----------------------------------------------------------------|
--------------------- Module Description -------------------------|
system that alters the scale of objects based on configured constants
-----------------------------------------------------------------*/

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <ezprint.cpp>


#include "../System.cpp"
#include "../../constants/space_variables.cpp"

extern ControlPanel control;

namespace pce {


class ScaleSystem : public ISystem {
public:
  ScaleSystem() { ezp::print_item("creating Scale System"); }

  glm::dvec3 ScaleObjectPosition(glm::dvec3 position) {
    glm::dvec3 scaled_position = position / space_const::POSITION_SCALE;
    return scaled_position;
  }

  double ScaleObjectRadius(double radius) {
    double scaled_radius = radius / space_const::RADIUS_SCALE;
    return scaled_radius;
  }
   
  void UpdateEntities() {
    ezp::print_item("updating scale system");

    for (auto const& entity : entities) {
      ezp::print_labeled_item("entity: ", entity);
      auto const& position = control.GetComponent<Position>(entity);
      auto& scaled_position = control.GetComponent<ScaledPosition>(entity);
      auto const& rigid_body = control.GetComponent<RigidBody>(entity);
      auto& scaled_rigid_body = control.GetComponent<ScaledRigidBody>(entity);

      scaled_position.center_point = ScaleObjectPosition(position.center_point);
      scaled_rigid_body.radius = ScaleObjectRadius(rigid_body.radius_mi);
      ezp::print_item("scaled center point: ");
      ezp::print_dvec3(scaled_position.center_point);
      ezp::print_labeled_item("scaled radius: ", scaled_rigid_body.radius);
    }
  }

};
}



#endif /* scale_system_cpp */
