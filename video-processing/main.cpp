#include <iostream>
#include "opencv2/opencv.hpp"
#include <chrono>

using namespace cv;
using namespace std;

char* video_file = "D:/cv/data/768x576.avi";

int main(int argc, const char** argv)
{
	
	VideoCapture video(video_file);

	if (!video.isOpened())
		return -1;

	int number_of_frames = video.get(CV_CAP_PROP_FRAME_COUNT);

	Mat edges;
	// fps
	auto start = chrono::high_resolution_clock::now();
	auto end = chrono::high_resolution_clock::now();
	double seconds;
	int fps;
	// number of points
	int flow_num = 11;

	Mat frame;
	Mat frame_gray;
	video >> frame;
	cvtColor(frame, frame_gray, COLOR_RGB2GRAY);

	int height = frame.cols;
	int width = frame.rows;
	vector<Point> initial_points;
	map<Point, uchar> intesivity_of_points;

	for (int i = height / (flow_num + 1); i < height - flow_num + 1; i += height / (flow_num + 1))
	{
		for (int j = width / (flow_num + 1); j < width - flow_num + 1; j += width / (flow_num + 1))
		{
			initial_points.push_back(Point(i, j));
		}
	}
	
	int size_of_window = 7;

	Mat previous_frame;
	previous_frame = frame_gray;

	for (int i = 1; i < number_of_frames; i++)
	{
		video >> frame;
		cvtColor(frame, frame_gray, COLOR_BGR2GRAY);

		for (int k = 0; k < flow_num * flow_num; k++)
		{
			// search points on next frame
			uchar previous_color = previous_frame.at<uchar>(initial_points[k]);
			Point start_point(initial_points[k].x - size_of_window / 2, initial_points[k].y - size_of_window / 2);
			Point next_point;
			uchar min_differece = 255;
			for (int p = 0; p < size_of_window; p++)
			{
				for (int q = 0; q < size_of_window; q++)
				{
					if (start_point.y + p < 0 || start_point.y + p > height ||
						start_point.x + q < 0 || start_point.x + q > width)
						continue;
					uchar new_color = frame_gray.at<uchar>(start_point.y + p, start_point.x + q);
					if (abs(new_color - previous_color) < min_differece)
					{
						next_point.x = start_point.x + q;
						next_point.y = start_point.y + p;
						min_differece = abs(new_color - previous_color);
					}
				}
			}
			line(frame, initial_points[k], next_point, Scalar(0, 0, 255), 5);
			initial_points[k] = next_point;
			//circle(frame, initial_points[k], 2, Scalar(0, 255, 0), 2);
		}
		previous_frame = frame_gray;

		// fps
		end = chrono::high_resolution_clock::now();
		seconds = chrono::duration_cast<chrono::nanoseconds>(end - start).count() / double(1000000000);
		fps = int(1 / seconds);
		string fps_text("fps: ");
		fps_text.append(to_string(fps));
		putText(frame, fps_text, Point(1, 15), FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 255, 0), 1);



		imshow("edges", frame);
		waitKey(5);
		start = end;
	}

	cvDestroyAllWindows();

	return 0;
}
