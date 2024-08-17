#include <iostream>
#include <limits>
#include <chrono>
#include "io.h"
#include "distance.h"
#include "others.h"
#include "config.h"


using namespace std;


int main(int argc, char *argv[]){
    float epsilon;
    epsilon = atof(argv[1]);
    int data_num;
    vector<int> lange;
    vector<int> metrics;
    vector<vector<float>> embeddings;
    vector<vector<float>> queries;
    float inf = numeric_limits<float>::infinity();
    int count = 0;
    float l = 0;

    //input
    read_embeddings(data_name, data_num, lange, metrics, embeddings);
    vector<vector<float>> norm(metrics.size(), vector<float>(2, 0.0));
    read_queries(query_name, queries);
    vector<vector<vector<int>>> graphs = read_all_graphs("index/graph");
    vector<vector<density>> centers = read_all_dents("index/dent", norm);
    vector<vector<float>> weights = read_weight(weight_name);

    //result
    vector<vector<int>> query_result(queries.size(), vector<int>());
    vector<chrono::duration<float>> query_time(queries.size());


    //searching
    for(int query_num=0; query_num < queries.size(); query_num++){
        auto start_time = chrono::high_resolution_clock::now();
        vector<int> start_node = select_start_node(embeddings, norm, centers, weights[query_num], lange, queries[query_num], metrics);
        vector<int> visit_check(embeddings.size(), 0);
        visit_check[start_node[0]] = 1;
        multimap<float, int> Candidate;
        multimap<float, int> Candidate_copy;
        Neighbor_v start_struct;
        float tau = inf;

        for(int metric=1; metric < start_node.size(); metric++){
            if(weights[query_num][start_node[metric]]!=0.0){

                if(Candidate.empty()){
                    start_struct.num = start_node[0];
                    start_struct.dist = dist_all(embeddings, norm, weights[query_num], lange, queries[query_num], start_node[0], metrics);
                    Candidate.emplace(start_struct.dist, start_struct.num);
                } else {
                    start_struct = select_best_node(embeddings, Candidate, queries[query_num], start_node[metric], metrics, norm, lange);
                }
                Candidate_copy.emplace(start_struct.dist, start_struct.num);


                while(!Candidate_copy.empty()){
                    multimap<float, int>::iterator it = Candidate_copy.begin();
                    int candidate_num = it->second;
                    Candidate_copy.erase(it);
                    for(auto& n_node : graphs[start_node[metric]][candidate_num]){
                        if(visit_check[n_node]!=1){
                            visit_check[n_node]=1;
                            if(Candidate.size() == epsilon * search_k){
                                multimap<float, int>::reverse_iterator i = Candidate.rbegin();
                                tau = i -> first;
                            }
                            float dist = dist_all(embeddings, norm, weights[query_num], lange, queries[query_num], n_node, metrics);
                            if(dist < tau){
                                Candidate.emplace(dist, n_node);
                                if(Candidate.size() >  epsilon * search_k){
                                    multimap<float, int>::reverse_iterator ri = Candidate.rbegin();
                                    Candidate.erase((++ri).base());
                                }
                                Candidate_copy.emplace(dist, n_node);
                            }
                        }
                    }
                }
            }
        }

        auto end_time = chrono::high_resolution_clock::now();
        chrono::duration<float> duration = end_time - start_time;

        addKItems(Candidate, query_result[query_num], search_k);
        query_time[query_num] = duration;
    }
    write_result_search("result.csv", query_result);
    chrono::duration<float> total_time(0);
    for(auto& time : query_time){
        total_time += time;
    }
    float average_time_ms = static_cast<float>((total_time / query_time.size()).count() * 1000);
    cout << average_time_ms << endl;
    return 0;
}
