#ifndef BallManager_cpp
#define BallManager_cpp


/*----------------------------------------------------------------|
--------------------- Module Description -------------------------|
class to manage ballz
-> creation with random characteristics
-----------------------------------------------------------------*/
#include <vector>
#include "../constants/static_variables.cpp"
#include "../tools/other/randomness_functions.cpp"


extern ControlPanel control; 

const double min_z = -90.0;
const double max_z = 0.0;
const double min_x = 0.0;
const double max_x = 90.0;
const double min_y = 0.0;
const double max_y = 90.0;

const double min_r = 2.0;
const double max_r = 10.0;

class BallManager {
public:
  BallManager() {}


  /* random parameters within ranges */
  glm::dvec3 GetRandomLocation() {
    const double z = -1.0 * get_rand_double_between_zero_and_input(min_z);
    const double x = get_rand_signed_double_between_two_doubles(min_x, max_x);
    const double y = get_rand_signed_double_between_two_doubles(min_y, max_y);

    const auto location = glm::dvec3(x, y, z);

    return location;
  }

  double GetRandomRadius() {
    const double rand_radius = get_rand_double_between_two_doubles(min_r, max_r);
    return rand_radius;
  }

  double GetRandomDirection() {
    const bool flip = coin_flip();
    if (flip == true) {
      return 1.0;
    } else {
      return -1.0;
    }
  }


  void MakeBall() {
    const double new_ball_radius = GetRandomRadius();
    glm::dvec3 new_ball_location = GetRandomLocation();
    double new_ball_direction = GetRandomDirection();

    Entity ball = control.CreateEntity();
    control.AddComponent(ball, RigidBody{
        .radius=new_ball_radius,
        .location=new_ball_location,
        .direction=new_ball_direction
    });
    control.AddComponent(ball, Transform{});
    control.AddComponent(ball, RotatedLocation{.rot_pos=new_ball_location});
    balls.push_back(ball);
  }

private:
  std::vector<Entity> balls;

};





#endif /* BallManager_cpp */
