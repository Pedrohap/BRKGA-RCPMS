#ifndef RCPMS_H
#define RCPMS_H

#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

// Numero de Tarefas
extern int n;

// Numero de Maquinas
extern int m;

// Numero de Moldes
extern int t;

// Tempo de Troca de Moldes;
extern int s;

// Vector que contem os molde em ordem, ou seja, o molde[0] tem o molde utilizado pela tarefa 0
extern vector<int> moldes;

// Vector que contem as tarefas em ordem, ou seja, a tarefa[0] tem o tempo de processamento da tarefa 0
extern vector<int> tarefas;

extern int getMaquina(double gene);

extern int funcao_avaliativa(vector<vector<int>> solucao);

class RCPMS
{
private:
public:
    RCPMS() {}
    ~RCPMS() {}

    int makespan (vector <vector <int>> solucao){
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

	int makespan = 0;

    for (int i = 0; i < m; i++){
        if(tempo_maquinas[i] > makespan){
            makespan = tempo_maquinas[i];
        }
    }

    return makespan;
    }

    vector<vector<int>> decodificador(const std::vector<double> &chromosome) const
    {
        vector<vector<int>> solucao(m);
        std::vector<std::pair<double, unsigned>> ranking(chromosome.size());

        for (unsigned i = 0; i < chromosome.size(); ++i)
        {
            ranking[i] = std::pair<double, unsigned>(chromosome[i], i);
        }

        // Here we sort 'permutation', which will then produce a permutation of [n] in pair::second:
        std::sort(ranking.begin(), ranking.end());

        vector<std::pair<double, unsigned>> tarefas = ranking;

        for (int i = 0; i < chromosome.size(); i++)
        {
            solucao[getMaquina(tarefas[i].first)].push_back(tarefas[i].second);
        }

        return solucao;
    }

    void print_solucao(vector<vector<int>> solucao)
    {
        for (int i = 0; i < solucao.size(); i++)
        {
            cout << "Maquina " << i << ":" << endl;
            for (int j = 0; j < solucao[i].size(); j++)
            {
                cout << solucao[i][j] << " ";
            }
            cout << endl;
        }
        cout << "TFT: " << funcao_avaliativa(solucao) << endl;
        cout << "Makespan: " << makespan(solucao) << endl;
    }

    void print_detalhado(vector<vector<int>> solucao)
    {

        vector<vector<pair<int, int>>> maquinas(m);

        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < solucao[i].size(); j++)
            {
                maquinas[i].push_back(make_pair(moldes[solucao[i][j]], tarefas[solucao[i][j]]));
            }
        }

        for (int i = 0; i < m; i++)
        {
            cout << "Maquina " << i << ":" << endl;
            for (int j = 0; j < maquinas[i].size(); j++)
            {
                cout << maquinas[i][j].first << " ";
            }
            cout << endl;
            for (int j = 0; j < maquinas[i].size(); j++)
            {
                cout << maquinas[i][j].second << " ";
            }
            cout << endl;
        }

        cout << "TFT: " << funcao_avaliativa(solucao) << endl;
        cout << "Makespan: " << makespan(solucao) << endl;
    }

    void csv_export(vector< vector <int> > solucao, double time, int best_generation,int total_generations)
    {
        cout << makespan(solucao) << "," << funcao_avaliativa(solucao) << "," << best_generation << "," << total_generations << "," << time << endl;
    }
};

#endif
