#ifndef orbit_system_cpp
#define orbit_system_cpp


#include <boost/multiprecision/cpp_int.hpp>

#include <ezprint.cpp>

#include "orbit_funktions.cpp"

#include "../System.cpp"

#include "../../constants/space_variables.cpp"

extern ControlPanel control;
namespace pce {


class OrbitSystem : public ISystem {
public:
  OrbitSystem() { 
    ezp::print_item("creating Orbit System"); 
    previous_time_ = 0.0;
  }


  void UpdateEntities(double time){
    const double time_increment = time - previous_time_;
    previous_time_ = time;
    for (auto const& entity : entities) {
      auto const& rigid_body = control.GetComponent<RigidBody>(entity);
      auto& motion = control.GetComponent<Motion>(entity);
      auto& position = control.GetComponent<Position>(entity);

      const double force_of_sun_gravity = pce::orbit::calculateGravitationalForceFromTheSun(
                                              position.center_point, rigid_body.mass_kg);
      
      // ezp::print_labeled_item("sun gravity: ", force_of_sun_gravity);

    }
  }

private:
  double previous_time_;
  
};
}





#endif /* orbit_system_cpp */
