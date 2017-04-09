#include "strap_Walds.h"

void Walds_strapper::get_good_points(vector<Point> input_points, vector<Point> &output_points)
{
	set<int> employment_indexes; // for 5
								 // 2
	auto iter_traject = trajectory.begin();
	auto iter_predict = predict_points.begin();
	auto iter_prob = probs.begin();

	for (; iter_traject != trajectory.end(); )
	{
		Point nearest_point_predict;
		Point nearest_point_point;
		double min_distance_predict = vmax / 4;
		double min_distance_point = vmax;
		int index_in_input_predict = -1;
		int index_in_input_point = -1;

		for (int j = 0; j < input_points.size(); j++)
		{
			double dist1 = norm(input_points[j] - *iter_predict);
			double dist2 = norm(input_points[j] - *iter_traject);
			if (employment_indexes.find(j) == employment_indexes.end())
			{
				if (dist1 < min_distance_predict)
				{
					min_distance_predict = dist1;
					nearest_point_predict = input_points[j];
					index_in_input_predict = j;
				}
				else if (dist2 < min_distance_point)
				{
					min_distance_point = dist2;
					nearest_point_point = input_points[j];
					index_in_input_point = j;
				}
			}

		}
		// when we find nearest point to predict
		if (index_in_input_predict != -1)
		{
			*iter_predict = nearest_point_predict + (nearest_point_predict - *iter_traject);
			employment_indexes.insert(index_in_input_predict);
			// probs

		}
		else if (index_in_input_point != -1) // else we find nearest point to last point trajectory
		{
			*iter_predict = nearest_point_point + (nearest_point_point - *iter_traject);
			employment_indexes.insert(index_in_input_point);
			// probs
			//
			//*iter_prob += 10 * log10(prob);
		}
		else
		{
			iter_traject = trajectory.erase(iter_traject);
			iter_predict = predict_points.erase(iter_predict);
			iter_prob = probs.erase(iter_prob);
		}
	}

	for (int p = 0; p < input_points.size(); p++)
	{
		auto finded = employment_indexes.find(p);
		if (finded == employment_indexes.end())
		{
			trajectory.push_back(input_points[p]);
			predict_points.push_back(input_points[p]);
			probs.push_back(10 * log10(p_A / p_B));
		}
	}
}