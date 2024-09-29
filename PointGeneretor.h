#pragma once
#include "matplotlibcpp.h"
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>
#include <iostream>
#include"Cluster.h"
namespace plt = matplotlibcpp;

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

    file.close();  // Close the file
    std::cout << "CSV file created: " << filename << std::endl;
}


std::vector<std::vector<double>> transform_points( std::vector<Point>& points) {
    std::vector<std::vector<double>> transformed_points;
    for ( auto& point : points) {
        transformed_points.push_back({ point.get_x(), point.get_y(), static_cast<double>(point.get_cluster_id()) });
    }
    return transformed_points;
}



void plotPoints() {
    // Define some example points
    std::vector<double> x = { 1.0, 2.0, 3.0, 4.0, 5.0 };
    std::vector<double> y = { 1.0, 4.0, 9.0, 16.0, 25.0 };

    // Define corresponding colors for each point
    std::vector<std::string> colors = { "C4", "g", "b", "m", "c" };

    // Plot each point with the specified color
    for (size_t i = 0; i < x.size(); ++i) {
        // Pass color as a named argument
        plt::scatter(std::vector<double>{x[i]}, std::vector<double>{y[i]}, 50, { {"color", colors[i]} });
    }

    // Set plot title and labels
    plt::title("Scatter Plot with Different Colors");
    plt::xlabel("X");
    plt::ylabel("Y");

    // Display the plot
    plt::show();
}

void plot_points2() {
    // Define some example points
    std::vector<double> x = { 1.0, 2.0, 3.0, 4.0, 5.0 };
    std::vector<double> y = { 1.0, 4.0, 9.0, 16.0, 25.0 };

    // Define corresponding colors for each point
    std::vector<std::string> colors = { "r", "g", "b", "m", "c" };

    // Plot each point with the specified color
    for (size_t i = 0; i < x.size(); ++i) {
        // Pass color as a named argument
        plt::scatter(std::vector<double>{x[i]}, std::vector<double>{y[i]}, 50, { {"color", colors[i]} });
    }

    // Set plot title and labels
    plt::title("Scatter Plot with Different Colors");
    plt::xlabel("X");
    plt::ylabel("Y");

    // Display the plot
    plt::show();
}


std::vector<std::vector<double>> readPointsFromCSV(const std::string& filename) {
    std::vector<std::vector<double>> points;  // Vector to store points
    std::ifstream file(filename);  // Open file for reading

    // Check if the file is opened successfully
    if (!file) {
        std::cerr << "Error opening file!" << std::endl;
        return points;  // Return empty vector on error
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

        points.push_back(coordinates);  // Add the point (vector of coordinates) to the vector of points
    }

    file.close();  // Close the file
    return points;  // Return the vector of points
}

std::vector<Point> readCSVToPoints(const std::string& filename) {
    std::vector<Point> points;
    std::ifstream file(filename);  // Open the CSV file

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
            points.emplace_back(p);  // Add the point to the vector
        }
    }

    file.close();
    return points;
}


std::vector<std::string> colors = {
    "black", "dimgray", "dimgrey", "gray", "grey", "darkgray", "darkgrey", "silver", "lightgray", "lightgrey",
    "gainsboro", "whitesmoke", "white", "snow", "rosybrown", "lightcoral", "indianred", "brown", "firebrick",
    "maroon", "darkred", "red", "mistyrose", "salmon", "tomato", "darksalmon", "coral", "orangered", "lightsalmon",
    "sienna", "seashell", "chocolate", "saddlebrown", "sandybrown", "peachpuff", "peru", "linen", "bisque",
    "darkorange", "burlywood", "antiquewhite", "tan", "navajowhite", "blanchedalmond", "papayawhip", "moccasin",
    "orange", "wheat", "oldlace", "floralwhite", "darkgoldenrod", "goldenrod", "cornsilk", "gold", "lemonchiffon",
    "khaki", "palegoldenrod", "darkkhaki", "ivory", "beige", "lightyellow", "lightgoldenrodyellow", "olive",
    "yellow", "olivedrab", "yellowgreen", "darkolivegreen", "greenyellow", "chartreuse", "lawngreen", "honeydew",
    "darkseagreen", "palegreen", "lightgreen", "forestgreen", "limegreen", "darkgreen", "green", "lime", "seagreen",
    "mediumseagreen", "springgreen", "mintcream", "mediumspringgreen", "mediumaquamarine", "aquamarine",
    "turquoise", "lightseagreen", "mediumturquoise", "azure", "lightcyan", "paleturquoise", "darkslategray",
    "darkslategrey", "slategrey", "slategray", "lightslategray", "lightslategrey", "cornflowerblue", "royalblue",
    "ghostwhite", "lavender", "midnightblue", "navy", "darkblue", "mediumblue", "blue", "slateblue", "darkslateblue",
    "mediumslateblue", "mediumpurple", "rebeccapurple", "blueviolet", "indigo", "darkorchid", "darkviolet",
    "mediumorchid", "thistle", "plum", "violet", "purple", "darkmagenta", "fuchsia", "magenta", "orchid",
    "mediumvioletred", "deeppink", "hotpink", "lavenderblush", "palevioletred", "crimson", "pink", "lightpink"
};

// Obtain a time-based seed for the random number generator
unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

// Shuffle the vector
//std::shuffle(colors.begin(), colors.end(), std::default_random_engine(seed));