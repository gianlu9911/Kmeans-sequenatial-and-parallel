#include <cmath>
#include <chrono>
#include <omp.h>
#include "PointGeneretor.h"
#include"Cluster.h"
#include<iostream>
#include <random>
#include"Sequential.h"
#include"Seq_SoA_kmeans.h"
using namespace std;


int main() {

    // init point
    int numPoints = 100;          // Example: number of points to generate
    int num_clusters = 5;
    int max_iteration = 10000;
    std::string filename = "data.csv";  // Example: output file name
    //generateRandomPoints(numPoints, dimensionality, filename);
    std::string run_condition = "sequentia";


    if(run_condition=="sequential") {
        // Read points from CSV and store in a vector
        Seq_AoS_kemans kmeans = Seq_AoS_kemans(numPoints, num_clusters, max_iteration, filename);

        auto start = std::chrono::high_resolution_clock::now();

        kmeans.execute();

        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);


        plotPoints(kmeans.get_points(), kmeans.get_clusters());

        std::cout << "Time taken by function: " << duration.count() << " microseconds" << std::endl;
    }
    else {
        readCSVtoVector(filename);
        init_clusterss(num_clusters);
        std::cout << clusterss.x[1] << std::endl;
        std::cout << clusterss.x.size() << std::endl;
        plot_struct(numPoints,num_clusters);
    }

    return 0;
}
   
