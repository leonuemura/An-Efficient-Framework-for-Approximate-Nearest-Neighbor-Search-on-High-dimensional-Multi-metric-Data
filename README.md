# An-Efficient-Framework-for-Approximate-Nearest-Neighbor-Search-on-High-dimensional-Multi-metric-Data
## Introduction
This framework provides an efficient solution for approximate nearest neighbor search (NNS) on high-dimensional multi-metric data. For each metric space, we construct a proximity graph that supports any search-start node. An approximate nearest neighbor is found by traversing these proximity graphs while carefully selecting start nodes to minimize redundant node accesses. This implementation is based on the algorithm presented in our paper.

## How to use

### 1. Dataset Format
The dataset used in this project should be formatted as follows:
- **First line**: The total number of data points in the dataset.
- **Second line**: The dimensionality of each metric. This line contains integers that represent the number of dimensions for each metric space.
- **Third line**: The distance function identifiers for each metric (e.g., L1 is represented by 1, and L2 by 2). These identifiers can be customized by modifying `distance.cpp`.
- **Fourth line and onwards**: The actual data points. Each line represents one data point, and the elements on each line are separated by spaces. Each data point is a vector of real numbers, corresponding to the dimensions specified in the second line.

### 2. Queryset Format
- The actual data points. Each line represents one data point, and the elements on each line are separated by spaces.

### 3. Weightset Format
- The weights used during the search. Each row corresponds to a row in the Queryset. Each row consists of weights for each metric, and the elements on each line are separated by spaces.

### 4. Index Construction
To construct an index from the dataset, use the following command:
```sh
g++ -o make_index make_index.cpp io.cpp distance.cpp others.cpp -O3 -mavx512f
./make_index
```

### 5. Search Operations
To search on an index, use the following command:
```sh
g++ -o search_index search_index.cpp io.cpp distance.cpp others.cpp -O3 -mavx512f
./search_index E
```
- **E** controls the search range. E is represented as a real number greater than or equal to 1, and the larger the value of E, the wider the search range becomes.

### 6. Change parameters
If you want to modify the dataset or parameters, you can do so in config.h. The config.h file is structured as follows:
- **data_name** refers to the filename of the Dataset.
- **query_name** refers to the filename of the Queryset.
- **weight_name** refers to the filename of the Weightset.
- **graph_k** refers to the number of edges for each node in the graph.
- **search_k** refers to the number of results during the search.
- **kmeans_chooselimit** refers to the number of iterations of k-means.
- **center_limit** refers to the number of centers for each metric.
- **sampling** refers to the sample size for calculating the mean and standard deviation.
