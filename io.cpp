#include "io.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

namespace fs = std::filesystem;

using namespace std;


void read_embeddings(const string& filename, int& max_line, vector<int>& lange, vector<int>& metrics, vector<vector<float>>& embeddings) {
    ifstream file(filename);
    string line;

    if (getline(file, line)) {
        max_line = stoi(line);
    }

    if (getline(file, line)) {
        istringstream iss(line);
        int value;
        while (iss >> value) {
            lange.push_back(value);
        }
    }

    if (getline(file, line)) {
        istringstream iss(line);
        int value;
        while (iss >> value) {
            metrics.push_back(value);
        }
    }

    while (getline(file, line)) {
        istringstream iss(line);
        vector<float> embedding;
        float value;
        while (iss >> value) {
            embedding.push_back(value);
        }
        embeddings.push_back(embedding);
    }
}

void read_queries(const string& filename, vector<vector<float>>& queries){
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        istringstream iss(line);
        vector<float> query;
        float value;
        while (iss >> value) {
            query.push_back(value);
        }
        queries.push_back(query);
    }
}

void write_result_graph(const vector<multimap<float, int>>& Neighbors, const string& filename) {
    ofstream file(filename);
    for (const auto& row : Neighbors) {
        for (const auto& neighbor : row) {
            file << neighbor.second << " ";
        }
        file << endl;
    }
}

void write_result_dent(const vector<density>& data, const vector<float>& ave, const string& filename) {
    ofstream file(filename);
    file << ave[0] << endl;
    file << ave[1] << endl;
    for (const density& center : data) {
        file << center.num << " " << center.dent << endl;
    }
}

void write_result_search(const string& filename, const vector<vector<int>>& data) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "ファイルを開けませんでした。" << endl;
        return;
    }

    // データをCSV形式でファイルに書き込む
    for (const vector<int>& row : data) {
        for (size_t i = 0; i < row.size(); ++i) {
            file << row[i];
            if (i < row.size() - 1) {
                file << " "; // カンマでデータを区切る
            }
        }
        file << endl; // 改行して次の行に移動
    }

    file.close();
}


vector<string> read_files(const string& directory){
    vector<string> filenames;

    // ディレクトリ内のファイル名を取得してソート
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            filenames.push_back(entry.path().string());
        }
    }

    sort(filenames.begin(), filenames.end());  // ファイル名を辞書順にソート

    return filenames;
}


vector<vector<int>> read_graph(const string& filename) {
    vector<vector<int>> result;
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        vector<int> row;
        istringstream iss(line);
        string token;

        while (getline(iss, token, ' ')) {
            int num;
            num = stoi(token);

            // Neighbor_v構造体を作成し、ベクトルに追加
            row.push_back(num);
        }
        result.push_back(row);
    }

    file.close();
    return result;
}

vector<vector<vector<int>>> read_all_graphs(const string& directory) {
    vector<vector<vector<int>>> all_results;
    vector<string> filenames = read_files(directory);

    // ソートされた順番でファイルを読み込む
    for (const auto& filename : filenames) {
        vector<vector<int>> result = read_graph(filename);
        all_results.push_back(result);
    }

    return all_results;
}

vector<density> read_dent(vector<vector<float>>& norm, const string& filename, int graph_num) {
    vector<density> row;
    ifstream file(filename);
    string firstLine;
    string secondLine;
    getline(file, firstLine);
    getline(file, secondLine);
    norm[graph_num][0] = stod(firstLine);
    norm[graph_num][1] = stod(secondLine);

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string token;
        density key;
        getline(iss, token, ' ');
        key.num = stoi(token);
        getline(iss, token, ' ');
        key.dent = stof(token);
        row.push_back(key);
    }

    file.close();
    return row;
}

vector<vector<density>> read_all_dents(const string& directory, vector<vector<float>>& norm){
    vector<vector<density>> all_results;
    vector<string> filenames = read_files(directory);

    // ソートされた順番でファイルを読み込む
    for(int i=0; i < filenames.size(); i++){
        vector<density> result = read_dent(norm, filenames[i], i);
        all_results.push_back(result);
    }

    return all_results;
}


vector<vector<float>> read_weight(const string& filename) {
    vector<vector<float>> result;
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        vector<float> row;
        istringstream iss(line);
        string token;

        while (getline(iss, token, ' ')) {
            float num;
            num = stof(token);

            // Neighbor_v構造体を作成し、ベクトルに追加
            row.push_back(num);
        }
        result.push_back(row);
    }

    file.close();
    return result;
}
