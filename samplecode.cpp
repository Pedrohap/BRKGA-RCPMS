#include <iostream>
#include <chrono>
#include "SampleDecoder.h"
#include "MTRand.h"
#include "BRKGA.h"
#include "RCPMS.h"


using namespace std::chrono;
using namespace std;

//Numero de Tarefas
int n;

//Numero de Maquinas
int m;

//Numero de Moldes
int t;

//Tempo de Troca de Moldes;
int s;

//Vector que contem os molde em ordem, ou seja, o molde[0] tem o molde utilizado pela tarefa 0
vector <int> moldes;

//Vector que contem as tarefas em ordem, ou seja, a tarefa[0] tem o tempo de processamento da tarefa 0
vector <int> tarefas;

void read_file(){
    cin >> n;
    cin >> m;
    cin >> t;
    cin >> s;

    for (int i = 0 ; i < n; i++){
        int temp;
        cin >> temp;
        moldes.push_back(temp);
    }

    for (int i = 0 ; i < n; i++){
        int temp;
        cin >> temp;
        tarefas.push_back(temp);
    }
}

int main(int argc, char* argv[]) {
	read_file();

	//const unsigned n = 3;		// size of chromosomes using the numbes os jobs available
	const unsigned p = 10*n;	// size of population 10 time numbers of machines
	const double pe = 0.20;		// fraction of population to be the elite-set
	const double pm = 0.10;		// fraction of population to be replaced by mutants
	const double rhoe = 0.70;	// probability that offspring inherit an allele from elite parent
	const unsigned K = 3;		// number of independent populations
	const unsigned MAXT = 4;	// number of threads for parallel decoding
	
	SampleDecoder decoder;			// initialize the decoder

	RCPMS rcpms;
	
	long unsigned rngSeed = std::chrono::system_clock::now().time_since_epoch().count();
	//const long unsigned rngSeed = 0;	// seed to the random number generator
	MTRand rng(rngSeed);				// initialize the random number generator

	high_resolution_clock::time_point t1 = high_resolution_clock::now();
    high_resolution_clock::time_point t2;
    duration<double> time_span;
	
	// initialize the BRKGA-based heuristic
	BRKGA< SampleDecoder, MTRand > algorithm(n, p, pe, pm, rhoe, decoder, rng, K, MAXT);
	
	unsigned generation = 0;		// current generation
	const unsigned X_INTVL = 100;	// exchange best individuals at every 100 generations
	const unsigned X_NUMBER = 2;	// exchange top 2 best
	const unsigned MAX_GENS = 10000;	// run for 1000 gens

	int best_generation;
	double best_fitness;
	do {
		algorithm.evolve();	// evolve the population for one generation

		if(generation == 1){
			best_fitness = algorithm.getBestFitness();
		}

		if(best_fitness > algorithm.getBestFitness() && generation > 1){
			best_generation = generation;
			best_fitness = algorithm.getBestFitness();

			//Generation debuger
			/*vector <double> chosen_one_debug = algorithm.getBestChromosome();

			vector <vector <int>> solucao_debug = rcpms.decodificador(chosen_one_debug);
			rcpms.print_detalhado(solucao_debug);
			cout << "Geração: " << generation << endl;*/
		}

		if((++generation) % X_INTVL == 0) {
			algorithm.exchangeElite(X_NUMBER);	// exchange top individuals
		}
	} while (generation < MAX_GENS);
	
	/*std::cout << "Best solution found has objective value = "
	 		<< algorithm.getBestFitness() << std::endl;*/

	t2 = high_resolution_clock::now();
  	time_span = duration_cast<duration<double>>(t2 - t1);

	vector <double> chosen_one = algorithm.getBestChromosome();

	vector <vector <int>> solucao = rcpms.decodificador(chosen_one);

	//Debug Printing
	//rcpms.print_detalhado(solucao,time_span.count(),best_generation,MAX_GENS);
	//End of debug Printin

	//Csv data export
	rcpms.csv_export(solucao,time_span.count(),best_generation,MAX_GENS);
	
	return 0;
}