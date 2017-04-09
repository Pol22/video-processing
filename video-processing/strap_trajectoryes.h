#pragma once
#include <vector>
#include <list>
#include <numeric>
#include "opencv2/opencv.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

using namespace cv;
using namespace std;

class Strap_trajectoryes
{
public:
	Strap_trajectoryes(){};

	void get_good_points(vector<Point> input_points, vector<Point> &output_points);

	void get_good_points_with_prediction(vector<Point> input_points, vector<Point> &output_points);

private:
	int width = 1280;
	int height = 720;
	double vmax = 20.0;
	list<Point> trajectory;
	list<Point> predict_points;
	list<deque<bool>> tag;
	// params
	int M = 4;
	int K = 3;
	int N = 5;
};