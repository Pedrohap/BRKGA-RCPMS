/*
 * SampleDecoder.h
 *
 * Any decoder must have the format below, i.e., implement the method decode(std::vector< double >&)
 * returning a double corresponding to the fitness of that vector. If parallel decoding is to be
 * used in the BRKGA framework, then the decode() method _must_ be thread-safe; the best way to
 * guarantee this is by adding 'const' to the end of decode() so that the property will be checked
 * at compile time.
 *
 * The chromosome inside the BRKGA framework can be changed if desired. To do so, just use the
 * first signature of decode() which allows for modification. Please use double values in the
 * interval [0,1) when updating, thus obeying the BRKGA guidelines.
 *
 *  Created on: Jan 14, 2011
 *      Author: rtoso
 */

#ifndef SAMPLEDECODER_H
#define SAMPLEDECODER_H

#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

//Numero de Tarefas
extern int n;

//Numero de Maquinas
extern int m;

//Numero de Moldes
extern int t;

//Tempo de Troca de Moldes;
extern int s;


//Vector que contem os molde em ordem, ou seja, o molde[0] tem o molde utilizado pela tarefa 0
extern vector <int> moldes;

//Vector que contem as tarefas em ordem, ou seja, a tarefa[0] tem o tempo de processamento da tarefa 0
extern vector <int> tarefas;


int getMaquina(double gene){
	string string_gene = std::to_string(gene);
	char last_value = string_gene.back();
  	int maquina_alvo = int(last_value - '0');
	maquina_alvo= (maquina_alvo % m)+1;

	return maquina_alvo-1;
}

//Retorna um vector de int que contem o tft de uma solução
int funcao_avaliativa(vector <vector <int>> solucao){
    vector < vector < pair < int,int > > > maquinas (m);

    for (int i = 0 ; i < m; i++){
        for (int j = 0 ; j < solucao[i].size() ; j++){
            maquinas[i].push_back(make_pair (moldes[solucao[i][j]],tarefas[solucao[i][j]]));
        }
    }

    vector < pair <int,int> > controlador_moldes (t, make_pair(-1,0));
    vector <int> tempo_maquinas(m,0);
    vector <int> molde_na_maquina(m,-1);
    vector <int> tft (m,0);

    for (int i = 0 ; i < n; i++){

        for (int indice_maquina = 0 ; indice_maquina < m; indice_maquina++){
            if (maquinas[indice_maquina].size() <= i){
                continue;
            } else {
                int molde_tarefa = maquinas[indice_maquina][i].first;

                if(controlador_moldes[molde_tarefa].first != indice_maquina && controlador_moldes[molde_tarefa].first != -1){
                    if (controlador_moldes[molde_tarefa].second > tempo_maquinas[indice_maquina]){
                        int diferenca_tempo = controlador_moldes[molde_tarefa].second - tempo_maquinas[indice_maquina];
                        tempo_maquinas[indice_maquina] += diferenca_tempo + maquinas[indice_maquina][i].second + s;
                        controlador_moldes[molde_tarefa] = make_pair(indice_maquina, tempo_maquinas[indice_maquina]);
                        molde_na_maquina[indice_maquina]=molde_tarefa;
                        tft[indice_maquina] += tempo_maquinas[indice_maquina];
                    } else {
                        tempo_maquinas[indice_maquina] += maquinas[indice_maquina][i].second + s;
                        controlador_moldes[molde_tarefa] = make_pair(indice_maquina, tempo_maquinas[indice_maquina]);
                        molde_na_maquina[indice_maquina]=molde_tarefa;
                        tft[indice_maquina] += tempo_maquinas[indice_maquina];
                    }
                } else if (controlador_moldes[molde_tarefa].first == -1 || controlador_moldes[molde_tarefa].first == indice_maquina){
                    if(molde_na_maquina[indice_maquina] != molde_tarefa && molde_na_maquina[indice_maquina] != -1){        
                        tempo_maquinas[indice_maquina] += maquinas[indice_maquina][i].second+s;
                        controlador_moldes[molde_tarefa] = make_pair(indice_maquina, tempo_maquinas[indice_maquina]);
                        molde_na_maquina[indice_maquina]=molde_tarefa;
                        tft[indice_maquina] += tempo_maquinas[indice_maquina];
                    } else{
                        tempo_maquinas[indice_maquina] += maquinas[indice_maquina][i].second;
                        controlador_moldes[molde_tarefa] = make_pair(indice_maquina, tempo_maquinas[indice_maquina]);
                        molde_na_maquina[indice_maquina]=molde_tarefa;
                        tft[indice_maquina] += tempo_maquinas[indice_maquina];
                    }
                }
            }
        }
    }

	int tft_total = 0;

    for (int i = 0; i < m; i++){
        tft_total+=tft[i];
    }

    return tft_total;
}

class SampleDecoder {
public:
	SampleDecoder(){}
	~SampleDecoder(){}

	double decode(const std::vector< double >& chromosome) const {
	vector <vector <int>> solucao(m);
	std::vector< std::pair< double, unsigned > > ranking(chromosome.size());

	for(unsigned i = 0; i < chromosome.size(); ++i) {
		ranking[i] = std::pair< double, unsigned >(chromosome[i], i);
	}

	// Here we sort 'permutation', which will then produce a permutation of [n] in pair::second:
	std::sort(ranking.begin(), ranking.end());

	vector< std::pair< double, unsigned > > tarefas = ranking;

	for (int i = 0 ; i < chromosome.size();i++){
		solucao[getMaquina(tarefas[i].first)].push_back(tarefas[i].second);
	}

	return funcao_avaliativa(solucao);
}


private:
};

#endif
