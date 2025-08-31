#include <iostream>
#include "payoff_double_digital.h"

int main(int argc, char **argv){

    double D = 10.0;
    double U = 15.0;

    PayoffDoubleDigital pay(U,D);

    std::cout << "Spot = 5.0 : " << pay(5) <<std::endl;
    std::cout << "Spot = 10.0: " << pay(10) <<std::endl;
    std::cout << "Spot = 20.0: " << pay(20) <<std::endl;

    return 0;
}