#ifndef OTHERS_H
#define OTHERS_H

#include <vector>
#include <map>

using namespace std;

struct Neighbor_v{      //構造体（要素番号と距離）
    int num;
    float dist;
    bool operator<(const Neighbor_v& other) const {
        return dist > other.dist;       //distの小ささで比較
    }
};

struct density { // nodeID, mdist
    int num;
    float dent;
};

struct start{
    int node;
    int graph;
};

int random(int high, int mynum);
int random_norm(int data_num);
vector<int> sample_norm(int data_num);
vector<int> sample(int k, int n, int mynum);
vector<multimap<float, int>> Reverse(const vector<multimap<float, int>>& Neighbors);
void erase_pair_from_multimap(multimap<float, int>& mmap, const float& key, const int& value);
void insert_reverse(vector<multimap<float, int>>& Updated_NN, const vector<multimap<float, int>>& Reversed_NN);
vector<int> kmeans_center(const vector<vector<float>>& dataset, int graph_num, vector<int> lange, const vector<int>& metrics);
bool containsValue(const vector<int> random, int value);
vector<density> density_add(vector<multimap<float, int>>& Neighbors, const vector<int>& centers);
vector<vector<float>> calc_ave_std(const vector<vector<float>>& embeddings, const vector<int>& lange, const vector<int>& metrics);
int update(multimap<float, int>& Neighbors, float dist, int num);
vector<int> select_start_node(const vector<vector<float>>& dataset, const vector<vector<float>>& norm, const  vector<vector<density>>& centers, const vector<float>& weights, const vector<int>& lange, const vector<float>& query, const vector<int>& metrics);
Neighbor_v select_best_node(const vector<vector<float>>& dataset, const multimap<float, int>& candidate, const vector<float>& query, int num, const vector<int>& metrics, const vector<vector<float>>& norm, vector<int>lange);
void addKItems(const multimap<float, int>& candidate, vector<int>& vec, int k);

#endif // OTHERS_H
