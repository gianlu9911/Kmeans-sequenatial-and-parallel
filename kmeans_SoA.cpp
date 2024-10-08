#include <cmath>
#include <chrono>
#include <omp.h>
#include "PointGeneretor.h"
#include"Cluster.h"
#include<iostream>
#include <random>
#include"Seq_SoA_kmeans.h"
using namespace std;


int main() {

    int numPoints = 100;          // Example: number of points to generate
    int num_clusters = 15;
    int max_iteration = 10000;
    std::string filename = "data.csv";  // Example: output file name
    //generateRandomPoints(numPoints, dimensionality, filename);

    readCSVtoVector(filename);

    auto start = std::chrono::high_resolution_clock::now();
    init_clusterss(num_clusters);
    execution(numPoints, num_clusters, max_iteration, 0.001);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Time taken by function: " << duration.count() << " microseconds" << std::endl;

    transform_vector_to_csv("output.csv", numPoints);
    transform_centroids_to_csv("centroids.csv", num_clusters);

    return 0;
}