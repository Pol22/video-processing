#pragma once
#include <vector>
#include <list>
#include <numeric>
#include "opencv2/opencv.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;
using namespace cv;

class Walds_strapper
{
public:
	Walds_strapper(int num_good_points, int num_all_points) 
	{
		num_all = num_all_points; // 130
		p_A = double(num_good_points) / num_all; // 3/13
		p_B = 1 - p_A; // 10/13
	};
	void get_good_points(vector<Point> input_points, vector<Point> &output_points);

private:
	double p_A;
	double p_B;
	int num_all;
	double square = 1280 * 720;
	// threshold
	double prob_threshold = 10.0;

	double vmax = 20.0;
	list<Point> trajectory;
	list<Point> predict_points;
	list<double> probs;
};