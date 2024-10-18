#include <random>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include <cmath>
#include <chrono> 

// 0.165797 seconds needed

// Assuming Points and Clusters are globally defined structures
struct PointsStruct {
    std::vector<float> x, y, minDistance;
    std::vector<int> cluster_id;
};

struct ClustersStruct {
    std::vector<float> x, y;
    std::vector<int> indices;
    std::vector<float> cum_sum_x, cum_sum_y;
    std::vector<int> size;
};

// Global data
PointsStruct Points;
ClustersStruct Clusters;

float euclidean_distance(float x1, float y1, float x2, float y2) {
    return std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

// Improved CSV writing functions
void write_to_csv(const std::string& filename, int numPoints) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return; // No more std::cerr here
    }

    // Write points
    file << "x,y,cluster_id\n"; // Header for points
    for (size_t i = 0; i < numPoints; ++i) {
        file << Points.x[i] << "," << Points.y[i] << "," << Points.cluster_id[i] << "\n";
    }

    file.close();
}

void write_centroids_to_csv(const std::string& centroid_filename, int numClusters) {
    std::ofstream file(centroid_filename);
    if (!file.is_open()) {
        return; // No more std::cerr here
    }

    // Write clusters
    file << "x,y,cluster_id\n"; // Header for clusters
    for (size_t i = 0; i < numClusters; ++i) {
        file << Clusters.x[i] << "," << Clusters.y[i] << "," << Clusters.indices[i] << "\n";
    }

    file.close();
}

int main() {
    // Start the timer
    auto start_time = std::chrono::high_resolution_clock::now();

    std::string output_points = "../data/output_sequential.csv"; // CSV filename
    std::string output_centroids = "../data/centroids_sequential.csv"; // CSV filename

    // Initialize number of points and clusters
    int numPoints = 10000000; // example value
    int numClusters = 5; // example value
    float min = -100.0f, max = 100.0f;

    // Resize Points and Clusters data
    Points.x.resize(numPoints);
    Points.y.resize(numPoints);
    Points.cluster_id.resize(numPoints, -1);  // Initialize cluster_id to -1
    Points.minDistance.resize(numPoints, std::numeric_limits<float>::max());

    Clusters.x.resize(numClusters);
    Clusters.y.resize(numClusters);
    Clusters.indices.resize(numClusters, -1);
    Clusters.cum_sum_x.resize(numClusters, 0.0f);  // For later updates
    Clusters.cum_sum_y.resize(numClusters, 0.0f);
    Clusters.size.resize(numClusters, 0); 

    // Random generator initialization
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);

    // Sequential initialization of points and clusters
    for (size_t i = 0; i < numPoints; ++i) {
        Points.x[i] = dis(gen);
        Points.y[i] = dis(gen);
        Points.cluster_id[i] = -1;  // Initially, no cluster assignment
    }

    for (size_t i = 0; i < numClusters; ++i) {
        Clusters.x[i] = dis(gen);
        Clusters.y[i] = dis(gen);
        Clusters.indices[i] = i; // Assign each cluster an index
    }

    // Sequential computation of distances and assignment to nearest cluster
    for (size_t point = 0; point < numPoints; ++point) {
        for (size_t cluster = 0; cluster < numClusters; ++cluster) {
            float distance = euclidean_distance(Points.x[point], Points.y[point], Clusters.x[cluster], Clusters.y[cluster]);
            if (distance < Points.minDistance[point]) {
                Points.minDistance[point] = distance;
                Points.cluster_id[point] = cluster;
            }
        }
    }

    // Reset cluster sizes before proceeding
    for (size_t cluster = 0; cluster < numClusters; ++cluster) {
        Clusters.size[cluster] = 0;  // Reset cluster sizes
    }

    // Sequential aggregation of centroids' sum
    for (size_t point = 0; point < numPoints; ++point) {
        int cluster = Points.cluster_id[point];
        Clusters.cum_sum_x[cluster] += Points.x[point];
        Clusters.cum_sum_y[cluster] += Points.y[point];
        Clusters.size[cluster]++;
    }

    // Finalize cluster centroids' positions
    for (size_t cluster = 0; cluster < numClusters; ++cluster) {
        if (Clusters.size[cluster] > 0) {
            Clusters.x[cluster] = Clusters.cum_sum_x[cluster] / Clusters.size[cluster];
            Clusters.y[cluster] = Clusters.cum_sum_y[cluster] / Clusters.size[cluster];
        }
    }

    // Write the generated points and clusters to a CSV file
    write_to_csv(output_points, numPoints);
    write_centroids_to_csv(output_centroids, numClusters);

    // Stop the timer
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> execution_time = end_time - start_time;
    std::cout << "Execution time: " << execution_time.count() << " seconds." << std::endl;

    return 0;
}
