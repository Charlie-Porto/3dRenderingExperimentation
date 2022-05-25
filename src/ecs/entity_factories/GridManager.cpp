#ifndef GridManager_cpp
#define GridManager_cpp

/*----------------------------------------------------------------|
--------------------- Module Description -------------------------|
class that creates and maintains a 3-dimentional grid system
-----------------------------------------------------------------*/

#include <vector>
#include <ezprint.cpp>

#include "LineManager.cpp"

namespace pce {

class GridManager {
public:
  GridManager(){ ezp::print_item("creating Grid Manager"); }
  void Init() {
    line_manager_.Init();
  }

  void CreateGrid(double grid_span, double line_spacing) {
    const double grid_end_point = grid_span/2.0; 
    double current_point = line_spacing;
    const std::vector<double> multipliers{1.0, -1.0};

    while (current_point <= grid_end_point) {
    //   for (auto const& multiplier_a : multipliers) {
    //     for (auto const& multiplier_b : multipliers) {
    //       const glm::dvec3 line_a_point_a = glm::dvec3(grid_end_point, 0.0, current_point);
    //       const glm::dvec3 line_a_point_b = glm::dvec3(-grid_end_point, 0.0, current_point);
    //     }
    //   }
      const glm::dvec3 line_a_point_a = glm::dvec3(grid_end_point, 0.0, current_point);
      const glm::dvec3 line_a_point_b = glm::dvec3(-grid_end_point, 0.0, current_point);
      const glm::dvec3 line_b_point_a = glm::dvec3(current_point, 0.0, grid_end_point);
      const glm::dvec3 line_b_point_b = glm::dvec3(current_point, 0.0, -grid_end_point);
      const glm::dvec3 line_c_point_a = glm::dvec3(grid_end_point, 0.0, -current_point);
      const glm::dvec3 line_c_point_b = glm::dvec3(-grid_end_point, 0.0, -current_point);
      const glm::dvec3 line_d_point_a = glm::dvec3(-current_point, 0.0, grid_end_point);
      const glm::dvec3 line_d_point_b = glm::dvec3(-current_point, 0.0, -grid_end_point);

      line_manager_.MakeLine(line_a_point_a, line_a_point_b);
      line_manager_.MakeLine(line_b_point_a, line_b_point_b);
      line_manager_.MakeLine(line_c_point_a, line_c_point_b);
      line_manager_.MakeLine(line_d_point_a, line_d_point_b);



      current_point += line_spacing;
    }
  }  

  void DrawGrid() {
    line_manager_.DrawLines();
  }


private:
LineManager line_manager_;
 
};
}



#endif /* GridManager_cpp */
