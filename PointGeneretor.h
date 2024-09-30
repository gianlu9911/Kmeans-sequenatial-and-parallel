#pragma once
#include "matplotlibcpp.h"
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>
#include <iostream>
#include"Cluster.h"
namespace plt = matplotlibcpp;


std::vector<std::string> COLORS = {
    "black", "crimson","mediumvioletred","greenyellow", "blue",  "magenta", "pink", "darkgray", "darkgrey", "silver", "lightgray", "lightgrey",
    "gainsboro", "whitesmoke", "white", "snow", "rosybrown", "lightcoral", "indianred", "brown", "firebrick",
    "maroon", "darkred", "red", "mistyrose", "salmon", "tomato", "darksalmon", "coral", "orangered", "lightsalmon",
    "sienna", "seashell", "chocolate", "saddlebrown", "sandybrown", "peachpuff", "peru", "linen", "bisque",
    "darkorange", "burlywood", "antiquewhite", "tan", "navajowhite", "blanchedalmond", "papayawhip", "moccasin",
    "orange", "wheat", "oldlace", "floralwhite", "darkgoldenrod", "goldenrod", "cornsilk", "gold", "lemonchiffon",
    "khaki", "palegoldenrod", "darkkhaki", "ivory", "beige", "lightyellow", "lightgoldenrodyellow", "olive",
    "yellow", "olivedrab", "yellowgreen", "darkolivegreen", "dimgrey", "chartreuse", "lawngreen", "honeydew",
    "darkseagreen", "palegreen", "lightgreen", "forestgreen", "limegreen", "darkgreen", "green", "lime", "seagreen",
    "mediumseagreen", "springgreen", "mintcream", "mediumspringgreen", "mediumaquamarine", "aquamarine",
    "turquoise", "lightseagreen", "mediumturquoise", "azure", "lightcyan", "paleturquoise", "darkslategray",
    "darkslategrey", "slategrey", "slategray", "lightslategray", "lightslategrey", "cornflowerblue", "royalblue",
    "ghostwhite", "lavender", "midnightblue", "navy", "darkblue", "mediumblue",  "slateblue", "darkslateblue",
    "mediumslateblue", "mediumpurple", "rebeccapurple", "blueviolet", "indigo", "darkorchid", "darkviolet",
    "mediumorchid", "thistle", "plum", "violet", "purple", "darkmagenta", "fuchsia", "orchid",
     "deeppink", "hotpink", "lavenderblush", "palevioletred", "pink", "lightpink"
};


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


void plotPoints(std::vector<Point> points, std::vector<Cluster> clusters) {
    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> id;

    std::vector<double> cx;
    std::vector<double> cy;
    std::vector<double> cid;

    //unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    //std::shuffle(COLORS.begin(), COLORS.end(), std::default_random_engine(seed));

    for (size_t i = 0; i < points.size(); ++i) {
        x.push_back(points[i].get_x());
        y.push_back(points[i].get_y());
        id.push_back(points[i].get_cluster_id());
    }

    for (size_t i = 0; i < clusters.size(); ++i) {
        cx.push_back(clusters[i].get_centroid().get_x());
        cy.push_back(clusters[i].get_centroid().get_y());
        cid.push_back(clusters[i].get_id());
    }


    for (size_t i = 0; i < x.size(); ++i) {
        //std::cout << x[i]<< std::endl;
        plt::scatter(std::vector<double>{x[i]}, std::vector<double>{y[i]}, 50, { {"color", COLORS[id[i]]} });
    }
    

    for (size_t i = 0; i < cx.size(); ++i) {
        plt::scatter(std::vector<double>{cx[i]}, std::vector<double>{cy[i]}, 50, { {"color", COLORS[cid[i]]},  {"marker", "x"} });
    }


    plt::title("Scatter Plot with Different Colors");
    plt::xlabel("X");
    plt::ylabel("Y");

    plt::show();
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