#include <iostream>
#include <opencv2\highgui\highgui.hpp>


using namespace cv;
using namespace std;

int main(int argc, const char** argv)
{
	Mat img = imread("1kadr.bmp", CV_LOAD_IMAGE_UNCHANGED);
	if (img.empty())
	{
		cout << "Error : Image cannot be loaded....." << endl;
		return -1;
	}
	namedWindow("MYWINDOW", CV_WINDOW_AUTOSIZE);
	imshow("MYWINDOW", img);

	waitKey(0);

	cvDestroyAllWindows();

	return 0;
}