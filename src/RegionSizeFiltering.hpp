/*
 * RegionSizeFiltering.hpp
 *
 *  Created on: Nov 10, 2014
 *      Author: Tomasz Posłuszny
 *
 *      Copyright. All rights reserved.
 *
 */
#ifndef REGIONSIZEFILTERING_HPP_
#define REGIONSIZEFILTERING_HPP_

#include "Common.hpp"
#include "ImageProcessor.hpp"

/*
 * INPUT: raw moving are mask
 * OUTPUT: moving area mask with filtered small regions
 */
class RegionSizeFiltering : public ImageProcessor {
public:
	RegionSizeFiltering(int min_white, int min_black);
	virtual ~RegionSizeFiltering();

	typedef std::vector< std::vector<int> > Neighborhood_Graph;
private:
	void process_implementation(Mat &a, void* data);
	int unify_label(const int& l);
	bool is_border(const Mat& m, const int& x, const int& y);
	int get_neighbor(const Mat& m, const Mat& lab, const int& x, const int& y);
	int get_neighbor(const Mat& lab, const int& x, const int& y);
	bool already_neighbor(const Neighborhood_Graph& g, const int& a, const int& b);

	int m_min_white;
	int m_min_black;
	std::vector<int> m_merge_proxy;
	std::vector<int> m_sizes;
	// TODO: replace with vector for time optimization
	// std::set<int> m_boundary_regions;
};

#endif /* REGIONSIZEFILTERING_HPP_ */
