#include <fstream>
#include <numeric>
#include "opencv2/opencv.hpp"

#define _USE_MATH_DEFINES
#include <math.h>


using namespace cv;
using namespace std;

// rand from 0 to 1
double Random()
{
	return double(rand()) / RAND_MAX;
}

// params model
double vmax = 20.0;
double r0 = 5.0;
double changeProb = 0.01;

list<Point> trajectory;
list<Point> predict_points;
list<deque<bool>> tag;
list<bool> followed_trajectory;
int M = 4;
int K = 3;
int N = 5;

// mode = true - with prediction
void get_good_points(vector<Point> input_points, vector<Point> &output_points, bool mode = true) 
{
	set<int> employment_indexes; // for 5
	// 2
	auto iter_traject = trajectory.begin();
	auto iter_tag = tag.begin();
	auto iter_follow = followed_trajectory.begin();
	auto iter_predict = predict_points.begin();

	for ( ; iter_traject != trajectory.end(); )
	{
		Point nearest_point;
		double min_distance = 0;
		if(mode)
			min_distance = vmax + 11; // with prediction
		else
			min_distance = vmax + 1; // without prediction

		int index_in_input = 0;

		for (int j = 0; j < input_points.size(); j++)
		{
			double dist = 0;
			if(mode)
				dist = norm(input_points[j] - *iter_predict); // with prediction
			else
				dist = norm(input_points[j] - *iter_traject); // without prediction

			if (dist < min_distance && employment_indexes.find(j) == employment_indexes.end())
			{
				min_distance = dist;
				nearest_point = input_points[j];
				index_in_input = j;
			}
		}

		double space = 0;
		if (mode)
			space = vmax + 10;
		else
			space = vmax;

		if (min_distance > space) // with prediction
		{
			iter_tag->push_back(false);
		}
		else
		{
			iter_tag->push_back(true);
			
			// prediction
			if (mode)
			{
				double dist_predict = norm(nearest_point - *iter_traject);
				double koef_line = double(nearest_point.y - iter_traject->y) / (nearest_point.x - iter_traject->x);
				double alpha = atan(koef_line);

				iter_predict->x = nearest_point.x + dist_predict / 2 * cos(alpha);
				iter_predict->y = nearest_point.y + dist_predict / 2 * sin(alpha);
			}
			*iter_traject = nearest_point;
			employment_indexes.insert(index_in_input);
		}

		// save last N points
		if (iter_tag->size() > N)
		{
			iter_tag->pop_front();
		}
		
		// 3
		if (iter_tag->size() == N)
		{
			int number_of_not_empty_points = std::accumulate(iter_tag->begin(), iter_tag->end(), 0);

			if (number_of_not_empty_points >= M && iter_tag->back() == true)
			{
				*iter_follow = true;
				output_points.push_back(*iter_traject);
			}
			// 4
			if (number_of_not_empty_points < K)
			{
				iter_follow = followed_trajectory.erase(iter_follow);
				iter_traject = trajectory.erase(iter_traject);
				iter_tag = tag.erase(iter_tag); 
				iter_predict = predict_points.erase(iter_predict);
			}
			else
			{
				iter_traject++;
				iter_tag++;
				iter_follow++;
				iter_predict++;
			}
		}
		else
		{
			iter_traject++;
			iter_tag++;
			iter_follow++;
			iter_predict++;
		}
	}

	// 5 and 1
		
	for (int p = 0; p < input_points.size(); p++)
	{
		auto finded = employment_indexes.find(p);
		if (finded == employment_indexes.end())
		{
			trajectory.push_back(input_points[p]);
			predict_points.push_back(input_points[p]);

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
	// error
	string errors_file("errors1.txt"); // error file
	ofstream err;
	err.open(errors_file);
	
	vector<Point> points;
	vector<double> vx;
	vector<double> vy;
	vector<Point> good_points;
	vector<Point> good_points_out;
	// vector<double> last_angle;
	vector<Point> all_points;


	for (int k = 0; k < number_of_targets; k++)
	{
		points.emplace_back(rand() % width, rand() % height);
        double r = vmax * Random();
        double fi = 2 * M_PI * Random();
		vx.push_back(r * cos(fi));
		vy.push_back(r * sin(fi));
		// last_angle.push_back(0.0);
        //printf("[%g,%g]",r,fi);
	}
	
    int number_of_noise_points = 100;


	for(int i = 0; ;i++)
	{
		good_points.clear();
		all_points.clear();
		good_points_out.clear();
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
			//int length_of_step = rand() % 30 + 1;
            //double angle = ((rand() % 181) - 90) / 45.0 * atan(1.0);
			//double sum_angle = last_angle[j] + angle;
			//last_angle[j] = sum_angle;

			//int next_x = points[j].x + length_of_step * cos(sum_angle);
			//int next_y = points[j].y + length_of_step * sin(sum_angle);

			int next_x = points[j].x + vx[j];
			int next_y = points[j].y + vy[j];
			
            //printf("[%g,%g]",vx[j],vy[j]);

            if(Random() < changeProb)
            {
                double r = r0 * Random();
                double fi = 2 * M_PI * Random();
				vx[j] = vx[j] + r * cos(fi);
				vy[j] = vy[j] + r * sin(fi);
				
                double vv = sqrt(vx[j]*vx[j] + vy[j]*vy[j]);
                if(vv > vmax)
                {
                    vx[j] = vx[j] / vv * vmax;
                    vy[j] = vy[j] / vv * vmax;
                }
				
            }

			if (next_x < 0)
            {
				next_x = -next_x;
                vx[j] = -vx[j];
            }
			if (next_x > width)
            {
				next_x = width - (next_x - width);
                vx[j] = -vx[j];
            }
			if (next_y < 0)
            {
				next_y = -next_y;
                vy[j] = -vy[j];
            }
			if (next_y > height)
            {
				next_y = height - (next_y - height);
                vy[j] = -vy[j];
            }

			Point next_point(next_x, next_y);
			text_point.x = next_x - 13;
			text_point.y = next_y + 9;
            //putText(frame, "+", text_point, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255));
            //line(frame, points[j], next_point, Scalar(0, 0, 255), 1);

			all_points.push_back(points[j]);
			good_points.push_back(points[j]);
			// next point
			points[j] = next_point;
		}

        //printf("\n\n");

		// shuffle vector with all points
		random_shuffle(all_points.begin(), all_points.end());

		// implemented
		get_good_points(all_points, good_points_out);
		
		// errors
		int num_good_points_out = 0;
		int num_noise_points_out = 0;
		
		for (int k = 0; k < good_points_out.size(); k++)
		{
			circle(frame, good_points_out[k], 10, Scalar(0, 255, 0), 2);
			if (find(good_points.begin(), good_points.end(), good_points_out[k]) == good_points.end())
				num_noise_points_out++;
			else
				num_good_points_out++;
		}

		err << i << " " << double(number_of_targets - num_good_points_out) / number_of_targets;
		err << " " << double(num_noise_points_out) / number_of_noise_points << endl;


        char key = waitKey(1);
		if (key == 27)
		{
			break;
			err.close();
		}
		imshow("frame", frame);

	}

	cvDestroyAllWindows();

	return 0;
}
