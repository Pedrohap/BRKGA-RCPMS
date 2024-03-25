#ifndef RCPMS_H
#define RCPMS_H

#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <string>
#include <utility>

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

class RCPMS
{
private:

    int trocas = 0;
    int tempo_de_espera = 0;
    int makespan_machine = 0;

    vector<int> tempo_da_tarefa_executada;
    vector< pair <char,int> > troca_espera;
    vector <int> global_makespans;
    vector <int> global_tft;
    vector <int> global_qtd_trocas;
    vector <int> global_tempo_espera_total;

    //Geração do Vector que armazenara um historico de vetores que entraram na maquina
    vector < vector <int> > historico_maquina;
    int reentrada_cont = 0;
    //

public:
    
    RCPMS() : tempo_da_tarefa_executada(n,0), troca_espera(n,make_pair ('N',0)), global_makespans(m,0), global_tft(m,0), global_qtd_trocas(m,0), global_tempo_espera_total(m,0)
    {}
    ~RCPMS() 
    {}

    int makespan (vector <vector <int>> solucao){
        //Geração do Vector que armazenara um historico de vetores que entraram na maquina
        for (int i = 0; i < m; i++){
            historico_maquina.push_back({});
        }
        //

        trocas = 0;

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
                            tempo_da_tarefa_executada[solucao[indice_maquina][i]]+=tempo_maquinas[indice_maquina];
                            tempo_de_espera += diferenca_tempo;
                            troca_espera[solucao[indice_maquina][i]]= make_pair('W',diferenca_tempo);

                            global_qtd_trocas[indice_maquina]++;
                            global_tempo_espera_total[indice_maquina]+=diferenca_tempo;

                            trocas++;

                            historico_maquina[indice_maquina].push_back(molde_tarefa);
                        } else {
                            tempo_maquinas[indice_maquina] += maquinas[indice_maquina][i].second + s;
                            controlador_moldes[molde_tarefa] = make_pair(indice_maquina, tempo_maquinas[indice_maquina]);
                            molde_na_maquina[indice_maquina]=molde_tarefa;
                            tft[indice_maquina] += tempo_maquinas[indice_maquina];
                            tempo_da_tarefa_executada[solucao[indice_maquina][i]]+=tempo_maquinas[indice_maquina];
                            troca_espera[solucao[indice_maquina][i]]= make_pair('S',s);

                            global_qtd_trocas[indice_maquina]++;

                            trocas++;

                            historico_maquina[indice_maquina].push_back(molde_tarefa);
                        }
                    } else if (controlador_moldes[molde_tarefa].first == -1 || controlador_moldes[molde_tarefa].first == indice_maquina){
                        if(molde_na_maquina[indice_maquina] != molde_tarefa && molde_na_maquina[indice_maquina] != -1){        
                            tempo_maquinas[indice_maquina] += maquinas[indice_maquina][i].second + s;
                            controlador_moldes[molde_tarefa] = make_pair(indice_maquina, tempo_maquinas[indice_maquina]);
                            molde_na_maquina[indice_maquina]=molde_tarefa;
                            tft[indice_maquina] += tempo_maquinas[indice_maquina];
                            tempo_da_tarefa_executada[solucao[indice_maquina][i]]+=tempo_maquinas[indice_maquina];
                            troca_espera[solucao[indice_maquina][i]]= make_pair('S',s);

                            global_qtd_trocas[indice_maquina]++;

                            trocas++;

                            historico_maquina[indice_maquina].push_back(molde_tarefa);
                        } else{
                            tempo_maquinas[indice_maquina] += maquinas[indice_maquina][i].second;
                            controlador_moldes[molde_tarefa] = make_pair(indice_maquina, tempo_maquinas[indice_maquina]);
                            molde_na_maquina[indice_maquina] = molde_tarefa;
                            tft[indice_maquina] += tempo_maquinas[indice_maquina];
                            tempo_da_tarefa_executada[solucao[indice_maquina][i]]+=tempo_maquinas[indice_maquina];

                            if (historico_maquina[indice_maquina].size() == 0 || historico_maquina[indice_maquina][historico_maquina[indice_maquina].size()-1] != molde_tarefa){
                                historico_maquina[indice_maquina].push_back(molde_tarefa);
                            }
                        }
                    }
                }
            }
        }

        

        int makespan = 0;

        for (int i = 0; i < m; i++){
            global_makespans[i] = tempo_maquinas[i];
            global_tft[i] = tft[i];
            if(tempo_maquinas[i] > makespan){
                makespan = tempo_maquinas[i];
                makespan_machine = i;
            }
        }

            
        vector <int> punicao_check;
        if (historico_maquina[makespan_machine].size()>1){
            for (int i = 0; i < historico_maquina[makespan_machine].size() ; i++){
                punicao_check.push_back(historico_maquina[makespan_machine][i]);

                for (int j = 0 ; j < punicao_check.size()-1 ; j++){
                    if (punicao_check[j] == historico_maquina[makespan_machine][i]){
                        reentrada_cont++;
                    }
                }

            }
        }

        return makespan;
    }

    int tft (vector <vector <int>> solucao){
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
        cout << "TFT: " << tft(solucao) << endl;
        cout << "Makespan: " << makespan(solucao) << endl;
    }

    void print_detalhado(vector< vector <int> > solucao, double time, int best_generation,int total_generations)
    {
        int value_makespan = makespan(solucao);
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

            cout << "Indice da tarefa: (S: Significa uma troca, W: Significa que ocorreu uma espera)" << endl;
            for (int j = 0; j < solucao[i].size(); j++)
            {
                if (troca_espera[solucao[i][j]].first == 'N'){
                    cout << solucao[i][j] << " ";
                } else if (troca_espera[solucao[i][j]].first == 'S'){
                    cout << troca_espera[solucao[i][j]].first << " ";
                    cout << solucao[i][j] << " ";
                } else {
                    cout << troca_espera[solucao[i][j]].first << " " << "S ";
                    cout << solucao[i][j] << " ";
                }
            }
            cout << endl;

            cout << "Molde da tarefa: (S: Significa uma troca, W: Significa que ocorreu uma espera)" << endl;
            for (int j = 0; j < maquinas[i].size(); j++)
            {
                if (troca_espera[solucao[i][j]].first == 'N'){
                    cout << maquinas[i][j].first << " ";
                } else if (troca_espera[solucao[i][j]].first == 'S') {
                    cout << troca_espera[solucao[i][j]].first << " ";
                    cout << maquinas[i][j].first << " ";
                } else {
                    cout << troca_espera[solucao[i][j]].first << " " << "S ";
                    cout << maquinas[i][j].first << " ";
                }
            }
            cout << endl;

            cout << "Tempo para a execução da tarefa: (O tempo da troca e da espera é demonstrado visualmente)" << endl;
            for (int j = 0; j < maquinas[i].size(); j++)
            {
                if (troca_espera[solucao[i][j]].first == 'N'){
                    cout << maquinas[i][j].second << " ";
                } else if (troca_espera[solucao[i][j]].first == 'S') {
                    cout << troca_espera[solucao[i][j]].second << " ";
                    cout << maquinas[i][j].second << " ";
                } else {
                    cout << troca_espera[solucao[i][j]].second << " " << s << " ";
                    cout << maquinas[i][j].second << " ";
                }
            }
            cout << endl;

            cout << "Tempo em que a tarefa foi executada: (OBS: O tempo da troca é contabilizado, mas não é mostrado individualmente como nos resultados superiores)" << endl;
            for (int j = 0; j < maquinas[i].size(); j++)
            {
                if (troca_espera[solucao[i][j]].first == 'N'){
                    cout << tempo_da_tarefa_executada[solucao[i][j]] << " ";
                } else if (troca_espera[solucao[i][j]].first == 'S') {
                    cout << "S ";
                    cout << tempo_da_tarefa_executada[solucao[i][j]] << " ";
                } else {
                    cout << "W S ";
                    cout << tempo_da_tarefa_executada[solucao[i][j]] << " ";
                }
            }
            cout << endl;
            cout << "TFT da Máquina: " << global_tft[i] << endl;
            cout << "Makespan da Máquina: " << global_makespans[i] << endl;
            cout << "Quantidades de trocas da Máquina: " << global_qtd_trocas[i] << endl;
            cout << "Tempo total de espera da Máquina: " << global_tempo_espera_total[i] << endl;

            cout << "-----------------------------------------------------------------" << endl;

        }

        cout << "Historico de moldes da Maquina " << makespan_machine << ": ";
        for (int i = 0; i < historico_maquina[makespan_machine].size() ; i++){
            cout <<historico_maquina[makespan_machine][i] << " ";
        }
        cout << endl;

        cout << "Contador de reentradas: " << reentrada_cont << endl;
        cout << "TFT: " << tft(solucao) << endl;
        cout << "Numero da Maquina do Mekespan:" << makespan_machine << endl;
        cout << "Makespan: " << value_makespan << endl;
        cout << "Quantidade de trocas: " << trocas << endl;
        cout << "Tempo de espera total: " << tempo_de_espera << endl;
        cout << endl ;
        cout << "Tempo de execução: " << time << "(s)" << endl;
        cout << "Melhor Geração: " << best_generation << endl;
        cout << "Total de Gerações: " << total_generations;

        cout << "-----------------------------------------------------------------" << endl;

        cout << "Solução minimizada para debug:" << endl;
        
        print_solucao(solucao);

    }

    void csv_export(vector< vector <int> > solucao, double time, int best_generation,int total_generations)
    {
        cout << makespan(solucao) << "," << tft(solucao) << "," << best_generation << "," << total_generations << "," << time << endl;
    }
};

#endif
