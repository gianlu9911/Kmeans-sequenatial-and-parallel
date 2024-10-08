#pragma once
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>
#include <iostream>
#include"Cluster.h"

void generateRandomPoints(int numPoints, int dimensionality, const std::string& filename) {
    std::ofstream file(filename);  // Open file for writing

    // Check if the file is opened successfully
    if (!file) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    // Seed the random number generator
    //std::srand(std::time(0));

    // Write random points to the file
    for (int i = 0; i < numPoints; ++i) {
        for (int d = 0; d < dimensionality; ++d) {
            double value = (std::rand() % 200 - 100)/10;  // Random number between -100 and 100
            file << value;
            if (d < dimensionality - 1) {
                file << ",";  // Separate dimensions with commas
            }
        }
        file << "\n";  // New line after each point
    }

    file.close();  
    std::cout << "CSV file created: " << filename << std::endl;
}




std::vector<Point> readCSVToPoints(const std::string& filename) {
    std::vector<Point> points;
    std::ifstream file(filename); 

    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return points;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string x_str, y_str;

        if (std::getline(ss, x_str, ',') && std::getline(ss, y_str, ',')) {
            double x = std::stod(x_str);
            double y = std::stod(y_str);
            Point p = Point(x, y);
            points.emplace_back(p);  
        }
    }

    file.close();
    return points;
}