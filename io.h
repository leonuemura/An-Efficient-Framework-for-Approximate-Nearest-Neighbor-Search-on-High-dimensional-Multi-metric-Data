#ifndef IO_H
#define IO_H

#include <vector>
#include <string>
#include "others.h"
#include <filesystem>

namespace fs = std::filesystem;

using namespace std;

void read_embeddings(const string& filename, int& max_line, vector<int>& lange, vector<int>& metrics, vector<vector<float>>& embeddings);
void read_queries(const string& filename, vector<vector<float>>& queries);
vector<string> read_files(const string& directory);
vector<vector<int>> read_graph(const string& directory);
vector<vector<vector<int>>> read_all_graphs(const string& directory);
vector<density> read_dent(vector<vector<float>>& norm, const string& filename, int graph_num);
vector<vector<density>> read_all_dents(const string& directory, vector<vector<float>>& norm);
vector<vector<float>> read_weight(const string& filename);
void write_result_graph(const vector<multimap<float, int>>& Neighbors, const string& filename);
void write_result_dent(const vector<density>& data, const vector<float>& ave, const string& filename);
void write_result_search(const string& filename, const vector<vector<int>>& data);

#endif // IO_H
