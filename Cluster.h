#pragma once
#include<vector>
#include <random>

class Point {
public:
	Point() {
		x = 0;
		y = 0;
		cluster_id = 0;
	}

	Point(double x, double y) {
		this->x = x;
		this->y = y;
		cluster_id = 0;
	}

	double get_x() { return x; }
	void set_x(double x) { this->x = x; }
	double get_y() { return y; }
	void set_y(double y) { this->y = y; }
	int get_cluster_id() { return cluster_id; }
	void set_cluster_id(int id) { cluster_id = id; }

private:
	double x;
	double y;
	int cluster_id;
};

double euclidean_dist(Point p,Point q) {
	return sqrt( pow((p.get_x() - q.get_x()),2) + pow((p.get_y() - q.get_y()), 2) );
};

class Cluster
{
public:
	Cluster() { 
		size = 0;
		centroid = Point();
		cum_sum = Point();
		centroid.set_cluster_id(-1);
		cum_sum.set_cluster_id(-1);
	}

	void init_cluster(int id, double min_value = 0, double max_value =1030, int seed= std::random_device{}()) {
		std::mt19937 gen(seed);
		std::uniform_real_distribution<> dis(min_value, max_value);
		double firstRandomDouble = dis(gen);
		double secondRandomDouble = dis(gen);

		centroid.set_x(firstRandomDouble);
		centroid.set_y(secondRandomDouble);

		this->id = id;
	}

	bool update_centroid() {
		bool convergence = false;
		double cx;
		double cy;

		
		if (size != 0){
			cx = cum_sum.get_x() / size;
			cy = cum_sum.get_y() / size;
		}
		else {
			cx = centroid.get_x();
			cy = centroid.get_y();
		}

		if (abs(centroid.get_x() - cx) < 0.01 && abs(centroid.get_y() - cy) < 0.01)
			convergence = true;
		centroid.set_x(cx);
		centroid.set_y(cy);
		

		cum_sum.set_x(0);
		cum_sum.set_y(0);
		size = 0;

		return convergence;
	}

	void add_point(Point p) {
		double cx = cum_sum.get_x();
		double cy = cum_sum.get_y();
		cum_sum.set_x(cx + p.get_x());
		cum_sum.set_y(cy + p.get_y());
		size++;
		p.set_cluster_id(id);
	}

	Point get_centroid() { return centroid; }
	int get_size() { return size; }
	Point get_cumulative_sum() { return cum_sum; }
	int get_id() { return id; }


private:
	Point centroid;
	int size;
	Point cum_sum;
	int id = 0;
};



