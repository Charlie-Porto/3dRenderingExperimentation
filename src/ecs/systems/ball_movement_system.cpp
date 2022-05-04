#ifndef ball_movement_system_cpp
#define ball_movement_system_cpp


/*----------------------------------------------------------------|
--------------------- Module Description -------------------------|
class to move ball around in 3D space
-----------------------------------------------------------------*/
#include "./../../constants/static_variables.cpp"
#include "../System.cpp"
#include "../../tools/math_functions/vector_functions.cpp"

extern ControlPanel Control;

class BallMovementSystem : public ISystem {
public:
  BallMovementSystem(){
    print_item("creating Ball Movement System");
    previous_time = 0;
  }

  void UpdateEntities(double time) {

    const double time_change = time - previous_time;
    for (auto const& entity: entities) {
      auto& rigid_body = control.GetComponent<RigidBody>(entity);

      // check if need to update direction
      const double max_distance = 150.0;
      const double ball_magnitude = vfunc::calculateMagnitude(rigid_body.location);
      if (ball_magnitude > max_distance) {
        rigid_body.direction *= -1.0;
      }

      rigid_body.location.z += z_speed*time_change*rigid_body.direction;
    }
    previous_time = time;
  }

private:
  double x_speed = -1;
  double y_speed = -1;
  double z_speed = 10;
  double previous_time;
};





#endif /* ball_movement_system_cpp */
