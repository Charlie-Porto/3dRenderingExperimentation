#ifndef static_variables_cpp
#define static_variables_cpp


/*----------------------------------------------------------------|
--------------------- Module Description -------------------------|
to store static consts used in the project
-----------------------------------------------------------------*/

namespace global_const {
  const int screen_x = 1000;
  const int screen_y = 672;

	const double meter_to_pixel_ratio = 0.5/8.4;
  const double screen_height_meters = double(screen_y) * meter_to_pixel_ratio;
  const double screen_width_meters = double(screen_x) * meter_to_pixel_ratio;

  const double width_angle_of_view = 120; // degrees
  const double height_angle_of_view = 120; // degrees
  const double zoom_scalar = 1;

  const double movement_speed = .5; // feet crawled per btn press
  const double hop_angle = 1; // degrees crawled per btn press
}


#endif /* static_variables_cpp */
