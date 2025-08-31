#ifndef __VANILLA_OPTION_H // preprocessor directives 
#define __VANILLA_OPTION_H // Helps us to avoid same code twice for the compiler
class VanillaOption {
    private:
        void init();
        void copy(const VanillaOption& rhs);

        double K; // Strike price
        double r; // Risk-free rate
        double T; //Maturity time
        double S; // Underlying assest price
        double sigma; //Volatility of underlying asset

    public:
        VanillaOption(); // Default constructor - has no parameters
        VanillaOption(const double& _K ,const double& _r,
                      const double& _T, const double&_S,
                      const double& _sigma); // Parameter constructor
        VanillaOption(const VanillaOption& rhs); // copy constructor
        VanillaOption& operator=(const VanillaOption& rhs); //Assignment operator - 
        //if not defined we can't assign two option to be equal in this case(will lead to an error if we do so)
        virtual ~VanillaOption(); // Destructor is virtual

        // Selector("getter") methods for our option paramters
        double getK() const;
        double getr() const;
        double getT() const;
        double getS() const;
        double getsigma() const;
        
        // option price calculation methods
        double calc_call_price() const;
        double calc_put_price() const;

};


#endif  // the program at this point stops Vanilla_option.h that was defined earlier
