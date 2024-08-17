#include "others.h"
#include "distance.h"
#include "config.h"
#include <random>
#include <algorithm>
#include <numeric>
#include <limits>
#include <cmath>

using namespace std;

int random(int high, int mynum) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, high);
    int num_random = dist(gen);
    while (num_random == mynum) {
        num_random = dist(gen);
    }
    return num_random;
}

int random_norm(int data_num) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, data_num - 1);
    return dist(gen);
}

vector<int> sample_norm(int data_num) {
    vector<int> random_vec;
    while (random_vec.size() < sampling) {
        int smp = random_norm(data_num);
        if (find(random_vec.begin(), random_vec.end(), smp) == random_vec.end()) {
            random_vec.push_back(smp);
        }
    }
    return random_vec;
}

vector<int> sample(int k, int n, int mynum) {
    vector<int> random_vec;
    while (random_vec.size() < n) {
        int neighbor = random(k - 1, mynum);
        if (find(random_vec.begin(), random_vec.end(), neighbor) == random_vec.end() && neighbor != mynum) {
            random_vec.push_back(neighbor);
        }
    }
    return random_vec;
}

vector<multimap<float, int>> Reverse(const vector<multimap<float, int>>& Neighbors) {
    vector<multimap<float, int>> R(Neighbors.size());
    for (int i = 0; i < Neighbors.size(); i++) {
        for (const auto& pair : Neighbors[i]) {
            R[pair.second].emplace(pair.first, i);
        }
    }
    return R;
}

void erase_pair_from_multimap(multimap<float, int>& mmap, const float& key, const int& value) {
    auto range = mmap.equal_range(key);
    for (auto it = range.first; it != range.second;) {
        if (it->second == value) {
            it = mmap.erase(it);
        } else {
            ++it;
        }
    }
}

void insert_reverse(vector<multimap<float, int>>& Updated_NN, const vector<multimap<float, int>>& Reversed_NN) {
    for (int i = 0; i < Reversed_NN.size(); i++) {
        for (const auto& neighbor : Reversed_NN[i]) {
            bool pairExists = false;
            for (const auto& existingNeighbor : Updated_NN[i]) {
                if (existingNeighbor.second == neighbor.second) {
                    pairExists = true;
                    break;
                }
            }
            if (!pairExists) {
                Updated_NN[i].insert(neighbor);
            }
        }
    }
}

vector<int> kmeans_center(const vector<vector<float>>& dataset, int graph_num, vector<int> lange, const vector<int>& metrics) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> distribution(0.0, 1.0);
    vector<vector<int>> centroids(kmeans_chooselimit, vector<int>());
    vector<float> select_dist(kmeans_chooselimit);
    float inf = numeric_limits<float>::infinity();
    float dist = inf;

    for (int m = 0; m < kmeans_chooselimit; m++) {
        centroids[m].push_back(static_cast<int>(distribution(gen) * dataset.size()));
        while (centroids[m].size() < center_limit) {
            vector<float> distances(dataset.size(), numeric_limits<float>::max());
            for (int i = 0; i < dataset.size(); i++) {
                for (int j = 0; j < centroids[m].size(); j++) {
                    dist = dist_raw(dataset[i], dataset[centroids[m][j]], graph_num, metrics, lange);
                    distances[i] = min(distances[i], dist);
                }
            }
            discrete_distribution<int> discreteDistribution(distances.begin(), distances.end());
            int rp_point = discreteDistribution(gen);
            if (containsValue(centroids[m], rp_point) == false) {
                centroids[m].push_back(rp_point);
            }
        }
    }

    for (int m = 0; m < kmeans_chooselimit; m++) {
        for (int i = 0; i < centroids[m].size(); i++) {
            dist = inf;
            for (int j = 0; j < centroids[m].size(); j++) {
                if (i != j) {
                    dist = min(dist, dist_raw(dataset[centroids[m][i]], dataset[centroids[m][j]], graph_num, metrics, lange));
                }
            }
            select_dist[m] += dist;
        }
    }

    float max_value = 0.0;
    int max_index = 0;
    for (int i = 0; i < select_dist.size(); i++) {
        if (select_dist[i] > max_value) {
            max_value = select_dist[i];
            max_index = i;
        }
    }
    return centroids[max_index];
}

bool containsValue(const vector<int> random, int value) {
    for (int i = 0; i < random.size(); i++) {
        if (random[i] == value) {
            return true;
        }
    }
    return false;
}

vector<density> density_add(vector<multimap<float, int>>& Neighbors, const vector<int>& centers) {
    vector<density> centroids;
    density center_dent;
    for (int i = 0; i < centers.size(); i++) {
        multimap<float, int>::reverse_iterator j = Neighbors[centers[i]].rbegin();
        center_dent.num = j->second;
        center_dent.dent = j->first;
        centroids.push_back(center_dent);
    }

    return centroids;
}

vector<vector<float>> calc_ave_std(const vector<vector<float>>& embeddings, const vector<int>& lange, const vector<int>& metrics) {
    vector<int> sample_vec = sample_norm(embeddings.size());

    vector<vector<vector<float>>> dist_array(sampling, vector<vector<float>>(sampling, vector<float>(metrics.size(), 0.0f)));
    vector<vector<float>> result_array(metrics.size(), vector<float>());
    vector<float> average(metrics.size(), 0.0f);
    vector<float> standard(metrics.size(), 0.0f);

    int dist_num = sampling * (sampling - 1) / 2;

    for (int i = 0; i < sampling; i++) {
        for (int j = i + 1; j < sampling; j++) {
            for (int k = 0; k < metrics.size(); k++) {
                dist_array[i][j][k] = dist_raw(embeddings[sample_vec[i]], embeddings[sample_vec[j]], k, metrics, lange);
                average[k] += dist_array[i][j][k];
            }
        }
    }

    for (int i = 0; i < metrics.size(); i++) {
        result_array[i].push_back(average[i] / dist_num);
    }

    for (int i = 0; i < sampling; i++) {
        for (int j = i + 1; j < sampling; j++) {
            for (int k = 0; k < metrics.size(); k++) {
                standard[k] += (dist_array[i][j][k] - result_array[k][0]) * (dist_array[i][j][k] - result_array[k][0]);
            }
        }
    }
    for (int i = 0; i < metrics.size(); i++) {
        result_array[i].push_back(sqrt(standard[i]));
    }

    return result_array;
}

int update(multimap<float, int>& Neighbors, float dist, int num) {
    bool existed = false;
    multimap<float, int> Neighbor_copy = Neighbors;
    for(auto& node : Neighbors){
        if(node.second == num){
            existed = true;
            erase_pair_from_multimap(Neighbors, node.first, node.second);
            Neighbors.emplace(dist, num);
            break;
        }
    }
    if(!existed){
        Neighbors.emplace(dist, num);
        multimap<float, int>::reverse_iterator i = Neighbors.rbegin();
        if(i -> second != num){
            erase_pair_from_multimap(Neighbors, i -> first, i -> second);
            return 1;
        } else {
            erase_pair_from_multimap(Neighbors, i -> first, i -> second);
            return 0;
        }
    } else {
        return 0;
    }
}


vector<int> select_start_node(const vector<vector<float>>& dataset, const vector<vector<float>>& norm, const  vector<vector<density>>& centers, const vector<float>& weights, const vector<int>& lange, const vector<float>& query, const vector<int>& metrics){
    vector<int> start_node;
    multimap<float, start> result;
    float inf = numeric_limits<float>::infinity();
    for(int i=0; i < metrics.size(); i++){
        int min_num = 0;
        float max_dent = 0.0;
        float min_dist = inf;
        float l = 0.0;

        for(int j=0; j < centers[i].size(); j++){
                l =  dist_norm(query, dataset[centers[i][j].num], i, metrics, norm, lange);
                if(l < min_dist){
                    min_num = centers[i][j].num;
                    max_dent = centers[i][j].dent * weights[i];
                    min_dist = l;
                }
            }
        result.emplace(max_dent, start{min_num, i});
    }
    map<float, start>::iterator it = result.begin();
    start_node.push_back((it -> second).node);
    for(const auto& node : result){
        start_node.push_back((node.second).graph);
    }
    return start_node;
}


Neighbor_v select_best_node(const vector<vector<float>>& dataset, const multimap<float, int>& candidate, const vector<float>& query, int num, const vector<int>& metrics, const vector<vector<float>>& norm, vector<int>lange){
    Neighbor_v start_node;
    float inf = numeric_limits<float>::infinity();
    float best_dist = inf;
    float l = 0.0;

    for(auto& node : candidate){
        l = dist_norm(query, dataset[node.second], num, metrics, norm, lange);
        if(l < best_dist){
            best_dist = l;
            start_node.num = node.second;
            start_node.dist = node.first;
        }
    }

    return start_node;
}


void addKItems(const multimap<float, int>& candidate, vector<int>& vec, int k) {
    auto it = candidate.begin(); // multimapの先頭を指すイテレータ
    while(k > 0){
        vec.push_back(it -> second);
        ++it;
        --k;
    }
}
