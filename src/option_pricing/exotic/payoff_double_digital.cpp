#ifndef __PAYOFF_DOUBLE_DIGITAL_CPP
#define __PAYOFF_DOUBLE_DIGITAL_CPP
#include "payoff_double_digital.h"

PayoffDoubleDigital::PayoffDoubleDigital(const double _U,const double _D){

    U = _U;
    D = _D;

}
PayoffDoubleDigital::~PayoffDoubleDigital(){}

double PayoffDoubleDigital::operator()(const double S)const{
    if(S>=D && S<=U){
        return 1.0;
    }else {
        return 0.0;
    }
    
}
#endif