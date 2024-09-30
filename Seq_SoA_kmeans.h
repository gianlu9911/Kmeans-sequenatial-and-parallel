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

}clusterss;


void plot_struct(int num_points=5, int num_clusters=5) {
    
    for (size_t i = 0; i < num_clusters; ++i) {
        plt::scatter(std::vector<double>{clusterss.x[i]}, std::vector<double>{clusterss.y[i]}, 50, { {"color", COLORS[clusterss.cluster_id[i]]},  {"marker", "x"} });
    }

    for (size_t i = 0; i < num_points; ++i) {
        plt::scatter(std::vector<double>{pointss.x[i]}, std::vector<double>{pointss.y[i]}, 50, { {"color", COLORS[pointss.cluster_id[i]]} });
    }

    plt::title("Scatter Plot with Different Colors");
    plt::xlabel("X");
    plt::ylabel("Y");

    plt::show();
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
    }
}