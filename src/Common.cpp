/*
 * Common.cpp
 *
 *  Created on: Nov 2, 2014
 *      Author: alucard
 */

#include "Common.hpp"

template <typename T>
T distance(const cv::Vec<T, 2>& a, const cv::Vec<T, 2>& b)
{
  T dx, dy, dist;
  dx = a[0] - b[0];
  dy = a[1] - b[1];
  dist = (T)(sqrt((double)(dx*dx + dy*dy)));
  return dist;
}

double norm_approximation(const double &dx, const double &dy)
{
	return 0.41*dx + 0.941246*dy;
}
