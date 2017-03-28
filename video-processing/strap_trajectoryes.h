#pragma once
#include <vector>
#include <list>
#include <numeric>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

class Strap_trajectoryes
{
public:
	Strap_trajectoryes(){};

	void get_good_points(vector<Point> input_points, vector<Point> &output_points)
	{
		set<int> employment_indexes; // for 5
									 // 2
		auto iter_traject = trajectory.begin();
		auto iter_tag = tag.begin();

		for (; iter_traject != trajectory.end(); )
		{
			Point nearest_point;
			double min_distance = vmax + 1; // without prediction

			int index_in_input = 0;

			for (int j = 0; j < input_points.size(); j++)
			{
				double dist = norm(input_points[j] - *iter_traject); // without prediction

				if (dist < min_distance && employment_indexes.find(j) == employment_indexes.end())
				{
					min_distance = dist;
					nearest_point = input_points[j];
					index_in_input = j;
				}
			}

			double space = vmax + 2;

			if (min_distance > space) // with prediction
			{
				iter_tag->push_back(false);
			}
			else
			{
				iter_tag->push_back(true);
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
					output_points.push_back(*iter_traject);
				}
				// 4
				if (number_of_not_empty_points < K)
				{
					iter_traject = trajectory.erase(iter_traject);
					iter_tag = tag.erase(iter_tag);
				}
				else
				{
					iter_traject++;
					iter_tag++;
				}
			}
			else
			{
				iter_traject++;
				iter_tag++;
			}
		}

		// 5 and 1

		for (int p = 0; p < input_points.size(); p++)
		{
			auto finded = employment_indexes.find(p);
			if (finded == employment_indexes.end())
			{
				trajectory.push_back(input_points[p]);

				deque<bool> _tag;
				_tag.push_back(true);
				tag.push_back(_tag);
			}
		}
	}

	void get_good_points_with_prediction(vector<Point> input_points, vector<Point> &output_points)
	{
		set<int> employment_indexes; // for 5
									 // 2
		auto iter_traject = trajectory.begin();
		auto iter_tag = tag.begin();
		auto iter_predict = predict_points.begin();

		for (; iter_traject != trajectory.end(); )
		{
			Point nearest_point;
			double min_distance = vmax + 11; // with prediction
			
			int index_in_input = 0;

			for (int j = 0; j < input_points.size(); j++)
			{
				double dist =  norm(input_points[j] - *iter_predict); // with prediction

				if (dist < min_distance && employment_indexes.find(j) == employment_indexes.end())
				{
					min_distance = dist;
					nearest_point = input_points[j];
					index_in_input = j;
				}
			}

			double space = vmax + 10;

			if (min_distance > space)
			{
				iter_tag->push_back(false);
			}
			else
			{
				iter_tag->push_back(true);

				// prediction
				*iter_predict = nearest_point + (nearest_point - *iter_traject);

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
					output_points.push_back(*iter_traject);
				}
				// 4
				if (number_of_not_empty_points < K)
				{
					iter_traject = trajectory.erase(iter_traject);
					iter_tag = tag.erase(iter_tag);
					iter_predict = predict_points.erase(iter_predict);
				}
				else
				{
					iter_traject++;
					iter_tag++;
					iter_predict++;
				}
			}
			else
			{
				iter_traject++;
				iter_tag++;
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
			}
		}
	}
private:
	double vmax = 20.0;
	list<Point> trajectory;
	list<Point> predict_points;
	list<deque<bool>> tag;

	int M = 4;
	int K = 3;
	int N = 5;
};