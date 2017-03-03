#include <iostream>
#include <numeric>
#include "opencv2/opencv.hpp"


using namespace cv;
using namespace std;


deque<deque<Point>> trajectory;
deque<deque<bool>> tag;
deque<bool> followed_trajectory;
int M = 7;
int K = 4;
int N = 10;


void get_good_points(vector<Point> input_points, vector<Point> &output_points)
{
	
	set<int> employment_indexes; // for 5
	// 2
	for (int i = 0; i < trajectory.size(); i++)
	{
		Point nearest_point;
		double min_distance = sqrt(1280 * 1280 + 720 * 720); // frame diagonal
		int index_in_input = 0;

		for (int j = 0; j < input_points.size(); j++)
		{
			double dist = norm(input_points[j] - trajectory[i].back());
			if (dist < min_distance)
			{
				min_distance = dist;
				nearest_point = input_points[j];
				index_in_input = j;
			}
		}

		if (min_distance > 30)
		{
			tag[i].push_back(false);
			trajectory[i].push_back(trajectory[i].back());
		}
		else
		{
			tag[i].push_back(true);
			trajectory[i].push_back(nearest_point);
			employment_indexes.insert(index_in_input);
		}

		// save last N points
		if (tag[i].size() > N)
		{
			tag[i].pop_front();
			trajectory[i].pop_front();
		}

		// 3
		if (tag[i].size() == N)
		{
			int number_of_not_empty_points = std::accumulate(tag[i].begin(), tag[i].end(), 0);

			if (number_of_not_empty_points >= M && tag[i].back() == true)
			{
				followed_trajectory[i] = true;
				output_points.push_back(trajectory[i].back());
			}
			// 4
			if (number_of_not_empty_points < K)
			{
				followed_trajectory.erase(followed_trajectory.begin() + i);
				trajectory.erase(trajectory.begin() + i);
				tag.erase(tag.begin() + i);
			}
		}
	}

	// 5 and 1
		
	for (int p = 0; p < input_points.size(); p++)
	{
		auto finded = employment_indexes.find(p);
		if (finded == employment_indexes.end())
		{
			deque<Point> _deque;
			_deque.push_back(input_points[p]);
			trajectory.push_back(_deque);

			deque<bool> _tag;
			_tag.push_back(true);
			tag.push_back(_tag);

			followed_trajectory.push_back(false);
		}
	}
}


int main(int argc, const char** argv)
{
	srand(time(NULL));
	int number_of_targets = 30;
	int width = 1280;
	int height = 720;
	Mat frame(height, width, CV_8UC3, Scalar(255, 255, 255));

	vector<Point> points;
	vector<double> last_angle;
	vector<Point> all_points;

	for (int k = 0; k < number_of_targets; k++)
	{
		points.emplace_back(rand() % width, rand() % height);
		last_angle.push_back(0.0);
	}
	
    int number_of_noise_points = 100;


	while(1 > 0)
	{
		all_points.clear();
        frame = Scalar(255, 255, 255);

        for (int p = 0; p < number_of_noise_points; p++)
        {
            Point noise_point(rand() % width, rand() % height);

			all_points.push_back(noise_point);

			Point text_point(noise_point.x - 13, noise_point.y + 9);

            putText(frame, "+", text_point, FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0));
        }

		for (int j = 0; j < number_of_targets; j++)
		{
			//circle(frame, points[j], 2, Scalar(0, 255, 0), 2);
			Point text_point(points[j].x - 13, points[j].y + 9);

			//Point text_point(points[j].x, points[j].y);
			putText(frame, "+", text_point, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255));
			int length_of_step = rand() % 30 + 1;
            double angle = ((rand() % 181) - 90) / 45.0 * atan(1.0);
			double sum_angle = last_angle[j] + angle;
			last_angle[j] = sum_angle;

			int next_x = points[j].x + length_of_step * cos(sum_angle);
			int next_y = points[j].y + length_of_step * sin(sum_angle);

			if (next_x < 0)
				next_x = -next_x;
			if (next_x > width)
				next_x = width - (next_x - width);
			if (next_y < 0)
				next_y = -next_y;
			if (next_y > height)
				next_y = height - (next_y - height);

			Point next_point(next_x, next_y);
			text_point.x = next_x - 13;
			text_point.y = next_y + 9;
            //putText(frame, "+", text_point, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255));
            //line(frame, points[j], next_point, Scalar(0, 0, 255), 1);

			all_points.push_back(points[j]);

			points[j] = next_point;
		}

		// shuffle vector with all points
		random_shuffle(all_points.begin(), all_points.end());

		vector<Point> good_points;
		// need to implement
		get_good_points(all_points, good_points);

		for (int k = 0; k < good_points.size(); k++)
		{
			circle(frame, good_points[k], 10, Scalar(0, 255, 0), 2);
		}


		imshow("frame", frame);

        int key = waitKey(1);
		if (key == 27)
			break;
	}

	cvDestroyAllWindows();

	return 0;
}
