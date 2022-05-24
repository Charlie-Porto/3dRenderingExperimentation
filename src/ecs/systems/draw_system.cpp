#ifndef draw_system_cpp
#define draw_system_cpp

/*----------------------------------------------------------------|
--------------------- Module Description -------------------------|
class DrawSystem: draws all on-screen components
-----------------------------------------------------------------*/

#include <vector>
#include <string>
#include <iostream>

#include "../../tools/other/drawingfunctions.cpp"
#include "../../tools/other/randomness_functions.cpp"
#include "../../sdl_core/sim_manager.cpp"
#include "../../constants/static_variables.cpp"

extern ControlPanel control;

const std::vector<int> mcolor = get_rand_color();

class DrawSystem : public ISystem {

public:
  DrawSystem() { std::cout<< "creating DrawSystem" <<'\n'; }

  std::vector<int> ConvertCartesianCoordinatesToSDL(Transform transform) {
    const int sdl_x = int(transform.x) + int(global_const::screen_x/2);
    const int sdl_y = int(-transform.y) + int(global_const::screen_y/2);
    const std::vector<int> v = {sdl_x, sdl_y};
    return v;
  }
  

  void UpdateEntities() {
    for (auto const& entity : entities) {
      auto const& transform = control.GetComponent<Transform>(entity);

      if (transform.if_on_screen == true) {

        const std::vector<int> sdl_transform = ConvertCartesianCoordinatesToSDL(transform);
       
        DrawCircle(sdl_transform[0], sdl_transform[1], transform.render_radius, mcolor, Simulation::renderer);
      }
    }
  }


};




#endif /* draw_system_cpp */
