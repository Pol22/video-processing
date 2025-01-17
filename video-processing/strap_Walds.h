#pragma once
#include "strap_trajectoryes.h"

class Walds_strapper
{
public:
	Walds_strapper(int num_noise_points) 
	{
		bad_prob = double(num_noise_points) / width / height;
	};
	void get_good_points(Mat input_points, vector<Point> &output_points);

private:
	int width = 1280;
	int height = 720;
	double vmax = 20.0;
	double sigma = vmax / 3;
	double bad_prob;

	// thresholds
	double low_threshold = 100.0; // ����� ������ ���������
	double high_threshold = 130.0; // ����� �������� ���������

	list<Point2f> trajectory;
	list<deque<double>> traj_probs;
	list<Point2f> predict_points;
	int N = 5;
};