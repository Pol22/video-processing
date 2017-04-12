#pragma once
#include "strap_trajectoryes.h"

class Walds_strapper
{
public:
	Walds_strapper(int num_noise_points) 
	{
		bad_prob = double(num_noise_points) / width / height;
	};
	void get_good_points(vector<Point> input_points, vector<Point> &output_points);

private:
	int width = 1280;
	int height = 720;
	double vmax = 20.0;
	double sigma = vmax / 3;
	double bad_prob;

	// thresholds
	double low_threshold = 10.0; // порог сброса траетории
	double high_threshold = 30.0; // порог принятия траетории

	list<Point> trajectory;
	list<deque<double>> traj_probs;
	list<Point> predict_points;
	int N = 5;
};