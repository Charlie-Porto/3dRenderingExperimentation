#ifndef LineManager_cpp
#define LineManager_cpp


/*----------------------------------------------------------------|
--------------------- Module Description -------------------------|
class to manage fuckin' lines. shit.
-----------------------------------------------------------------*/

#include <vector>
#include "../../constants/static_variables.cpp"
#include "../../tools/other/randomness_functions.cpp"
#include "../../sdl_core/sim_manager.cpp"

extern ControlPanel control; 

class LineManager {
public:
  LineManager(){}

  void Init() {
    MakeXAxisLine();
    MakeYAxisLine();
    MakeZAxisLine();
  }

  // to move this function elsewhere (used also in draw_system)
  std::vector<int> ConvertCartesianCoordinatesToSDL(Transform transform) {
    const int sdl_x = int(transform.x) + int(global_const::screen_x/2);
    const int sdl_y = int(-transform.y) + int(global_const::screen_y/2);
    const std::vector<int> v = {sdl_x, sdl_y};
    return v;
  }

  void MakeXAxisLine() {
    glm::dvec3 x_axis_point_a = glm::dvec3(-300.0, 0.0, 0.0);
    glm::dvec3 x_axis_point_b = glm::dvec3(300.0, 0.0, 0.0);
    MakeLine(x_axis_point_a, x_axis_point_b);
  }
  void MakeYAxisLine() {
    glm::dvec3 y_axis_point_a = glm::dvec3(0.0, -300.0, 0.0);
    glm::dvec3 y_axis_point_b = glm::dvec3(0.0, 300.0, 0.0);
    MakeLine(y_axis_point_a, y_axis_point_b);
  }
  void MakeZAxisLine() {
    glm::dvec3 z_axis_point_a = glm::dvec3(0.0, 0.0, -300.0);
    glm::dvec3 z_axis_point_b = glm::dvec3(0.0, 0.0, 300.0);
    MakeLine(z_axis_point_a, z_axis_point_b);
  }


  void MakeLine(const glm::dvec3& a, const glm::dvec3& b) {
    Entity entity_point_a = control.CreateEntity();
    Entity entity_point_b = control.CreateEntity();

    control.AddComponent(entity_point_a, RigidBody{
        .radius=20.0,
        .location=a,
        .direction=0.0 // stationary
    });
    control.AddComponent(entity_point_b, RigidBody{
        .radius=20.0,
        .location=b,
        .direction=0.0 // stationary
    });
    control.AddComponent(entity_point_a, RotatedLocation{});
    control.AddComponent(entity_point_b, RotatedLocation{});
    control.AddComponent(entity_point_a, Transform{});
    control.AddComponent(entity_point_b, Transform{});

    lines.push_back({entity_point_a, entity_point_b});
  }

  void DrawLines() {
    SDL_SetRenderDrawColor(Simulation::renderer, 255, 202, 170, 255);
    for (const auto& line : lines) {
      const auto& point_a_transform = control.GetComponent<Transform>(line[0]);
      const auto& point_b_transform = control.GetComponent<Transform>(line[1]);
      // if (point_a_transform.if_on_screen == true &&
          // point_b_transform.if_on_screen == true){
        const std::vector<int> a_sdl_coordinates = ConvertCartesianCoordinatesToSDL(
                                                      point_a_transform);
        const std::vector<int> b_sdl_coordinates = ConvertCartesianCoordinatesToSDL(
                                                      point_b_transform);

        SDL_RenderDrawLine(
          Simulation::renderer,
          a_sdl_coordinates[0], a_sdl_coordinates[1],
          b_sdl_coordinates[0], b_sdl_coordinates[1]
        );                                           
      // } 
    }
    SDL_SetRenderDrawColor(Simulation::renderer, 0, 0, 0, 255);
  }


private:
  std::vector<std::vector<Entity>> lines;
};




#endif /* LineManager_cpp */
