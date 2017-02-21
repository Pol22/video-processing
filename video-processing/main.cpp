#include <iostream>
#include "opencv2/opencv.hpp"


using namespace cv;
using namespace std;


int main(int argc, const char** argv)
{
	srand(time(NULL));
	int number_of_targets = 30;
    int number_of_frames = 200;
	int width = 1280;
	int height = 720;
	Mat frame(height, width, CV_8UC3, Scalar(255, 255, 255));

	vector<Point> points;
	vector<double> last_angle;

	for (int k = 0; k < number_of_targets; k++)
	{
		points.emplace_back(rand() % width, rand() % height);
		last_angle.push_back(0.0);
	}
	
    int number_of_noise_points = 100;

	for (int i = 0; i < number_of_frames; i++)
	{
		
        frame = Scalar(255, 255, 255);

        for (int p = 0; p < number_of_noise_points; p++)
        {
            Point noise_point(rand() % width, rand() % height);
            putText(frame, "+", noise_point, FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0));
        }

		for (int j = 0; j < number_of_targets; j++)
		{
			//circle(frame, points[j], 2, Scalar(0, 255, 0), 2);
			Point text_point(points[j].x - 13, points[j].y + 9);
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
			points[j] = next_point;
		}


		imshow("frame", frame);
        waitKey(50);
	}

	cvDestroyAllWindows();

	return 0;
}
