#include <omp.h>
#include <random>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include <cmath>
#include <chrono>

struct alignas(64) PointsStruct {
    std::vector<float> x, y, minDistance;
    std::vector<int> cluster_id;
};

struct alignas(64) ClustersStruct {
    std::vector<float> x, y;
    std::vector<int> indices;
    std::vector<float> cum_sum_x, cum_sum_y;
    std::vector<int> size;
};
PointsStruct Points;
ClustersStruct Clusters;


void write_to_csv(const std::string& filename, int numPoints) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return;
    }
    file << "x,y,cluster_id\n";
    for (size_t i = 0; i < numPoints; ++i) {
        file << Points.x[i] << "," << Points.y[i] << "," << Points.cluster_id[i] << "\n";
    }
    file.close();
}

void write_centroids_to_csv(const std::string& centroid_filename, int numClusters) {
    std::ofstream file(centroid_filename);
    if (!file.is_open()) {
        return;
    }
    file << "x,y,cluster_id\n";
    for (size_t i = 0; i < numClusters; ++i) {
        file << Clusters.x[i] << "," << Clusters.y[i] << "," << Clusters.indices[i] << "\n";
    }
    file.close();
}

int main() {
    auto start_time = std::chrono::high_resolution_clock::now();

    int numThreads = omp_get_max_threads();
    omp_set_num_threads(numThreads);

    std::string output_points = "../data/output_parallel.csv";
    std::string output_centroids = "../data/centroids_parallel.csv";

    int numPoints = 10000000;
    int numClusters = 5;
    float min = -100.0f, max = 100.0f;

    // Resize Points and Clusters data
    Points.x.resize(numPoints);
    Points.y.resize(numPoints);
    Points.cluster_id.resize(numPoints, -1);
    Points.minDistance.resize(numPoints, std::numeric_limits<float>::max());

    Clusters.x.resize(numClusters);
    Clusters.y.resize(numClusters);
    Clusters.indices.resize(numClusters, -1);
    Clusters.cum_sum_x.resize(numClusters, 0.0f);
    Clusters.cum_sum_y.resize(numClusters, 0.0f);
    Clusters.size.resize(numClusters, 0); 

    #pragma omp parallel 
    {
        
        std::random_device rd;
        std::mt19937 gen(rd() + omp_get_thread_num());
        std::uniform_real_distribution<float> dis(min, max);

        // Random point initialization
        #pragma omp for
        for (size_t i = 0; i < numPoints; ++i) {
            Points.x[i] = dis(gen);
            Points.y[i] = dis(gen);
            Points.cluster_id[i] = -1;
        }

        // Random centroid initialization
        #pragma omp for
        for (size_t i = 0; i < numClusters; ++i) {
            Clusters.x[i] = dis(gen);
            Clusters.y[i] = dis(gen);
            Clusters.indices[i] = i;
        }
    }


    // Conpute points-centroid distance -> Assign points to nearest cluster
    #pragma omp parallel for schedule(dynamic, 100)
    for (size_t point = 0; point < numPoints; ++point) {
        float px = Points.x[point];
        float py = Points.y[point];
        float minDist = Points.minDistance[point];
        int nearestCluster = Points.cluster_id[point];

        for (size_t cluster = 0; cluster < numClusters; ++cluster) {
            float dx = px - Clusters.x[cluster];
            float dy = py - Clusters.y[cluster];
            float squared_distance = dx * dx + dy * dy; // No sqrt(), compare squared distances

            if (squared_distance < minDist) {
                minDist = squared_distance;
                nearestCluster = cluster;
            }
        }
        Points.minDistance[point] = minDist;
        Points.cluster_id[point] = nearestCluster;
    }


    
    for (size_t cluster = 0; cluster < numClusters; ++cluster) {
        Clusters.size[cluster] = 0;
    }

    // Updateclusters -> COMPUTE CUMULATIVE SUMS
    #pragma omp parallel
    {
        std::vector<float> local_cum_sum_x(numClusters, 0.0f);
        std::vector<float> local_cum_sum_y(numClusters, 0.0f);
        std::vector<int> local_size(numClusters, 0);

        #pragma omp for
        for (size_t point = 0; point < numPoints; ++point) {
            int cluster = Points.cluster_id[point];
            local_cum_sum_x[cluster] += Points.x[point];
            local_cum_sum_y[cluster] += Points.y[point];
            local_size[cluster]++;
        }

        // Use atomic operations 
        for (size_t cluster = 0; cluster < numClusters; ++cluster) {
            #pragma omp atomic
            Clusters.cum_sum_x[cluster] += local_cum_sum_x[cluster];

            #pragma omp atomic
            Clusters.cum_sum_y[cluster] += local_cum_sum_y[cluster];

            #pragma omp atomic
            Clusters.size[cluster] += local_size[cluster];
        }
    }

    // Compute new centroids
    #pragma omp parallel for
    for (size_t cluster = 0; cluster < numClusters; ++cluster) {
        if (Clusters.size[cluster] > 0) {
            Clusters.x[cluster] = Clusters.cum_sum_x[cluster] / Clusters.size[cluster];
            Clusters.y[cluster] = Clusters.cum_sum_y[cluster] / Clusters.size[cluster];
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> execution_time = end_time - start_time;
    std::cout << "Execution time: " << execution_time.count() << " seconds." << std::endl;

    // Now write the CSV files (excluded from timing)
    write_to_csv(output_points, numPoints);
    write_centroids_to_csv(output_centroids, numClusters);

    return 0;
}
