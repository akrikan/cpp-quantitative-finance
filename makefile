# Updated for Apple Silicon Macs
chap3: main1.cpp vanilla_option.o
	clang++ -o chap3 main1.cpp vanilla_option.o -arch arm64

vanilla_option.o: vanilla_option.cpp
	clang++ -c vanilla_option.cpp -arch arm64

chap4: main2.cpp payoff_double_digital.o payoff.o
	clang++ -o chap4 main2.cpp payoff_double_digital.o payoff.o -arch arm64

payoff_double_digital.o: payoff_double_digital.cpp
	clang++ -c payoff_double_digital.cpp -arch arm64

payoff.o: payoff.cpp
	clang++ -c payoff.cpp -arch arm64

chap5: main3.cpp simplematrix.o
	clang++ -o chap5 main3.cpp simplematrix.o -arch arm64

simplematrix.o: simplematrix.cpp
	clang++ -c simplematrix.cpp -arch arm64

clean:
	rm -f *.o chap3 chap4 chap5

.PHONY: clean