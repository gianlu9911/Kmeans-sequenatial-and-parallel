#include <cmath>
#include <chrono>
#include <omp.h>
#include "PointGeneretor.h"
#include"Cluster.h"
#include<iostream>
#include <random>
#include"Seq_AoS_kmeans.h"
using namespace std;


int main() {

    // init point
    int numPoints = 100;          // Example: number of points to generate
    int num_clusters = 5;
    int max_iteration = 10000;
    std::string filename = "data.csv";  // Example: output file name
    //generateRandomPoints(numPoints, dimensionality, filename);
    std::string run_condition = "sequenti";


    // Read points from CSV and store in a vector
    Seq_AoS_kemans kmeans = Seq_AoS_kemans(numPoints, num_clusters, max_iteration, filename);
    auto start = std::chrono::high_resolution_clock::now();
    kmeans.execute();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Time taken by function: " << duration.count() << " microseconds" << std::endl;

    return 0;
}
   
