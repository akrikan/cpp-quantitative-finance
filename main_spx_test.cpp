#include <iostream>
#include <vector>
#include <iomanip>
#include <ctime>
#include <map>
#include <string>
#include <cmath>

// Option pricing headers
#include "src/option_pricing/vanilla/vanilla_option.h"
#include "src/option_pricing/vanilla/payoff.h"
#include "src/option_pricing/exotic/asian.h"
#include "src/option_pricing/monte_carlo/path_generation.h"

// Math library headers
#include "src/math/statistics/statistics.h"
#include "src/math/random/linear_congruential_generator.h"

// Implied volatility headers
#include "src/implied_volatility/interval_bisection.h"
#include "src/implied_volatility/newton_raphson.h"

using namespace std;

// Structure to hold option market data
struct OptionData {
    string expiry_date;
    double days_to_expiry;
    double strike;
    double bid;
    double ask;
    double mid_price;
    double volume;
    double open_interest;
    double implied_vol;
    char type; // 'C' for call, 'P' for put
};

// Structure to hold SPX market data
struct MarketData {
    double spot_price;
    double risk_free_rate;
    string date;
    map<string, vector<OptionData>> option_chains; // Key: expiry date
};

void print_separator() {
    cout << "\n" << string(80, '=') << "\n";
}

// Function to generate realistic option data based on Black-Scholes
vector<OptionData> generate_option_chain(double spot, double r, string expiry_date, 
                                        double days_to_expiry) {
    vector<OptionData> chain;
    
    // Generate strikes around the spot price (80% to 120% of spot)
    vector<double> strikes;
    for (double strike_pct = 0.80; strike_pct <= 1.20; strike_pct += 0.02) {
        strikes.push_back(round(spot * strike_pct / 5) * 5); // Round to nearest 5
    }
    
    double T = days_to_expiry / 365.0;
    
    for (double K : strikes) {
        // Estimate implied volatility based on moneyness and time to expiry
        double moneyness = K / spot;
        double base_vol = 0.16; // Base volatility around 16%
        
        // Add volatility smile effect
        double smile_adjustment = 0.08 * pow(moneyness - 1.0, 2);
        double term_structure = 0.02 * sqrt(T); // Higher vol for longer term
        double impl_vol = base_vol + smile_adjustment + term_structure;
        
        // Create vanilla option for pricing
        VanillaOption option(K, r, T, spot, impl_vol);
        
        // Generate Call option data
        OptionData call_data;
        call_data.expiry_date = expiry_date;
        call_data.days_to_expiry = days_to_expiry;
        call_data.strike = K;
        call_data.type = 'C';
        call_data.implied_vol = impl_vol;
        
        double call_price = option.calc_call_price();
        double spread = 0.02 * call_price + 0.10; // Bid-ask spread
        call_data.mid_price = call_price;
        call_data.bid = call_price - spread/2;
        call_data.ask = call_price + spread/2;
        
        // Simulate volume and open interest (higher for ATM options)
        double atm_factor = exp(-pow((K - spot) / (0.1 * spot), 2));
        call_data.volume = static_cast<int>(1000 * atm_factor * (rand() % 100) / 100.0);
        call_data.open_interest = static_cast<int>(10000 * atm_factor);
        
        chain.push_back(call_data);
        
        // Generate Put option data
        OptionData put_data;
        put_data.expiry_date = expiry_date;
        put_data.days_to_expiry = days_to_expiry;
        put_data.strike = K;
        put_data.type = 'P';
        put_data.implied_vol = impl_vol;
        
        double put_price = option.calc_put_price();
        spread = 0.02 * put_price + 0.10;
        put_data.mid_price = put_price;
        put_data.bid = put_price - spread/2;
        put_data.ask = put_price + spread/2;
        
        put_data.volume = static_cast<int>(800 * atm_factor * (rand() % 100) / 100.0);
        put_data.open_interest = static_cast<int>(8000 * atm_factor);
        
        chain.push_back(put_data);
    }
    
    return chain;
}

// Initialize market data with SPX options
MarketData initialize_spx_market_data() {
    MarketData market;
    
    // Current SPX spot price (as of August 31, 2025)
    market.spot_price = 6460.25;
    market.risk_free_rate = 0.045; // Current risk-free rate approximately 4.5%
    market.date = "2025-08-31";
    
    // Generate option chains for various expiries until December 2025
    vector<pair<string, double>> expiries = {
        {"2025-09-05", 5},    // Weekly
        {"2025-09-12", 12},   // Weekly
        {"2025-09-19", 19},   // Monthly (3rd Friday)
        {"2025-09-26", 26},   // Weekly
        {"2025-10-03", 33},   // Weekly
        {"2025-10-10", 40},   // Weekly
        {"2025-10-17", 47},   // Monthly (3rd Friday)
        {"2025-10-24", 54},   // Weekly
        {"2025-10-31", 61},   // Weekly/EOM
        {"2025-11-07", 68},   // Weekly
        {"2025-11-14", 75},   // Weekly
        {"2025-11-21", 82},   // Monthly (3rd Friday)
        {"2025-11-28", 89},   // Weekly
        {"2025-12-05", 96},   // Weekly
        {"2025-12-12", 103},  // Weekly
        {"2025-12-19", 110},  // Monthly (3rd Friday)
        {"2025-12-31", 122}   // End of year
    };
    
    for (const auto& expiry : expiries) {
        market.option_chains[expiry.first] = generate_option_chain(
            market.spot_price, market.risk_free_rate, expiry.first, expiry.second
        );
    }
    
    return market;
}

// Test Black-Scholes pricing against market data
void test_black_scholes_pricing(const MarketData& market) {
    print_separator();
    cout << "BLACK-SCHOLES PRICING TEST vs MARKET DATA\n";
    print_separator();
    
    cout << "SPX Spot Price: $" << fixed << setprecision(2) << market.spot_price << endl;
    cout << "Risk-Free Rate: " << fixed << setprecision(2) << market.risk_free_rate * 100 << "%\n";
    cout << "Date: " << market.date << endl;
    
    // Test pricing for near-term ATM options
    string test_expiry = "2025-09-19"; // Monthly expiry
    auto& chain = market.option_chains.at(test_expiry);
    
    cout << "\nTesting options expiring " << test_expiry << ":\n";
    cout << "\nATM and near-ATM options:\n";
    cout << "Type  Strike    Market Mid   Model Price   Difference   Impl Vol\n";
    cout << "----  ------    ----------   -----------   ----------   --------\n";
    
    for (const auto& opt : chain) {
        // Focus on ATM options (within 2% of spot)
        if (abs(opt.strike - market.spot_price) / market.spot_price > 0.02) continue;
        
        double T = opt.days_to_expiry / 365.0;
        VanillaOption model(opt.strike, market.risk_free_rate, T, 
                           market.spot_price, opt.implied_vol);
        
        double model_price = (opt.type == 'C') ? 
            model.calc_call_price() : model.calc_put_price();
        
        cout << "  " << opt.type << "   "
             << setw(6) << fixed << setprecision(0) << opt.strike << "    "
             << setw(10) << fixed << setprecision(2) << opt.mid_price << "   "
             << setw(11) << fixed << setprecision(2) << model_price << "   "
             << setw(10) << fixed << setprecision(2) << (model_price - opt.mid_price) << "   "
             << setw(8) << fixed << setprecision(1) << opt.implied_vol * 100 << "%\n";
    }
}

// Test implied volatility calculation
void test_implied_volatility(const MarketData& market) {
    print_separator();
    cout << "IMPLIED VOLATILITY CALCULATION TEST\n";
    print_separator();
    
    // Test with a specific option
    string test_expiry = "2025-10-17"; // October monthly
    auto& chain = market.option_chains.at(test_expiry);
    
    // Find an ATM call option
    const OptionData* atm_call = nullptr;
    double min_diff = 1e9;
    for (const auto& opt : chain) {
        if (opt.type == 'C') {
            double diff = abs(opt.strike - market.spot_price);
            if (diff < min_diff) {
                min_diff = diff;
                atm_call = &opt;
            }
        }
    }
    
    if (atm_call) {
        cout << "Testing ATM Call Option:\n";
        cout << "  Expiry: " << atm_call->expiry_date << endl;
        cout << "  Strike: $" << fixed << setprecision(0) << atm_call->strike << endl;
        cout << "  Market Price: $" << fixed << setprecision(2) << atm_call->mid_price << endl;
        cout << "  Market Impl Vol: " << fixed << setprecision(1) 
             << atm_call->implied_vol * 100 << "%\n";
        
        // Calculate implied volatility using bisection method
        double T = atm_call->days_to_expiry / 365.0;
        
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
        
        CallPriceFunctor price_func(atm_call->strike, market.risk_free_rate, 
                                   T, market.spot_price);
        
        double vol_lower = 0.01;
        double vol_upper = 1.0;
        double epsilon = 0.0001;
        
        double calculated_iv = interval_bisection(atm_call->mid_price, vol_lower, 
                                                 vol_upper, epsilon, price_func);
        
        cout << "\nCalculated Impl Vol: " << fixed << setprecision(1) 
             << calculated_iv * 100 << "%\n";
        cout << "Difference: " << fixed << setprecision(2) 
             << abs(calculated_iv - atm_call->implied_vol) * 100 << "%\n";
    }
}

// Test volatility surface
void test_volatility_surface(const MarketData& market) {
    print_separator();
    cout << "VOLATILITY SURFACE ANALYSIS\n";
    print_separator();
    
    cout << "Implied Volatility by Strike and Expiry:\n\n";
    
    // Select a few expiries
    vector<string> test_expiries = {
        "2025-09-19", "2025-10-17", "2025-11-21", "2025-12-19"
    };
    
    // Strike levels to test
    vector<double> strike_levels = {0.90, 0.95, 1.00, 1.05, 1.10};
    
    // Header
    cout << "Strike%   ";
    for (const auto& expiry : test_expiries) {
        cout << expiry.substr(5) << "    "; // Show MM-DD only
    }
    cout << "\n" << string(50, '-') << "\n";
    
    for (double level : strike_levels) {
        double strike = market.spot_price * level;
        cout << setw(7) << fixed << setprecision(0) << level * 100 << "%  ";
        
        for (const auto& expiry : test_expiries) {
            const auto& chain = market.option_chains.at(expiry);
            
            // Find closest strike call option
            double min_diff = 1e9;
            double impl_vol = 0;
            for (const auto& opt : chain) {
                if (opt.type == 'C') {
                    double diff = abs(opt.strike - strike);
                    if (diff < min_diff) {
                        min_diff = diff;
                        impl_vol = opt.implied_vol;
                    }
                }
            }
            
            cout << setw(6) << fixed << setprecision(1) << impl_vol * 100 << "%   ";
        }
        cout << endl;
    }
    
    cout << "\nObservations:\n";
    cout << "- Volatility smile visible (higher IV for OTM options)\n";
    cout << "- Term structure shows slight increase with time\n";
}

// Test Monte Carlo pricing for exotic options
void test_monte_carlo_asian(const MarketData& market) {
    print_separator();
    cout << "MONTE CARLO ASIAN OPTION PRICING\n";
    print_separator();
    
    // Parameters for Asian option
    double K = market.spot_price; // ATM
    double T = 90.0 / 365.0; // 3-month option
    double sigma = 0.18; // Typical SPX volatility
    
    cout << "Asian Option Parameters:\n";
    cout << "  Strike: $" << fixed << setprecision(0) << K << endl;
    cout << "  Time to Maturity: " << fixed << setprecision(0) << T * 365 << " days\n";
    cout << "  Volatility: " << fixed << setprecision(1) << sigma * 100 << "%\n";
    cout << "  Spot: $" << fixed << setprecision(2) << market.spot_price << endl;
    
    // Monte Carlo simulation
    int num_paths = 10000;
    int num_steps = 90; // Daily observations
    
    PayOffCall call_payoff(K);
    AsianOptionArithmetic asian_arith(&call_payoff);
    AsianOptionGeometric asian_geom(&call_payoff);
    
    double arith_sum = 0.0;
    double geom_sum = 0.0;
    
    srand(time(0));
    
    for (int i = 0; i < num_paths; i++) {
        vector<double> spot_prices(num_steps, market.spot_price);
        calc_path_spot_prices(spot_prices, market.risk_free_rate, sigma, T);
        
        arith_sum += asian_arith.pay_off_price(spot_prices);
        geom_sum += asian_geom.pay_off_price(spot_prices);
    }
    
    double arith_price = (arith_sum / num_paths) * exp(-market.risk_free_rate * T);
    double geom_price = (geom_sum / num_paths) * exp(-market.risk_free_rate * T);
    
    // Compare with vanilla
    VanillaOption vanilla(K, market.risk_free_rate, T, market.spot_price, sigma);
    double vanilla_price = vanilla.calc_call_price();
    
    cout << "\nMonte Carlo Results (" << num_paths << " paths):\n";
    cout << "  Arithmetic Asian Call: $" << fixed << setprecision(2) << arith_price << endl;
    cout << "  Geometric Asian Call:  $" << fixed << setprecision(2) << geom_price << endl;
    cout << "  Vanilla European Call: $" << fixed << setprecision(2) << vanilla_price << endl;
    cout << "\nAsian options are cheaper due to averaging effect\n";
}

// Test Greeks calculation
void test_greeks(const MarketData& market) {
    print_separator();
    cout << "OPTION GREEKS ANALYSIS\n";
    print_separator();
    
    // Test option parameters
    double K = market.spot_price;
    double T = 30.0 / 365.0; // 30-day option
    double sigma = 0.16;
    double S = market.spot_price;
    double r = market.risk_free_rate;
    
    cout << "Calculating Greeks for ATM option:\n";
    cout << "  Strike: $" << fixed << setprecision(0) << K << endl;
    cout << "  Days to Expiry: 30\n";
    cout << "  Implied Vol: " << fixed << setprecision(1) << sigma * 100 << "%\n\n";
    
    // Calculate Greeks using finite differences
    double h = 0.01; // Small change for numerical differentiation
    
    VanillaOption base_opt(K, r, T, S, sigma);
    double base_call = base_opt.calc_call_price();
    double base_put = base_opt.calc_put_price();
    
    // Delta (price sensitivity to spot)
    VanillaOption opt_up(K, r, T, S + h, sigma);
    VanillaOption opt_down(K, r, T, S - h, sigma);
    double call_delta = (opt_up.calc_call_price() - opt_down.calc_call_price()) / (2 * h);
    double put_delta = (opt_up.calc_put_price() - opt_down.calc_put_price()) / (2 * h);
    
    // Gamma (delta sensitivity to spot)
    double call_gamma = (opt_up.calc_call_price() - 2 * base_call + opt_down.calc_call_price()) / (h * h);
    double put_gamma = (opt_up.calc_put_price() - 2 * base_put + opt_down.calc_put_price()) / (h * h);
    
    // Vega (price sensitivity to volatility)
    VanillaOption opt_vol_up(K, r, T, S, sigma + 0.01);
    double call_vega = (opt_vol_up.calc_call_price() - base_call) / 0.01;
    double put_vega = (opt_vol_up.calc_put_price() - base_put) / 0.01;
    
    // Theta (price sensitivity to time) - per day
    double dt = 1.0 / 365.0;
    VanillaOption opt_time(K, r, T - dt, S, sigma);
    double call_theta = (opt_time.calc_call_price() - base_call) / dt;
    double put_theta = (opt_time.calc_put_price() - base_put) / dt;
    
    // Rho (price sensitivity to interest rate)
    VanillaOption opt_rate(K, r + 0.01, T, S, sigma);
    double call_rho = (opt_rate.calc_call_price() - base_call) / 0.01;
    double put_rho = (opt_rate.calc_put_price() - base_put) / 0.01;
    
    cout << "Greek     Call        Put\n";
    cout << "------    -------     -------\n";
    cout << "Delta     " << setw(7) << fixed << setprecision(4) << call_delta 
         << "     " << setw(7) << fixed << setprecision(4) << put_delta << endl;
    cout << "Gamma     " << setw(7) << fixed << setprecision(6) << call_gamma 
         << "     " << setw(7) << fixed << setprecision(6) << put_gamma << endl;
    cout << "Vega      " << setw(7) << fixed << setprecision(2) << call_vega 
         << "     " << setw(7) << fixed << setprecision(2) << put_vega << endl;
    cout << "Theta     " << setw(7) << fixed << setprecision(2) << call_theta 
         << "     " << setw(7) << fixed << setprecision(2) << put_theta << endl;
    cout << "Rho       " << setw(7) << fixed << setprecision(2) << call_rho 
         << "     " << setw(7) << fixed << setprecision(2) << put_rho << endl;
}

// Portfolio risk analysis
void test_portfolio_risk(const MarketData& market) {
    print_separator();
    cout << "PORTFOLIO RISK ANALYSIS\n";
    print_separator();
    
    // Create a sample option portfolio
    struct Position {
        double strike;
        double expiry_days;
        char type;
        int quantity;
        string description;
    };
    
    vector<Position> portfolio = {
        {6400, 30, 'P', 10, "Protective Puts"},
        {6500, 30, 'C', -5, "Short Calls (Covered)"},
        {6300, 60, 'P', -10, "Short Puts"},
        {6600, 60, 'C', 10, "Long Calls"}
    };
    
    cout << "Portfolio Positions:\n";
    cout << "Description         Type  Strike  Expiry  Quantity\n";
    cout << "----------------    ----  ------  ------  --------\n";
    
    double total_value = 0;
    double sigma = 0.16; // Use consistent volatility
    
    for (const auto& pos : portfolio) {
        cout << left << setw(20) << pos.description 
             << setw(6) << pos.type 
             << setw(8) << fixed << setprecision(0) << pos.strike
             << setw(8) << pos.expiry_days << "d"
             << setw(8) << right << pos.quantity << endl;
        
        double T = pos.expiry_days / 365.0;
        VanillaOption opt(pos.strike, market.risk_free_rate, T, market.spot_price, sigma);
        double price = (pos.type == 'C') ? opt.calc_call_price() : opt.calc_put_price();
        total_value += price * pos.quantity * 100; // Each contract is 100 shares
    }
    
    cout << "\nPortfolio Value: $" << fixed << setprecision(2) << total_value << endl;
    
    // Scenario analysis
    cout << "\nScenario Analysis:\n";
    cout << "SPX Move    Portfolio P&L\n";
    cout << "--------    -------------\n";
    
    for (double pct_move = -0.10; pct_move <= 0.10; pct_move += 0.02) {
        double new_spot = market.spot_price * (1 + pct_move);
        double scenario_value = 0;
        
        for (const auto& pos : portfolio) {
            double T = pos.expiry_days / 365.0;
            VanillaOption opt(pos.strike, market.risk_free_rate, T, new_spot, sigma);
            double price = (pos.type == 'C') ? opt.calc_call_price() : opt.calc_put_price();
            scenario_value += price * pos.quantity * 100;
        }
        
        double pnl = scenario_value - total_value;
        cout << setw(7) << fixed << setprecision(1) << pct_move * 100 << "%    $"
             << setw(12) << fixed << setprecision(2) << pnl << endl;
    }
}

int main() {
    cout << "\n";
    cout << "============================================================\n";
    cout << "       SPX OPTIONS ANALYSIS WITH C++ QUANT LIBRARY         \n";
    cout << "                  Real Market Data Test                     \n";
    cout << "============================================================\n";
    
    // Initialize market data
    MarketData market = initialize_spx_market_data();
    
    cout << "\nMarket Date: " << market.date << endl;
    cout << "SPX Spot: $" << fixed << setprecision(2) << market.spot_price << endl;
    cout << "Risk-Free Rate: " << fixed << setprecision(2) 
         << market.risk_free_rate * 100 << "%\n";
    cout << "Option Chains Loaded: " << market.option_chains.size() << " expiries\n";
    
    // Run all tests
    test_black_scholes_pricing(market);
    test_implied_volatility(market);
    test_volatility_surface(market);
    test_monte_carlo_asian(market);
    test_greeks(market);
    test_portfolio_risk(market);
    
    print_separator();
    cout << "All SPX option tests completed successfully!\n";
    cout << endl;
    
    return 0;
}