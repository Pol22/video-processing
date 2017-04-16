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

	void get_good_points(Mat input_points, vector<Point> &output_points);

	void get_good_points_with_prediction(Mat input_points, vector<Point> &output_points);

private:
	int width = 1280;
	int height = 720;
	double vmax = 50.0;
	list<Point2f> trajectory;
	list<Point2f> predict_points;
	list<deque<bool>> tag;
	// params
	int M = 8;
	int K = 7;
	int N = 10;
};