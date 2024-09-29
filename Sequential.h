#pragma once
#include <cmath>
#include <chrono>
#include <omp.h>
#include "PointGeneretor.h"
#include"Cluster.h"
#include<iostream>
#include <random>
using namespace std;


class Seq_AoS_kemans {
public:
	Seq_AoS_kemans(int num_points, int num_clusters, int max_iterations, std::string filename) {
		points = readCSVToPoints(filename);
		this->num_points = num_points;
		this->num_clusters = num_clusters;
		this->max_iterations = max_iterations;
	}

	void execute() {
        for (int i = 0; i < num_clusters; i++) {
            Cluster c = Cluster();
            c.init_cluster(i, 0, 60);
            clusters.push_back(c);

            //std::cout << clusters[i].get_centroid().get_x() << " , " << clusters[i].get_centroid().get_y() << std::endl;
        }


        int iteration = 0;
        bool c;
        bool true_convergence;

        do {
            true_convergence = true;
            iteration++;


            for (int p = 0; p < num_points; p++) {
                int assing = -1;
                double min_dist = 100000000;

                for (int c = 0; c < num_clusters; c++) {
                    double dist = euclidean_dist(points[p], clusters[c].get_centroid());
                    //std::cout << "(" <<points[p].get_x() <<", "<<points[p].get_y()<<") - ("<< clusters[c].get_centroid().get_x()<<", "<< clusters[c].get_centroid().get_y()<<") "<<dist<< std::endl;
                    if (dist < min_dist) {
                        min_dist = dist;
                        assing = c;
                    }
                }
                points[p].set_cluster_id(assing);
                clusters[assing].add_point(points[p]);
            }



            for (int i = 0; i < num_clusters; i++) {
                c = clusters[i].update_centroid();
                if (!c)
                    true_convergence = false;
            }

        } while (!true_convergence && iteration < max_iterations);
        std::cout << "convergence: " << true_convergence << " iterations: " << iteration << std::endl;
	}

	std::vector < Point > get_points() { return points; }
	std::vector<Cluster>get_clusters() { return clusters; }
private:
	std::vector<Cluster> clusters;
	std::vector<Point> points;
	int num_points;
	int num_clusters;
	int max_iterations;


};
