#include "strap_Walds.h"

void Walds_strapper::get_good_points(vector<Point> input_points, vector<Point> &output_points)
{
	set<int> employment_indexes;

	auto iter_traject = trajectory.begin();
	auto iter_predict = predict_points.begin();
	auto iter_probs = traj_probs.begin();

	for (; iter_traject != trajectory.end(); )
	{
		Point nearest_point;
		double min_distance = vmax * 10;

		int index_in_input = -1;

		for (int j = 0; j < input_points.size(); j++)
		{
			double dist = norm(input_points[j] - *iter_predict);

			if (dist < min_distance && employment_indexes.find(j) == employment_indexes.end())
			{
				min_distance = dist;
				nearest_point = input_points[j];
				index_in_input = j;
			}
		}

		if (index_in_input > -1)
		{
			double good_prob = 1 / (sigma * sqrt(2 * M_PI)) * exp(-(min_distance * min_distance) / (2 * sigma * sigma));
			iter_probs->push_back(10 * log10(good_prob / bad_prob));

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
		if (iter_probs->size() > N)
		{
			iter_probs->pop_front();
		}

		// 3
		if (iter_probs->size() == N)
		{
			double sum_true_ratio = std::accumulate(iter_probs->begin(), iter_probs->end(), 0.0);

			if (sum_true_ratio > high_threshold)
			{
				output_points.push_back(*iter_traject);
			}
			// 4
			if (sum_true_ratio < low_threshold)
			{
				iter_traject = trajectory.erase(iter_traject);
				iter_probs = traj_probs.erase(iter_probs);
				iter_predict = predict_points.erase(iter_predict);
			}
			else
			{
				iter_traject++;
				iter_probs++;
				iter_predict++;
			}
		}
		else
		{
			iter_traject++;
			iter_probs++;
			iter_predict++;
		}
	}

	for (int p = 0; p < input_points.size(); p++)
	{
		auto finded = employment_indexes.find(p);
		if (finded == employment_indexes.end())
		{
			trajectory.push_back(input_points[p]);
			predict_points.push_back(input_points[p]);
			
			deque<double> _probs;
			double good_prob = 1 / (sigma * sqrt(2 * M_PI));
			_probs.push_back(10 * log10(good_prob / bad_prob));
			traj_probs.push_back(_probs);
		}
	}
}