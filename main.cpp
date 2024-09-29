#include <cmath>
#include <chrono>
#include <omp.h>
#include "PointGeneretor.h"
#include"Cluster.h"
#include<iostream>
#include <random>
using namespace std;


int main() {

    // init point
    int numPoints = 10;          // Example: number of points to generate
    int dimensionality = 2;      // Example: dimensionality of each point
    int num_clusters = 5;
    std::string filename = "points.csv";  // Example: output file name
    //generateRandomPoints(numPoints, dimensionality, filename);


    // Read points from CSV and store in a vector
    std::vector<Point> points = readCSVToPoints(filename);
    std::vector<Cluster> clusters;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < num_clusters;  i++) {
        Cluster c = Cluster();
        c.init_cluster(i, 0, 100, i);
        clusters.push_back(c);

        //std::cout << clusters[i].get_centroid().get_x() << " , " << clusters[i].get_centroid().get_y() << std::endl;
    }
   
    for (int p = 0; p < numPoints; p++) {
        int assing = -1;
        double min_dist = 100000000;
        
        for (int c = 0; c < num_clusters; c++) {
            double dist = euclidean_dist(points[p], clusters[c].get_centroid());
            //std::cout << "(" <<points[p].get_x() <<", "<<points[p].get_y()<<") - ("<< clusters[c].get_centroid().get_x()<<", "<< clusters[c].get_centroid().get_y()<<") "<<dist<< std::endl;
            if (dist < min_dist){
                min_dist = dist;
                assing = c;
            }
        }
        points[p].set_cluster_id(assing);
        clusters[assing].add_point(points[p]);
    }

    



    


    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    plotPoints();

    std::cout << "Time taken by function: " << duration.count() << " microseconds" << std::endl;

    return 0;
}
   
