#ifndef ezprint_cpp
#define ezprint_cpp

#include <iostream>
#include <string>

#include <glm/vec3.hpp>
#include <glm/ext/quaternion_double.hpp>

/*
  Purpose: this module contains functions to speed up the printing strings and integers/doubles/etc. labeled by strings.
*/

namespace ezp {

template <typename T> void print_item(T& item) {
  std::cout << item << '\n';
}

template <typename T>
void print_labeled_item(std::string label, T item) {
  std::cout << label << item << '\n';
}

void print_dquat(glm::dquat q) {
  print_labeled_item("w: ", q.w);
  print_labeled_item("x: ", q.x);
  print_labeled_item("y: ", q.y);
  print_labeled_item("z: ", q.z);
}

void print_dvec3(glm::dvec3 v) {
  print_labeled_item("x: ", v.x);
  print_labeled_item("y: ", v.y);
  print_labeled_item("z: ", v.z);
}

}

#endif /* ezprint_cpp */