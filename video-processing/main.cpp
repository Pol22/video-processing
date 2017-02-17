#include <iostream>
#include <opencv2\opencv.hpp>


using namespace cv;
using namespace std;


int main(int argc, const char** argv)
{
	srand(time(NULL));
	int number_of_targets = 30;
	int number_of_frames = 20;
	int width = 1280;
	int height = 720;
	Mat frame(height, width, CV_8UC3, Scalar(255, 255, 255));

	vector<Point> points;
	for (int k = 0; k < number_of_targets; k++)
	{
		points.emplace_back(rand() % width, rand() % height);
	}
	
	int number_of_noise_points = 300;

	for (int p = 0; p < number_of_noise_points; p++)
	{
		Point noise_point(rand() % width, rand() % height);
		putText(frame, "+", noise_point, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255));
	}

	for (int i = 0; i < number_of_frames; i++)
	{
		
		for (int j = 0; j < number_of_targets; j++)
		{
			//circle(frame, points[j], 2, Scalar(0, 255, 0), 2);
			Point text_point(points[j].x - 13, points[j].y + 9);
			putText(frame, "+", text_point, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255));
			int length_of_step = rand() % 30 + 1;
			int angle = rand() % 360;
			int next_x = points[j].x + length_of_step * cos(angle);
			int next_y = points[j].y + length_of_step * sin(angle);

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
			putText(frame, "+", text_point, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255));
			line(frame, points[j], next_point, Scalar(0, 0, 255), 1);
			points[j] = next_point;
		}


		imshow("frame", frame);
		waitKey(500);
	}

	cvDestroyAllWindows();

	return 0;
}