#ifndef __PAYOFF_DOUBLE_DIGITAL_H
#define __PAYOFF_DOUBLE_DIGITAL_H

#include "../vanilla/payoff.h"
class PayoffDoubleDigital : public PayOff{

private:
    double U; //Upper strike price
    double D; //Lower strike price

public:
    PayoffDoubleDigital(const double _U, const double _D);
    virtual ~PayoffDoubleDigital();
    virtual double operator() (const double S)const; //Payoff is 1 if spot within strike barriers , 0 otherwise

};




#endif