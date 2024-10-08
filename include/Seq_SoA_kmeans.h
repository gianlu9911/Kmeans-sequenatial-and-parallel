#pragma once
#include<vector>
#include <cmath>
#include <chrono>
#include <omp.h>
#include "PointGeneretor.h"
#include<iostream>
#include <random>
using namespace std;

struct {
	std::vector<double> x;
	std::vector<double> y;
	std::vector<int> cluster_id;

}pointss;

struct {
	std::vector<double> x;
	std::vector<double> y;
	std::vector<int> cluster_id;
    std::vector<int> sizes;
    std::vector<double> cum_sum_x;
    std::vector<double> cum_sum_y;

}clusterss;

double euclidead_distance(double x, double y, double cx, double cy) {
    return sqrt( pow( (x-cx), 2) + pow( (y-cy), 2));
}

// Read from csv and INITILIZE pointss vectors
void readCSVtoVector(const std::string& filename) {
    std::ifstream file(filename);  // Open file for reading

    // Check if the file is opened successfully
    if (!file) {
        std::cerr << "Error opening file!" << std::endl;
        return; 
    }

    std::string line;
    while (std::getline(file, line)) {  // Read file line by line
        std::stringstream ss(line);
        std::string value;
        std::vector<double> coordinates;

        // Split line by commas and convert to double
        while (std::getline(ss, value, ',')) {
            coordinates.push_back(std::stod(value));  // Convert to double and add to coordinates
        }

        pointss.x.push_back(coordinates[0]);  
        pointss.y.push_back(coordinates[1]);  
        pointss.cluster_id.push_back(1);

    }

    file.close();  // Close the file
}

// Initilize clusterss
void init_clusterss(int num_clusters, double min=0, double max=100) {
    std::random_device rd;
    std::mt19937 generator(rd());

    // Create a distribution in the range [min, max]
    std::uniform_real_distribution<double> distribution(min, max);

    // Generate the sequence of random double pairs
    for (int i = 0; i < num_clusters; ++i) {
        double first = distribution(generator);
        double second = distribution(generator);
        clusterss.x.push_back(first);
        clusterss.y.push_back(second);
        clusterss.cluster_id.push_back(i);
        clusterss.sizes.push_back(0);
        clusterss.cum_sum_x.push_back(0);
        clusterss.cum_sum_y.push_back(0);
    }
}

void assign_point(int num_points = 100, int num_clusters=5) {
    for (int i = 0; i < num_points; i++) {
        double distance = 0;
        double min_dist = 10000;
        int assign;

        for (int j = 0; j < num_clusters; j++) {
            distance = euclidead_distance(pointss.x[i], pointss.y[i], clusterss.x[j], clusterss.y[j]);
            if (distance < min_dist) {
                min_dist = distance;
                assign = j;
            }
        }
        pointss.cluster_id[i] = assign;
        clusterss.sizes[assign] += 1;
        clusterss.cum_sum_x[assign] += pointss.x[i];
        clusterss.cum_sum_y[assign] += pointss.y[i];

        }
    }

bool update_centroids(int num_clusters, double tollerance = 0.001) {
    bool condition = true;
    for (int i = 0; i < num_clusters; i++) {

        if (clusterss.sizes[i] != 0) {
            double ncx = clusterss.cum_sum_x[i] / clusterss.sizes[i];
            double ncy = clusterss.cum_sum_y[i] / clusterss.sizes[i];

            if (euclidead_distance(clusterss.x[i], clusterss.y[i], ncx, ncy) > tollerance) {
                condition = false;
            }

            clusterss.x[i] = ncx;
            clusterss.y[i] = ncy;

        }
        clusterss.cum_sum_x[i] = 0;
        clusterss.cum_sum_y[i] = 0;
        clusterss.sizes[i] = 0;
    }
    return condition;
}


void execution(int num_points, int num_clusters, int max_iteration = 10000, double tollerance=0.001) {
    int iteration = 0;
    bool condition;
    do {
        assign_point(num_points, num_clusters);
        condition = update_centroids(num_clusters, tollerance);

    } while (!condition && iteration < max_iteration);
}

void transform_vector_to_csv(std::string filename, int num_points) {
    // Open the CSV file inside 'data' folder
    std::ofstream myFile("../data/" + filename);
    
    // Check if the file was successfully opened
    if(!myFile.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return;
    }

    std::cout << "Writing to file: data/" << filename << std::endl;

    // Optionally, add a header for the CSV
    myFile << "x,y,cluster_id\n";

    // Write the data from the vectors to the file
    for(int i = 0; i < num_points; ++i) {
        myFile << pointss.x[i] << "," << pointss.y[i] << "," << pointss.cluster_id[i] << "\n";
    }

    // Close the file
    myFile.close();
}

void transform_centroids_to_csv(std::string filename, int num_centroids) {
    // Open the CSV file inside 'data' folder
    std::ofstream myFile("../data/" + filename);
    
    // Check if the file was successfully opened
    if(!myFile.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return;
    }

    std::cout << "Writing to file: data/" << filename << std::endl;

    // Optionally, add a header for the CSV
    myFile << "x,y,cluster_id\n";

    // Write the data from the vectors to the file
    for(int i = 0; i < num_centroids; ++i) {
        myFile << clusterss.x[i] << "," << clusterss.y[i] << "," << clusterss.cluster_id[i] << "\n";
    }

    // Close the file
    myFile.close();
}