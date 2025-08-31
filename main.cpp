#include <iostream>
#include <vector>
#include <iomanip>
#include <ctime>

// Option pricing headers
#include "src/option_pricing/vanilla/vanilla_option.h"
#include "src/option_pricing/vanilla/payoff.h"
#include "src/option_pricing/exotic/payoff_double_digital.h"
#include "src/option_pricing/exotic/asian.h"
#include "src/option_pricing/monte_carlo/path_generation.h"

// Math library headers
#include "src/math/matrix/simplematrix.h"
#include "src/math/statistics/statistics.h"
#include "src/math/random/linear_congruential_generator.h"

// Implied volatility headers
#include "src/implied_volatility/interval_bisection.h"

using namespace std;

void print_separator() {
    cout << "\n" << string(60, '=') << "\n";
}

void demo_vanilla_options() {
    print_separator();
    cout << "VANILLA OPTION PRICING (Black-Scholes)\n";
    print_separator();
    
    // Create a vanilla option
    double K = 100.0;    // Strike price
    double r = 0.05;     // Risk-free rate
    double T = 1.0;      // Time to maturity
    double S = 100.0;    // Current spot price
    double sigma = 0.2;  // Volatility
    
    VanillaOption option(K, r, T, S, sigma);
    
    // Calculate prices
    double call_price = option.calc_call_price();
    double put_price = option.calc_put_price();
    
    cout << "Option Parameters:\n";
    cout << "  Strike (K):       " << K << endl;
    cout << "  Risk-free rate:   " << r << endl;
    cout << "  Time to maturity: " << T << " years" << endl;
    cout << "  Spot price (S):   " << S << endl;
    cout << "  Volatility:       " << sigma << endl;
    cout << "\nPrices:\n";
    cout << "  Call Price: $" << fixed << setprecision(2) << call_price << endl;
    cout << "  Put Price:  $" << fixed << setprecision(2) << put_price << endl;
}

void demo_payoff_functions() {
    print_separator();
    cout << "PAYOFF FUNCTIONS\n";
    print_separator();
    
    double K = 100.0;
    PayOffCall call_payoff(K);
    PayOffPut put_payoff(K);
    PayoffDoubleDigital digital_payoff(110.0, 90.0);
    
    cout << "Strike = " << K << endl;
    cout << "\nSpot    Call Payoff    Put Payoff    Digital Payoff\n";
    cout << "-----   -----------    ----------    --------------\n";
    
    for (double S = 80; S <= 120; S += 10) {
        cout << setw(5) << S << "   "
             << setw(11) << call_payoff(S) << "    "
             << setw(10) << put_payoff(S) << "    "
             << setw(14) << digital_payoff(S) << endl;
    }
}

void demo_asian_options() {
    print_separator();
    cout << "ASIAN OPTIONS (Monte Carlo Pricing)\n";
    print_separator();
    
    // Option parameters
    double K = 100.0;
    double r = 0.05;
    double T = 1.0;
    double S_0 = 100.0;
    double sigma = 0.2;
    
    // Monte Carlo parameters
    int num_paths = 10000;
    int num_steps = 252; // Daily steps for one year
    
    // Create payoff and Asian option objects
    PayOffCall call_payoff(K);
    AsianOptionArithmetic asian_arithmetic(&call_payoff);
    AsianOptionGeometric asian_geometric(&call_payoff);
    
    // Run Monte Carlo simulation
    double arith_price = 0.0;
    double geom_price = 0.0;
    
    srand(time(0));
    
    for (int i = 0; i < num_paths; i++) {
        vector<double> spot_prices(num_steps, S_0);
        calc_path_spot_prices(spot_prices, r, sigma, T);
        
        arith_price += asian_arithmetic.pay_off_price(spot_prices);
        geom_price += asian_geometric.pay_off_price(spot_prices);
    }
    
    arith_price = (arith_price / num_paths) * exp(-r * T);
    geom_price = (geom_price / num_paths) * exp(-r * T);
    
    cout << "Asian Option Parameters:\n";
    cout << "  Strike: " << K << endl;
    cout << "  Risk-free rate: " << r << endl;
    cout << "  Time to maturity: " << T << " years" << endl;
    cout << "  Initial spot: " << S_0 << endl;
    cout << "  Volatility: " << sigma << endl;
    cout << "\nMonte Carlo Simulation:\n";
    cout << "  Number of paths: " << num_paths << endl;
    cout << "  Number of steps: " << num_steps << endl;
    cout << "\nPrices:\n";
    cout << "  Arithmetic Asian Call: $" << fixed << setprecision(2) << arith_price << endl;
    cout << "  Geometric Asian Call:  $" << fixed << setprecision(2) << geom_price << endl;
}

void demo_matrix_operations() {
    print_separator();
    cout << "MATRIX OPERATIONS\n";
    print_separator();
    
    // Create a 3x3 matrix
    SimpleMatrix<double> mat(3, 3, 0.0);
    
    // Fill with some values
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            mat.value(i, j) = (i + 1) * (j + 1);
        }
    }
    
    cout << "3x3 Matrix:\n";
    for (int i = 0; i < 3; i++) {
        cout << "  ";
        for (int j = 0; j < 3; j++) {
            cout << setw(4) << mat.value(i, j) << " ";
        }
        cout << endl;
    }
    
    // Create identity matrix
    SimpleMatrix<double> identity(3, 3, 0.0);
    for (int i = 0; i < 3; i++) {
        identity.value(i, i) = 1.0;
    }
    
    cout << "\n3x3 Identity Matrix:\n";
    for (int i = 0; i < 3; i++) {
        cout << "  ";
        for (int j = 0; j < 3; j++) {
            cout << setw(4) << identity.value(i, j) << " ";
        }
        cout << endl;
    }
}

void demo_random_number_generation() {
    print_separator();
    cout << "RANDOM NUMBER GENERATION\n";
    print_separator();
    
    unsigned long num_draws = 10;
    unsigned long init_seed = 1;
    
    LinearCongruentialGenerator lcg(num_draws, init_seed);
    vector<double> uniform_draws(num_draws);
    lcg.get_uniform_draws(uniform_draws);
    
    cout << "Linear Congruential Generator:\n";
    cout << "  Number of draws: " << num_draws << endl;
    cout << "  Initial seed: " << init_seed << endl;
    cout << "\nUniform draws (0,1):\n";
    for (unsigned long i = 0; i < num_draws; i++) {
        cout << "  Draw " << setw(2) << i+1 << ": " 
             << fixed << setprecision(6) << uniform_draws[i] << endl;
    }
    
    // Convert to normal distribution
    StandardNormalDistribution snd;
    vector<double> normal_draws(num_draws);
    snd.random_draws(uniform_draws, normal_draws);
    
    cout << "\nNormal draws (mean=0, std=1):\n";
    for (unsigned long i = 0; i < num_draws; i++) {
        cout << "  Draw " << setw(2) << i+1 << ": " 
             << fixed << setprecision(6) << normal_draws[i] << endl;
    }
}

void demo_statistics() {
    print_separator();
    cout << "STATISTICAL DISTRIBUTIONS\n";
    print_separator();
    
    StandardNormalDistribution snd;
    
    cout << "Standard Normal Distribution:\n";
    cout << "  Mean: " << snd.mean() << endl;
    cout << "  Variance: " << snd.var() << endl;
    cout << "  Std Dev: " << snd.stdev() << endl;
    
    cout << "\nPDF and CDF values:\n";
    cout << "  x      PDF(x)     CDF(x)\n";
    cout << "------  --------   --------\n";
    
    for (double x = -2.0; x <= 2.0; x += 0.5) {
        cout << setw(6) << fixed << setprecision(2) << x << "  "
             << setw(8) << fixed << setprecision(4) << snd.pdf(x) << "   "
             << setw(8) << fixed << setprecision(4) << snd.cdf(x) << endl;
    }
    
    cout << "\nQuantiles (inverse CDF):\n";
    cout << "  p       inv_cdf(p)\n";
    cout << "------   ----------\n";
    double quantiles[] = {0.05, 0.25, 0.5, 0.75, 0.95};
    for (int i = 0; i < 5; i++) {
        cout << setw(6) << fixed << setprecision(2) << quantiles[i] << "   "
             << setw(10) << fixed << setprecision(4) << snd.inv_cdf(quantiles[i]) << endl;
    }
}

void demo_implied_volatility() {
    print_separator();
    cout << "IMPLIED VOLATILITY (Bisection Method)\n";
    print_separator();
    
    // Market price of a call option
    double market_price = 10.5;
    double K = 100.0;
    double r = 0.05;
    double T = 1.0;
    double S = 100.0;
    
    cout << "Market Option Data:\n";
    cout << "  Market Price: $" << market_price << endl;
    cout << "  Strike: " << K << endl;
    cout << "  Spot: " << S << endl;
    cout << "  Risk-free rate: " << r << endl;
    cout << "  Time to maturity: " << T << " years" << endl;
    
    // Create a function object for bisection
    class CallPriceFunctor {
    public:
        double K, r, T, S;
        CallPriceFunctor(double _K, double _r, double _T, double _S)
            : K(_K), r(_r), T(_T), S(_S) {}
        
        double operator()(double sigma) const {
            VanillaOption opt(K, r, T, S, sigma);
            return opt.calc_call_price();
        }
    };
    
    CallPriceFunctor price_func(K, r, T, S);
    
    // Find implied volatility using bisection
    double vol_lower = 0.01;
    double vol_upper = 1.0;
    double epsilon = 0.0001;
    
    double implied_vol = interval_bisection(market_price, vol_lower, vol_upper, 
                                           epsilon, price_func);
    
    cout << "\nImplied Volatility: " << fixed << setprecision(2) 
         << implied_vol * 100 << "%" << endl;
    
    // Verify by calculating price with implied vol
    VanillaOption verify_option(K, r, T, S, implied_vol);
    double calculated_price = verify_option.calc_call_price();
    
    cout << "Verification:\n";
    cout << "  Calculated Price: $" << fixed << setprecision(2) << calculated_price << endl;
    cout << "  Difference: $" << fixed << setprecision(4) 
         << abs(calculated_price - market_price) << endl;
}

int main() {
    cout << "\n";
    cout << "========================================\n";
    cout << "   C++ QUANTITATIVE FINANCE LIBRARY    \n";
    cout << "        Demonstration Program           \n";
    cout << "========================================\n";
    
    // Run all demonstrations
    demo_vanilla_options();
    demo_payoff_functions();
    demo_asian_options();
    demo_matrix_operations();
    demo_random_number_generation();
    demo_statistics();
    demo_implied_volatility();
    
    print_separator();
    cout << "All demonstrations completed successfully!\n";
    cout << endl;
    
    return 0;
}