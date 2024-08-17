#ifndef DISTANCE_H
#define DISTANCE_H

#include <vector>
#include <immintrin.h>
#include "config.h"

using namespace std;

float l1_distance(const vector<float>& a, const vector<float>& b, int start, int end);
float l2_distance(const vector<float>& a, const vector<float>& b, int start, int end);
float dist_raw(const vector<float>& a, const vector<float>& b, int graph_num, const vector<int>& metrics, const vector<int>& lange);
float dist_norm(const vector<float>& a, const vector<float>& b, int graph_num, const vector<int>& metrics, const vector<vector<float>>& norm, const vector<int>& lange);
float dist_all(const vector<vector<float>>& dataset, const vector<vector<float>>& norm, const vector<float>& weights, vector<int>& lange, vector<float> query, int num, const vector<int>& metrics);

#endif // DISTANCE_H
