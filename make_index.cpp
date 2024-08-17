#include <iostream>
#include <limits>
#include "io.h"
#include "distance.h"
#include "others.h"
#include "config.h"


using namespace std;

int main() {
    int data_num;
    vector<int> lange;
    vector<int> metrics;
    vector<vector<float>> embeddings;
    float inf = numeric_limits<float>::infinity();
    int count = 0;
    float l = 0;

    // dataset
    read_embeddings(data_name, data_num, lange, metrics, embeddings);

    for (int i = 1; i < lange.size(); i++) {
        lange[i] = lange[i - 1] + lange[i];
    }

    // initializing
    vector<vector<multimap<float, int>>> Neighbors(metrics.size(), vector<multimap<float, int>>(embeddings.size(), multimap<float, int>{}));
    vector<multimap<float, int>> Reversed_NN;
    vector<vector<int>> center(metrics.size());
    vector<vector<density>> center_density(metrics.size());

    // culculating ave and std
    vector<vector<float>> norm = calc_ave_std(embeddings, lange, metrics); //[0] is ave,[1] is std.

    //selecting graph_k initial neighbors for each point randomly
    for (int i = 0; i < embeddings.size(); i++) {
        vector<int> sample_result = sample(embeddings.size(), graph_k, i);
        for (int j = 0; j < metrics.size(); j++) {
            for (int k = 0; k < graph_k; k++) {
                Neighbors[j][i].emplace(inf, sample_result[k]);
            }
        }
    }

    for (int graph_num = 0; graph_num < metrics.size(); graph_num++) {
        while (1) {
            Reversed_NN = Reverse(Neighbors[graph_num]);
            vector<multimap<float, int>> Updated_NN(Neighbors[graph_num]);
            insert_reverse(Updated_NN, Reversed_NN);

            count = 0;

            for (int i = 0; i < embeddings.size(); i++) {
                for (auto& n_node : Updated_NN[i]) {
                    for (auto& nn_node : Updated_NN[n_node.second]) {
                        if (i != nn_node.second) {
                            l = dist_norm(embeddings[i], embeddings[nn_node.second], graph_num, metrics, norm, lange);
                            count += update(Neighbors[graph_num][i], l, nn_node.second);
                        }
                    }
                }
            }
            if (count == 0) break;
        }

        string graph_name = "index/graph/graph" + to_string(graph_num + 1) + ".csv";
        string dent_name = "index/dent/dent" + to_string(graph_num + 1) + ".csv";
        write_result_graph(Neighbors[graph_num], graph_name.c_str());
        center[graph_num] = kmeans_center(embeddings, graph_num, lange, metrics);
        center_density[graph_num] = density_add(Neighbors[graph_num], center[graph_num]);
        write_result_dent(center_density[graph_num], norm[graph_num], dent_name.c_str());
    }

    return 0;
}
