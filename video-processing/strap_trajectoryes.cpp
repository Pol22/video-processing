#include "strap_trajectoryes.h"

void Strap_trajectoryes::get_good_points(vector<Point> input_points, vector<Point> &output_points)
{
	set<int> employment_indexes; // for 5
								 // 2
	auto iter_traject = trajectory.begin();
	auto iter_tag = tag.begin();

	for (; iter_traject != trajectory.end(); )
	{
		Point nearest_point;
		double min_distance = vmax; // without prediction

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

		if (min_distance > vmax) // with prediction
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

void Strap_trajectoryes::get_good_points_with_prediction(vector<Point> input_points, vector<Point> &output_points)
{
	set<int> employment_indexes; // for 5
								 // 2
	auto iter_traject = trajectory.begin();
	auto iter_tag = tag.begin();
	auto iter_predict = predict_points.begin();

	for (; iter_traject != trajectory.end(); )
	{
		Point nearest_point;
		double min_distance = vmax + 3;

		int index_in_input = -1;

		for (int j = 0; j < input_points.size(); j++)
		{
			double dist = norm(input_points[j] - *iter_predict); // with prediction

			if (dist < min_distance && employment_indexes.find(j) == employment_indexes.end())
			{
				min_distance = dist;
				nearest_point = input_points[j];
				index_in_input = j;
			}
		}

		if (min_distance > vmax)
		{
			iter_tag->push_back(false);
		}
		else
		{
			iter_tag->push_back(true);

			// prediction
			*iter_predict = nearest_point + (nearest_point - *iter_traject);

			if (iter_predict->x < 0)
				iter_predict->x = -iter_predict->x;
			if (iter_predict->x > width)
				iter_predict->x = width - (iter_predict->x - width);
			if (iter_predict->y < 0)
				iter_predict->y = -iter_predict->y;
			if (iter_predict->y > height)
				iter_predict->y = height - (iter_predict->y - height);

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