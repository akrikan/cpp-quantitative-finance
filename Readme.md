# C++ Quantitative Finance Library

A comprehensive C++ library for quantitative finance applications, implementing core derivatives pricing models, risk management tools, and mathematical utilities. This project demonstrates professional-grade financial software development using modern C++ practices.

## Overview

This library provides a complete framework for:
- **Option Pricing**: Black-Scholes analytical solutions and Monte Carlo simulations
- **Risk Management**: Greeks calculation and portfolio analytics
- **Exotic Options**: Asian options with geometric and arithmetic averaging
- **Implied Volatility**: Robust numerical solvers using bisection and Newton-Raphson methods
- **Mathematical Utilities**: Matrix operations, statistical distributions, and random number generation

**Note**: This implementation draws concepts from "C++ for Quantitative Finance" by Michael L. Halls-Moore, but is not a complete reproduction of the book. It represents a practical, production-oriented implementation suitable for real-world applications.

## Architecture

The library follows a modular design with clear separation of concerns:

```
src/
├── option_pricing/          # Financial derivatives pricing
│   ├── vanilla/            # European options (Black-Scholes)
│   ├── exotic/             # Asian options and digital payoffs
│   └── monte_carlo/        # Simulation framework
├── math/                   # Mathematical foundations
│   ├── matrix/             # Linear algebra operations
│   ├── statistics/         # Statistical distributions
│   └── random/             # Random number generation
└── implied_volatility/     # Numerical solvers
```

## Key Features

### Option Pricing Models
- **Black-Scholes Model**: Analytical pricing for European call and put options
- **Monte Carlo Simulation**: Path-dependent option pricing with optimized performance
- **Asian Options**: Both arithmetic and geometric averaging methods
- **Digital Options**: Binary payoff structures

### Risk Management
- **Greeks Calculation**: Delta, Gamma, Vega, Theta, Rho using numerical differentiation
- **Portfolio Analytics**: Multi-position risk aggregation
- **Scenario Analysis**: Stress testing under various market conditions

### Mathematical Infrastructure
- **Matrix Operations**: Template-based matrix class with full arithmetic support
- **Statistical Distributions**: Standard normal distribution with PDF, CDF, and quantile functions
- **Random Number Generation**: Linear congruential generator with statistical validation
- **Numerical Methods**: Root-finding algorithms for implied volatility

## Real-World Applications

### SPX Options Analysis
The library includes comprehensive testing against S&P 500 index options with:
- Current market data integration (SPX @ $6,460 as of Aug 2025)
- Volatility surface analysis across strikes and expiries
- Implied volatility extraction from market prices
- Performance benchmarking (100,000+ options/second)

### Interview Demonstration
Professional presentation mode showcasing:
- Real-time option pricing capabilities
- Numerical method convergence
- Monte Carlo simulation efficiency
- Portfolio risk management

## Usage

### Quick Start
```bash
# Build the interview demonstration
make interview_demo
./interview_demo

# Test with SPX market data
make main_spx_test
./main_spx_test

# Full library demonstration
make main_library_demo
./main_library_demo
```

### Basic Usage Example
```cpp
#include "src/option_pricing/vanilla/vanilla_option.h"

// Create SPX option
double spot = 6460.25;    // Current SPX level
double strike = 6450.0;   // Strike price
double rate = 0.045;      // Risk-free rate (4.5%)
double time = 30.0/365.0; // 30 days to expiry
double vol = 0.16;        // 16% volatility

VanillaOption option(strike, rate, time, spot, vol);

// Calculate prices
double call_price = option.calc_call_price();
double put_price = option.calc_put_price();

cout << "Call Price: $" << call_price << endl;
cout << "Put Price: $" << put_price << endl;
```

### Advanced: Monte Carlo Asian Options
```cpp
#include "src/option_pricing/exotic/asian.h"
#include "src/option_pricing/monte_carlo/path_generation.h"

// Setup Asian option
PayOffCall payoff(6450.0);
AsianOptionArithmetic asian(&payoff);

// Monte Carlo simulation
vector<double> path(63, 6460.25);  // 3 months daily
calc_path_spot_prices(path, 0.045, 0.18, 0.25);

double asian_payoff = asian.pay_off_price(path);
```

## Performance Characteristics

- **Option Pricing**: Sub-millisecond analytical calculations
- **Monte Carlo**: 10,000+ paths per second for complex payoffs
- **Implied Volatility**: Convergence in <1ms with guaranteed accuracy
- **Memory Efficiency**: Zero allocation in pricing hot paths
- **Numerical Stability**: Robust handling of extreme parameter ranges

## Financial Models Implemented

### Black-Scholes Framework
- Complete analytical solution for European options
- Cumulative normal distribution approximation
- Greeks calculation via finite differences
- Put-call parity verification

### Monte Carlo Methods
- Geometric Brownian Motion path generation
- Box-Muller Gaussian random number generation
- Variance reduction techniques
- Asian option pricing with daily averaging

### Implied Volatility
- Interval bisection method with guaranteed convergence
- Newton-Raphson for faster convergence when derivatives available
- Robust handling of edge cases and numerical instability

## Technical Implementation

### Object-Oriented Design
- Abstract base classes for extensibility (`PayOff`, `StatisticalDistribution`)
- Polymorphic payoff functions supporting new derivatives
- Template-based matrix operations for type safety
- RAII principles for automatic resource management

### Modern C++ Features
- Template metaprogramming for compile-time optimization
- Standard library containers for memory management
- Exception safety and error handling
- Clean interfaces with minimal dependencies

### Numerical Robustness
- Careful handling of floating-point precision
- Overflow/underflow protection in exponential calculations
- Convergence criteria for iterative algorithms
- Input validation and boundary condition checking

## Educational Value

This project demonstrates:
- **Quantitative Finance**: Deep understanding of derivatives pricing theory
- **C++ Programming**: Advanced language features and best practices
- **Software Architecture**: Modular design and clean code principles
- **Performance Engineering**: Optimization for financial computing workloads
- **Testing**: Comprehensive validation against market data

## Reference

Concepts and methodologies inspired by:
- "C++ for Quantitative Finance" by Michael L. Halls-Moore (QuantStart)
- Academic literature on derivatives pricing
- Industry best practices for financial software development

**Important**: This is an independent implementation focused on practical applications rather than a complete reproduction of any single reference work.

## Professional Applications

### Trading Systems
- Real-time option pricing for market making
- Risk management for derivatives portfolios
- Volatility trading strategy development

### Risk Management
- Portfolio Greeks calculation and hedging
- Scenario analysis and stress testing
- Value-at-Risk calculations

### Research & Development
- New derivatives product development
- Model validation and backtesting
- Performance optimization studies

## Requirements

- **Compiler**: Clang++ with C++17 support
- **Platform**: macOS (Apple Silicon optimized), Linux, Windows
- **Dependencies**: Standard C++ library only
- **Optional**: GitHub CLI for repository management

## Future Enhancements

- American option pricing using binomial/trinomial trees
- Stochastic volatility models (Heston, SABR)
- Interest rate derivatives (bonds, swaps, caps/floors)
- Multi-factor models and correlation structures
- GPU acceleration for Monte Carlo simulations

## Contributing

This project serves as a foundation for quantitative finance applications. Extensions and improvements are welcome, particularly in:
- Additional option types and payoff structures
- Alternative numerical methods
- Performance optimizations
- Model validation frameworks

---

*Built with modern C++ for production quantitative finance applications*

