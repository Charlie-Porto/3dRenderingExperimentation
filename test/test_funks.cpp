#ifndef test_funks_cpp
#define test_funks_cpp

/*----------------------------------------------------------------|
--------------------- Module Description -------------------------|
free functions to assist testing
-----------------------------------------------------------------*/
#include <ezprint.cpp>

namespace pce{
namespace test{

void test_connection() {
  ezp::print_item("test_funks connected");
}

template <typename T> void report_result(T result, T correct_result, std::string case_num) {
  if (result == correct_result) {
    ezp::print_labeled_item(case_num, ": pass");
  } else {
    ezp::print_labeled_item(case_num, ": FAIL");
  }
}

}
}

#endif /* test_funks_cpp */
