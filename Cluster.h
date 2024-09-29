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

	void init_cluster(int id, double min_value = 0, double max_value = 1000, int seed=42) {
		std::mt19937 gen(seed);
		std::uniform_real_distribution<> dis(0, 10);
		double firstRandomDouble = dis(gen);
		double secondRandomDouble = dis(gen);

		centroid.set_x(firstRandomDouble);
		centroid.set_y(secondRandomDouble);

		this->id = id;
	}

	void update_centroid() {
		centroid.set_x(cum_sum.get_x() / size);
		centroid.set_y(cum_sum.get_y() / size);

		cum_sum.set_x(0);
		cum_sum.set_y(0);
		size = 0;
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


private:
	Point centroid;
	int size;
	Point cum_sum;
	int id = 0;
};



