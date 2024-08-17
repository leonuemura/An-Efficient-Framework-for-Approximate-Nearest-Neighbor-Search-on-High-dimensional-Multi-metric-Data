#include "distance.h"
#include <cmath>
#include <algorithm>

using namespace std;

float l1_distance(const vector<float>& a, const vector<float>& b, int start, int end) {
    int simd_end = start - 1 + ((end - start) / 16) * 16;

    __m512 manhattan = _mm512_setzero_ps();

    for (int i = start; i < simd_end; i += 16) {
        __m512 diff = _mm512_sub_ps(_mm512_loadu_ps(&a[i]), _mm512_loadu_ps(&b[i]));
        manhattan = _mm512_add_ps(manhattan, _mm512_max_ps(diff, _mm512_sub_ps(_mm512_setzero_ps(), diff))); // 絶対値を取る
    }

    alignas(64) float result[16];
    _mm512_store_ps(result, manhattan);
    float simd_result = 0.0;
    for (int i = 0; i < 16; ++i) {
        simd_result += result[i];
    }

    float scalar_result = 0.0;
    for (int i = simd_end + 1; i < end; ++i) {
        scalar_result += abs(a[i] - b[i]);
    }

    return simd_result + scalar_result;
}

float l2_distance(const vector<float>& a, const vector<float>& b, int start, int end) {
    int simd_end = start - 1 + ((end - start) / 16) * 16;

    __m512 euclidean = _mm512_setzero_ps();

    for (int i = start; i < simd_end; i += 16) {
        __m512 diff = _mm512_sub_ps(_mm512_loadu_ps(&a[i]), _mm512_loadu_ps(&b[i]));
        euclidean = _mm512_fmadd_ps(diff, diff, euclidean);
    }

    alignas(64) float result[16];
    _mm512_store_ps(result, euclidean);
    float simd_result = 0.0;
    for (int i = 0; i < 16; ++i) {
        simd_result += result[i];
    }

    float scalar_result = 0.0;
    for (int i = simd_end + 1; i < end; ++i) {
        scalar_result += (a[i] - b[i]) * (a[i] - b[i]);
    }

    return sqrt(simd_result + scalar_result);
}

float dist_raw(const vector<float>& a, const vector<float>& b, int graph_num, const vector<int>& metrics, const vector<int>& lange) {
    float l = 0.0;
    switch (metrics[graph_num]) {
        case 1:
            if (graph_num == 0) {
                l = l1_distance(a, b, 0, lange[graph_num]);
            } else {
                l = l1_distance(a, b, lange[graph_num - 1], lange[graph_num]);
            }
            break;
        case 2:
            if (graph_num == 0) {
                l = l2_distance(a, b, 0, lange[graph_num]);
            } else {
                l = l2_distance(a, b, lange[graph_num - 1], lange[graph_num]);
            }
            break;
    }
    return l;
}

float dist_norm(const vector<float>& a, const vector<float>& b, int graph_num, const vector<int>& metrics, const vector<vector<float>>& norm, const vector<int>& lange) {
    float l = 0.0;
    switch (metrics[graph_num]) {
        case 1:
            if (graph_num == 0) {
                l = ((l1_distance(a, b, 0, lange[graph_num]) - norm[graph_num][0]) / norm[graph_num][1] + 3) / 6;
            } else {
                l = ((l1_distance(a, b, lange[graph_num - 1], lange[graph_num]) - norm[graph_num][0]) / norm[graph_num][1] + 3) / 6;
            }
            break;
        case 2:
            if (graph_num == 0) {
                l = ((l2_distance(a, b, 0, lange[graph_num]) - norm[graph_num][0]) / norm[graph_num][1] + 3) / 6;
            } else {
                l = ((l2_distance(a, b, lange[graph_num - 1], lange[graph_num]) - norm[graph_num][0]) / norm[graph_num][1] + 3) / 6;
            }
            break;
    }
    return l;
}

float dist_all(const vector<vector<float>>& dataset, const vector<vector<float>>& norm, const vector<float>& weights, vector<int>& lange, vector<float> query, int num, const vector<int>& metrics){
    float alldist = 0.0;
    for(int i=0; i < metrics.size(); i++){
        alldist += dist_norm(query, dataset[num], i, metrics, norm, lange) * weights[i];
    }

    return alldist;
}
