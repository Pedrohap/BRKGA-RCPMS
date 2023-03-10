# Targets:
all:
		g++ -O3 -fopenmp -march=native -std=c++11 -o "Population" Population.h
		g++ -O3 -fopenmp -march=native -std=c++11 -o "BRKGA" BRKGA.h
		g++ -O3 -fopenmp -march=native -std=c++11 -o "MTRand" MTRand.h
		g++ -O3 -fopenmp -march=native -std=c++11 -o "SampleDecoder" SampleDecoder.h
		g++ -O3 -fopenmp -march=native -std=c++11 -o "RCPMS" RCPMS.h
		g++ -O3 -fopenmp -march=native -std=c++11 -o "samplecode" samplecode.cpp
