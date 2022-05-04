#ifndef CubeManager_cpp
#define CubeManager_cpp


/*----------------------------------------------------------------|
--------------------- Module Description -------------------------|
class to make and manage cubes. aw shit broh!!!!
-----------------------------------------------------------------*/


#include <utility>
#include <vector>
#include "../static_variables.cpp"
#include "../tools/randomness_functions.cpp"
#include "../tools/Vec3.cpp"
#include "../sim_manager.cpp"
#include "../tools/Cube.cpp"

extern ControlPanel control; 

class CubeManager {
public:
  void Init(){
    MakeCubeOfSideLength(20.0);
  }

  std::vector<int> ConvertCartesianCoordinatesToSDL(Transform transform) {
    const int sdl_x = int(transform.x) + int(global_const::screen_x/2);
    const int sdl_y = int(-transform.y) + int(global_const::screen_y/2);
    const std::vector<int> v = {sdl_x, sdl_y};
    return v;
  }

  void MakeCubeOfSideLength(double side_length){
    print_item("making cube");
    const Vec3<double> cube_center = Vec3<double>({10.0, -5.0, 2.0});
    const double half_side = side_length/2.0;

    const std::vector<double> y_crawls = {half_side, -half_side};
    const std::vector<double> x_crawls = {half_side, -half_side};
    const std::vector<double> z_crawls = {half_side, -half_side};

    Cube new_cube_object = Cube();

    for (auto const& y_dir : y_crawls) {
      for (auto const& x_dir : y_crawls) {
        for (auto const& z_dir : y_crawls) {
          Entity new_cube_entity = control.CreateEntity();
          new_cube_object.AddEntityReference(new_cube_entity);

          std::vector<double> npoint = {cube_center.vect[1] + y_dir,
                                        cube_center.vect[0] + x_dir,
                                        cube_center.vect[2] + z_dir};
          
          Vec3<double> new_point = Vec3(npoint);
          new_cube_object.AddPoint(new_point);

          control.AddComponent(new_cube_entity, RigidBody{
                                                    .radius=1.0,
                                                    .location=new_point,
                                                    .direction=0.0});

          control.AddComponent(new_cube_entity, Transform{});

          cubes.push_back(std::make_pair(new_cube_entity, new_cube_object));
        }
      }
    }
  }

  void DrawCubes() {
    SDL_SetRenderDrawColor(Simulation::renderer, 255, 202, 170, 255);
    for (auto pair : cubes) {
      const std::vector<Vec3<double>> points = pair.second.GetPoints();
      for (int i = 1; i < points.size(); ++i) {
        for (int j = 0; j < i; ++j) {
          int if_line = 0;
          int if_same_x = int(points[i].vect[0] == points[j].vect[0]);
          int if_same_y = int(points[i].vect[1] == points[j].vect[1]);
          int if_same_z = int(points[i].vect[2] == points[j].vect[2]);

          if (if_line > 1) {
            // draw line between transpose.x and transpose.y
            // to do later
            auto const& transform_a = control.GetComponent<Transform>(pair.second.entity_refs[i]);
            auto const& transform_b = control.GetComponent<Transform>(pair.second.entity_refs[j]);

            std::vector<int> a = ConvertCartesianCoordinatesToSDL(transform_a);
            std::vector<int> b = ConvertCartesianCoordinatesToSDL(transform_b);

            SDL_RenderDrawLine(Simulation::renderer,
                               a[0], a[1],
                               b[0], b[1]
                              );                                           
          }
        }
      }
    }
    SDL_SetRenderDrawColor(Simulation::renderer, 0, 0, 0, 255);
  }

private:
  // eight vertexes per cube
  std::vector<std::pair<Entity, Cube>> cubes;
};




#endif /* CubeManager_cpp */
