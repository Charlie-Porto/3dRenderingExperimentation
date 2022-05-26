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



  // this function currently does not work -- to fix
  void CreateTheGrid(double max_point, double line_spacing, std::vector<int> slots) {
    ezp::print_item("greating grid");
    double current_point = line_spacing;
    int grid_line_num = 0;
    while (current_point <= max_point) {
      std::vector<double> points{max_point, current_point};
      std::vector<double> signs{1.0, -1.0};
      for (int i = 0; i < 2; ++i) {
        for (int j = 1; j > -1; --j) {
          // we only want the cases where we have one of each, so pass if i == j
          if (i == j) { continue; }
          for (auto const& sign : signs) {
            std::vector<double> dva = {0, 0, 0};
            std::vector<double> dvb = {0, 0, 0};
            dva[slots[0]] = points[i];
            dva[slots[1]] = points[j] * sign;
            dvb[slots[0]] = -points[i];
            dvb[slots[1]] = points[j] * sign;

            const auto endpoint_a = glm::dvec3(dva[0], dva[1], dva[2]);
            const auto endpoint_b = glm::dvec3(dvb[0], dvb[1], dvb[2]);
            ++grid_line_num;
            ezp::print_labeled_item("making grid line number: ", grid_line_num);
            line_manager_.MakeLine(endpoint_a, endpoint_b);
          }
        }
      }
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
