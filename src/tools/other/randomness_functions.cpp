#ifndef randomness_functions_cpp
#define randomness_functions_cpp

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <iostream>


/* 
    Purpose: this module contains various functions for (psuedo-) random number generation.
    
    Prerequisites: set_srand() must be called before random numbers can be generated. 
*/


void set_srand()
{
    srand(time(NULL));
}

/* get random number between two inputs */
int get_rand_btwn (int num1, int num2)
{
    int rand_num = ( rand() % abs(num2-num1) + num1);
    return rand_num;
}

/* get random number between 0 and an input */
int mod_rand(int num)
{
    int rand_num = rand() % num;
    return rand_num;
}

/* either 0 or 1 */
bool coin_flip()
{
    int coin = mod_rand(2);
    return coin;
}

/* get random color */
std::vector<int> get_rand_color()
{
    int rand1 = rand()%255;
    int rand2 = rand()%255;
    int rand3 = rand()%255;

    std::vector<int> new_color = {rand1, rand2, rand3, 255};
    return new_color;
}

/* distance function */
int get_distance_btwn(std::vector<int> params_a, std::vector<int> params_b)
{
    /* place parameters at center of object */ 
    params_a[0] += int(params_a[2]/2);
    params_a[1] += int(params_a[2]/2);

    params_b[0] += int(params_b[2]/2);
    params_b[1] += int(params_b[2]/2);

    int x_distance = int(abs(params_a[0] - params_b[0]));
    int y_distance = int(abs(params_a[1] - params_b[1]));

    int distance = sqrt((x_distance*x_distance) + (y_distance*y_distance));
    
    return distance;
}

double get_rand_double_between_zero_and_input(double a) {
    double z = double(a) * 1000.0;
    int big_rand_int = mod_rand(int(z));
    double rand_double = double(big_rand_int)/1000.0;
    return rand_double;
}

double get_rand_double_between_two_doubles(double a, double b) {
    const double difference = a - b;
    const double normalized_random_double = get_rand_double_between_zero_and_input(difference);
    const double rand_double = normalized_random_double + a;
    return rand_double;
}

double get_rand_signed_double_between_two_doubles(double a, double b) {
    const double rand_double = get_rand_double_between_two_doubles(a, b);
    double sign = 1;
    const bool coin = coin_flip();
    if (coin) {  sign *= -1.0; }
    const double rand_signed_double = rand_double * sign;
    return rand_signed_double;
}





#endif /* randomness_functions_cpp */ 