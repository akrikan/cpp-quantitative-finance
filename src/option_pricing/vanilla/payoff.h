#ifndef __PAY_OFF_H
#define __PAY_OFF_H

#include<algorithm>

class PayOff{
public:
    PayOff(); //Default (no parameter)  constructor
    virtual ~PayOff(){}; // Virtual destructor

    virtual double operator() (const double S)const = 0;//Pure virtual method
};

class PayOffCall : public PayOff {
    private:
        double K; // strike price
    
    public:
        PayOffCall(const double K_); // not used {} since that will give us redefinition error
        virtual ~PayOffCall() {}; // Destructor virtual for further inheritance
        virtual double operator() (const double S) const ;

};

class PayOffPut : public PayOff {
    private:
        double K; // strike price
    
    public:
        PayOffPut(const double K_);
        virtual ~PayOffPut(){}; // Destructor virtual for further inheritance
        virtual double operator() (const double S)const ;

};

#endif