#ifndef CONFIG_H
#define CONFIG_H

#include <string>


const std::string data_name = "dataset/data.csv";
const std::string query_name = "dataset/query.csv";
const std::string weight_name = "dataset/weight.csv";
const int graph_k = 20; //hp*graph_k is the number of outedges for each node
const int search_k = 10; //searching parameter
const int kmeans_chooselimit = 10; //the number of iterations of k-means
const int center_limit = 100; //the number of centers for each metric
const int sampling = 1000; //the sample size for calculating the mean and standard deviation

#endif // CONFIG_H
