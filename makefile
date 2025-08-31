# C++ Quantitative Finance Library Makefile
# Updated for Apple Silicon and organized structure

CXX = clang++
CXXFLAGS = -std=c++17 -arch arm64 -O3 -Wall -Wextra
INCLUDES = -I./src

# Source directories
VANILLA_DIR = src/option_pricing/vanilla
EXOTIC_DIR = src/option_pricing/exotic
MONTE_CARLO_DIR = src/option_pricing/monte_carlo
MATH_DIR = src/math
STATS_DIR = src/math/statistics
RANDOM_DIR = src/math/random
MATRIX_DIR = src/math/matrix
IV_DIR = src/implied_volatility

# Object files
OBJS = vanilla_option.o payoff.o payoff_double_digital.o asian.o statistics.o linear_congruential_generator.o

# Main targets
all: interview_demo main_spx_test main_library_demo

# Interview demonstration
interview_demo: interview_demo.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o interview_demo interview_demo.cpp $(OBJS)

# SPX market data test
main_spx_test: main_spx_test.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o main_spx_test main_spx_test.cpp $(OBJS)

# Full library demonstration
main_library_demo: main.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o main_library_demo main.cpp $(OBJS)

# Object file compilation
vanilla_option.o: $(VANILLA_DIR)/vanilla_option.cpp $(VANILLA_DIR)/vanilla_option.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $(VANILLA_DIR)/vanilla_option.cpp

payoff.o: $(VANILLA_DIR)/payoff.cpp $(VANILLA_DIR)/payoff.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $(VANILLA_DIR)/payoff.cpp

payoff_double_digital.o: $(EXOTIC_DIR)/payoff_double_digital.cpp $(EXOTIC_DIR)/payoff_double_digital.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $(EXOTIC_DIR)/payoff_double_digital.cpp

asian.o: $(EXOTIC_DIR)/asian.cpp $(EXOTIC_DIR)/asian.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $(EXOTIC_DIR)/asian.cpp

statistics.o: $(STATS_DIR)/statistics.cpp $(STATS_DIR)/statistics.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $(STATS_DIR)/statistics.cpp

linear_congruential_generator.o: $(RANDOM_DIR)/linear_congruential_generator.cpp $(RANDOM_DIR)/linear_congruential_generator.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $(RANDOM_DIR)/linear_congruential_generator.cpp

# Individual chapter examples (legacy compatibility)
chap3: main1.cpp vanilla_option.o
	$(CXX) $(CXXFLAGS) -o chap3 main1.cpp vanilla_option.o

chap4: main2.cpp payoff.o payoff_double_digital.o
	$(CXX) $(CXXFLAGS) -o chap4 main2.cpp payoff.o payoff_double_digital.o

chap5: main3.cpp
	$(CXX) $(CXXFLAGS) -o chap5 main3.cpp

# Clean targets
clean:
	rm -f *.o interview_demo main_spx_test main_library_demo chap3 chap4 chap5

clean_output:
	rm -rf output/*

# Testing
test: interview_demo
	./interview_demo

# Quick demonstration
demo: main_spx_test
	./main_spx_test

# Full library test
full_test: main_library_demo
	./main_library_demo

.PHONY: all clean clean_output test demo full_test