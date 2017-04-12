#include <fstream>
#include "strap_Walds.h"

// rand from 0 to 1
double Random()
{
	return double(rand()) / RAND_MAX;
}

// params model
double vmax = 20.0;
double r0 = 10.0;
double changeProb = 0.1;

int main1()
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
	}

	int number_of_noise_points = 100;
	// Strap trajectoryes
	//Strap_trajectoryes strapper;
	Walds_strapper strapper(number_of_noise_points);

	for (int i = 0; ; i++)
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

			int next_x = points[j].x + vx[j];
			int next_y = points[j].y + vy[j];

			//printf("[%g,%g]",vx[j],vy[j]);

			if (Random() < changeProb)
			{
				double r = r0 * Random();
				double fi = 2 * M_PI * Random();
				vx[j] = vx[j] + r * cos(fi);
				vy[j] = vy[j] + r * sin(fi);

				double vv = sqrt(vx[j] * vx[j] + vy[j] * vy[j]);
				if (vv > vmax)
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


		// shuffle vector with all points
		std::random_shuffle(all_points.begin(), all_points.end());

		// algorithms
		//strapper.get_good_points_with_prediction(all_points, good_points_out);
		strapper.get_good_points(all_points, good_points_out);

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
			err.close();
			break;
		}
		imshow("frame", frame);

	}

	cvDestroyAllWindows();

	return 0;
}

int main(int argc, const char** argv)
{
	return main1();
}
